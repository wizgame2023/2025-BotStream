/*!
@file CameraManager.cpp
@brief �J�����̐��䏈��
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	CameraManager::CameraManager(const shared_ptr<Stage>& stagePtr, float range, float targetRange) :
		MyGameObject(stagePtr),
		m_cameraAngle(XMConvertToRadians(270.0f)),
		m_range(range),
		m_targetRange(targetRange),
		m_lockOn(false),
		m_lockOnNum(-1)
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


		//���b�N�I���̗L���͈͂�����
		stage->AddGameObject<LockOnRange>(m_targetRange * 2, player);

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


		//���b�N�I������
		auto enemyManager = stage->GetSharedGameObject<EnemyManager>(L"EnemyManager");
		//������shared_ptr��weak_ptr�ɂ������񂾂��ǂǂ�����΂����񂾂낤�H
		vector<shared_ptr<Enemy>> enemyVec = enemyManager->GetEnemyVec(true);//�܂��A�����Ă����Ԃ�Enemy���󂯎��
		vector<shared_ptr<Enemy>> targetVec;//���b�N�I�����z��
		//shared_ptr<Enemy> targetObj;//�^�[�Q�b�g�Ώ�

		//Enemy��Player�Ƃ̋����𑪂��ă��b�N�I�����ɂȂ�̂��m�F����
		for (auto enemy : enemyVec)
		{
			Vec3 enemyPos = enemy->GetComponent<Transform>()->GetPosition();
			
			//���b�N�I���͈͂̉~�̒��ɂ��邩�v�Z����
			float targetRange = (enemyPos.x - playerPos.x) * (enemyPos.x - playerPos.x) +
								(enemyPos.z - playerPos.z) * (enemyPos.z - playerPos.z);
			float radiusRange = m_targetRange * m_targetRange;

			//���b�N�I���Ώۂ�������z��Ɋi�[����
			if (targetRange <= radiusRange)
			{
				if (!enemy->FindTag(L"���b�N�I�����"))
				{
					stage->AddGameObject<LockOnLook>(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), enemy, Vec3(0.0f, 4.0f, 0.0f));
				}

				targetVec.push_back(enemy);
				enemy->AddTag(L"���b�N�I�����");//���b�N�I�����̃^�O�ǉ�
			}
			else if(enemy->FindTag(L"���b�N�I�����"))
			{
				enemy->RemoveTag(L"���b�N�I�����");
			}
		}

		//���b�N�I����₪���Ȃ��Ȃ烍�b�N�I���ł��Ȃ����I����������
		if (targetVec.size() <= 0)
		{
			for (auto enemy : enemyVec)
			{
				enemy->RemoveTag(L"���b�N�I���Ώ�");
			}
			m_lockOn = false;
			m_lockOnNum = -1;
			m_targetObj = NULL;
		}

		float playerAngle = player->GetAngle();
		//RB�{�^���������Ɣ͈͓��ɑΏۂ�����Ȃ烍�b�N�I�������łȂ����Player�������Ă�������ɉ�]����
		if (m_controler.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			for (auto enemy : enemyVec)
			{
				enemy->RemoveTag(L"���b�N�I���Ώ�");
			}

			//�����ŒN�����b�N�I���Ώۂɂ��邩���߂�
			if (targetVec.size() > 0)
			{
				m_lockOnNum++;

				//�I�����Ă��鐔�l�����b�N�I�����̐����傫���Ȃ�Ȃ��悤�ɂ���
				if (m_lockOnNum > targetVec.size() - 1)
				{
					m_lockOnNum = targetVec.size() - 1;
				}
			}
			if (targetVec.size() <= 0)
			{
				//Player�̌����Ă�������̋����킹�ɂȂ�悤�Ɋp�x��ύX����
				MovePlayerAngle(playerAngle);
			}

			//���b�N�I�����Ă��������f����
			if (m_lockOnNum >= targetVec.size() - 1 && m_lockOnNum >= 0)
			{
				m_lockOn = true;
				m_targetObj = targetVec[m_lockOnNum];
				targetVec[m_lockOnNum]->AddTag(L"���b�N�I���Ώ�");
			}

		}
		//�p�x���Z�b�g(�f�o�b�N�p)
		if (m_controler.wPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
			m_cameraAngle = XMConvertToRadians(270.0f);
		}

		//���b�N�I��
		if (m_lockOn)
		{
			LockOn(m_targetObj, player);
		}//���b�N�I���ł��Ȃ���
		else if (!m_lockOn)
		{
			//�����_��Player�̈ʒu����������ɂ�����
			m_lockStageCamera->SetAt(playerPos);
		}






		//�J������Player�ɒǏ]
		m_lockStageCamera->SetEye(Vec3(playerPos.x + (cos(m_cameraAngle) * m_range), playerPos.y + 10.0f, playerPos.z + (sin(m_cameraAngle) * m_range)));

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
	void CameraManager::LockOn(shared_ptr<GameObject> lockOnObj,shared_ptr<Player> originObj)
	{
		Vec3 lockOnPos = lockOnObj->GetComponent<Transform>()->GetPosition();
		Vec3 originPos = originObj->GetComponent<Transform>()->GetPosition();

		//���b�N�I���Ώۂ�Player�̋����x�N�g���𑪂��Ċp�x�擾
		Vec3 lockOnVec = lockOnPos - originPos;
		auto m_targetRad = atan2(lockOnVec.z, lockOnVec.x);

		//�v�Z�����p�x�����ăJ��������񂳂���
		m_cameraAngle = m_targetRad + XMConvertToRadians(180.0f);

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

	//���b�N�I���͈͂�m�点�邽�߂̃I�u�W�F�N�g
	//�������@���b�N�I���L���͈�,�������@���b�N�I���͈͂̒��S�ƂȂ�I�u�W�F�N�g
	LockOnRange::LockOnRange(const shared_ptr<Stage>& stagePtr,float range,shared_ptr<Player> player):
		ObjectMove(stagePtr),
		m_pos(Vec3(0.0f)),
		m_rot(Vec3(0.0f)),
		m_scale(Vec3(range, 0.1f, range)),
		m_range(range),
		m_player(player)
	{

	}
	LockOnRange::~LockOnRange()
	{

	}

	void LockOnRange::OnCreate()
	{
		//Transform�ݒ�
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

		//�h���[���b�V���̐ݒ�
		m_ptrDraw = AddComponent<PNTStaticDraw>();
		m_ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		m_ptrDraw->SetOwnShadowActive(false);//�e�͏���
		m_ptrDraw->SetDrawActive(true);

	}

	void LockOnRange::OnUpdate()
	{
		float delta = App::GetApp()->GetElapsedTime();
		auto stage = GetStage();
		auto playerLock = m_player.lock();

		//Player�������Ă����玩����������
		if (!playerLock)
		{
			GetStage()->RemoveGameObject<LockOnRange>(GetThis<LockOnRange>());
			return;
		}

		//����Player�ɂ��Ă����悤�ɂ���
		Vec3 PlayerPos = playerLock->GetComponent<Transform>()->GetPosition();
		m_pos = PlayerPos;
		m_pos.y = PlayerPos.y - 0.5f;
		m_trans->SetPosition(m_pos);
		
	}

	//���b�N�I�����Ă���Ώۂ𕪂���悤�ɂ��鏈��
	LockOnLook::LockOnLook(const shared_ptr<Stage>& stagePtr,Vec3 rot, Vec3 scale,weak_ptr<Actor> parentObj,Vec3 pushPos) :
		ObjectMove(stagePtr),
		m_rot(rot),
		m_scale(scale),
		m_parentObj(parentObj),
		m_pushPos(pushPos)
	{

	}
	LockOnLook::~LockOnLook()
	{

	}

	void LockOnLook::OnCreate()
	{
		auto parentLock = m_parentObj.lock();
		auto parentPos = parentLock->GetComponent<Transform>()->GetPosition();

		//Transform�ݒ�
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(parentPos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

		//�h���[���b�V���̐ݒ�
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CONE");
		ptrDraw->SetDiffuse(Col4(0.0f,1.0f,0.0f,1.0f));
		ptrDraw->SetOwnShadowActive(false);//�e�͏���
		ptrDraw->SetDrawActive(true);
	}
	void LockOnLook::OnUpdate()
	{
		auto parentLock = m_parentObj.lock();
		//�ǐՑΏۂ������Ă����玩����������
		if (!parentLock)
		{
			GetStage()->RemoveGameObject<LockOnLook>(GetThis<LockOnLook>());
			return;
		}
		//�����ŒǐՑΏۂ̃^�O�Ƀ��b�N�I���Ώۂ⃍�b�N�I�����Ȃǂ̃^�O���Ȃ���Ώ��������
		if (!parentLock->FindTag(L"���b�N�I�����"))
		{
			GetStage()->RemoveGameObject<LockOnLook>(GetThis<LockOnLook>());
			return;
		}


		if (parentLock->FindTag(L"���b�N�I���Ώ�"))
		{
			auto ptrDraw = GetComponent<PNTStaticDraw>();
			ptrDraw->SetDiffuse(Col4(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else
		{
			auto ptrDraw = GetComponent<PNTStaticDraw>();
			ptrDraw->SetDiffuse(Col4(0.0f, 1.0f, 0.0f, 1.0f));
		}
		//�ǐՑΏۂƓ������W�ɂ���(PushPos�Ƃ�����O����)
		auto parentPos = parentLock->GetComponent<Transform>()->GetPosition();
		m_trans->SetPosition(parentPos + m_pushPos);
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
