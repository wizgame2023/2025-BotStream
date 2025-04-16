/*!
@file PlayerState.cpp
@brief �v���C���[�̃X�e�[�g
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//Player�̕������[�V����
	void PlayerWalkState::Enter()
	{

	}
	void PlayerWalkState::Update(float deltaTime)
	{
		// ���̓f�o�C�X�擾
		auto inputDevice = App::GetApp()->GetInputDevice();
		auto controller = inputDevice.GetControlerVec()[0];
		Vec3 stick = Vec3(controller.fThumbLX, 0, controller.fThumbLY);
		
		//�ړ�����
		Vec3 move = m_player->GetMoveVector();
		m_player->PlayerMove(PlayerState_Walk);

		//�����A�j���[�V�����Đ�
		if (move.length() != 0)
		{
			m_player->ChangeAnim(L"Walk");
		}
		else 
		{
			m_player->ChangeAnim(L"Idle");
		}

		//�����X�e�[�g�̃A�j���[�V�����Đ�
		m_player->GetComponent<PNTBoneModelDraw>()->UpdateAnimation(deltaTime * 5);
	
		//����X�e�[�g�ɕύX����
		if (controller.wPressedButtons & XINPUT_GAMEPAD_A)
		{
			m_player->ChangeState(L"Dodge");
		}
		////�U���X�e�[�g�ɕύX���� �U������������Ă��Ȃ��̂ŃR�����g�A�E�g
		//if (controller.wPressedButtons & XINPUT_GAMEPAD_X)
		//{
		//	m_player->ChangeState(L"Attack1");
		//}

	
	}
	void PlayerWalkState::Exit()
	{

	}

	//����X�e�[�g
	void PlayerDodgeState::Enter()
	{

	}
	void PlayerDodgeState::Update(float deltaTime)
	{
		// ���̓f�o�C�X�擾
		auto inputDevice = App::GetApp()->GetInputDevice();
		m_controller = inputDevice.GetControlerVec()[0];

		//�ړ�����
		Vec3 move = m_player->GetMoveVector();
		m_player->PlayerMove(PlayerState_Dodge);

		bool dodgeFlag = m_player->GetDodgeFlag();
		if (!dodgeFlag)
		{
			Exit(0);
		}

	}
	void PlayerDodgeState::Exit(int a)//�I������
	{
		//// ���̓f�o�C�X�擾
		//auto inputDevice = App::GetApp()->GetInputDevice();
		//auto controller = inputDevice.GetControlerVec()[0];

		//�_�b�V���X�e�[�g�ɂ��邩�����X�e�[�g�ɂ��邩
		if (m_controller.wButtons & XINPUT_GAMEPAD_A)
		{
			m_player->ChangeState(L"Dash");
		}
		else
		{
			m_player->ChangeState(L"PlayerWalk");
		}

	}

	//�_�b�V���X�e�[�g
	void PlayerDashState::Enter()
	{

	}
	void PlayerDashState::Update(float deltaTime)
	{
		// ���̓f�o�C�X�擾
		auto inputDevice = App::GetApp()->GetInputDevice();
		auto controller = inputDevice.GetControlerVec()[0];

		//�ړ�����
		Vec3 move = m_player->GetMoveVector();
		m_player->PlayerMove(PlayerState_Dash);

		//�����A�j���[�V�����Đ�
		if (move.length() != 0)
		{
			m_player->ChangeAnim(L"Walk");
		}
		else
		{
			m_player->ChangeAnim(L"Idle");
		}

		//�����X�e�[�g�̃A�j���[�V�����Đ�
		m_player->GetComponent<PNTBoneModelDraw>()->UpdateAnimation(deltaTime * 7);

		//�����X�e�[�g�ɕύX����
		if (controller.wReleasedButtons & XINPUT_GAMEPAD_A)
		{
			m_player->ChangeState(L"PlayerWalk");
		}
		////�U���X�e�[�g�ɕύX���� �U������������Ă��Ȃ��̂ŃR�����g�A�E�g
		//if (controller.wPressedButtons & XINPUT_GAMEPAD_X)
		//{
		//	m_player->ChangeState(L"Attack1");
		//}

	}
	void PlayerDashState::Exit()
	{

	}


}
//end basecross
