/*!
@file CameraManager.cpp
@brief �J�����̐��䏈��
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	CameraManager::CameraManager(const shared_ptr<Stage>& stagePtr) :
		MyGameObject(stagePtr),
		m_cameraAngle(XMConvertToRadians(270.0f)),
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
		m_lockStageCamera = m_stageCamera.lock();	

		//�J������Player�̋����x�N�g���𑪂��đ傫�������߂�
		auto player = stage->GetSharedGameObject<Player>(L"Player");
		Vec3 playerPos = player->GetComponent<Transform>()->GetPosition();
		
		//�J������Player�ɒǏ]
		m_lockStageCamera->SetEye(Vec3(playerPos.x + (cos(m_cameraAngle) * m_range), playerPos.y + 10.0f, playerPos.z + (sin(m_cameraAngle) * m_range)));
		//�����_��Player�̈ʒu����������ɂ�����
		m_lockStageCamera->SetAt(playerPos);

		//Vec3 CameraPos = m_lockStageCamera->GetEye();
		
		////�N�H�[�^�j�I���̒l���擾���ăJ������Player�̍���������悤�ɂ���I�u�W�F�N�g���o�����I
		//Vec3 CameraVec = CameraPos - playerPos;
		//Vec3 CameraScale = CameraVec = Vec3(abs(CameraVec.x), abs(CameraVec.y), abs(CameraVec.z));
		//Vec3 up = Vec3(0, 1, 0);
		//Mat4x4 CameraQt = (Mat4x4)XMMatrixLookAtLH(CameraPos, playerPos, -up);
		
		////���C�L���X�g����
		//auto raycast = stage->AddGameObject<CameraRayCast>(Vec3(CameraVec.x/2, CameraVec.y / 2, -CameraVec.z / 2), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f,1.0f,CameraScale.z));
		//raycast->GetComponent<Transform>()->SetQuaternion((Quat)CameraQt);




		//�����X�e�[�W�p�̃J�������擾�ł��Ȃ�������return���Ď������폜���܂�
		if (!m_lockStageCamera)
		{
			GetStage()->RemoveGameObject<CameraManager>(GetThis<CameraManager>());
			return;
		}
	}

	//�X�V
	void CameraManager::OnUpdate()
	{
		auto delta = App::GetApp()->GetElapsedTime();
		m_lockStageCamera = m_stageCamera.lock();
		auto stage = GetStage();

		//�����X�e�[�W�p�̃J�������擾�ł��Ȃ�������return���Ď������폜���܂�
		if (!m_lockStageCamera)
		{
			GetStage()->RemoveGameObject<CameraManager>(GetThis<CameraManager>());
			return;
		}


		auto player = stage->GetSharedGameObject<Player>(L"Player");
		Vec3 playerPos = player->GetComponent<Transform>()->GetPosition();

		Vec3 hitPos;//���������ꏊ��ۑ�����ϐ�
		//�����蔻��e�X�g
		TRIANGLE testTriangle;
		size_t testsize;
		//m_ptrDraw->HitTestStaticMeshSegmentTriangles(playerPos, m_lockStageCamera->GetEye(), hitPos,testTriangle,testsize);


		// �C���v�b�g�f�o�C�X�I�u�W�F�N�g
		InputDevice inputDevice = App::GetApp()->GetInputDevice(); // �l�X�ȓ��̓f�o�C�X���Ǘ����Ă���I�u�W�F�N�g���擾
		//�R���g���[���[�̎擾
		CONTROLER_STATE m_controler = inputDevice.GetControlerVec()[0];
		Vec2 contrloerVec = Vec2(m_controler.fThumbRX, m_controler.fThumbRY);

		//���X�e�B�b�N���X���ăJ������Player�̎������]���鏈��
		float addAngle = 3.0f * contrloerVec.x * delta;//�ǉ�����p�x�����߂�
		m_cameraAngle += addAngle;//�ǉ�


		float playerAngle = player->GetAngle();
		//RB�{�^���������Ɣ͈͓��ɑΏۂ�����Ȃ烍�b�N�I�������łȂ����Player�������Ă�������ɉ�]����
		if (m_controler.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			//Player�̌����Ă�������̋����킹�ɂȂ�悤�Ɋp�x��ύX����
			MovePlayerAngle(playerAngle);
		}
		//�p�x���Z�b�g(�f�o�b�N�p)
		if (m_controler.wPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
			m_cameraAngle = XMConvertToRadians(270.0f);
		}


		//�J������Player�ɒǏ]
		m_lockStageCamera->SetEye(Vec3(playerPos.x + (cos(m_cameraAngle) * m_range), playerPos.y + 10.0f, playerPos.z + (sin(m_cameraAngle) * m_range)));
		//�����_��Player�̈ʒu����������ɂ�����
		m_lockStageCamera->SetAt(playerPos);

		//�p�x�̒���0~360�x�܂ł����o�Ȃ��悤�ɂ���
		AdjustmentAngle();


		////�f�o�b�N�p
		wstringstream wss(L"");
		auto scene = App::GetApp()->GetScene<Scene>();

		wss /* << L"�f�o�b�O�p������ "*/
			<< L"\nPlayer���猩�ăJ�����̊p�x: " << XMConvertToDegrees(m_cameraAngle)
			<< L"\nPlayer�̌����Ă���p�x: " << XMConvertToDegrees(-playerAngle)
			<< L"\n���������ꏊx: " << hitPos.x
			<< L"\n���������ꏊy: " << hitPos.y
			<< L"\n���������ꏊz: " << hitPos.z
			//<<L"\n�R���g���[���[�̓��� x:"<<contrloerVec.x<<L" y:"<<contrloerVec.y
			//<<L"\nFPS:"<< 1.0f/delta
			<< endl;

		scene->SetDebugString(wss.str());

	}

	//���b�N�I���@�\ �Ώۂ̃I�u�W�F�N�g�𒆐S�_�Ƃ���
	void CameraManager::LockOn(shared_ptr<GameObject> lockOnObj)
	{
		Vec3 lockOnPos = lockOnObj->GetComponent<Transform>()->GetPosition();

		m_lockStageCamera->SetAt(lockOnPos);
	}

	//Player�̌����Ă�������̋����킹�ɂȂ�悤�Ɋp�x��ύX����
	void CameraManager::MovePlayerAngle(float playerAngle)
	{
		m_cameraAngle = -playerAngle + XMConvertToRadians(180.0f);
	}

	//�p�x�̒���0~360�x�܂ł����o�Ȃ��悤�ɂ���
	void CameraManager::AdjustmentAngle()
	{
		if (m_cameraAngle >= XMConvertToRadians(360.0f))
		{
			m_cameraAngle -= XMConvertToRadians(360.0f);
		}
		else if (m_cameraAngle < XMConvertToRadians(0.0f))
		{
			m_cameraAngle += XMConvertToRadians(360.0f);
		}
	}


	//�J�����̃��C�L���X�g�p
	CameraRayCast::CameraRayCast(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale) :
		ObjectMove(stagePtr),
		m_pos(pos),
		m_rot(rot),
		m_scale(scale)
	{

	}
	CameraRayCast::~CameraRayCast()
	{

	}

	void CameraRayCast::OnCreate()
	{
		//Transform�ݒ�
		auto m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

		//�h���[���b�V���̐ݒ�
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetTextureResource(L"StoneBrick");
		//ptrDraw->SetDiffuse(m_color);
		//ptrDraw->SetOwnShadowActive(false);//�e�͏���
		ptrDraw->SetDrawActive(true);

	}
	void CameraRayCast::OnUpdate()
	{

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
		ptrDraw->SetDrawActive(true);
		//ptrDraw->HitTestSkinedMeshSegmentTriangles();

		//ptrDraw->HitT
	}



}
//end basecross
