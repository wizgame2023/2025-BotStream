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
		//�U���X�e�[�g�ɕύX���� �U������������Ă��Ȃ��̂ŃR�����g�A�E�g
		if (m_controller.wPressedButtons & XINPUT_GAMEPAD_X)
		{
			m_player->ChangeState(L"Attack1");
			m_player->AddEffect(PlayerEffect_Attack1);//�U���G�t�F�N�g���o��
		}

	
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
		//�U���X�e�[�g�ɕύX���� �U������������Ă��Ȃ��̂ŃR�����g�A�E�g
		if (m_controller.wPressedButtons & XINPUT_GAMEPAD_X)
		{
			m_player->ChangeState(L"Attack1");
			m_player->AddEffect(PlayerEffect_Attack1);//�U���G�t�F�N�g���o��
		}

	}
	void PlayerDashState::Exit()
	{

	}


	//�U���X�e�[�g(��ԍŏ��ɏo�Ă���U��)
	void PlayerAttack1State::Enter()
	{
		//�U���̓����蔻����o��
		auto stage = m_player->GetStage();
		//�U���̓����蔻��(��)
		m_AttackObj = stage->AddGameObject<Cube>(Vec3(0.0f, 2.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
	}
	void PlayerAttack1State::Update(float deltaTime)
	{
		//�U������̒�`
		if (m_timeOfAttack <= 0) {
			auto tmp = m_player->GetAttackPtr()->GetHitInfo();
			tmp.HitOnce = true;
			tmp.Damage = 10;
			tmp.HitVel_Stand = Vec3(-20, 0, 0);
			tmp.HitTime_Stand = .8f;
			m_player->DefAttack(.5f, tmp);
			m_player->GetAttackPtr()->SetPos(Vec3(1, 1, 0));
		}

		//�U���̎��Ԍv��
		m_timeOfAttack += deltaTime;

		//�P�\���Ԉȓ��ɍU���{�^�����������玟�̍U���X�e�[�g�ɑJ�ڂł���
		if (m_timeOfAttack < m_graceTimeOfNextAttack)
		{
			if (m_controller.wPressedButtons & XINPUT_GAMEPAD_X)
			{
				m_nestAttackFlag = true;
				m_player->AddEffect(PlayerEffect_Attack2);//�U���G�t�F�N�g���o��
			}
		}
		//���̍U���Ɉڍs�ł��Ȃ���ԂȂ�U������̐F��ς���(�f�o�b�N�p)
		if (m_timeOfAttack >= m_graceTimeOfNextAttack)
		{
			m_AttackObj->GetComponent<PNTStaticDraw>()->SetDiffuse(Col4(1.0f, 0.0f, 0.0f, 1.0f));
			m_AttackObj->GetComponent<PNTStaticDraw>()->SetEmissive(Col4(1.0f, 0.0f, 0.0f, 1.0f));
		}
		//�U���̎��Ԃ��z������ʂ̃X�e�[�g�Ɉړ�����
		if (m_timeOfAttack >= m_timeMaxOfAttack)
		{
			Exit(0);
		}
		
	}
	void PlayerAttack1State::Exit(int a)
	{
		auto stage = m_player->GetStage();

		//�X�e�[�g�J��
		if (m_nestAttackFlag)
		{
			m_player->ChangeState(L"Attack2");//�܂�Attack2�X�e�[�g�쐬���ĂȂ�			
		}
		else
		{
			m_timeOfAttack = 0.0f;//���Z�b�g
			stage->RemoveGameObject<Cube>(m_AttackObj);//�U������폜
			m_player->ChangeState(L"PlayerWalk");
		}
	}


	//�U���X�e�[�g(�Q�Ԗڂɏo��U��)
	void PlayerAttack2State::Enter()
	{

	}
	void PlayerAttack2State::Update(float deltaTime)
	{
		//�U���̎��Ԍv��
		m_timeOfAttack += deltaTime;

		//�P�\���Ԉȓ��ɍU���{�^�����������玟�̍U���X�e�[�g�ɑJ�ڂł���
		if (m_timeOfAttack < m_graceTimeOfNextAttack)
		{
			//m_nestAttackFlag = true;
		}
		//���̍U���Ɉڍs�ł��Ȃ���ԂȂ�U������̐F��ς���(�f�o�b�N�p)
		if (m_timeOfAttack >= m_graceTimeOfNextAttack)
		{
			m_AttackObj->GetComponent<PNTStaticDraw>()->SetDiffuse(Col4(1.0f, 0.0f, 0.0f, 1.0f));
			m_AttackObj->GetComponent<PNTStaticDraw>()->SetEmissive(Col4(1.0f, 0.0f, 0.0f, 1.0f));
		}
		//�U���̎��Ԃ��z������ʂ̃X�e�[�g�Ɉړ�����
		if (m_timeOfAttack >= m_timeMaxOfAttack)
		{
			Exit(0);
		}
	}
	void PlayerAttack2State::Exit(int a)
	{
		m_player->ChangeState(L"PlayerWalk");
	}

}
//end basecross
