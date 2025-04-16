/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	Player::Player(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale) :
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
	}

	void Player::OnUpdate()
	{
		Actor::OnUpdate();
		auto cntl = App::GetApp()->GetInputDevice().GetControlerVec();
		auto angle = GetAngle();

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
			//Jump();
			//Dash();
			Dodge();
		}

		//if (cntl[0].wPressedButtons & XINPUT_GAMEPAD_B)
		//{
		//	EfkPlaying(L"Laser", angle + XM_PIDIV2, Vec3(0, 1, 0));
		//}

		//if (cntl[0].wPressedButtons & XINPUT_GAMEPAD_X)
		//{
		//	EfkPlaying(L"Sword", angle + XM_PI, Vec3(0, 1, 0));
		//}

		//デバック用文字列
		DebugLog();

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

	//エフェクトを出す処理
	void Player::AddEffect(int addEffect)
	{
		switch (addEffect)
		{
		case PlayerEffect_Attack1:
			EfkPlaying(L"Sword", GetAngle() + XM_PI, Vec3(0, 1, 0));
			break;
		case PlayerEffect_Attack2:
			EfkPlaying(L"Sword", GetAngle() + XM_PI, Vec3(0, 1, 0), Col4(0.22f, 1.0f, 0.48f, 1.0f));
			break;
		case PlayerEffect_Beam:
			EfkPlaying(L"Laser", GetAngle() + XM_PIDIV2, Vec3(0, 1, 0));
			break;
		default:
			break;
		}
	}


	//Playerの向いている方向のゲッター
	float Player::GetAngle()
	{
		return -m_angle;
	}

	//回避フラグのゲッター
	bool Player::GetDodgeFlag()
	{
		return m_dodgeFlag;
	}

	//Playerの向いている方向のセッター
	void Player::SetAngle(float angle)
	{
		m_angle = angle;
	}

	//デバック用文字列表示関数
	void Player::DebugLog()
	{
		// インプットデバイスオブジェクト
		auto inputDevice = App::GetApp()->GetInputDevice(); // 様々な入力デバイスを管理しているオブジェクトを取得

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

	// エフェクトのプレイ
	void Player::EfkPlaying(wstring EfkKey, float rad, Vec3 rotate)
	{
		rotate.normalize();
		auto trans = GetComponent<Transform>();
		auto plPos = trans->GetPosition();

		auto efkHandler = EffectManager::Instance().PlayEffect(EfkKey, plPos);
		EffectManager::Instance().SetRotation(efkHandler, Vec3(rotate.x, rotate.y, rotate.z), rad);
	}
	void Player::EfkPlaying(wstring EfkKey, float rad, Vec3 rotate,Col4 changeColor)
	{
		rotate.normalize();
		auto trans = GetComponent<Transform>();
		auto plPos = trans->GetPosition();

		auto efkHandler = EffectManager::Instance().PlayEffect(EfkKey, plPos);
		EffectManager::Instance().SetAllColor(efkHandler, changeColor);//エフェクトの色を変える
		EffectManager::Instance().SetRotation(efkHandler, Vec3(rotate.x, rotate.y, rotate.z), rad);
	}

	void Player::OnLanding()
	{
		if (m_disableLandDetect > 0) {
			m_disableLandDetect -= _delta;
		}
		else {
			if (m_LandDetect->GetLand() != m_isLand) {
				//着地した判定
				if (!m_isLand)
				{
					m_velocity.y = 0;
					EfkPlaying(L"Landing", GetAngle(), Vec3(0, 1, 0));

				}

				m_isLand = !m_isLand;
			}
		}

	}
}
//end basecross

