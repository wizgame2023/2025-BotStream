/*!
@file Player.cpp
@brief �v���C���[�Ȃǎ���
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

	//�쐬
	void Player::OnCreate()
	{
		Actor::OnCreate();
		//Transform�ݒ�
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

		//�h���[���b�V���̐ݒ�
		auto ptrDraw = GetComponent<PNTBoneModelDraw>();
		ptrDraw->SetMultiMeshResource(L"Spearmen");//���̃��b�V��
		ptrDraw->AddAnimation(L"Idle", 0, 1, true, 60.0f);//�������
		ptrDraw->AddAnimation(L"Walk", 0, 100, true, 60.0f);//�������
		ptrDraw->SetSamplerState(SamplerState::LinearWrap);
		ptrDraw->SetMeshToTransformMatrix(spanMat);
		ptrDraw->SetTextureResource(L"SpearmenTexture");

		//�R���W�����쐬
		auto ptrColl = AddComponent<CollisionSphere>();//�R���W�����X�t�B�A�̕����ǂɂԂ��锻��Ɉ�a�����Ȃ�
		ptrColl->SetAfterCollision(AfterCollision::Auto);

		AddTag(L"Player");//Player�p�̃^�O
		//���n����̐����A�q�I�u�W�F�N�g�ɂ���
		m_LandDetect = GetStage()->AddGameObject<LandDetect>();
		m_LandDetect->GetComponent<Transform>()->SetParent(dynamic_pointer_cast<GameObject>(GetThis<Actor>()));

	}

	void Player::OnUpdate()
	{
		_delta = App::GetApp()->GetElapsedTime();
		
		//��������(��)
		PlayerMove();
		
		//���n����(���������Ԓ��Ȃ炻������Z����)
		if (m_disableLandDetect > 0) {
			m_disableLandDetect -= _delta;
		}
		else {
			if (m_LandDetect->GetLand() != m_isLand) {
				//���n��������
				if (!m_isLand) m_velocity.y = 0;
				m_isLand = !m_isLand;
			}
		}

		//����
		if (!m_isLand) {
			Gravity();
		}
		else {
			Friction();
			Jump();
		}

		//�f�o�b�N�p������
		DebugLog();

		//�A�j���[�V�����Đ�
		GetComponent<PNTBoneModelDraw>()->UpdateAnimation(_delta * 5);
		GetComponent<Transform>()->SetPosition((m_velocity * _delta) + GetComponent<Transform>()->GetPosition());
	}

	void Player::Jump() {
		// ���̓f�o�C�X�擾
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

		//�v���C���[�̌���
		if (move.length() != 0)
		{
			m_angle = -atan2(move.z, move.x);
			m_rot.y = m_angle;

			m_trans->SetRotation(m_rot);
		}

		//�A�j���[�V�����Đ�
		if (move.length() != 0)
		{
			ChangeAnim(L"Walk");
		}
		else {
			ChangeAnim(L"Idle");
		}

		SpeedLimit(move.length());

	}

	//�ō����x
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
		//�����̏I�[���x
		if (m_velocity.y < m_fallTerminal) m_velocity.y = m_fallTerminal;
	}

	//���C(�n��̂�)
	void Player::Friction() {
		//�Ö��C
		if (GetMoveVector() == Vec3(0)) {
			m_velocity.x -= m_velocity.x * m_friction * (1000.0f / 60.0f) * _delta;
			m_velocity.z -= m_velocity.z * m_friction * (1000.0f / 60.0f) * _delta;
			if (m_velocity.length() > m_frictionThreshold) m_velocity.x = 0;
			return;
		}
		//�����C
		if (GetMoveVector() != Vec3(0)) {
			m_velocity.x -= m_velocity.x * m_frictionDynamic * (1000.0f / 60.0f) * _delta;
			m_velocity.z -= m_velocity.z * m_frictionDynamic * (1000.0f / 60.0f) * _delta;
		}
	}

	//�d��
	void Player::Gravity() {
		m_velocity.y += m_gravity * _delta;
	}

	Vec3 Player::GetMoveVector() {
		// ���̓f�o�C�X�擾
		auto inputDevice = App::GetApp()->GetInputDevice(); 
		auto controller = inputDevice.GetControlerVec()[0];
		Vec3 stick = Vec3(controller.fThumbLX, 0, controller.fThumbLY);

		if (abs(stick.x) > m_stickDeadZone || abs(stick.z) > m_stickDeadZone) {
			auto trans = GetTransform();
			auto camera = OnGetDrawCamera();
			
			//�X�e�B�b�N�̌����Ƌ���
			float moveSize = stick.length();
			float moveAngle = atan2(-stick.x, stick.z);

			//�����̈ʒu�ƃJ�����̈ʒu����J�����̊p�x���Z�o
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

	//Player�̌����Ă�������̃Q�b�^�[
	float Player::GetAngle()
	{
		return m_angle;
	}

	//Player�̌����Ă�������̃Z�b�^�[
	void Player::SetAngle(float angle)
	{
		m_angle = angle;
	}

	//�f�o�b�N�p������\���֐�
	void Player::DebugLog()
	{
		// �C���v�b�g�f�o�C�X�I�u�W�F�N�g
		auto inputDevice = App::GetApp()->GetInputDevice(); // �l�X�ȓ��̓f�o�C�X���Ǘ����Ă���I�u�W�F�N�g���擾

		////�f�o�b�N�p
		wstringstream wss(L"");
		auto scene = App::GetApp()->GetScene<Scene>();

		wss /* << L"�f�o�b�O�p������ "*/
			<< L"\n Pos.x " << m_pos.x << " Pos.z " << m_pos.z
			<< L" Vel.x " << m_velocity.x << L"\ Vel.y " << m_velocity.y << L" Vel.z " << m_velocity.z
			<< endl << "onLand: " << m_isLand << " LandDetect: " << m_LandDetect->GetLand() << endl;

		scene->SetDebugString(wss.str());
	}
}
//end basecross

