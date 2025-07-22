/*!
@file CameraState.cpp
@brief �J���������̃X�e�[�g
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	/// <summary>
	/// �J�����̃X�e�[�g�̌��ƂȂ�N���X
	/// </summary>
	void CameraStateBase::Enter()
	{
		//�R���g���[���[���󂯎��
		auto inputDevice = App::GetApp()->GetInputDevice();
		m_controller = inputDevice.GetControlerVec()[0];
		m_controllerVec = Vec2(m_controller.fThumbRX, m_controller.fThumbRY);
	}
	
	void CameraStateBase::Update(float deltaTime)
	{
		//�ߐڐ퓬���Ă������̃t���O���󂯎��
		m_meleeFlag = m_cameraManager->GetMeleeFlag();
		//���݂ǂ̍U�������Ă��邩�󂯎��
		m_gunNow = m_cameraManager->GetGunNow();
		m_meleeNow = m_cameraManager->GetMeleeNow();

		//�R���g���[���[���󂯎��
		auto inputDevice = App::GetApp()->GetInputDevice();
		m_controller = inputDevice.GetControlerVec()[0];
		m_controllerVec = Vec2(m_controller.fThumbRX, m_controller.fThumbRY);

		//�X�e�[�W�擾
		m_stage = m_cameraManager->GetStage();

		////�����t���̉�]����
		//m_cameraManager->InertialRotation();
		//X����]�̐�������
		m_cameraManager->CameraAngleXLimit(XMConvertToRadians(120.0f), XMConvertToRadians(60.0f));

		//�p�x�̒���0~360�x�܂ł����o�Ȃ��悤�ɂ���
		m_cameraManager->AdjustmentAngle();
		////�J�����̈ʒu�X�V
		//m_cameraManager->CameraPosUpdate();
	}

	void CameraStateBase::Exit()
	{

	}
	//////////////////////////////////////////////////


	///�J�����̒ʏ��Ԃ̃X�e�[�g(�ߐڃ��[�h)
	void CameraNomalState::Enter()
	{
		CameraStateBase::Enter();
	}

	void CameraNomalState::Update(float deltaTime)
	{
		CameraStateBase::Update(deltaTime);

		//�J�����̈ʒu�X�V
		m_cameraManager->CameraPosUpdate(8, 0, 17.0f, 100.0f,CameraManager::CameraMoveMode::DirectMove);

		//�ʏ탂�[�h���̃J�������쏈��
		m_cameraManager->CameraControlNomalMode();
		//�����t���̉�]����
		m_cameraManager->InertialRotation();

		//���݃X�e�[�g��Ԃ�n��
		m_cameraManager->SetStateMode(CameraManager::DebagState::Normal);

		//����,LB�{�^���������Ă�����e�X�e�[�g�Ɉڍs����
		if (!m_meleeNow)
		{
			if (!m_meleeFlag)
			{
				m_cameraManager->ChangeState(L"NormalToGun");
			}
		}

		//�E�X�e�B�b�N����������A�ړ��X�e�[�g�Ɉڍs����
		if (m_controller.wPressedButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
		{
			m_cameraManager->ChangeState(L"Reset");
		}
	}

	void CameraNomalState::Exit()
	{

	}
	//


	//�J�����̏e���[�h�̃X�e�[�g
	void CameraGunState::Enter()
	{
		CameraStateBase::Enter();
	}

	void CameraGunState::Update(float deltaTime)
	{	
		CameraStateBase::Update(deltaTime);

		//����,LB�{�^���������Ă��Ȃ�������ʏ탂�[�h�ɖ߂�
		if (m_meleeFlag)
		{
			m_cameraManager->ChangeState(L"GunToNormal");
			return;
		}

		//���݃X�e�[�g��Ԃ�n��
		m_cameraManager->SetStateMode(CameraManager::DebagState::Gun);

		//�J�����̈ʒu�X�V
		m_cameraManager->CameraPosUpdate(3.0f, 3.0f, 8.0f,100.0f, CameraManager::CameraMoveMode::DirectMove);
		//�����t���̉�]����
		m_cameraManager->InertialRotation(0.7f, 60.0f);

		//�e���[�h���̃J�������쏈��
		m_cameraManager->CameraControlShotMode();
	}

	void CameraGunState::Exit()
	{

	}
	//


	//�J�����̃��Z�b�g���[�h�̃X�e�[�g
	void CameraResetState::Enter()
	{
		CameraStateBase::Enter();
	}

	void CameraResetState::Update(float deltaTime)
	{
		CameraStateBase::Update(deltaTime);

		//�J�����̈ʒu�X�V
		m_cameraManager->CameraPosUpdate();

		//�ړ����[�h���̃J�������쏈��
		m_cameraManager->CameraControlTransitionMode();
	}

	void CameraResetState::Exit()
	{

	}
	//


	//�J�������ʏ킩��e���[�h�ɐ؂�ւ��邽�߂̈ړ��X�e�[�g
	void CameraNormalToGunState::Enter()
	{
		CameraStateBase::Enter();
	}

	void CameraNormalToGunState::Update(float deltaTime)
	{
		CameraStateBase::Update(deltaTime);

		//�����ALB�{�^���������Ă��Ȃ�������ʏ탂�[�h�Ɉڍs���悤�Ƃ���
		if (m_meleeFlag)
		{
			m_cameraManager->ChangeState(L"GunToNormal");
			return;
		}

		//���݃X�e�[�g��Ԃ�n��
		m_cameraManager->SetStateMode(CameraManager::DebagState::NormalToGun);

		//�J�����̈ʒu�X�V
		auto moveEnd = m_cameraManager->CameraPosUpdate(3.0f, 3.0f, 8.0f, 40.0f);
		//�����t���̉�]����
		m_cameraManager->InertialRotation(0.7f, 60.0f);
		//�e���[�h���̃J�������쏈��
		m_cameraManager->CameraControlShotMode();

		//�ړ��������I�������X�e�[�g���e���[�h�ɕύX����
		if (moveEnd)
		{
			m_cameraManager->ChangeState(L"Gun");
		}
	}

	void CameraNormalToGunState::Exit()
	{

	}
	//


	//�e���[�h����ʏ탂�[�h�ɖ߂鏈��
	void CameraGunToNormalState::Enter()
	{

	}

	void CameraGunToNormalState::Update(float deltaTime)
	{
		CameraStateBase::Update(deltaTime);

		//�����ALB�{�^���������Ă�����e���[�h�Ɉڍs���悤�Ƃ���
		if (!m_meleeFlag)
		{
			m_cameraManager->ChangeState(L"NormalToGun");
			return;
		}

		//���݃X�e�[�g��Ԃ�n��
		m_cameraManager->SetStateMode(CameraManager::DebagState::GunToNormal);

		//�J�����̈ʒu�X�V
		auto moveEnd = m_cameraManager->CameraPosUpdate(8.0f, 0.0f, 17.0f, 40.0f);

		//�e���[�h���̃J�������쏈��
		m_cameraManager->CameraControlNomalMode();

		//�ړ��������I�������X�e�[�g���e���[�h�ɕύX����
		if (moveEnd)
		{
			m_cameraManager->ChangeState(L"Normal");
		}
	}

	void CameraGunToNormalState::Exit()
	{

	}

	//���[�r�[�p�̃X�e�[�g(�X�e�[�W�J�n����i�K��)//
	void CameraStartMovieState_First::Enter()
	{
		CameraStateBase::Enter();

		auto player = m_cameraManager->GetStage()->GetSharedGameObject<Player>(L"Player");
		auto playerPos = player->GetPosition();

		//�J�����̃|�W�V��������
		//Player��肿����ƑO�ɂ���
		m_cameraManager->SetCameraEye(Vec3(playerPos.x, 3.0f, playerPos.z + 10.0f));

	}

	void CameraStartMovieState_First::Update(float deltaTime)
	{
		//CameraStateBase::Update(deltaTime);

		//�ړ����[�h���̃J�������쏈��
		m_cameraManager->CameraControlStartMovieMode();

		//�p�x�̒���0~360�x�܂ł����o�Ȃ��悤�ɂ���
		m_cameraManager->AdjustmentAngle();
	}

	void CameraStartMovieState_First::Exit()
	{

	}
	//

	//���[�r�[�p�̃X�e�[�g(�X�e�[�W�J�n����i�K��)//
	void CameraStartMovieState_Second::Enter()
	{
		CameraStateBase::Enter();
		auto player = m_cameraManager->GetStage()->GetSharedGameObject<Player>(L"Player");
		auto playerPos = player->GetPosition();

		float pushY = 3.0f - playerPos.y;
		m_cameraManager->SetPushPos(Vec3(0.0f, pushY, 10.0f));
		m_cameraManager->SetPushStart(false);
	}

	void CameraStartMovieState_Second::Update(float deltaTime)
	{
		//CameraStateBase::Update(deltaTime);

		//�ړ����[�h���̃J�������쏈��
		m_cameraManager->CameraControlStartMovieMode();
		//�p�x�̒���0~360�x�܂ł����o�Ȃ��悤�ɂ���
		m_cameraManager->AdjustmentAngle();

		//�J�����̈ʒu�X�V
		auto moveEnd = m_cameraManager->CameraPosUpdate(8, 0, 17.0f,60.0f);

		//���[�r�[�p�R���g���[���[�擾
		auto RT_movieController = m_cameraManager->GetStage()->GetSharedGameObject<RT_MovieController>(L"MovieController");

		//�ړ��������I������炻�̂��Ƃ����[�r�[�R���g���[���[�ɓ`���Ēʏ��Ԃɖ߂�
		if (moveEnd)
		{
			RT_movieController->SetCameraEnd(true);
			m_cameraManager->ChangeState(L"Normal");
		}
	}

	void CameraStartMovieState_Second::Exit()
	{

	}
	//


	//�{�X���[�r�[�p�̃J�����X�e�[�g(��Ԗ�)//
	void CameraBossMovieState_First::Enter()
	{
		//�{�X���擾
		auto boss = m_cameraManager->GetStage()->GetSharedGameObject<EnemyBase>(L"Boss");
		m_boss = boss;

	}
	
	void CameraBossMovieState_First::Update(float deltaTime)
	{
		auto lockBoss = m_boss.lock();
		// ���b�N�o���Ȃ�������return
		if (!lockBoss) return;

		auto bossPos = lockBoss->GetPosition();

		//�{�X�̍���������
		m_cameraManager->SetCameraEye(bossPos + Vec3(12.0f, 5.0f, -5.0f));
		m_cameraManager->SetCameraAt(bossPos+Vec3(2.0f,-3.0f,0.0f));

		m_timeCount += deltaTime;
		
		if (m_timeCount > m_timeMax)
		{
			//��Ԗڂ̃X�e�[�g�Ɉڍs����
			m_cameraManager->ChangeState(L"BossMovie_Second");
		}

	}

	void CameraBossMovieState_First::Exit()
	{
		//���Z�b�g
		m_timeCount = 0.0f;
	}
	//

	//�{�X���[�r�[�p�̃J�����X�e�[�g(��Ԗ�)//
	void CameraBossMovieState_Second::Enter()
	{
		//�{�X���擾
		auto boss = m_cameraManager->GetStage()->GetSharedGameObject<EnemyBase>(L"Boss");
		m_boss = boss;

	}

	void CameraBossMovieState_Second::Update(float deltaTime)
	{
		auto lockBoss = m_boss.lock();
		// ���b�N�o���Ȃ�������return
		if (!lockBoss) return;

		auto bossPos = lockBoss->GetPosition();

		//�{�X�̍���������
		m_cameraManager->SetCameraEye(bossPos + Vec3(-12.0f, 5.0f, -5.0f));
		m_cameraManager->SetCameraAt(bossPos + Vec3(2.0f, -3.0f, 0.0f));

		m_timeCount += deltaTime;

		if (m_timeCount > m_timeMax)
		{
			//�O�Ԗڂ̃X�e�[�g�Ɉڍs����
			m_cameraManager->ChangeState(L"BossMovie_Three");
		}

	}

	void CameraBossMovieState_Second::Exit()
	{
		//���Z�b�g
		m_timeCount = 0.0f;
	}
	//


	//�{�X���[�r�[�p�̃J�����X�e�[�g(�O�Ԗ�)//
	void CameraBossMovieState_Three::Enter()
	{
		//�{�X���擾
		auto boss = m_cameraManager->GetStage()->GetSharedGameObject<EnemyBase>(L"Boss");
		m_boss = boss;
		// �{�X�͑ҋ@��ԂɂȂ�
		boss->ChangeAnim(L"Idle");

	}

	void CameraBossMovieState_Three::Update(float deltaTime)
	{
		auto lockBoss = m_boss.lock();
		// ���b�N�o���Ȃ�������return
		if (!lockBoss) return;

		auto bossPos = lockBoss->GetPosition();

		//�{�X�̍���������
		m_cameraManager->SetCameraEye(bossPos + Vec3(0.0f, 2.0f, -20.0f));
		m_cameraManager->SetCameraAt(bossPos + Vec3(0.0f, 0.0f, 0.0f));

		m_timeCount += deltaTime;

		if (m_timeCount > m_timeMax)
		{
			//�l�Ԗڂ̃X�e�[�g�Ɉڍs����
			m_cameraManager->ChangeState(L"BossMovie_Fourth");
		}

	}

	void CameraBossMovieState_Three::Exit()
	{
		//���Z�b�g
		m_timeCount = 0.0f;
	}
	//


	//�{�X���[�r�[�p�̃J�����X�e�[�g(�l�Ԗ�)//
	void CameraBossMovieState_Fourth::Enter()
	{
		//�{�X���擾
		auto boss = m_cameraManager->GetStage()->GetSharedGameObject<EnemyBase>(L"Boss");
		m_boss = boss;

		auto playerPos = m_cameraManager->GetStage()->GetSharedGameObject<Player>(L"Player")->GetPosition();
		m_cameraManager->SetPushPos(m_cameraManager->GetCameraEye() - playerPos);

		//�A�j���[�V�����ύX
		boss->ChangeAnim(L"Bonus");
	}

	void CameraBossMovieState_Fourth::Update(float deltaTime)
	{
		auto lockBoss = m_boss.lock();
		// ���b�N�o���Ȃ�������return
		if (!lockBoss) return;

		auto bossPos = lockBoss->GetPosition();

		//�{�X�̐��ʂ�����
		//m_cameraManager->SetCameraEye(bossPos + Vec3(0.0f, 2.0f, -20.0f));
		m_cameraManager->SetCameraAt(bossPos + Vec3(0.0f, 0.0f, 0.0f));

		m_timeCount += deltaTime;

		//�J�����̈ʒu�X�V
		auto moveEnd = m_cameraManager->CameraPosUpdate(8, 0, 17.0f,60.0f);
		m_cameraManager->SetPushStart(false);

		if (moveEnd)
		{
			m_cameraManager->ChangeState(L"Normal");
		}
	}

	void CameraBossMovieState_Fourth::Exit()
	{
		//���Z�b�g
		m_timeCount = 0.0f;

		m_cameraManager->GetStage()->GetSharedGameObject<RT_MovieController>(L"MovieController")->SetCameraEnd(true);
	}
	//


	//�{�X��|�����Ƃ��̃��[�r�[�p�̃J�����X�e�[�g(��Ԗ�)//
	void CameraBossDieMovieState_First::Enter()
	{
		//�{�X���擾
		auto boss = m_cameraManager->GetStage()->GetSharedGameObject<EnemyBase>(L"Boss");
		m_boss = boss;
		Vec3 fwd = boss->GetForward();
		m_bossAngle = atan2(fwd.z, fwd.x);
		//m_bossAngle = XMConvertToRadians(-90.0f);
		auto test = XMConvertToDegrees(m_bossAngle);

		auto playerPos = m_cameraManager->GetStage()->GetSharedGameObject<Player>(L"Player")->GetPosition();
		m_cameraManager->SetPushPos(m_cameraManager->GetCameraEye() - playerPos);

	}

	void CameraBossDieMovieState_First::Update(float deltaTime)
	{
		auto lockBoss = m_boss.lock();
		// ���b�N�o���Ȃ�������return
		if (!lockBoss) return;

		auto bossPos = lockBoss->GetPosition();

		//�{�X�̏�����̐��ʂ�����
		m_cameraManager->SetCameraEye(bossPos + Vec3(cos(m_bossAngle) * 15.0f, 5.0f,sin(m_bossAngle) * 15.0f));
		m_cameraManager->SetCameraAt(bossPos + Vec3(0.0f, 0.0f, 0.0f));

		m_timeCount += deltaTime;

		if (m_timeCount > m_timeMax)
		{
			//���̃X�e�[�g�Ɉڍs����
			m_cameraManager->ChangeState(L"Normal");
		}


		////�J�����̈ʒu�X�V
		//auto moveEnd = m_cameraManager->CameraPosUpdate(8, 0, 17.0f);
		//m_cameraManager->SetPushStart(false);

		//if (moveEnd)
		//{
		//	m_cameraManager->ChangeState(L"Normal");
		//}
	}

	void CameraBossDieMovieState_First::Exit()
	{
		//���Z�b�g
		m_timeCount = 0.0f;

		m_cameraManager->GetStage()->GetSharedGameObject<RT_MovieController>(L"MovieController")->SetCameraEnd(true);
	}
	//

}