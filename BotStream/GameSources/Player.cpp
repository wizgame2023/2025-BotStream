/*!
@file Player.cpp
@brief �v���C���[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	Player::Player(const shared_ptr<Stage>& stagePtr,Vec3 pos,Vec3 rot, Vec3 scale) :
		Actor(stagePtr),
		m_pos(pos),
		m_rot(rot),
		m_scale(scale)
	{

	}
	Player::~Player()
	{

	}

	//�쐬
	void Player::OnCreate()
	{
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
		auto ptrDraw = AddComponent<PNTBoneModelDraw>();
		ptrDraw->SetMultiMeshResource(L"Spearmen");//���̃��b�V��
		ptrDraw->AddAnimation(L"Walk", 0, 100, true, 60.0f);//�������
		ptrDraw->SetSamplerState(SamplerState::LinearWrap);
		ptrDraw->SetMeshToTransformMatrix(spanMat);
		ptrDraw->SetTextureResource(L"SpearmenTexture");

		//�R���W�����쐬
		auto ptrColl = AddComponent<CollisionSphere>();//�R���W�����X�t�B�A�̕����ǂɂԂ��锻��Ɉ�a�����Ȃ�
		ptrColl->SetAfterCollision(AfterCollision::Auto);

		AddTag(L"Player");//Player�p�̃^�O
	}

	void Player::OnUpdate()
	{
		float delta = App::GetApp()->GetElapsedTime();
		
		//��������(��)
		PlayerMove();

		//�f�o�b�N�p������
		DebagLog();
	}

	void Player::PlayerMove()
	{
		//auto pos = GetComponent<Transform>()->GetPosition();//�|�W�V�����擾
		auto Delta = App::GetApp()->GetElapsedTime();
		float m_speed = 3.0f;//���̃X�s�[�h

		// �C���v�b�g�f�o�C�X�I�u�W�F�N�g
		auto inputDevice = App::GetApp()->GetInputDevice(); // �l�X�ȓ��̓f�o�C�X���Ǘ����Ă���I�u�W�F�N�g���擾
		//�R���g���[���[�̎擾
		auto m_controler = inputDevice.GetControlerVec()[0];
		Vec2 contrloerVec = Vec2(m_controler.fThumbLX, m_controler.fThumbLY);


		//���X�e�b�N�̌����Ƀv���C���[���i��
		if (m_controler.bConnected)
		{
			m_pos.z += (m_controler.fThumbLY * m_speed * Delta) * 2;
			m_pos.x += (m_controler.fThumbLX * m_speed * Delta) * 2;

			m_trans->SetPosition(m_pos);//�|�W�V�����X�V
		}

		//�X�e�b�N���X����ƃA�j���[�V�������Đ������
		if (!m_controler.fThumbLX == 0 || !m_controler.fThumbLY == 0)
		{
			auto Name = GetComponent<PNTBoneModelDraw>()->GetCurrentAnimation();
			if (Name != L"Walk")
			{
				//�A�j���[�V�����ύX
				GetComponent<PNTBoneModelDraw>()->ChangeCurrentAnimation(L"Walk");
			}

			//�A�j���[�V�����Đ�
			GetComponent<PNTBoneModelDraw>()->UpdateAnimation(Delta*5);
		}


		float deg = 0;
		//���X�e�b�N�̌����Ƀv���C���[������
		if (m_controler.bConnected)
		{
			//�X�e�B�b�N�̌X�������W�A���ɂ���
			float rad = -atan2(m_controler.fThumbLY, m_controler.fThumbLX);
			//���W�A���̌X�����f�B�O���[�p�ɂ���
			deg = rad * 180 / 3.14f;
			m_rot.y = rad;
			//�Q�[���p�b�g�̌X����������Ή�]�x�͍X�V���Ȃ�
			if (m_controler.fThumbLY != 0.0f && m_controler.fThumbLX != 0.0f)
			{
				m_trans->SetRotation(m_rot);
			}

		}

	}

	void Player::DebagLog()
	{
		// �C���v�b�g�f�o�C�X�I�u�W�F�N�g
		auto inputDevice = App::GetApp()->GetInputDevice(); // �l�X�ȓ��̓f�o�C�X���Ǘ����Ă���I�u�W�F�N�g���擾
		////�R���g���[���[�̎擾
		//auto m_controler = inputDevice.GetControlerVec()[0];
		//Vec2 contrloerVec = Vec2(m_controler.fThumbLX, m_controler.fThumbLY);

		////�f�o�b�N�p
		wstringstream wss(L"");
		auto scene = App::GetApp()->GetScene<Scene>();
		//auto gameStage = scene->GetGameStage();
		//m_Pos = GetComponent<Transform>()->GetPosition();

		wss /* << L"�f�o�b�O�p������ "*/
			<< L"\nPos.x " << m_pos.x << "\nPos.z " << m_pos.z
			//<<L"\n�R���g���[���[�̓��� x:"<<contrloerVec.x<<L" y:"<<contrloerVec.y
			//<<L"\nFPS:"<< 1.0f/delta
			<< endl;

		scene->SetDebugString(wss.str());
	}
}
//end basecross

