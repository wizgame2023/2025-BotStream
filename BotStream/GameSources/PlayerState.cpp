/*!
@file PlayerState.cpp
@brief �v���C���[�̃X�e�[�g
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void PlayerStateBase::Update(float deltaTime)
	{

		//�R���g���[���[���󂯎��
		auto inputDevice = App::GetApp()->GetInputDevice();
		m_controller = inputDevice.GetControlerVec()[0];

		//�J�����}�l�[�W�����烍�b�N�I���Ώۂ̈ʒu���擾����
		shared_ptr<CameraManager> camera = nullptr;
		camera = m_player->GetStage()->GetSharedGameObject<CameraManager>(L"CameraManager");
		shared_ptr<Actor> targetObj = nullptr;
		targetObj = camera->GetTargetObj();
		//�^�[�Q�b�g�Ώۂ����鎞�ΏۂƂ̋����𑪂�
		if (targetObj)
		{
			//Vec3 targetPos = Vec3(0.0f);
			//Vec3 playerPos = Vec3(0.0f);
			//targetPos = targetObj->GetComponent<Transform>()->GetPosition();
			//playerPos = m_player->GetComponent<Transform>()->GetPosition();
			////���b�N�I���ΏۂƂ̋������v�Z����
			m_targetDistance = camera->GetTargetDis();
		}
		else if (!targetObj)
		{
			m_targetDistance = 0.0f;
		}


	};
	//�^�[�Q�b�g�ΏۂƂ̋������擾����
	float PlayerStateBase::GetTargetDistance()
	{
		return m_targetDistance;
	}

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
		//�U���X�e�[�g�ɕύX����@���������������]�U�������łȂ���Βʏ�U��
		//���b�N�I���Ώۂ̋����ɂ���čU�����@��ς��郍�b�N�I�����ĂȂ���΋ߋ����̂�
		float meleeRange = 3.0f;
		if (m_controller.wButtons & XINPUT_GAMEPAD_X)
		{
			m_timeOfPushAttackButton += deltaTime;//�����Ă��鎞�Ԃ𑪂�
		}
		if (m_controller.wReleasedButtons & XINPUT_GAMEPAD_X)
		{
			//�U������ۋ����ɂ���čU�����@��ς���
			if (m_targetDistance < meleeRange)
			{//�ߋ���
				if (m_timeOfPushAttackButton >= 1.5f)
				{
					m_player->ChangeState(L"AttackEx");
					m_player->AddEffect(PlayerEffect_AttackEx);//�U���G�t�F�N�g���o��
				}
				if (m_timeOfPushAttackButton < 1.5f)
				{
					m_player->ChangeState(L"Attack1");
					m_player->AddEffect(PlayerEffect_Attack1);//�U���G�t�F�N�g���o��
				}
			}
			else if (m_targetDistance >= meleeRange)
			{//������
				m_player->ChangeState(L"AttackLong");
				m_player->AddEffect(PlayerEffect_Beam);//�U���G�t�F�N�g���o��
			}

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

	}
	void PlayerDodgeState::Exit()//�I������
	{

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
		//�U���X�e�[�g�ɕύX����
		if (m_controller.wButtons & XINPUT_GAMEPAD_X)
		{
			m_timeOfPushAttackButton += deltaTime;//�����Ă��鎞�Ԃ𑪂�
		}
		if (m_controller.wReleasedButtons & XINPUT_GAMEPAD_X)
		{
			if (m_timeOfPushAttackButton >= 1.5f)
			{
				m_player->ChangeState(L"AttackEx");
				m_player->AddEffect(PlayerEffect_AttackEx);//�U���G�t�F�N�g���o��
			}
			if (m_timeOfPushAttackButton < 1.5f)
			{
				m_player->ChangeState(L"Attack1");
				m_player->AddEffect(PlayerEffect_Attack1);//�U���G�t�F�N�g���o��
			}
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
		// ���̓f�o�C�X�擾
		PlayerStateBase::Update(deltaTime);

		//�U���̎��Ԍv��
		m_timeOfAttack += deltaTime;
		//�P�\���Ԉȓ��ɍU���{�^�����������玟�̍U���X�e�[�g�ɑJ�ڂł���
		if (m_timeOfAttack < m_graceTimeOfNextAttack)
		{
			//�U���X�e�[�g�ɕύX����
			if (m_controller.wPressedButtons & XINPUT_GAMEPAD_X)
			{			
				m_nestAttackFlag = true;	
			}
		}
		//���̍U���Ɉڍs�ł��Ȃ���ԂȂ�U������̐F��ς���(�f�o�b�N�p)
		if (m_timeOfAttack >= m_graceTimeOfNextAttack)
		{
			m_AttackObj->GetComponent<PNTStaticDraw>()->SetDiffuse(Col4(1.0f, 0.0f, 0.0f, 0.0f));
			m_AttackObj->GetComponent<PNTStaticDraw>()->SetEmissive(Col4(1.0f, 0.0f, 0.0f, 0.0f));
		}
		//�U���̎��Ԃ��z������ʂ̃X�e�[�g�Ɉړ�����
		if (m_timeOfAttack >= m_timeMaxOfAttack)
		{	
			auto stage = m_player->GetStage();

			//�X�e�[�g�J��
			if (m_nestAttackFlag)
			{
				m_player->ChangeState(L"Attack2");////���̍U���X�e�[�g�Ɉړ�
				m_player->AddEffect(PlayerEffect_Attack2);//�U���G�t�F�N�g���o��
			}
			else
			{
				stage->RemoveGameObject<Cube>(m_AttackObj);//�U������폜
				m_player->ChangeState(L"PlayerWalk");
			}
		}
		
	}
	void PlayerAttack1State::Exit()
	{
		auto stage = m_player->GetStage();
		stage->RemoveGameObject<Cube>(m_AttackObj);//�U������폜
		m_timeOfAttack = 0.0f;//���Z�b�g
		m_nestAttackFlag = false;
	}


	//�U���X�e�[�g(�Q�Ԗڂɏo��U��)
	void PlayerAttack2State::Enter()
	{
		//�U���̓����蔻����o��
		auto stage = m_player->GetStage();
		//�U���̓����蔻��(��)
		m_AttackObj = stage->AddGameObject<Cube>(Vec3(0.0f, 2.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f),Col4(0.0f,0.0f,1.0f,1.0f));
	}
	void PlayerAttack2State::Update(float deltaTime)
	{
		// ���̓f�o�C�X�擾
		PlayerStateBase::Update(deltaTime);

		//�U���̎��Ԍv��
		m_timeOfAttack += deltaTime;

		//�P�\���Ԉȓ��ɍU���{�^�����������玟�̍U���X�e�[�g�ɑJ�ڂł���
		if (m_timeOfAttack < m_graceTimeOfNextAttack)
		{
			//�U���X�e�[�g�ɕύX����
			if (m_controller.wPressedButtons & XINPUT_GAMEPAD_X)
			{
				m_nestAttackFlag = true;
			}
		}
		//���̍U���Ɉڍs�ł��Ȃ���ԂȂ�U������̐F��ς���(�f�o�b�N�p)
		if (m_timeOfAttack >= m_graceTimeOfNextAttack)
		{
			m_AttackObj->GetComponent<PNTStaticDraw>()->SetDiffuse(Col4(1.0f, 0.0f, 0.0f, 0.0f));
			m_AttackObj->GetComponent<PNTStaticDraw>()->SetEmissive(Col4(1.0f, 0.0f, 0.0f, 0.0f));
		}
		//�U���̎��Ԃ��z������ʂ̃X�e�[�g�Ɉړ�����
		if (m_timeOfAttack >= m_timeMaxOfAttack)
		{
			auto stage = m_player->GetStage();
			m_timeOfAttack = 0.0f;//���Z�b�g

			//�X�e�[�g�J��
			if (m_nestAttackFlag)
			{
				stage->RemoveGameObject<Cube>(m_AttackObj);//�U������폜
				m_player->ChangeState(L"Attack3");//���̍U���X�e�[�g�Ɉړ�
				m_player->AddEffect(PlayerEffect_Attack3);//�U���G�t�F�N�g���o��
			}
			else
			{
				stage->RemoveGameObject<Cube>(m_AttackObj);//�U������폜
				m_player->ChangeState(L"PlayerWalk");
			}
		}
	}
	void PlayerAttack2State::Exit()
	{
		auto stage = m_player->GetStage();
		stage->RemoveGameObject<Cube>(m_AttackObj);//�U������폜
		m_timeOfAttack = 0.0f;//���Z�b�g
		m_nestAttackFlag = false;
	}


	//�U���X�e�[�g(3�Ԗڂɏo��U��)
	void PlayerAttack3State::Enter()
	{
		//�U���̓����蔻����o��
		auto stage = m_player->GetStage();
		//�U���̓����蔻��(��)
		m_AttackObj = stage->AddGameObject<Cube>(Vec3(0.0f, 2.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Col4(1.0f, 0.0f, 1.0f, 1.0f));
	}
	void PlayerAttack3State::Update(float deltaTime)
	{
		// ���̓f�o�C�X�擾
		PlayerStateBase::Update(deltaTime);

		//�U���̎��Ԍv��
		m_timeOfAttack += deltaTime;

		//�P�\���Ԉȓ��ɍU���{�^�����������玟�̍U���X�e�[�g�ɑJ�ڂł���
		if (m_timeOfAttack < m_graceTimeOfNextAttack)
		{
			//�U���X�e�[�g�ɕύX����
			if (m_controller.wPressedButtons & XINPUT_GAMEPAD_X)
			{
				m_nestAttackFlag = true;
			}
		}
		//���̍U���Ɉڍs�ł��Ȃ���ԂȂ�U������̐F��ς���(�f�o�b�N�p)
		if (m_timeOfAttack >= m_graceTimeOfNextAttack)
		{
			m_AttackObj->GetComponent<PNTStaticDraw>()->SetDiffuse(Col4(1.0f, 0.0f, 0.0f, 0.0f));
			m_AttackObj->GetComponent<PNTStaticDraw>()->SetEmissive(Col4(1.0f, 0.0f, 0.0f, 0.0f));
		}
		//�U���̎��Ԃ��z������ʂ̃X�e�[�g�Ɉړ�����
		if (m_timeOfAttack >= m_timeMaxOfAttack)
		{
			auto stage = m_player->GetStage();
			m_timeOfAttack = 0.0f;//���Z�b�g

			//�X�e�[�g�J��
			if (m_nestAttackFlag)
			{
				stage->RemoveGameObject<Cube>(m_AttackObj);//�U������폜
				m_player->ChangeState(L"AttackEx");//���̍U���X�e�[�g�Ɉړ�
				m_player->AddEffect(PlayerEffect_AttackEx);//�U���G�t�F�N�g���o��
			}
			else
			{
				stage->RemoveGameObject<Cube>(m_AttackObj);//�U������폜
				m_player->ChangeState(L"PlayerWalk");
			}
		}
	}
	void PlayerAttack3State::Exit()
	{
		auto stage = m_player->GetStage();
		stage->RemoveGameObject<Cube>(m_AttackObj);//�U������폜
		m_timeOfAttack = 0.0f;//���Z�b�g
		m_nestAttackFlag = false;
	}


	//�U���X�e�[�g(3�Ԗڂɏo��U��)
	void PlayerAttackExState::Enter()
	{
		//�U���̓����蔻����o��
		auto stage = m_player->GetStage();
		//�U���̓����蔻��(��)
		m_AttackObj = stage->AddGameObject<Cube>(Vec3(0.0f, 2.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Col4(0.0f, 0.0f, 0.0f, 1.0f));
	}
	void PlayerAttackExState::Update(float deltaTime)
	{
		//�U���̎��Ԍv��
		m_timeOfAttack += deltaTime;

		//�U���̎��Ԃ��z������ʂ̃X�e�[�g�Ɉړ�����
		if (m_timeOfAttack >= m_timeMaxOfAttack)
		{
			auto stage = m_player->GetStage();
			m_timeOfAttack = 0.0f;//���Z�b�g

			m_player->ChangeState(L"PlayerWalk");
		}
	}
	void PlayerAttackExState::Exit()
	{
		auto stage = m_player->GetStage();
		stage->RemoveGameObject<Cube>(m_AttackObj);//�U������폜
		m_timeOfAttack = 0.0f;//���Z�b�g
	}


	//void PlayerAttackLongState::Enter()
	//{
	//	//�����o��
	//}
	//void PlayerAttackLongState::Update(float deltaTime)
	//{
	//	//�U���̎��Ԍv��
	//	m_timeOfAttack += deltaTime;

	//	//�U���̎��Ԃ��z������ʂ̃X�e�[�g�Ɉړ�����
	//	if (m_timeOfAttack >= m_timeMaxOfAttack)
	//	{
	//		auto stage = m_player->GetStage();
	//		m_timeOfAttack = 0.0f;//���Z�b�g

	//		m_player->ChangeState(L"PlayerWalk");
	//	}

	//}
	//void PlayerAttackLongState::Exit()
	//{
	//	auto stage = m_player->GetStage();
	//	stage->RemoveGameObject<Cube>(m_AttackObj);//�U������폜
	//	m_timeOfAttack = 0.0f;//���Z�b�g
	//}

}
//end basecross
