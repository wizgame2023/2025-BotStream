/*!
@file Player.cpp
@brief �v���C���[�Ȃǎ���
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

		m_AttackCol = GetStage()->AddGameObject<AttackCollision>();
		m_AttackCol->GetComponent<Transform>()->SetParent(dynamic_pointer_cast<GameObject>(GetThis<Actor>()));

		m_AttackCol->GetComponent<Transform>()->SetPosition(Vec3(0, 1, 0));

		AddTag(L"Player");//Player�p�̃^�O

		m_stateMachine = shared_ptr<PlayerStateMachine>(new PlayerStateMachine(GetThis<GameObject>()));

	}

	void Player::OnUpdate()
	{
		Actor::OnUpdate();
		auto cntl = App::GetApp()->GetInputDevice().GetControlerVec();
		auto angle = GetAngle();

		//�X�e�[�g����
		m_stateMachine->Update();
		//m_stateMachine->ChangeState(L"Walk");//�X�e�[�g�ύX

		//��������(��)
		//PlayerMove();

		//���n����(���������Ԓ��Ȃ炻������Z����)
		OnLanding();

		//����
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

		//�f�o�b�N�p������
		DebugLog();

		//�A�j���[�V�����Đ�
		//GetComponent<PNTBoneModelDraw>()->UpdateAnimation(_delta * 5);
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

	//�X�e�[�g�ύX���� �����ɓ��ꂽ�X�e�[�g�ɕύX����
	void Player::ChangeState(wstring stateName)
	{
		m_stateMachine->ChangeState(stateName);
	}

	//�_�b�V������
	void Player::Dash()
	{
		// ���̓f�o�C�X�擾
		auto inputDevice = App::GetApp()->GetInputDevice();
		auto controller = inputDevice.GetControlerVec()[0];

		//����������A�������Ń_�b�V��
		if (controller.wButtons & XINPUT_GAMEPAD_A && m_dodgeFlag) {
			m_dashFlag = true;
		}
		//�_�b�V�����Ă����A�{�^���𗣂�����Ń_�b�V������
		if (controller.wPressedButtons & XINPUT_GAMEPAD_A && m_dashFlag) {
			m_dashFlag = false;
		}
	}

	//����̃t���O��n������
	void Player::Dodge()
	{
		// ���̓f�o�C�X�擾
		auto inputDevice = App::GetApp()->GetInputDevice();
		auto controller = inputDevice.GetControlerVec()[0];

		if (controller.wPressedButtons & XINPUT_GAMEPAD_A) {
			m_dodgeFlag = true;//�������
		}

	}

	//�v���C���[�̈ړ�����
	void Player::PlayerMove(int playerState)
	{
		Vec3 move = GetMoveVector(playerState);
		m_accel = move * m_baseAccel;
		m_velocity += move;

		//�v���C���[�̌���
		if (move.length() != 0)
		{
			m_angle = atan2(move.z, move.x);
			m_rot.y = -m_angle;

			m_trans->SetRotation(m_rot);
		}

		SpeedLimit(move.length());
	}

	//�ړ��x�N�g���̌v�Z����
	Vec3 Player::GetMoveVector(int playerState) 
	{
		// ���̓f�o�C�X�擾
		auto inputDevice = App::GetApp()->GetInputDevice();
		auto controller = inputDevice.GetControlerVec()[0];
		Vec3 stick = Vec3(controller.fThumbLX, 0, controller.fThumbLY);
		Vec3 totalVec;

		if (playerState == PlayerState_Walk || playerState == PlayerState_Dash)//�k���A�_�b�V������
		{
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
			totalVec = Vec3(cos(totalAngle), 0, sin(totalAngle));
			totalVec.normalize();

			//�X�e�[�g�ɂ���Ĉړ��x�N�g���̑傫����ς���
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
		//�������
		if (playerState == PlayerState_Dodge)
		{
			//�������
			float timeSpeed = 80.0f;
			m_dodgeTime += XMConvertToRadians(_delta * timeSpeed);

			//�񎟊֐��I�ȓ����ŉ���s��������
			//���͌����Ă�������ɑO�����������
			float dodge = 8.0f;
			totalVec.x = cos(m_angle) * (dodge * abs(cos(m_dodgeTime)));
			totalVec.z = sin(m_angle) * (dodge * abs(cos(m_dodgeTime)));

			//������I�������_�b�V���������ł���
			if (m_dodgeTime > XMConvertToRadians(20.0f))
			{
				//A�{�^����������������葖�邻���łȂ���΃_�b�V��������������Ȃ�
				if (controller.bConnected && controller.wButtons & XINPUT_GAMEPAD_A)
				{
					m_dashFlag = true;
					m_dodgeTime = 0.0f;
					m_dodgeFlag = false;//��������I��
				}
				else
				{
					m_dodgeTime = 0.0f;
					m_dodgeFlag = false;//��������I��
				}

			}
		}
		return totalVec;
	}

	//�G�t�F�N�g���o������
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


	//Player�̌����Ă�������̃Q�b�^�[
	float Player::GetAngle()
	{
		return -m_angle;
	}

	//����t���O�̃Q�b�^�[
	bool Player::GetDodgeFlag()
	{
		return m_dodgeFlag;
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
		auto quat = GetComponent<Transform>()->GetQuaternion();
		wss /* << L"�f�o�b�O�p������ "*/
			<< L"\n Pos.x " << m_pos.x << " Pos.z " << m_pos.z
			<< L" Vel.x " << m_velocity.x << L"\ Vel.y " << m_velocity.y << L" Vel.z " << m_velocity.z
			<< endl << "onLand: " << m_isLand << " LandDetect: " << m_LandDetect->GetLand()
			<< L"\nQuat : (" << L"\n" << quat.x << L"\n" << quat.y << L"\n" << quat.z << L"\n" << quat.w
			<< L"\nAngle : " << GetAngle() << endl;

		scene->SetDebugString(wss.str());
	}

	// �G�t�F�N�g�̃v���C
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
		EffectManager::Instance().SetAllColor(efkHandler, changeColor);//�G�t�F�N�g�̐F��ς���
		EffectManager::Instance().SetRotation(efkHandler, Vec3(rotate.x, rotate.y, rotate.z), rad);
	}

	void Player::OnLanding()
	{
		if (m_disableLandDetect > 0) {
			m_disableLandDetect -= _delta;
		}
		else {
			if (m_LandDetect->GetLand() != m_isLand) {
				//���n��������
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

