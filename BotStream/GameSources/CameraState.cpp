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
		//�R���g���[���[���󂯎��
		auto inputDevice = App::GetApp()->GetInputDevice();
		m_controller = inputDevice.GetControlerVec()[0];
		m_controllerVec = Vec2(m_controller.fThumbRX, m_controller.fThumbRY);

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
		m_cameraManager->CameraPosUpdate(8,0,17.0f);

		//�ʏ탂�[�h���̃J�������쏈��
		m_cameraManager->CameraControlNomalMode();
		//�����t���̉�]����
		m_cameraManager->InertialRotation();

		//����,LB�{�^���������Ă�����e�X�e�[�g�Ɉڍs����
		if (m_controller.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
			m_cameraManager->ChangeState(L"Gun");
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
		//����,LB�{�^���������Ă��Ȃ�������ʏ탂�[�h�ɖ߂�
		if (!(m_controller.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER))
		{
			m_cameraManager->ChangeState(L"Normal");
			return;
		}

		CameraStateBase::Update(deltaTime);

		//�J�����̈ʒu�X�V
		m_cameraManager->CameraPosUpdate(3.0f, 3.0f, 8.0f);
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

		//�ړ����[�h���̃J�������쏈��
		m_cameraManager->CameraControlTransitionMode();
	}

	void CameraNormalToGunState::Exit()
	{

	}
	//

}