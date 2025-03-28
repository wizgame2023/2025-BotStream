/*!
@file CameraManager.cpp
@brief �J�����̐��䏈��
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	CameraManager::CameraManager(const shared_ptr<Stage>& stagePtr) :
		MyGameObject(stagePtr),
		m_cameraAngle(XMConvertToRadians(-90.0f)),
		m_range(20.0f)
	{

	}
	CameraManager::~CameraManager()
	{

	}

	//�쐬
	void CameraManager::OnCreate()
	{
		//�X�e�[�W��Ŏg����J�������擾����
		auto stage = GetStage();
		m_stageCamera = OnGetDrawCamera();//�X�e�[�W�̃J�����擾
		auto lockStageCamera = m_stageCamera.lock();

		//�����X�e�[�W�p�̃J�������擾�ł��Ȃ�������return���Ď������폜���܂�
		if (!lockStageCamera)
		{
			GetStage()->RemoveGameObject<CameraManager>(GetThis<CameraManager>());
			return;
		}
	}

	//�X�V
	void CameraManager::OnUpdate()
	{
		auto delta = App::GetApp()->GetElapsedTime();
		auto lockStageCamera = m_stageCamera.lock();
		auto stage = GetStage();

		//�����X�e�[�W�p�̃J�������擾�ł��Ȃ�������return���Ď������폜���܂�
		if (!lockStageCamera)
		{
			GetStage()->RemoveGameObject<CameraManager>(GetThis<CameraManager>());
			return;
		}

		auto player = stage->GetSharedGameObject<Player>(L"Player");
		Vec3 playerPos = player->GetComponent<Transform>()->GetPosition();
		
		// �C���v�b�g�f�o�C�X�I�u�W�F�N�g
		auto inputDevice = App::GetApp()->GetInputDevice(); // �l�X�ȓ��̓f�o�C�X���Ǘ����Ă���I�u�W�F�N�g���擾
		//�R���g���[���[�̎擾
		auto m_controler = inputDevice.GetControlerVec()[0];
		Vec2 contrloerVec = Vec2(m_controler.fThumbRX, m_controler.fThumbRY);

		//���X�e�B�b�N���X���ăJ������Player�̎������]���鏈��
		float addAngle = 3.0f * contrloerVec.x * delta;//�ǉ�����p�x�����߂�
		m_cameraAngle += addAngle;//�ǉ�

		//RB�{�^���������Ɣ͈͓��ɑΏۂ�����Ȃ烍�b�N�I�������łȂ����Player�������Ă�������ɉ�]����
		if (m_controler.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			auto test = 0;
		}


		//�J������Player�ɒǏ]
		lockStageCamera->SetEye(Vec3(playerPos.x + (cos(m_cameraAngle) * m_range), playerPos.y + 10.0f, playerPos.z + (sin(m_cameraAngle) * m_range)));

		//�����_��Player�̈ʒu����������ɂ�����
		lockStageCamera->SetAt(playerPos);
	}


	//�f�o�b�N�p�̂����̎l�p���z
	Cube::Cube(const shared_ptr<Stage>& stagePtr,Vec3 pos,Vec3 rot, Vec3 scale,Col4 color) :
		ObjectNotMove(stagePtr),
		m_pos(pos),
		m_rot(rot),
		m_scale(scale),
		m_color(color)
	{

	}
	Cube::~Cube()
	{

	}

	void Cube::OnCreate()
	{
		//Transform�ݒ�
		auto m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

		//�h���[���b�V���̐ݒ�
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetDiffuse(m_color);
		ptrDraw->SetOwnShadowActive(false);//�e�͏���
	}



}
//end basecross
