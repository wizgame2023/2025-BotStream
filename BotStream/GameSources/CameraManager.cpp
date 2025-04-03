/*!
@file CameraManager.cpp
@brief �J�����̐��䏈��
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	CameraManager::CameraManager(const shared_ptr<Stage>& stagePtr, float range, float targetRange,float meleeRange) :
		MyGameObject(stagePtr),
		m_cameraAngle(XMConvertToRadians(270.0f)),
		m_range(range),
		m_targetRange(targetRange),
		m_lockOnFlag(false),
		m_lockOnUse(false),
		m_lockOnNum(-1),
		m_meleeRange(meleeRange),
		m_stickFlag(false)
	{

	}
	CameraManager::~CameraManager()
	{

	}

	//�쐬
	void CameraManager::OnCreate()
	{
		//�X�e�[�W��Ŏg����J�������擾����
		m_stage = GetStage();
		m_stageCamera = OnGetDrawCamera();//�X�e�[�W�̃J�����擾
		m_lockStageCamera = m_stageCamera.lock();	

		//�J������Player�̋����x�N�g���𑪂��đ傫�������߂�
		auto player = m_stage->GetSharedGameObject<Player>(L"Player");
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
		m_stage->AddGameObject<LockOnRange>(m_targetRange, player);

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
		m_delta = App::GetApp()->GetElapsedTime();
		m_lockStageCamera = m_stageCamera.lock();
		//shared_ptr<Stage> stage = GetStage();

		//�����X�e�[�W�p�̃J�������擾�ł��Ȃ�������return���Ď������폜���܂�
		if (!m_lockStageCamera)
		{
			GetStage()->RemoveGameObject<CameraManager>(GetThis<CameraManager>());
			return;
		}


		auto player = m_stage->GetSharedGameObject<Player>(L"Player");
		Vec3 playerPos = player->GetComponent<Transform>()->GetPosition();

		Vec3 hitPos;//���������ꏊ��ۑ�����ϐ�
		//�����蔻��e�X�g
		TRIANGLE testTriangle;
		size_t testsize;
		//m_ptrDraw->HitTestStaticMeshSegmentTriangles(playerPos, m_lockStageCamera->GetEye(), hitPos,testTriangle,testsize);

		// �C���v�b�g�f�o�C�X�I�u�W�F�N�g
		InputDevice inputDevice = App::GetApp()->GetInputDevice(); // �l�X�ȓ��̓f�o�C�X���Ǘ����Ă���I�u�W�F�N�g���擾
		//�R���g���[���[�̎擾
		m_controler = inputDevice.GetControlerVec()[0];
		Vec2 contrloerVec = Vec2(m_controler.fThumbRX, m_controler.fThumbRY);

		//���X�e�B�b�N���X���ăJ������Player�̎������]���鏈��
		float addAngle = 3.0f * contrloerVec.x * m_delta;//�ǉ�����p�x�����߂�
		m_cameraAngle += -addAngle;//�ǉ�

		//���b�N�I������
		auto enemyManager = m_stage->GetSharedGameObject<EnemyManager>(L"EnemyManager");
		//������shared_ptr��weak_ptr�ɂ������񂾂��ǂǂ�����΂����񂾂낤�H
		vector<shared_ptr<Enemy>> enemyVec = enemyManager->GetEnemyVec(true);//�܂��A�����Ă����Ԃ�Enemy���󂯎��
		//vector<shared_ptr<Enemy>> targetVec;//���b�N�I�����z��

		//���b�N�I�����͂ǂ̃I�u�W�F�N�g�B�ɂȂ�̂�����
		LockOnCandidate(enemyVec, playerPos);

		//���b�N�I����₪���Ȃ��Ȃ烍�b�N�I���ł��Ȃ����I����������
		if (m_targets.size() <= 0)
		{
			LockOff(enemyVec);//���b�N�I���̉���
		}

		ObjectFactory::Create<Cube>(GetStage(), Vec3(-10.0f, 0.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Col4(0.0f, 1.0f, 0.0f, 1.0f));

		float playerAngle = player->GetAngle();
		//RB�{�^�����������̏����@
		//���b�N�I�����o���ă��b�N�I���̂��ĂȂ��Ȃ�g���A�g���Ă���g��Ȃ� �ǂ���ł��Ȃ���΂����łȂ��Ȃ�v���C���[�̌����Ă�������Ɍ���
		if (m_controler.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			for (auto enemy : enemyVec)
			{
				enemy->RemoveTag(L"���b�N�I���Ώ�");
			}

			//���b�N�I�����邩���߂鏈��
			if (m_lockOnFlag && !m_lockOnUse)
			{
				m_lockOnNum++;
				m_lockOnUse = true;//���b�N�I���g�p
				m_targetObj = m_targets[m_lockOnNum];
				m_targets[m_lockOnNum]->AddTag(L"���b�N�I���Ώ�");
			}
			else if (m_lockOnFlag && m_lockOnUse)
			{
				LockOff(enemyVec);//���b�N�I���̉���
			}
			else if (!m_lockOnFlag)
			{
				//Player�̌����Ă�������̋����킹�ɂȂ�悤�Ɋp�x��ύX����
				MovePlayerAngle(playerAngle);
			}
		}

		//�p�x���Z�b�g(�f�o�b�N�p)
		if (m_controler.wPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
			m_cameraAngle = XMConvertToRadians(270.0f);
		}


		//���b�N�I������Ƃ��̏���
		if (m_lockOnUse)
		{
			//���b�N�I���g�p���ɒN�����b�N�I������̂�����
			//�^�[�Q�b�g�Ώۂ���X�e�B�b�N���X���Ă�������̃^�[�Q�b�g���ɕς��
			if (contrloerVec.x <= -0.9f && m_stickFlag)//�^�[�Q�b�g�̍��ׂɂ�����Ɉڂ�
			{
				//�O�p�̍����}�C�i�X�ň�ԋ߂��^�[�Q�b�g��₪�^�[�Q�b�g�ΏۂƂȂ�
				auto targetAngle = m_targesDeta[m_lockOnNum].lockOnAngle;
				float targetdeg = XMConvertToDegrees(targetAngle);
				float min = 999999;
				for (int i = 0; i <= m_targesDeta.size()-1; i++)
				{		
					//�����Ă���^�[�Q�b�g��₪�����ɂ���킯�łȂ����return
					if (m_targesDeta[i].leftOrRight != Left) continue;
					m_targets;
					float targetsDeg = XMConvertToDegrees(m_targesDeta[i].lockOnAngle);
					float difference = abs(m_targesDeta[i].lockOnAngle - targetAngle);

					if (min > difference && difference != 0.0f)
					{
						min = difference;
						m_targetObj->RemoveTag(L"���b�N�I���Ώ�");
						m_lockOnNum = i;
					}
				}

				m_stickFlag = false;//�X�e�B�b�N���󂯎��Ȃ��悤�ɂ���
			}
			if (contrloerVec.x >= 0.9f && m_stickFlag)//�^�[�Q�b�g�̉E�ׂɂ�����Ɉڂ�
			{
				//�O�p�̍����v���X�ň�ԋ߂��^�[�Q�b�g��₪�^�[�Q�b�g�ΏۂƂȂ�
				auto targetAngle = m_targesDeta[m_lockOnNum].lockOnAngle;
				float targetdeg = XMConvertToDegrees(targetAngle);
				float min = 999999;
				for (int i = 0; i <= m_targesDeta.size() - 1; i++)
				{
					//�����Ă���^�[�Q�b�g��₪�����ɂ���킯�łȂ����return
					if (m_targesDeta[i].leftOrRight != Right) continue;

					float targetsDeg = XMConvertToDegrees(m_targesDeta[i].lockOnAngle);
					float difference = abs(m_targesDeta[i].lockOnAngle-targetAngle);

					if (min > difference && difference != 0.0f)
					{
						min = difference;
						m_targetObj->RemoveTag(L"���b�N�I���Ώ�");
						m_lockOnNum = i;
					}
				}

				m_stickFlag = false;//�X�e�B�b�N���󂯎��Ȃ��悤�ɂ���
			}

			//�X�e�B�b�N���X������X�e�B�b�N�����ɖ߂�������͂��󂯓����
			if (!m_stickFlag && contrloerVec.x == 0.0f)
			{
				m_stickFlag = true;
			}

			//���b�N�I�����Ă��������f����
			if (m_lockOnNum >= 0)
			{
				m_lockOnUse = true;//���b�N�I���g�p
				m_targetObj = m_targets[m_lockOnNum];
				m_targets[m_lockOnNum]->AddTag(L"���b�N�I���Ώ�");
			}

			//�J������Pos
			auto cameraPos = m_lockStageCamera->GetEye();

			//���b�N�I����؂�ւ���Ƃ��ɃX�e�B�b�N���X���ă��b�N�I���Ώۂ�ύX���鏈��
			Vec3 targetPos = m_targetObj->GetComponent<Transform>()->GetPosition();
			//Player���猩�ă��b�N�I���Ώۂ̂�������x�N�g�������߂�
			Vec3 lockOnVec = targetPos - playerPos;
			
			m_targesDeta.clear();//������
			//���̑O�ɑO������Ƃ��ă��b�N�I����₪���Ă����������E�������m�F����
			for (auto enemy : m_targets)
			{
				Vec3 enemyPos = enemy->GetComponent<Transform>()->GetPosition();

				//Player���猩�ă��b�N�I�����̂�������x�N�g�������߂�
				Vec3 lockOnCanVec = enemyPos - playerPos;

				//�O�ς����߂�
				Vec3 outsideCorner = Vec3(lockOnVec.y * 0.0f - lockOnCanVec.y * 0.0f,//x
									lockOnVec.z * lockOnCanVec.x - lockOnCanVec.z * lockOnVec.x,//y
									lockOnVec.x * 0.0f - lockOnCanVec.x * 0.0f//z
									);
				//�����}�C�i�X�E���v���X
				auto cross = outsideCorner.y;	
				//cross��0���傫��������false�t�ɏ�����������true
				int leftOrRight;
				if (cross < 0) leftOrRight = Left;
				if (cross > 0) leftOrRight = Right;
				if (cross == 0) leftOrRight = Middle;

				//�Ȃ��p�����߂�
				float aSqrt = sqrt((lockOnVec.x*lockOnVec.x)+(lockOnVec.z*lockOnVec.z));
				float bSqrt = sqrt((lockOnCanVec.x*lockOnCanVec.x)+(lockOnCanVec.z*lockOnCanVec.z));
				//�����鐔�l
				float divide = (lockOnVec.x * lockOnCanVec.x) + (lockOnVec.z * lockOnCanVec.z);

				auto rad = divide / (aSqrt * bSqrt);
				auto deg = XMConvertToDegrees(rad);

				targetsDeta targetDeta = targetsDeta(deg, leftOrRight);
				//�v���C���[���猩�ēG�̂���p�x��z��ɓ����
				m_targesDeta.push_back(targetDeta);
			}

			LockOn(m_targetObj, player);//���b�N�I��
		}//���b�N�I���ł��Ȃ���
		else if (!m_lockOnUse)
		{
			//�����_��Player�̈ʒu����������ɂ�����
			m_lockStageCamera->SetAt(playerPos);
		}






		//�J������Player�ɒǏ]
		m_lockStageCamera->SetEye(Vec3(playerPos.x + (cos(m_cameraAngle) * m_range), playerPos.y + 10.0f+10.0f, playerPos.z + (sin(m_cameraAngle) * m_range)));

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

			//if (m_lockOnNum >= 0)
			//{		
			//	auto targetAngle = m_lockOnAngle[m_lockOnNum];
			//	float a = targetAngle;
	
			//	wss << L"���b�N�I���p�x " << XMConvertToDegrees(targetAngle);
			//}

		scene->SetDebugString(wss.str());

	}

	//���b�N�I���������߂�֐�
	void CameraManager::LockOnCandidate(vector<shared_ptr<Enemy>> enemyVec,Vec3 playerPos)
	{
		m_targets.clear();//�z��̏�����
		m_lockOnFlag = false;//�t���O�̏�����

		for (auto enemy : enemyVec)
		{
			Vec3 enemyPos = enemy->GetComponent<Transform>()->GetPosition();

			//���b�N�I���͈͂̉~�̒��ɂ��邩�v�Z����
			float targetRange = (enemyPos.x - playerPos.x) * (enemyPos.x - playerPos.x) +
				(enemyPos.z - playerPos.z) * (enemyPos.z - playerPos.z);
			float radiusRange = m_targetRange * m_targetRange;

			//���b�N�I�����Ȃ�z��ɃI�u�W�F�N�g��Pos������
			if (targetRange <= radiusRange)
			{
				if (!enemy->FindTag(L"���b�N�I�����"))
				{
					m_stage->AddGameObject<LockOnLook>(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), enemy, Vec3(0.0f, 4.0f, 0.0f));
				}

				m_targetsPos.push_back(enemyPos);//���b�N�I������Pos��z��ɓ����
				m_targets.push_back(enemy);
				enemy->AddTag(L"���b�N�I�����");//���b�N�I�����̃^�O�ǉ�
				m_lockOnFlag = true;//���b�N�I���g�p�\
			}
			else if (enemy->FindTag(L"���b�N�I�����"))//���b�N�I����₩��O�ꂽ�̂����b�N�I���ΏۂȂ烍�b�N�I������߂�
			{
				enemy->RemoveTag(L"���b�N�I�����");
				if (enemy->FindTag(L"���b�N�I���Ώ�"))
				{
					LockOff(enemyVec);//���b�N�I���̉���
				}
			}

			//auto  = cameraPos - enemyPos

		}

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

	//���b�N�I���̉����@�\
	void CameraManager::LockOff(vector<shared_ptr<Enemy>> enemyVec)
	{
		for (auto enemy : enemyVec)
		{
			enemy->RemoveTag(L"���b�N�I���Ώ�");
		}
		m_lockOnFlag = false;//���b�N�I���ł��Ȃ�
		m_lockOnUse = false;//���b�N�I�����Ȃ�
		m_lockOnNum = -1;
		m_targetObj = NULL;
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
		m_scale(Vec3(range*2, 1.0f, range*2)),
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

		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(XMConvertToRadians(180.0f), 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//Transform�ݒ�
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(parentPos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

		//�h���[���b�V���̐ݒ�
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CONE");
		ptrDraw->SetDiffuse(Col4(0.0f,1.0f,0.0f,1.0f));
		ptrDraw->SetMeshToTransformMatrix(spanMat);
		ptrDraw->SetOwnShadowActive(false);//�e�͏���
		ptrDraw->SetDrawActive(true);
		ptrDraw->SetEmissive(Col4(0.0f, 1.0f, 0.0f, 1.0f)); // ���Ȕ����J���[�i���C�e�B���O�ɂ��A�e���������ʂ�����j
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
			ptrDraw->SetEmissive(Col4(1.0f, 0.0f, 0.0f, 1.0f)); // ���Ȕ����J���[�i���C�e�B���O�ɂ��A�e���������ʂ�����j
		}
		else
		{
			auto ptrDraw = GetComponent<PNTStaticDraw>();
			ptrDraw->SetDiffuse(Col4(0.0f, 1.0f, 0.0f, 1.0f));
			ptrDraw->SetEmissive(Col4(0.0f, 1.0f, 0.0f, 1.0f)); // ���Ȕ����J���[�i���C�e�B���O�ɂ��A�e���������ʂ�����j
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
		ptrDraw->SetEmissive(m_color); // ���Ȕ����J���[�i���C�e�B���O�ɂ��A�e���������ʂ�����j
		ptrDraw->SetOwnShadowActive(true); // �e�̉f�荞�݂𔽉f������

		////�e��t����
		//auto m_ptrShadow = AddComponent<Shadowmap>();
		//m_ptrShadow->SetMeshResource(L"DEFAULT_CUBE");
		////m_ptrShadow->SetMeshToTransformMatrix(spanMat);


		//ptrDraw->HitTestSkinedMeshSegmentTriangles();

		//ptrDraw->HitT
	}



}
//end basecross
