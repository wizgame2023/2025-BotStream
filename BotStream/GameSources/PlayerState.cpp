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
		PlayerStateBase::Update(deltaTime);
		Vec3 stick = Vec3(m_controller.fThumbLX, 0, m_controller.fThumbLY);
		
		//�ړ�����
		Vec3 move = m_player->GetMoveVector(PlayerState_Walk);
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
		if (m_controller.wPressedButtons & XINPUT_GAMEPAD_A)
		{
			m_player->ChangeState(L"Dodge");
		}
		////�U���X�e�[�g�ɕύX���� �U������������Ă��Ȃ��̂ŃR�����g�A�E�g
		//if (m_controller.wPressedButtons & XINPUT_GAMEPAD_X)
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
		PlayerStateBase::Update(deltaTime);

		//�ړ�����
		Vec3 move = m_player->GetMoveVector(PlayerState_Dodge);
		m_player->PlayerMove(PlayerState_Dodge);

		bool dodgeFlag = m_player->GetDodgeFlag();
		if (!dodgeFlag)
		{
			Exit(0);
		}

	}
	void PlayerDodgeState::Exit(int a)//�I������
	{
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
		PlayerStateBase::Update(deltaTime);

		//�ړ�����
		Vec3 move = m_player->GetMoveVector(PlayerState_Dash);
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
		if (m_controller.wReleasedButtons & XINPUT_GAMEPAD_A)
		{
			m_player->ChangeState(L"PlayerWalk");
		}
		////�U���X�e�[�g�ɕύX���� �U������������Ă��Ȃ��̂ŃR�����g�A�E�g
		//if (m_controller.wPressedButtons & XINPUT_GAMEPAD_X)
		//{
		//	m_player->ChangeState(L"Attack1");
		//}

	}
	void PlayerDashState::Exit()
	{

	}


	//�U���X�e�[�g(��ԍŏ��ɏo�Ă���U��)
	void PlayerAttack1State::Enter()
	{

	}
	void PlayerAttack1State::Update(float deltaTime)
	{
		
	}
	void PlayerAttack1State::Exit()
	{

	}


}
//end basecross
