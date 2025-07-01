/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	Player::Player(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, int hp, int attack, int defense) :
		Actor(stagePtr, pos, rot, scale),
		m_dodgeCoolTime(0.0f),
		m_dodgeTime(0.0f),
		m_originallyHPMax(hp)
	{

	}
	Player::~Player()
	{

	}

	//作成
	void Player::OnCreate()
	{
		//HP設定
		m_HPMax = 100.0f;
		m_HPCurrent = m_HPMax;

		//Playerはスロー処理の影響にならない
		m_ignoreDeltaScale = true;

		//ActorのOnCreateを呼んでます
		Actor::OnCreate();

		//Transform設定
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(0.7f, 0.35f, 0.7f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f),
			Vec3(0.0f, -1.5f, 0.0f)
		);

		//ドローメッシュの設定
		auto ptrDraw = GetComponent<PNTBoneModelDraw>();
		ptrDraw->SetMultiMeshResource(L"PlayerModelTestVer2.0");//仮のメッシュ
		ptrDraw->AddAnimation(L"Idle", 21, 21, true, 60.0f);//立ち状態
		ptrDraw->AddAnimation(L"Walk", 165, 65, true, 24.0f);//歩き状態
		ptrDraw->AddAnimation(L"Dodge", 232, 11, false, 24.0f);//回避
		ptrDraw->AddAnimation(L"Dash", 244, 28, true, 24.0f);//走り
		ptrDraw->AddAnimation(L"DashEnd", 273, 27, false, 24.0f);//走りをやめる
		ptrDraw->AddAnimation(L"DashAttack", 331, 51, false, 24.0f);//突進切り
		ptrDraw->AddAnimation(L"Attack1", 395, 14, false, 24.0f);//Attack1
		ptrDraw->AddAnimation(L"Attack2", 410, 34, false, 24.0f);//Attack2
		ptrDraw->AddAnimation(L"Attack3", 445, 38, false, 24.0f);//Attack3
		ptrDraw->AddAnimation(L"AttackEx", 484, 50, false, 24.0f);//AttackEx
		ptrDraw->AddAnimation(L"AttackExEnd", 531, 21, false, 24.0f);//AttackEx終了
		ptrDraw->AddAnimation(L"AttackEnd", 484, 8, false, 24.0f);//Attack終了
		ptrDraw->AddAnimation(L"Walk_Gun", 570, 50, true, 24.0f);//歩き状態(銃所持)
		ptrDraw->AddAnimation(L"Shot_Gun", 646, 3, true, 24.0f);//銃を撃つ
		ptrDraw->AddAnimation(L"GunEnd", 624, 3, false, 24.0f);//銃を戻す
		ptrDraw->AddAnimation(L"Hit", 632, 7, false, 24.0f);//やられ


		ptrDraw->SetSamplerState(SamplerState::LinearWrap);
		ptrDraw->SetMeshToTransformMatrix(spanMat);
		ptrDraw->SetEmissive(Col4(1.0f, 1.0f, 1.0f, 1.0f));
		//ptrDraw->SetTextureResource(L"SpearmenTexture");

		//影をつける（シャドウマップを描画する）
		auto shadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		shadowPtr->SetMultiMeshResource(L"PlayerModelTestVer2.0");
		shadowPtr->SetMeshToTransformMatrix(spanMat);

		//コリジョン作成
		auto ptrColl = AddComponent<CollisionSphere>(); // コリジョンスフィアの方が壁にぶつかる判定に違和感がない
		ptrColl->SetAfterCollision(AfterCollision::Auto);
		ptrColl->SetDrawActive(false);

		//接地判定
		m_LandDetect->SetBindPos(Vec3(0, -1.8f, 0));
		m_LandDetect->GetComponent<CollisionSphere>()->SetMakedRadius(1.0f);
		m_LandDetect->GetComponent<Transform>()->SetScale(Vec3(2.0f, 2.0f, 2.0f));

		AddTag(L"Player");//Player用のタグ
		m_stateMachine = shared_ptr<PlayerStateMachine>(new PlayerStateMachine(GetThis<GameObject>()));

		//ジャスト回避時の演出用スプライト
		m_JastDodgeSprite = GetStage()->AddGameObject<Sprite>(L"SlowTex",Vec2(1280,800));
		m_JastDodgeSprite->SetColor(Col4(1.0f, 1.0f, 1.0f, 0.0f));
		
		//最初に流れる音SE
		PlaySnd(L"StartVoiceSE", 0.9f, 0);
	}

	void Player::OnUpdate()
	{
		//装備しているパーツは何があるのか確認する
		auto testParts = m_equippedParts;

		//もしポーズフラグがオンであればアップデート処理は出来なくなる
		if (m_PauseFlag)
		{
			return;
		}

		//親クラス処理
		Actor::OnUpdate();
		auto testVol = GetSEVol();
		//地面に立っているときは地面にめり込まないようにする
		if (m_isLand)
		{
			m_pos = GetPosition();	
			if (m_pos.y < 1.0f)
			{
				m_pos.y = 1.0f;
				SetPosition(m_pos);
				auto test = GetVelocity();
				test.y = 0.0f;
				SetVelocity(test);
			}

		}
		m_pos = GetPosition();

		auto cntl = App::GetApp()->GetInputDevice().GetControlerVec();
		auto angle = GetAngle();
		auto stage = GetStage();

		//リロード処理
		ReloadBullet(3.0f);

		//UIバーを更新する
		auto playerUI = stage->GetSharedGameObject<PlayerGaugeUI>(L"PlayerUI");//Playerバーを取得
		playerUI->SetPLHPSprite(m_HPCurrent);
		playerUI->SetPLSPSprite(m_SPCurrent);

		//回避クールタイム計算
		if (!m_dodgeFlag)
		{
			m_dodgeCoolTime += _delta;
			if (m_dodgeCoolTime >= m_maxDodgeCoolTime)
			{
				m_dodgeCoolTime = 0.0f;//リセット
				m_dodgeFlag = true;//回避できるようにする
			}
		}

		//ジャスト回避処理
		JastDodge(0.3f, 1.0f);


		//ステート処理
		m_stateMachine->Update(_delta);

		auto keybord = App::GetApp()->GetInputDevice().GetKeyState();

		// テストエフェクト // 後で消す //----------------------------
		if (keybord.m_bPressedKeyTbl[VK_RETURN])
		{
			//EfkPlaying(L"Beam", angle + XM_PIDIV2, Vec3(0, 1, 0));
			//EfkPlaying(L"Dodge", angle + XM_PI, Vec3(0, 1, 0));
			//EfkPlaying(L"Dash", angle + XM_PIDIV2, Vec3(0, 1, 0));
			//EfkPlaying(L"PathBullet", angle + XM_PIDIV2, Vec3(0, 1, 0));
			//EfkPlaying(L"Slap", angle + XM_PIDIV2, Vec3(0, 1, 0));
			//EfkPlaying(L"SpinAttack", GetAngle(), Vec3(0, 1, 0));
			//EfkPlaying(L"Charge", GetAngle(), Vec3(0, 1, 0));
			//EfkPlaying(L"Slash01Efk", GetAngle() + XM_PIDIV2, Vec3(0, 1, 0));
			//EfkPlaying(L"WaveEfk", GetAngle(), Vec3(0, 1, 0));

			//EfkPlaying(L"DamageEfk", GetAngle(), Vec3(0, 1, 0));
			//EfkPlaying(L"Damage", GetAngle() + XM_PIDIV2, Vec3(0, 1, 0));

			//EfkPlaying(L"SlashHitEfk", GetAngle() + XM_PI, Vec3(0, 1, 1));

		}
		//-------------------------------------------------------------
		

		//アニメーション再生
		GetComponent<PNTBoneModelDraw>()->UpdateAnimation(m_addTimeAnimation);
		//移動ですり抜けない処理
		auto objVec = stage->GetGameObjectVec();

		Vec3 hitPos; // 出力用：レイの交差地点(衝突点)
		TRIANGLE triangle; // レイが交差したポリゴンを構成する頂点の座標
		size_t triangleNumber; // レイが交差したポリゴンの番号
		float min = 9999999.9f;//Playerから見てカメラの障害となる距離の最小値

		Vec3 beforPos = GetPosition();//移動前
		Vec3 afterPos = (m_velocity * _delta) + GetPosition();//移動後
		//障害物になりえるオブジェクト達にカメラの機能を邪魔していないか見る
		for (auto obj : objVec)
		{
			auto obstacles = dynamic_pointer_cast<GameObject>(obj);//当たり判定の対象
			float hitLength = min;//Playerと障害物の距離の長さ

			//障害物になりえそうならカメラの表示に邪魔をしていないか確認をする
			if (obstacles)
			{
				//カメラの障害になりえるオブジェクトしかカメラを邪魔をしているか評価しない
				if (!obstacles->FindTag(L"CameraObstacles")) continue;

				auto ptrDraw = obstacles->GetComponent<SmBaseDraw>();
				ptrDraw->HitTestStaticMeshSegmentTriangles(beforPos, afterPos, hitPos, triangle, triangleNumber);
				Vec3 playerorObstaclesVec = hitPos - beforPos;
				hitLength = abs(playerorObstaclesVec.x) + abs(playerorObstaclesVec.y) + abs(playerorObstaclesVec.z);
			}

			//minよりhitLengthが短かったら位置更新する
			if (hitPos != Vec3(0.0f, 0.0f, 0.0f) && min > hitLength)
			{
				min = hitLength;
				hitPos.y = afterPos.y;//Y座標は変えないようにする
				afterPos = hitPos;
			}
		}
		GetComponent<Transform>()->SetPosition(afterPos);//移動処理

		DebugLog();//デバックログ
		//めり込み防止処理
		//ImmersedInCheck();
	}

	//ジャンプ処理
	void Player::Jump() {
		// 入力デバイス取得
		auto inputDevice = App::GetApp()->GetInputDevice();
		auto controller = inputDevice.GetControlerVec()[0];

		if (controller.wPressedButtons & XINPUT_GAMEPAD_A) {
			m_velocity.y = m_jumpPower;
			m_isLand = false;
			m_landDetectDisableTime = 1.0f;
		}
	}

	//ステート変更処理 引数に入れたステートに変更する
	void Player::ChangeState(wstring stateName)
	{
		m_stateMachine->ChangeState(stateName);
	}

	//ダッシュ処理
	void Player::Dash()
	{
		// 入力デバイス取得
		auto inputDevice = App::GetApp()->GetInputDevice();
		auto controller = inputDevice.GetControlerVec()[0];

		//回避した後にA長押しでダッシュ
		if (controller.wButtons & XINPUT_GAMEPAD_A && m_dodgeFlag) {
			m_dashFlag = true;
		}
		//ダッシュしている際Aボタンを離したらでダッシュ解除
		if (controller.wPressedButtons & XINPUT_GAMEPAD_A && m_dashFlag) {
			m_dashFlag = false;
		}
	}

	//回避のフラグを渡す処理
	void Player::Dodge()
	{
		// 入力デバイス取得
		auto inputDevice = App::GetApp()->GetInputDevice();
		auto controller = inputDevice.GetControlerVec()[0];

		if (controller.wPressedButtons & XINPUT_GAMEPAD_A) {
			m_dodgeFlag = true;//回避した
		}

	}

	//リロード処理
	void Player::ReloadBullet(float reloadTime)
	{
		if (m_bulletNum <= 0)
		{

			m_reloadTimeCount += _delta;
			//時間経過したら球を補充させる
			if (m_reloadTimeCount >= reloadTime)
			{
				m_SE = m_SEManager->Start(L"Reload", 0, 0.9f * m_SEVol);//リロードSE
				m_reloadTimeCount = 0.0f;//リセット
				m_bulletNum = m_bulletNumMax;
			}
		}
	}

	// アニメーションの更新
	void Player::UpdateAnimation(float addTime)
	{
		GetComponent<PNTBoneModelDraw>()->UpdateAnimation(addTime);
	}

	// ジャスト回避処理(一定時間Player以外がスローになる)
	void Player::JastDodge(float deltaScale, float slowTime)
	{
		// ジャスト回避できていなかったらこの処理はしない
		if (!m_jastDodge) return;

		// スロー処理
		auto waveStage = GetWaveStage(true);
		waveStage->SetDeltaScale(deltaScale);
		m_timeOfJastDodgeMax = slowTime;

		// 時間経過でスロー状態用のスプライトのアルファ値をプラス
		if (m_timeOfJastDodgeMax * 0.8f > m_timeOfJastDodgeCount)
		{
			m_alphaColorjastDodge += _delta * 1.5f;
		}
		if (m_timeOfJastDodgeMax * 0.8f <= m_timeOfJastDodgeCount)
		{
			m_alphaColorjastDodge -= _delta * 6.0f;
		}	
		
		// 一定以上のアルファ値にならないようにする
		if (m_alphaColorjastDodge >= 0.7f)
		{
			m_alphaColorjastDodge = 0.7f;
		}

		m_JastDodgeSprite->SetColor(Col4(1.0f, 1.0f, 1.0f, m_alphaColorjastDodge));

		// ジャスト回避中は無敵状態
		AddTag(L"invincible");

		//時間経過でジャスト回避処理は終了する
		m_timeOfJastDodgeCount += _delta;
		if (m_timeOfJastDodgeCount >= m_timeOfJastDodgeMax)
		{
			waveStage->SetDeltaScale(1.0f);

			// リセット
			m_jastDodge = false;
			m_timeOfJastDodgeCount = 0.0f;
			m_alphaColorjastDodge = 0.0f;
			m_JastDodgeSprite->SetColor(Col4(1.0f, 1.0f, 1.0f, 0.0f));

			// 無敵解除
			RemoveTag(L"invincible");
		}
	}

	//めり込み処理チェック処理
	void Player::ImmersedInCheck()
	{
		m_pos = GetPosition();

		float immersedInTimeMax = 0.1f;
		//めり込んで居なければカウントをリセット
		if (m_pos.y >= 1.0f)
		{
			m_immersedInTime = 0.0f;
		}

		//めり込んでいる時間をカウントする
		if (m_pos.y < 1.0f)
		{
			m_immersedInTime += _delta;
		}

		//一定時間めり込んでいたら強制的にめり込まないようにする
		if (m_immersedInTime >= immersedInTimeMax)
		{
			m_pos.y = 1.0f;
			SetPosition(m_pos);
			m_velocity.y = 0.0f;

			//リセット
			m_immersedInTime = 0.0f;
		}

	}

	// プレイヤーの移動処理
	void Player::PlayerMove(int playerState)
	{
		Vec3 move = GetMoveVector(playerState);
		//パーツのステータスによって元のスピードに追加される
		Vec3 addmove = move * m_equippedParts.addSpeed;
		m_accel = (move + addmove) * m_baseAccel;
		m_velocity += (move + addmove);
		//SetVelocity((move + addmove));
		//プレイヤーの向き
		if (move.length() != 0)
		{
			//m_Angleの方向に回転するようにする
			//m_angle = atan2(move.z, move.x);
			m_rot.y = -m_angle;

			m_trans->SetRotation(m_rot);
		}
		SpeedLimit(move.length());
	}

	//移動ベクトルの計算処理
	Vec3 Player::GetMoveVector(int playerState)
	{
		// 入力デバイス取得
		InputDevice inputDevice = App::GetApp()->GetInputDevice();
		m_controller = inputDevice.GetControlerVec()[0];
		m_stickL = Vec3(m_controller.fThumbLX, 0, m_controller.fThumbLY);
		Vec3 totalVec;

		if (playerState == PlayerState_Walk || playerState == PlayerState_Dash)//徒歩、ダッシュ処理
		{
			//スティックの向きと距離
			float moveSize = m_stickL.length();

			//進む方向を決める
			float totalAngle = MoveAngle(m_stickL);
			totalVec = Vec3(cos(totalAngle), 0, sin(totalAngle));
			totalVec.normalize();

			//ステートによって移動ベクトルの大きさを変える
			switch (playerState)
			{
			case PlayerState_Walk:
				totalVec *= (moveSize * 2.0f);
				break;
			case PlayerState_Dash:
				totalVec *= (moveSize * 2.75f);
				break;
			default:
				break;
			}

		}
		// 回避処理
		if (playerState == PlayerState_Dodge)
		{
			// 回避処理
			m_dodgeTime += _delta * 3.0f;

			// 回避のスピード曲線
			auto cosCurve = (cos(m_dodgeTime) + 1.0f) / 2;

			// 二次関数的な動きで回避行動をする
			// 今は向いている方向に前方回避をする
			float dodgeSpeed = 400.0f * 2.5f;
			totalVec.x = (cos(m_angle) * (dodgeSpeed * (cosCurve)) * _delta);
			totalVec.z = (sin(m_angle) * (dodgeSpeed * (cosCurve)) * _delta);

			// 回避が終わったらダッシュ処理ができる
			if (m_dodgeTime > XM_PI / 3)
			{
				// Aボタンを押し続ける限り走るそうでなければダッシュ回避処理をしない
				if (m_controller.bConnected && m_controller.wButtons & XINPUT_GAMEPAD_A)
				{
					m_dashFlag = true;
					m_dodgeTime = 0.0f;
					m_endDodgeFlag = false;// 回避処理終了
				}
				else
				{
					m_dodgeTime = 0.0f;
					m_endDodgeFlag = false;// 回避処理終了
				}

			}
		}
		// 攻撃ステート時の移動
		if (playerState == PlayerState_Attack1)
		{
			// 移動スピード
			float speed = 0.7f;

			// 前に進む
			totalVec.z = sin(m_angle) * speed;
			totalVec.x = cos(m_angle) * speed;
		}
		// 攻撃ステート時の移動
		if (playerState == PlayerState_Attack2)
		{
			// 移動スピード
			float speed = 0.7f;

			// 前に進む
			totalVec.z = sin(m_angle) * speed;
			totalVec.x = cos(m_angle) * speed;
		}
		// 攻撃ステート時の移動
		if (playerState == PlayerState_Attack3)
		{
			// 移動スピード
			float speed = 0.7f;

			// 前に進む
			totalVec.z = sin(m_angle) * speed;
			totalVec.x = cos(m_angle) * speed;
		}
		return totalVec;
	}

	// 動く方向を決める処理
	float Player::MoveAngle(Vec3 stickL)
	{
		auto trans = GetTransform();
		auto camera = OnGetDrawCamera();

		// スティックの向きと距離
		float moveSize = stickL.length();
		float moveAngle = atan2(-stickL.x, stickL.z);

		// 自分の位置とカメラの位置からカメラの角度を算出
		auto front = trans->GetPosition() - camera->GetEye();
		front.y = 0;
		front.normalize();
		float frontAngle = atan2(front.z, front.x);

		// 合計の角度
		float totalAngle = frontAngle + moveAngle;
		if (stickL != Vec3(0.0f, 0.0f, 0.0f))
		{
			m_angle = totalAngle;// 今向いている方向を渡す
		}
		return totalAngle;
	}

	////エフェクトを出す処理
	//void Player::AddEffect(int addEffect)
	//{
	//	switch (addEffect)
	//	{
	//	case PlayerEffect_Attack1:
	//		EfkPlaying(L"Sword", GetAngle() + XM_PI, Vec3(0, 1, 0));
	//		break;
	//	case PlayerEffect_Attack2:
	//		EfkPlaying(L"Sword", GetAngle() + XM_PI, Vec3(0, 1, 0), Col4(0.22f, 1.0f, 0.48f, 1.0f));
	//		break;
	//	case PlayerEffect_Attack3:
	//		EfkPlaying(L"Sword", GetAngle() + XM_PI, Vec3(0, 1, 0), Col4(1.0f, 0.94f, 0.45f, 1.0f));
	//		break;
	//	case PlayerEffect_AttackEx:
	//		EfkPlaying(L"Sword", GetAngle() + XM_PI, Vec3(0, 1, 0), Col4(0.22f, 1.0f, 0.48f, 1.0f));
	//		EfkPlaying(L"Sword", GetAngle(), Vec3(0, 1, 0));
	//		break;
	//	case PlayerEffect_Beam:
	//		EfkPlaying(L"Laser", GetAngle() + XM_PIDIV2, Vec3(0, 1, 0));
	//		break;
	//	default:
	//		break;
	//	}
	//}

	//HPのゲッター
	int Player::GetHP()
	{
		return m_HPCurrent;
	}
	//HPのゲッター
	int Player::GetMaxHP()
	{
		return m_HPMax;
	}
	//HPのゲッター
	int Player::GetSP()
	{
		return m_SPCurrent;
	}
	//HPのセッター
	void Player::SetHP(int setHP)
	{
		m_HPCurrent = setHP;
	}
	//SPのセッター
	void Player::SetSP(int setSP)
	{
		m_SPCurrent = setSP;
	}
	//HPのゲッター
	int Player::GetMaxSP()
	{
		return m_SPMax;
	}

	//回避フラグのセッター
	void Player::SetDodgeFlag(bool setDodgeFlag)
	{
		m_dodgeFlag = setDodgeFlag;
	}
	//回避フラグのゲッター
	bool Player::GetDodgeFlag()
	{
		return m_dodgeFlag;
	}

	//回避終了フラグのセッター
	void Player::SetEndDodgeFlag(bool setEndDodgeFlag)
	{
		m_endDodgeFlag = setEndDodgeFlag;
	}
	//回避終了フラグのゲッター
	bool Player::GetEndDodgeFlag()
	{
		return m_endDodgeFlag;
	}

	//装備しているパーツのセッタ
	void Player::SetEquippedParts(PartsStatus parts)
	{
		m_equippedParts = parts;

		//パーツ更新前の追加HPを確認
		auto beforAddHP = m_HPMax - m_originallyHPMax;

		//最大HP更新
		m_HPMax = m_originallyHPMax + m_equippedParts.addHP;
		
		//追加HPが前よりも多かったらそれに合わせて現在HPを増やす
		if (beforAddHP < m_equippedParts.addHP)
		{
			auto plusHP = m_equippedParts.addHP - beforAddHP;	
			m_HPCurrent += plusHP;
		}
	}
	//装備しているパーツのゲッタ
	PartsStatus Player::GetEquippedParts()
	{
		return m_equippedParts;
	}

	//衝突判定
	void Player::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		//無敵判定時ならダメージ判定は受けない
		if (!FindTag(L"invincible"))
		{
			DetectBeingAttacked(Other);
		}
		//攻撃をうまく回避出来たら一定時間スローモーションになる
		if (FindTag(L"Dodge"))
		{
			auto attack = dynamic_pointer_cast<AttackCollision>(Other);

			//attackが入っていなければreturnする
			if (!attack) return;
			if (attack->GetHitInfo().Type == AttackType::Enemy)
			{
				m_jastDodge = true;
				// SE再生
				PlaySnd(L"JastDodgeSE", 2.0f, 0);
				m_timeOfJastDodgeCount = 0.0f;
			}
		}

		////コリジョンが地面を接触してしまったら少し弾ませる
		//if (FindTag(L"Ground"))
		//{
		//	m_pos.y = 2.0f;
		//	SetPosition(m_pos);
		//}
	}

	//ダメージを受けたらヒットステートに移動する
	void Player::OnDamaged()
	{//現在はHitステートを作ってノックバック処理の作成をする
		m_stateMachine->ChangeState(L"Hit");
	}

	////ノックバックの移動処理
	void Player::hitbackMove()
	{
		//エラー対策
	}

	//デバック用文字列表示関数
	void Player::DebugLog()
	{
		auto waveStage = GetWaveStage(false);
		float deltaScale = 0.0f;
		if (waveStage)
		{
			deltaScale = GetWaveStage(false)->GetDeltaScale();
		}

		////デバック用
		wstringstream wss(L"");
		auto scene = App::GetApp()->GetScene<Scene>();
		auto quat = GetComponent<Transform>()->GetQuaternion();
		auto efkMana = EffectManager::Instance().GetTotalInstanceCount();
		auto testVel = (m_velocity * _delta);
		auto pos = GetPosition();
		wss /* << L"デバッグ用文字列 "*/
			<< L"\n Pos.x " << m_pos.x << " Pos.y " << pos.y << " Pos.z " << m_pos.z
			<< L"\n Vel.x " << testVel.x << " Vel.y " << testVel.y << " Vel.z " << testVel.z
			<< L"\n 回避フラグ：  " << m_dodgeFlag
			<< L"\n Vel.x " << m_velocity.x << L"\ Vel.y " << m_velocity.y << L" Vel.z " << m_velocity.z
			<< endl << "onLand: " << m_isLand << " LandDetect: " << m_LandDetect->GetLand()
			<< L"\nQuat : (" << L"\n" << quat.x << L"\n" << quat.y << L"\n" << quat.z << L"\n" << quat.w
			<<L"\nDeltaTime" << _delta
			<<L"\nDeltaScale" << deltaScale
			<< L"\nAngle : " << GetAngle()
			<< L"\ninstance : " << efkMana
			//<< L"\nLandDetect : "<<m_LandDetect
			<< endl;

		scene->SetDebugString(wss.str());
	}



	//球のクラス
	void Bullet::OnCreate()
	{
		Actor::OnCreate();

		//Transform設定
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

		////テストで撃つ場所にキューブを生成してみる
		//GetStage()->AddGameObject<Cube>(m_pos, m_rot, Vec3(1.0f, 1.0f, 1.0f));

		////ドローメッシュの設定
		//auto ptrDraw = AddComponent<PNTStaticDraw>();
		//ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		//ptrDraw->SetDiffuse(Col4(0.24f, 0.7f, 0.43f, 1.0f));
		//ptrDraw->SetOwnShadowActive(false);//影は消す
		//ptrDraw->SetDrawActive(true);
		//ptrDraw->SetEmissive(Col4(0.24f, 0.7f, 0.43f, 1.0f)); // 自己発光カラー（ライティングによる陰影を消す効果がある）
		//ptrDraw->SetOwnShadowActive(true); // 影の映り込みを反映させる

		//当たり判定
		auto ptrColl = AddComponent<CollisionSphere>(); // コリジョンスフィアの方が壁にぶつかる判定に違和感がない
		ptrColl->SetAfterCollision(AfterCollision::None);
		ptrColl->SetDrawActive(false);

		//原点オブジェクトが消えていたら自分も消える
		auto originLock = m_originObj.lock();
		if (!originLock)
		{
			GetStage()->RemoveGameObject<Bullet>(GetThis<Bullet>());
			return;
		}
		auto cameraManager = GetStage()->GetSharedGameObject<CameraManager>(L"CameraManager");

		if (originLock->FindTag(L"Player"))
		{
			//弾がカメラの注視点の方向に進むように角度を計算する

			auto cameraAt = cameraManager->GetCameraAt();
			//float a = originPos.length();
			Vec2 XZVec = Vec2(cameraAt.x - m_pos.x, cameraAt.z - m_pos.z);//XZ方面の距離ベクトル
			float lengthY = cameraAt.y - m_pos.y;
			float lengthXZ = XZVec.length();

			//X軸とY軸のカメラの角度を決める
			m_angleXAxis = -atan2f(lengthY, lengthXZ) + XMConvertToRadians(90.0f);
			m_AngleYAxis = -(cameraManager->GetAngle(L"Y")) - XMConvertToRadians(180.0f);
		}
		else if (originLock->FindTag(L"Enemy"))
		{
			auto playerAngleVec = originLock->GetComponent<Transform>()->GetForward();
			m_AngleYAxis = atan2f(playerAngleVec.z, -playerAngleVec.x);
			m_AngleYAxis -= XMConvertToRadians(90.0f);
		}

		auto test = XMConvertToDegrees(m_AngleYAxis);

		//攻撃判定の定義
		auto tmp = GetAttackPtr()->GetHitInfo();
		auto player = GetStage()->GetSharedGameObject<Player>(L"Player");

		switch (m_actorType)
		{
		case ActorName_Player:
			tmp.Type = AttackType::Player;//攻撃のタイプはプレイヤー	
			tmp.Damage = 5 + player->GetEquippedParts().addAttack;//ダメージ
			tmp.HitEffect = L"GunHitEfk";

			break;
		case ActorName_Enemy:
			tmp.Type = AttackType::Enemy;//攻撃のタイプは敵
			tmp.Damage = 5;//ダメージ
			tmp.HitEffect = L"EnemyLongHitEfk";
			break;
		default:
			break;
		}

		tmp.StunDamage = 1;
		tmp.HitOnce = true;//一回しかヒットしないか
		tmp.HitVel_Stand = Vec3(-5, 5, 0);//ヒットバック距離
		tmp.HitTime_Stand = 0.1f;//のけぞり時間
		tmp.InvincibleOnHit = true;

		//tmp.PauseTime = 5.0f;
		//tmp.ForceRecover = true;
		GetAttackPtr()->SetPos(Vec3(0, 0, 0));
		auto AttackPtr = GetAttackPtr();
		AttackPtr->SetCollScale(1.0f);

		DefAttack(5.0f, tmp);

		// エフェクト生成
		//AddEffect(PlayerEffect_Shot);
		m_gunLine = EfkPlaying(L"GunLine", m_AngleYAxis + XMConvertToRadians(90.0f), Vec3(cos(m_angleXAxis) * sin(m_AngleYAxis),
			cos(m_AngleYAxis),
			sin(m_angleXAxis) * sin(m_AngleYAxis)),
			Col4(0.22f, 1.0f, 0.48f, 1.0f));

		//クォータニオン回転
		Quat Qt = Quat(0.0f, sin((m_AngleYAxis + XMConvertToRadians(90.0f)) / 2), 0.0f, cos((m_AngleYAxis + XMConvertToRadians(90.0f)) / 2));
		//Qt *= Quat(sin((m_angleXAxis + XMConvertToRadians(90.0f)) / 2), 0.0f, 0.0f, cos((m_angleXAxis + XMConvertToRadians(90.0f)) / 2));
		//EffectManager::Instance().SetRotationFromQuaternion(m_gunLine, Quat(0.0f, sin((m_AngleYAxis+XMConvertToRadians(90.0f)) / 2), 0.0f, cos((m_AngleYAxis + XMConvertToRadians(90.0f)) / 2)));
		EffectManager::Instance().SetRotationFromQuaternion(m_gunLine,Qt);

	}

	void Bullet::OnUpdate()
	{
		//もしポーズフラグがオンであればアップデート処理は出来なくなる
		if (m_PauseFlag)
		{
			return;
		}

		//エフェクト(弾を追いかけさせる)
		EffectManager::Instance().SetPosition(m_gunLine, GetPosition());

		Actor::OnUpdate();

		auto tmp = GetAttackPtr()->GetHitInfo();

		auto delta = App::GetApp()->GetElapsedTime();
		//移動距離を計算する
		Vec3 moveVec;

		//原点オブジェクトを受け取る
		auto originLock = m_originObj.lock();

		//誰が撃っているかによってどのように弾が発射されるか決まる
		if (originLock->FindTag(L"Player"))
		{
			moveVec.x = m_speed * cos(m_AngleYAxis) * sin(m_angleXAxis) * delta;
			moveVec.y = m_speed * cos(m_angleXAxis) * delta;
			moveVec.z = m_speed * sin(-m_AngleYAxis) * sin(m_angleXAxis) * delta;
		}
		if (originLock->FindTag(L"Enemy"))
		{
			moveVec.x = m_speed * cos(m_AngleYAxis) * delta;
			moveVec.z = m_speed * sin(-m_AngleYAxis) * delta;
		}

		//プレイヤーの位置を取得して移動する
		auto pos = m_trans->GetPosition();
		m_trans->SetPosition(pos + moveVec);

		m_canMoveDistance -= abs(moveVec.x) + abs(moveVec.z) + abs(moveVec.y);
		//一定距離移動したら消える
		if (m_canMoveDistance <= 0.0f)
		{
			GetStage()->RemoveGameObject<Bullet>(GetThis<Bullet>());
			auto hitInfo = originLock->GetAttackPtr()->GetHitInfo();
			GetStage()->RemoveGameObject<LandDetect>(m_LandDetect);
			GetStage()->RemoveGameObject<AttackCollision>(m_AttackCol);
		}
	}

	//削除時処理
	void Bullet::OnDestroy()
	{
		EffectManager::Instance().StopEffect(m_gunLine);
	}

	//当たり判定
	void Bullet::OnCollisionEnter(shared_ptr<GameObject>& obj)
	{
		//auto a = 0;
		////敵や障害物に弾が当たったら消える
		//auto enemy = dynamic_pointer_cast<EnemyBase>(obj);
		//if (obj->FindTag(L"Enemy") || obj->FindTag(L"Terrain"))
		//{
		//	GetStage()->RemoveGameObject<Bullet>(GetThis<Bullet>());
		//	GetStage()->RemoveGameObject<LandDetect>(m_LandDetect);
		//	GetStage()->RemoveGameObject<AttackCollision>(m_AttackCol);
		//	EffectManager::Instance().StopEffect(m_gunLine);
		//}
	}

	void Bullet::OnCollisionExcute(shared_ptr<GameObject>& obj)
	{
		// 発射しているオブジェクトが誰か確認する
		auto player = dynamic_pointer_cast<Player>(m_originObj.lock());
		auto enemy = dynamic_pointer_cast<EnemyZako>(m_originObj.lock());

		if (player)
		{
			if (obj->FindTag(L"Enemy") || obj->FindTag(L"Terrain"))
			{
				GetStage()->RemoveGameObject<Bullet>(GetThis<Bullet>());
				GetStage()->RemoveGameObject<LandDetect>(m_LandDetect);
				GetStage()->RemoveGameObject<AttackCollision>(m_AttackCol);
				EffectManager::Instance().StopEffect(m_gunLine);
			}
		}
		if (enemy)
		{
			if (obj->FindTag(L"Player") || obj->FindTag(L"Terrain"))
			{
				GetStage()->RemoveGameObject<Bullet>(GetThis<Bullet>());
				GetStage()->RemoveGameObject<LandDetect>(m_LandDetect);
				GetStage()->RemoveGameObject<AttackCollision>(m_AttackCol);
				EffectManager::Instance().StopEffect(m_gunLine);
			}
		}

	}

	//攻撃をしているのは誰か決める処理
	void Bullet::SetAttackActor(int actorName)
	{
		auto tmp = GetAttackPtr()->GetHitInfo();

		switch (actorName)
		{
		case ActorName_Player:
			tmp.Type = AttackType::Player;//攻撃のタイプはプレイヤー
			break;
		case ActorName_Enemy:
			tmp.Type = AttackType::Enemy;//攻撃のタイプは敵
			break;
		default:
			break;
		}

		GetAttackPtr()->SetHitInfo(tmp);
	}

}
//end basecross

