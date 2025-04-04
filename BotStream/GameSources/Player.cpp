/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	Player::Player(const shared_ptr<Stage>& stagePtr,Vec3 pos,Vec3 rot, Vec3 scale) :
		Actor(stagePtr, pos, rot, scale)
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
		//着地判定の生成、子オブジェクトにする
		m_LandDetect = GetStage()->AddGameObject<LandDetect>();
		m_LandDetect->GetComponent<Transform>()->SetParent(dynamic_pointer_cast<GameObject>(GetThis<Actor>()));

	}

	void Player::OnUpdate()
	{
		_delta = App::GetApp()->GetElapsedTime();
		
		//動く処理(仮)
		PlayerMove();
		
		//着地判定(無効化時間中ならそれを減算する)
		if (m_disableLandDetect > 0) {
			m_disableLandDetect -= _delta;
		}
		else {
			if (m_LandDetect->GetLand() != m_isLand) {
				//着地した判定
				if (!m_isLand) m_velocity.y = 0;
				m_isLand = !m_isLand;
			}
		}

		//処理
		if (!m_isLand) {
			Gravity();
		}
		else {
			Friction();
			Jump();
		}

		//デバック用文字列
		DebugLog();

		//アニメーション再生
		GetComponent<PNTBoneModelDraw>()->UpdateAnimation(_delta * 5);
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

	void Player::PlayerMove()
	{
		Vec3 move = GetMoveVector();
		m_velocity += move;

		//プレイヤーの向き
		if (move.length() != 0)
		{
			m_angle = -atan2(move.z, move.x);
			m_rot.y = m_angle;

			m_trans->SetRotation(m_rot);
		}

		//アニメーション再生
		if (move.length() != 0)
		{
			ChangeAnim(L"Walk");
		}
		else {
			ChangeAnim(L"Idle");
		}

		SpeedLimit(move.length());

	}

	//最高速度
	void Player::SpeedLimit(float multiply) {
		float limit = m_speedMax * multiply;
		auto angle = m_velocity;
		angle.y = 0;
		if (angle.length() > 0) {
			angle.normalize();
			if (angle.x > 0) {
				if (m_velocity.x > angle.x * limit) m_velocity.x = angle.x * limit;
			}
			else {
				if (m_velocity.x < angle.x * limit) m_velocity.x = angle.x * limit;
			}
			if (angle.z > 0) {
				if (m_velocity.z > angle.z * limit) m_velocity.z = angle.z * limit;
			}
			else {
				if (m_velocity.z < angle.z * limit) m_velocity.z = angle.z * limit;
			}
		}
		//落下の終端速度
		if (m_velocity.y < m_fallTerminal) m_velocity.y = m_fallTerminal;
	}

	//摩擦(地上のみ)
	void Player::Friction() {
		//静摩擦
		if (GetMoveVector() == Vec3(0)) {
			m_velocity.x -= m_velocity.x * m_friction * (1000.0f / 60.0f) * _delta;
			m_velocity.z -= m_velocity.z * m_friction * (1000.0f / 60.0f) * _delta;
			if (m_velocity.length() > m_frictionThreshold) m_velocity.x = 0;
			return;
		}
		//動摩擦
		if (GetMoveVector() != Vec3(0)) {
			m_velocity.x -= m_velocity.x * m_frictionDynamic * (1000.0f / 60.0f) * _delta;
			m_velocity.z -= m_velocity.z * m_frictionDynamic * (1000.0f / 60.0f) * _delta;
		}
	}

	//重力
	void Player::Gravity() {
		m_velocity.y += m_gravity * _delta;
	}

	Vec3 Player::GetMoveVector() {
		// 入力デバイス取得
		auto inputDevice = App::GetApp()->GetInputDevice(); 
		auto controller = inputDevice.GetControlerVec()[0];
		Vec3 stick = Vec3(controller.fThumbLX, 0, controller.fThumbLY);

		if (abs(stick.x) > m_stickDeadZone || abs(stick.z) > m_stickDeadZone) {
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
			Vec3 totalVec = Vec3(cos(totalAngle), 0, sin(totalAngle));
			totalVec.normalize();
			totalVec *= moveSize;

			return totalVec;
		}
		return Vec3(0);
	}

	//Playerの向いている方向のゲッター
	float Player::GetAngle()
	{
		return m_angle;
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

		wss /* << L"デバッグ用文字列 "*/
			<< L"\n Pos.x " << m_pos.x << " Pos.z " << m_pos.z
			<< L" Vel.x " << m_velocity.x << L"\ Vel.y " << m_velocity.y << L" Vel.z " << m_velocity.z
			<< endl << "onLand: " << m_isLand << " LandDetect: " << m_LandDetect->GetLand() << endl;

		scene->SetDebugString(wss.str());
	}
}
//end basecross

