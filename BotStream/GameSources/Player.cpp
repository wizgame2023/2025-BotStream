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
		m_dodgeTime(0.0f)
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
			Vec3(0.0f, -1.2f, 0.0f)
		);

		//ドローメッシュの設定
		auto ptrDraw = GetComponent<PNTBoneModelDraw>();
		ptrDraw->SetMultiMeshResource(L"PlayerModelTestVer2.0");//仮のメッシュ
		ptrDraw->AddAnimation(L"Idle", 0, 1, true, 60.0f);//立ち状態
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
		//ptrDraw->SetTextureResource(L"SpearmenTexture");

		//コリジョン作成
		auto ptrColl = AddComponent<CollisionSphere>();//コリジョンスフィアの方が壁にぶつかる判定に違和感がない
		ptrColl->SetAfterCollision(AfterCollision::Auto);
		ptrColl->SetDrawActive(false);

		//接地判定
		m_LandDetect->SetBindPos(Vec3(0, -2.4f, 0));
		m_LandDetect->GetComponent<Transform>()->SetScale(Vec3(5.0f, 5.0f, 5.0f));

		AddTag(L"Player");//Player用のタグ
		m_stateMachine = shared_ptr<PlayerStateMachine>(new PlayerStateMachine(GetThis<GameObject>()));

		//UI追加
		m_playerBulletUI = GetStage()->AddGameObject<PlayerBulletUI>(GetThis<Player>(), Vec2(165.0f, -250.0f), m_bulletNum);//現在の球数を出すUI

		//auto stage = GetStage();
		//auto playerButton = stage->GetSharedGameObject<PlayerButtonUI>(L"PlayerButton");

		//最初に流れる音
		m_SEManager->Start(L"StartVoiceSE", 0, 0.9f);


		auto trans = GetComponent<Transform>();
		m_EfkPos = trans->GetPosition();

		//m_testEffect = EffectManager::Instance().PlayEffect(L"Dash", m_EfkPos);
	}

	void Player::OnUpdate()
	{
		//auto num = EffectManager::Instance().PlayEffect(L"ArmorBreak", m_pos);
		//num;

		//もしポーズフラグがオンであればアップデート処理は出来なくなる
		if (m_poseFlag)
		{
			return;
		}

		//親クラス処理
		Actor::OnUpdate();

		EffectManager::Instance().SetPosition(m_testEffect, m_EfkPos);


		////着地判定(無効化時間中ならそれを減算する)
		//OnLanding();

		////物理的な処理
		//if (m_doPhysics) {
		//	if (!m_isLand) {
		//		Gravity();
		//	}
		//	else {
		//		Friction();
		//	}
		//}

		//地面に立っているときは地面にめり込まないようにする
		if (m_isLand)
		{
			m_pos = GetPosition();
			if (m_pos.y < 1.0f)
			{
				m_pos.y = 1.0f;
				SetPosition(m_pos);
			}
		}

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


		//ステート処理
		m_stateMachine->Update();

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
			//EfkPlaying(L"DamageEfk", GetAngle() + XM_PIDIV2, Vec3(0, 1, 0));

			//EfkPlaying(L"Sword", GetAngle() + XM_PI, Vec3(0, 1, 1));

		}
		//-------------------------------------------------------------



		//if (cntl[0].wPressedButtons & XINPUT_GAMEPAD_B)
		//{
		//	EfkPlaying(L"Laser", angle + XM_PIDIV2, Vec3(0, 1, 0));
		//}

		//if (cntl[0].wPressedButtons & XINPUT_GAMEPAD_X)
		//{
		//	EfkPlaying(L"Sword", angle + XM_PI, Vec3(0, 1, 0));
		//}

		//デバック用文字列
		//DebugLog();

		//アニメーション再生
		GetComponent<PNTBoneModelDraw>()->UpdateAnimation(m_addTimeAnimation);
		//GetComponent<Transform>()->SetPosition((m_velocity * _delta) + GetPosition());//移動処理

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

				auto ptrDraw = obstacles->GetComponent<PNTStaticDraw>();//Bc対応にしない
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
				m_SE = m_SEManager->Start(L"Reload", 0, 0.9f);//リロードSE
				m_reloadTimeCount = 0.0f;//リセット
				m_bulletNum = m_bulletNumMax;
			}
		}
	}

	//アニメーションの更新
	void Player::UpdateAnimation(float addTime)
	{
		GetComponent<PNTBoneModelDraw>()->UpdateAnimation(addTime);
	}

	//プレイヤーの移動処理
	void Player::PlayerMove(int playerState)
	{
		Vec3 move = GetMoveVector(playerState);
		m_accel = move * m_baseAccel;
		m_velocity += move;

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
			//auto trans = GetTransform();
			//auto camera = OnGetDrawCamera();

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
				totalVec *= moveSize * 2.0f;
				break;
			case PlayerState_Dash:
				totalVec *= moveSize * 2.75f;
				break;
			default:
				break;
			}

		}
		//回避処理
		if (playerState == PlayerState_Dodge)
		{
			//回避処理
			float timeSpeed = 80.0f;
			m_dodgeTime += XMConvertToRadians(_delta * timeSpeed);

			//二次関数的な動きで回避行動をする
			//今は向いている方向に前方回避をする
			float dodge = 8.0f * 2.5f;
			totalVec.x = cos(m_angle) * (dodge * abs(cos(m_dodgeTime)));
			totalVec.z = sin(m_angle) * (dodge * abs(cos(m_dodgeTime)));

			//回避が終わったらダッシュ処理ができる
			if (m_dodgeTime > XMConvertToRadians(20.0f))
			{
				//Aボタンを押し続ける限り走るそうでなければダッシュ回避処理をしない
				if (m_controller.bConnected && m_controller.wButtons & XINPUT_GAMEPAD_A)
				{
					m_dashFlag = true;
					m_dodgeTime = 0.0f;
					m_endDodgeFlag = false;//回避処理終了
				}
				else
				{
					m_dodgeTime = 0.0f;
					m_endDodgeFlag = false;//回避処理終了
				}

			}
		}
		//攻撃ステート時の移動
		if (playerState == PlayerState_Attack1)
		{
			//移動スピード
			float speed = 0.7f;

			//前に進む
			totalVec.z = sin(m_angle) * speed;
			totalVec.x = cos(m_angle) * speed;
		}
		//攻撃ステート時の移動
		if (playerState == PlayerState_Attack2)
		{
			//移動スピード
			float speed = 0.7f;

			//前に進む
			totalVec.z = sin(m_angle) * speed;
			totalVec.x = cos(m_angle) * speed;
		}
		//攻撃ステート時の移動
		if (playerState == PlayerState_Attack3)
		{
			//移動スピード
			float speed = 0.7f;

			//前に進む
			totalVec.z = sin(m_angle) * speed;
			totalVec.x = cos(m_angle) * speed;
		}
		return totalVec;
	}

	//動く方向を決める処理
	float Player::MoveAngle(Vec3 stickL)
	{
		auto trans = GetTransform();
		auto camera = OnGetDrawCamera();

		//スティックの向きと距離
		float moveSize = stickL.length();
		float moveAngle = atan2(-stickL.x, stickL.z);

		//自分の位置とカメラの位置からカメラの角度を算出
		auto front = trans->GetPosition() - camera->GetEye();
		front.y = 0;
		front.normalize();
		float frontAngle = atan2(front.z, front.x);

		//合計の角度
		float totalAngle = frontAngle + moveAngle;
		if (stickL != Vec3(0.0f, 0.0f, 0.0f))
		{
			m_angle = totalAngle;//今向いている方向を渡す
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

	//衝突判定
	void Player::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		//無敵判定時ならダメージ判定は受けない
		if (!FindTag(L"invincible"))
		{
			DetectBeingAttacked(Other);
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
		////デバック用
		wstringstream wss(L"");
		auto scene = App::GetApp()->GetScene<Scene>();
		auto quat = GetComponent<Transform>()->GetQuaternion();
		auto pos = GetPosition();
		wss /* << L"デバッグ用文字列 "*/
			<< L"\n Pos.x " << m_pos.x << " Pos.y " << pos.y <<" Pos.z " << m_pos.z
			<< L"\n 回避フラグ：  " << m_dodgeFlag
			<< L"\n Vel.x " << m_velocity.x << L"\ Vel.y " << m_velocity.y << L" Vel.z " << m_velocity.z
			<< endl << "onLand: " << m_isLand << " LandDetect: " << m_LandDetect->GetLand()
			<< L"\nQuat : (" << L"\n" << quat.x << L"\n" << quat.y << L"\n" << quat.z << L"\n" << quat.w
			<< L"\nAngle : " << GetAngle() << endl;

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

		//ドローメッシュの設定
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDraw->SetDiffuse(Col4(0.24f, 0.7f, 0.43f, 1.0f));
		ptrDraw->SetOwnShadowActive(false);//影は消す
		ptrDraw->SetDrawActive(true);
		ptrDraw->SetEmissive(Col4(0.24f, 0.7f, 0.43f, 1.0f)); // 自己発光カラー（ライティングによる陰影を消す効果がある）
		ptrDraw->SetOwnShadowActive(true); // 影の映り込みを反映させる

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
		switch (m_actorType)
		{
		case ActorName_Player:
			tmp.Type = AttackType::Player;//攻撃のタイプはプレイヤー	
			tmp.Damage = 8;//ダメージ

			break;
		case ActorName_Enemy:
			tmp.Type = AttackType::Enemy;//攻撃のタイプは敵
			tmp.Damage = 5;//ダメージ
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

	}

	void Bullet::OnUpdate()
	{
		//もしポーズフラグがオンであればアップデート処理は出来なくなる
		if (m_poseFlag)
		{
			return;
		}

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

	//当たり判定
	void Bullet::OnCollisionEnter(shared_ptr<GameObject>& obj)
	{
		auto a = 0;
		//敵や障害物に弾が当たったら消える
		auto enemy = dynamic_pointer_cast<EnemyBase>(obj);
		if (obj->FindTag(L"Enemy") || obj->FindTag(L"Terrain"))
		{
			GetStage()->RemoveGameObject<Bullet>(GetThis<Bullet>());
			GetStage()->RemoveGameObject<LandDetect>(m_LandDetect);
			GetStage()->RemoveGameObject<AttackCollision>(m_AttackCol);
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


	//雑魚敵の処理をいったんここに書きますマージ終わったらEnemy.cpp.hに戻す
	void EnemyZako::OnCreate()
	{
		Actor::OnCreate();

		m_HPMax = 40.0f;
		m_HPCurrent = m_HPMax;

		//Transform設定
		m_trans = GetComponent<Transform>();
		SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(1.0f / 5, 1.0f / 5, 1.0f / 5),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		//ドローメッシュの設定
		auto ptrDraw = GetComponent<PNTBoneModelDraw>();

		//攻撃タイプによって見た目が変わる
		if (m_AttackType == Zako_Long)
		{
			ptrDraw->SetMeshResource(L"Enemy_A");
		}
		if (m_AttackType == Zako_Melee)
		{
			ptrDraw->SetMeshResource(L"Enemy_C");
		}
		ptrDraw->SetDiffuse(Col4(0.5f));
		ptrDraw->SetSamplerState(SamplerState::LinearWrap);
		ptrDraw->SetMeshToTransformMatrix(spanMat);

		//アニメーション追加(攻撃タイプによって追加アニメーションが変わる)
		ptrDraw->AddAnimation(L"Stand", 0, 1, 24.0f);
		ptrDraw->AddAnimation(L"Walk", 0, 224, 24.0f);
		ptrDraw->AddAnimation(L"Shot", 225, 136, false, 24.0f);
		ptrDraw->AddAnimation(L"Down", 362, 62, false, 24.0f);
		ptrDraw->AddAnimation(L"Hit", 350, 11, false, 24.0f);
		ptrDraw->AddAnimation(L"Stan", 320, 19, false, 24.0f);
		if (m_AttackType == Zako_Melee)
		{
			ptrDraw->AddAnimation(L"Melee_Jamp", 625, 74, false, 24.0f);
			ptrDraw->AddAnimation(L"Melee_Scratch", 700, 22, false, 24.0f);
		}

		//コリジョン作成
		auto ptrColl = AddComponent<CollisionSphere>();//コリジョンスフィアの方が壁にぶつかる判定に違和感がない
		ptrColl->SetAfterCollision(AfterCollision::Auto);
		ptrColl->SetDrawActive(false);//デバック用

		AddTag(L"Enemy");
		AddTag(L"EnemyZako");

		m_player = GetStage()->GetSharedGameObject<Player>(L"Player");

		//接地判定の設定
		m_LandDetect->SetBindPos(Vec3(0, -2.5f, 0));
		m_LandDetect->GetComponent<Transform>()->SetScale(Vec3(2.0f, 2.0f, 2.0f));
		//m_LandDetect->SetCollScale(3.0f);

		//ステートマシン生成
		m_state = shared_ptr<EnemyZakoStateMachine>(new EnemyZakoStateMachine(GetThis<GameObject>()));

		//頭上にHPバーを表示させる
		m_HPFrame = GetStage()->AddGameObject<BillBoard>(GetThis<GameObject>(), L"BossGaugeFrame", 4, 5.0f, Vec3(2.0f, 0.5f, 5.0f));
		m_HPBer = GetStage()->AddGameObject<BillBoardGauge>(GetThis<GameObject>(), L"BossHPMater", 3, 5.0f, Vec3(2.0f, 0.5f, 5.0f));
		m_HPBer->SetPercent(1.0f);
	}

	void EnemyZako::OnUpdate()
	{
		//もしポーズフラグがオンであればアップデート処理は出来なくなる
		if (m_poseFlag)
		{
			return;
		}
		//もし、使わない状態から使う状態に変更された時メンバ変数の初期化をする	
		if (!m_beforUsed)
		{
			if (m_used)
			{
				m_HPCurrent = m_HPMax;
				m_attackFlag = false;
				m_timeCountOfAttackCool = 3.0f;
				//初期ステートに戻す
				ChangeState(L"Stand");
			}
		}	
		//現在の使用状況と見比べて変わっていないか見る
		m_beforUsed = m_used;

		EnemyBase::OnUpdate();

		//着地判定(無効化時間中ならそれを減算する)
		OnLanding();
		//HPバーの処理
		UpdateHPBer();
		//攻撃のクールタイム
		TimeOfAttackCool();

		//HPがゼロになったら消えるための準備をする
		if (m_HPCurrent <= 0)
		{
			RemoveTag(L"LockOnCan");
			RemoveTag(L"LockOnTarget");

			//m_used = false;
		}

		//アニメーション更新
		GetComponent<PNTBoneModelDraw>()->UpdateAnimation(m_addTimeAnimation);
		//位置更新
		GetComponent<Transform>()->SetPosition((m_velocity * _delta) + GetComponent<Transform>()->GetPosition());
	}

	//HPバーの処理
	void EnemyZako::UpdateHPBer()
	{
		//ビルボードの処理 自分が使用されているかどうかでビルボードが出るかでないか決める
		if (!m_used)
		{
			m_HPFrame->SetScale(Vec3(0.0f));
			m_HPBer->SetScale(Vec3(0.0f));
		}
		if (m_used)
		{
			m_HPFrame->SetScale(Vec3(2.0f, 0.5f, 5.0f));
			m_HPBer->SetScale(Vec3(2.0f, 0.5f, 5.0f));

			//HPの割合によってゲージが減る
			float HPPercent = (float)m_HPCurrent / (float)m_HPMax;
			m_HPBer->SetPercent(HPPercent);

		}


	}

	//攻撃のクールタイム
	void EnemyZako::TimeOfAttackCool()
	{
		//攻撃のクールタイム
		if (!m_attackFlag)
		{
			m_timeCountOfAttackCool += _delta;
			//クールタイム過ぎたら攻撃できるようになる
			if (m_timeCountOfAttackCool >= m_timeOfAttackCool)
			{
				m_timeCountOfAttackCool = 0.0f;//リセット
				m_attackFlag = true;
			}
		}
	}

	//ダメージを受けた際の処理
	void EnemyZako::HitBackStandBehavior()
	{
		//ダメージを受けた後のHPによってステートの遷移を変える
		m_hitbacktime -= _delta;
		if (m_hitbacktime <= 0) {
			if (m_HPCurrent <= 0)
			{
				ChangeState(L"Die");
			}
			else
			{
				ChangeState(L"Stand");
			}
		}
	}

	//コリジョン判定
	void EnemyZako::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		DetectBeingAttacked(Other);
	}

	//ダメージを受けた際の処理
	void EnemyZako::OnDamaged()
	{
		//hpがあるならダメージ処理する
		if (m_HPCurrent > 0)
		{
			m_state->ChangeState(L"Hit");
		}
	}

	//遠距離の雑魚敵
	void EnemyZakoLong::OnCreate()
	{
		Actor::OnCreate();

		//いったん雑魚敵のHPは50とする
		m_HPMax = 25.0f;
		m_HPCurrent = m_HPMax;

		//Transform設定
		m_trans = GetComponent<Transform>();
		SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(1.0f / 5, 1.0f / 5, 1.0f / 5),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		//ドローメッシュの設定
		auto ptrDraw = GetComponent<PNTBoneModelDraw>();
		ptrDraw->SetMeshResource(L"Enemy_A");
		ptrDraw->SetDiffuse(Col4(0.5f));
		ptrDraw->SetSamplerState(SamplerState::LinearWrap);
		ptrDraw->SetMeshToTransformMatrix(spanMat);

		//アニメーション追加(攻撃タイプによって追加アニメーションが変わる)
		ptrDraw->AddAnimation(L"Stand", 0, 1, 24.0f);
		ptrDraw->AddAnimation(L"Walk", 0, 224, 24.0f);
		ptrDraw->AddAnimation(L"Shot", 225, 136, 24.0f);
		ptrDraw->AddAnimation(L"Down", 362, 424, 24.0f);
		ptrDraw->AddAnimation(L"Hit", 543, 29, false, 24.0f);
		ptrDraw->AddAnimation(L"Stan", 463, 78, false, 24.0f);

		//コリジョン作成
		auto ptrColl = AddComponent<CollisionSphere>();//コリジョンスフィアの方が壁にぶつかる判定に違和感がない
		ptrColl->SetAfterCollision(AfterCollision::Auto);
		ptrColl->SetDrawActive(false);//デバック用

		AddTag(L"Enemy");
		AddTag(L"EnemyZako");

		m_player = GetStage()->GetSharedGameObject<Player>(L"Player");

		//接地判定の設定
		m_LandDetect->SetBindPos(Vec3(0, -2.5f, 0));
		m_LandDetect->GetComponent<Transform>()->SetScale(Vec3(1.0f, 1.0f, 1.0f));

		//ステートマシン生成
		m_state = shared_ptr<EnemyZakoStateMachine>(new EnemyZakoStateMachine(GetThis<GameObject>()));

		//頭上にHPバーを表示させる
		m_HPFrame = GetStage()->AddGameObject<BillBoard>(GetThis<GameObject>(), L"BossGaugeFrame", 4, 5.0f, Vec3(2.0f, 0.5f, 5.0f));
		m_HPBer = GetStage()->AddGameObject<BillBoardGauge>(GetThis<GameObject>(), L"BossHPMater", 3, 5.0f, Vec3(2.0f, 0.5f, 5.0f));
		m_HPBer->SetPercent(1.0f);
	}


	void EnemyZakoFlying::OnCreate()
	{
		Actor::OnCreate();

		//いったん雑魚敵のHPは50とする
		m_HPMax = 50.0f;
		m_HPCurrent = m_HPMax;

		//Transform設定
		m_trans = GetComponent<Transform>();
		SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

		// 重力OFF
		m_doPhysics = false;


		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(1.0f / 5, 1.0f / 5, 1.0f / 5),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		//ドローメッシュの設定
		auto ptrDraw = GetComponent<PNTBoneModelDraw>();
		//攻撃タイプによって見た目が変わる
		if (m_AttackType == Zako_Long)
		{
			ptrDraw->SetMeshResource(L"Enemy_A");
		}
		if (m_AttackType == Zako_Melee)
		{
			ptrDraw->SetMultiMeshResource(L"Enemy_B");
		}
		ptrDraw->SetDiffuse(Col4(0.5f));
		//ptrDraw->SetEmissive(Col4(1));
		ptrDraw->SetSamplerState(SamplerState::LinearWrap);
		ptrDraw->SetMeshToTransformMatrix(spanMat);
		//ptrDraw->SetTextureResource(L"Tx_Boss1");

		//アニメーション追加(攻撃タイプによって追加アニメーションが変わる)
		ptrDraw->AddAnimation(L"Stand", 0, 1, 60.0f);
		ptrDraw->AddAnimation(L"Walk", 126, 49, 60.0f);
		ptrDraw->AddAnimation(L"Charge", 0, 125, 60.0f);
		ptrDraw->AddAnimation(L"Down", 637, 88, 60.0f);

		//コリジョン作成
		auto ptrColl = AddComponent<CollisionSphere>();//コリジョンスフィアの方が壁にぶつかる判定に違和感がない
		ptrColl->SetAfterCollision(AfterCollision::Auto);
		ptrColl->SetDrawActive(true);//デバック用

		AddTag(L"Enemy");
		AddTag(L"EnemyZako");

		m_player = GetStage()->GetSharedGameObject<Player>(L"Player");

		//接地判定の設定
		m_LandDetect->SetBindPos(Vec3(0, -1.0f, 0));
		m_LandDetect->GetComponent<Transform>()->SetScale(Vec3(7.0f, 7.0f, 7.0f));
		//m_LandDetect->SetCollScale(3.0f);

		//ステートマシン生成
		m_state = shared_ptr<EnemyZakoFlyingStateMachine>(new EnemyZakoFlyingStateMachine(GetThis<GameObject>()));

		//頭上にHPバーを表示させる
		m_HPFrame = GetStage()->AddGameObject<BillBoard>(GetThis<GameObject>(), L"BossGaugeFrame", 4, 5.0f, Vec3(2.0f, 0.5f, 5.0f));
		m_HPBer = GetStage()->AddGameObject<BillBoardGauge>(GetThis<GameObject>(), L"BossHPMater", 3, 5.0f, Vec3(2.0f, 0.5f, 5.0f));
		m_HPBer->SetPercent(1.0f);

		//m_damageBill = GetStage()->AddGameObject<EnemyDamageBill>(GetThis<GameObject>(), L"Numbers", 2, 7.0f, Vec3(0.5f, 2.0f, 1.0f));

		//auto m_billBoard2 = GetStage()->AddGameObject<BillBoard>(GetThis<GameObject>(), L"BossHPMater", 3, 5.0f, Vec3(2.0f, 0.5f, 5.0f));
	}

	void EnemyZakoFlying::OnUpdate()
	{
		//もしポーズフラグがオンであればアップデート処理は出来なくなる
		if (m_poseFlag)
		{
			return;
		}

		EnemyBase::OnUpdate();

		//着地判定(無効化時間中ならそれを減算する)
		OnLanding();

		//物理的な処理
		if (m_doPhysics) {
			if (!m_isLand) {
				Gravity();
			}
			else {
				//Friction();
			}
		}


		//HPバーの処理
		UpdateHPBer();
		//攻撃のクールタイム
		TimeOfAttackCool();

		//HPがゼロになったら消える
		if (m_HPCurrent <= 0)
		{
			RemoveTag(L"LockOnCan");
			RemoveTag(L"LockOnTarget");

			m_used = false;

			//GetStage()->RemoveGameObject<EnemyZako>(GetThis<EnemyZako>());
			//GetStage()->RemoveGameObject<LandDetect>(m_LandDetect);
			//GetStage()->RemoveGameObject<AttackCollision>(m_AttackCol);
		}

		//アニメーション更新
		GetComponent<PNTBoneModelDraw>()->UpdateAnimation(m_addTimeAnimation);

		GetComponent<Transform>()->SetPosition((m_velocity * _delta) + GetComponent<Transform>()->GetPosition());
	}

	//HPバーの処理
	void EnemyZakoFlying::UpdateHPBer()
	{
		//ビルボードの処理
		if (!m_used)
		{
			m_HPFrame->SetScale(Vec3(0.0f));
			m_HPBer->SetScale(Vec3(0.0f));
		}
		if (m_used)
		{
			m_HPFrame->SetScale(Vec3(2.0f, 0.5f, 5.0f));
			m_HPBer->SetScale(Vec3(2.0f, 0.5f, 5.0f));

			//HPの割合によってゲージが減る
			float HPPercent = (float)m_HPCurrent / (float)m_HPMax;
			m_HPBer->SetPercent(HPPercent);

		}


	}

	//攻撃のクールタイム
	void EnemyZakoFlying::TimeOfAttackCool()
	{
		//攻撃のクールタイム
		if (!m_attackFlag)
		{
			m_timeCountOfAttackCool += _delta;
			//クールタイム過ぎたら攻撃できるようになる
			if (m_timeCountOfAttackCool >= m_timeOfAttackCool)
			{
				m_timeCountOfAttackCool = 0.0f;//リセット
				m_attackFlag = true;
			}
		}
	}

	//コリジョン判定
	void EnemyZakoFlying::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		DetectBeingAttacked(Other);
	}

	//ダメージを受けた際の処理
	void EnemyZakoFlying::OnDamaged()
	{
		////攻撃時はノックバックしないようにする(実験)(強すぎるので別の方向性で強くする)
		//if (!FindTag(L"AttackNow"))
		//{
		//	m_state->ChangeState(L"Hit");
		//}
		//else if (FindTag(L"AttackNow"))
		//{
		//	m_HPCurrent -= CalculateDamage(m_getHitInfo.Damage);
		//}

		m_state->ChangeState(L"Hit");
	}

}
//end basecross

