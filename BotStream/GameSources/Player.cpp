/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	Player::Player(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, int hp, int attack, int defense) :
		Actor(stagePtr, pos, rot, scale),
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
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//ドローメッシュの設定
		auto ptrDraw = GetComponent<PNTBoneModelDraw>();
		ptrDraw->SetMultiMeshResource(L"Spearmen");//仮のメッシュ
		ptrDraw->AddAnimation(L"Idle", 0, 1, true, 60.0f);//歩き状態
		ptrDraw->AddAnimation(L"Walk", 0, 100, true, 60.0f);//歩き状態
		ptrDraw->SetSamplerState(SamplerState::LinearWrap);
		ptrDraw->SetMeshToTransformMatrix(spanMat);
		ptrDraw->SetTextureResource(L"SpearmenTexture");

		//コリジョン作成
		auto ptrColl = AddComponent<CollisionSphere>();//コリジョンスフィアの方が壁にぶつかる判定に違和感がない
		ptrColl->SetAfterCollision(AfterCollision::Auto);

		AddTag(L"Player");//Player用のタグ
		m_stateMachine = shared_ptr<PlayerStateMachine>(new PlayerStateMachine(GetThis<GameObject>()));

		//UI追加
		m_playerBulletUI = GetStage()->AddGameObject<PlayerBulletUI>(GetThis<Player>(), Vec2(165.0f, -250.0f), m_bulletNum);//現在の球数を出すUI

		//auto stage = GetStage();
		//auto playerButton = stage->GetSharedGameObject<PlayerButtonUI>(L"PlayerButton");


		//SE受け取り
		m_SEManager = App::GetApp()->GetXAudio2Manager();
		m_SEManager->Start(L"StartVoiceSE", 0, 0.9f);

	}

	void Player::OnUpdate()
	{
		Actor::OnUpdate();
		auto cntl = App::GetApp()->GetInputDevice().GetControlerVec();
		auto angle = GetAngle();
		auto stage = GetStage();

		//リロード処理
		ReloadBullet(3.0f);

		//UIバーを更新する
		auto playerUI = stage->GetSharedGameObject<PlayerGaugeUI>(L"PlayerUI");//Playerバーを取得
		playerUI->SetPLHPSprite(m_HPCurrent);
		playerUI->SetPLSPSprite(m_SPCurrent);


		//// 仮：Yボタンでプレイヤーの(見かけ上の)HPが減る
		//if (cntl[0].wPressedButtons & XINPUT_GAMEPAD_Y)
		//{
		//	m_HPCurrent = m_HPCurrent - 10.0f;  // ← 10ずつ減る想定
		//}
		//// 仮：Bボタンで必殺技溜め
		//if (cntl[0].wPressedButtons & XINPUT_GAMEPAD_B)
		//{
		//	m_SPCurrent = m_SPCurrent + 10.0f; // 今の設定だと10回押すと最大になる
		//}


		//ステート処理
		m_stateMachine->Update();
		//m_stateMachine->ChangeState(L"Walk");//ステート変更

		//動く処理(仮)
		//PlayerMove();

		//着地判定(無効化時間中ならそれを減算する)
		OnLanding();

		//処理
		if (!m_isLand) {
			Gravity();
		}
		else {
			Friction();
			Dodge();
		}

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
			EfkPlaying(L"Charge", GetAngle(), Vec3(0, 1, 0));
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
		//GetComponent<PNTBoneModelDraw>()->UpdateAnimation(_delta * 5);
		GetComponent<Transform>()->SetPosition((m_velocity * _delta) + GetComponent<Transform>()->GetPosition());
	}

	void Player::Jump() {
		// 入力デバイス取得
		auto inputDevice = App::GetApp()->GetInputDevice();
		auto controller = inputDevice.GetControlerVec()[0];

		if (controller.wPressedButtons & XINPUT_GAMEPAD_A) {
			m_velocity.y = m_jumpPower;
			m_isLand = false;
			m_disableLandDetect = 1.0f;
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

	//プレイヤーの移動処理
	void Player::PlayerMove(int playerState)
	{
		Vec3 move = GetMoveVector(playerState);
		m_accel = move * m_baseAccel;
		m_velocity += move;

		//プレイヤーの向き
		if (move.length() != 0)
		{
			m_angle = atan2(move.z, move.x);
			m_rot.y = -m_angle;

			m_trans->SetRotation(m_rot);
		}

		SpeedLimit(move.length());
	}

	//移動ベクトルの計算処理
	Vec3 Player::GetMoveVector(int playerState)
	{
		// 入力デバイス取得
		auto inputDevice = App::GetApp()->GetInputDevice();
		auto controller = inputDevice.GetControlerVec()[0];
		Vec3 stick = Vec3(controller.fThumbLX, 0, controller.fThumbLY);
		Vec3 totalVec;

		if (playerState == PlayerState_Walk || playerState == PlayerState_Dash)//徒歩、ダッシュ処理
		{
			auto trans = GetTransform();
			auto camera = OnGetDrawCamera();

			//スティックの向きと距離
			float moveSize = stick.length();
			float moveAngle = atan2(-stick.x, stick.z);

			//自分の位置とカメラの位置からカメラの角度を算出
			auto front = trans->GetPosition() - camera->GetEye();
			front.y = 0;
			front.normalize();
			float frontAngle = atan2(front.z, front.x);

			float totalAngle = frontAngle + moveAngle;
			totalVec = Vec3(cos(totalAngle), 0, sin(totalAngle));
			totalVec.normalize();

			//ステートによって移動ベクトルの大きさを変える
			switch (playerState)
			{
			case PlayerState_Walk:
				totalVec *= moveSize;
				break;
			case PlayerState_Dash:
				totalVec *= moveSize * 2.5f;
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
			float dodge = 8.0f;
			totalVec.x = cos(m_angle) * (dodge * abs(cos(m_dodgeTime)));
			totalVec.z = sin(m_angle) * (dodge * abs(cos(m_dodgeTime)));

			//回避が終わったらダッシュ処理ができる
			if (m_dodgeTime > XMConvertToRadians(20.0f))
			{
				//Aボタンを押し続ける限り走るそうでなければダッシュ回避処理をしない
				if (controller.bConnected && controller.wButtons & XINPUT_GAMEPAD_A)
				{
					m_dashFlag = true;
					m_dodgeTime = 0.0f;
					m_dodgeFlag = false;//回避処理終了
				}
				else
				{
					m_dodgeTime = 0.0f;
					m_dodgeFlag = false;//回避処理終了
				}

			}
		}
		return totalVec;
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

	//回避フラグのゲッター
	bool Player::GetDodgeFlag()
	{
		return m_dodgeFlag;
	}

	void Player::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		DetectBeingAttacked(Other);//攻撃を受けた判定
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
		wss /* << L"デバッグ用文字列 "*/
			<< L"\n Pos.x " << m_pos.x << " Pos.z " << m_pos.z
			<< L" Vel.x " << m_velocity.x << L"\ Vel.y " << m_velocity.y << L" Vel.z " << m_velocity.z
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

		//ドローメッシュの設定
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDraw->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 1.0f));
		ptrDraw->SetOwnShadowActive(false);//影は消す
		ptrDraw->SetDrawActive(true);
		ptrDraw->SetEmissive(Col4(1.0f, 1.0f, 1.0f, 1.0f)); // 自己発光カラー（ライティングによる陰影を消す効果がある）
		ptrDraw->SetOwnShadowActive(true); // 影の映り込みを反映させる

		//原点オブジェクトが消えていたら自分も消える
		auto originLock = m_originObj.lock();
		if (!originLock)
		{
			GetStage()->RemoveGameObject<Bullet>(GetThis<Bullet>());
			return;
		}
		
		if (originLock->FindTag(L"Player"))
		{
			m_playerAngle = originLock->GetAngle();
		}
		else if (originLock->FindTag(L"Enemy"))
		{
			auto playerAngleVec = originLock->GetComponent<Transform>()->GetForward();
			m_playerAngle = atan2f(playerAngleVec.z, -playerAngleVec.x);
			m_playerAngle -= XMConvertToRadians(90.0f);
		}

		//攻撃判定の定義
		auto tmp = GetAttackPtr()->GetHitInfo();
		//tmp.Type = AttackType::Player;//攻撃のタイプは敵
		//SetAttackActor(m_actorType);//攻撃しているActorを指定する
		switch (m_actorType)
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

		tmp.HitOnce = true;//一回しかヒットしないか
		tmp.Damage = 10;//ダメージ
		tmp.HitVel_Stand = Vec3(-5, 5, 0);//ヒットバック距離
		tmp.HitTime_Stand = 1.0f;//のけぞり時間
		//tmp.PauseTime = 5.0f;
		//tmp.ForceRecover = true;
		DefAttack(.5f, tmp);
		GetAttackPtr()->SetPos(Vec3(0, 0, 0));
		auto AttackPtr = GetAttackPtr();
		AttackPtr->SetCollScale(1.0f);

		DefAttack(5.0f, tmp);

	}

	void Bullet::OnUpdate()
	{
		Actor::OnUpdate();

		auto tmp = GetAttackPtr()->GetHitInfo();
		tmp.Type;

		auto delta = App::GetApp()->GetElapsedTime();
		//移動距離を計算する
		Vec3 moveVec;

		//原点オブジェクトを受け取る
		auto originLock = m_originObj.lock();

		moveVec.x = m_speed * cos(m_playerAngle) * delta;
		moveVec.z = m_speed * sin(-m_playerAngle) * delta;

		//プレイヤーの位置を取得して移動する
		auto pos = m_trans->GetPosition();
		m_trans->SetPosition(pos + moveVec);

		m_canMoveDistance -= moveVec.x + moveVec.z;
		//一定時間移動したら消える
		if (m_canMoveDistance <= 0.0f)
		{
			GetStage()->RemoveGameObject<Bullet>(GetThis<Bullet>());
			auto hitInfo = originLock->GetAttackPtr()->GetHitInfo();
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

	}


	//雑魚敵の処理をいったんここに書きますマージ終わったらEnemy.cpp.hに戻す
	void EnemyZako::OnCreate()
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

		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(1.0f/10, 1.0f, 1.0f/10),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f),
			Vec3(0.0f, -0.0f, 0.0f)
		);

		//ドローメッシュの設定
		auto ptrDraw = GetComponent<PNTBoneModelDraw>();
		ptrDraw->SetMeshResource(L"Enemy_A");
		ptrDraw->SetDiffuse(Col4(0.5f));
		//ptrDraw->SetEmissive(Col4(1));
		ptrDraw->SetSamplerState(SamplerState::LinearWrap);
		ptrDraw->SetMeshToTransformMatrix(spanMat);
		//ptrDraw->SetTextureResource(L"Tx_Boss1");

		//コリジョン作成
		auto ptrColl = AddComponent<CollisionObb>();//コリジョンスフィアの方が壁にぶつかる判定に違和感がない
		ptrColl->SetAfterCollision(AfterCollision::Auto);
		ptrColl->SetDrawActive(false);//デバック用

		AddTag(L"Enemy");
		AddTag(L"EnemyZako");

		m_player = GetStage()->GetSharedGameObject<Player>(L"Player");

		m_LandDetect->SetCollScale(1.5f);
		
		//ステートマシン生成
		m_state = shared_ptr<EnemyZakoStateMachine>(new EnemyZakoStateMachine(GetThis<GameObject>()));

	}

	void EnemyZako::OnUpdate()
	{
		EnemyBase::OnUpdate();

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

		GetComponent<Transform>()->SetPosition((m_velocity * _delta) + GetComponent<Transform>()->GetPosition());
	}

	//コリジョン判定
	void EnemyZako::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		DetectBeingAttacked(Other);
	}

	//ダメージを受けた際の処理
	void EnemyZako::OnDamaged()
	{
		m_state->ChangeState(L"Hit");
	}



}
//end basecross

