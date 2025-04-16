/*!
@file PlayerStaet.h
@brief �v���C���[�X�e�[�g
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class Player;
	//�v���C���[�X�e�[�g�̌��ƂȂ�N���X
	class PlayerStateBase :public StateBase
	{
	protected:
		shared_ptr<Player> m_player;//�v���C���[�̎���
		CONTROLER_STATE m_controller;//�R���g���[���[
	public:
		PlayerStateBase(shared_ptr<GameObject>& obj) :
			StateBase(obj),
			m_player(dynamic_pointer_cast<Player>(obj))
		{

		}
		~PlayerStateBase()
		{
		}

		
		virtual void Enter() {};
		virtual void Update(float deltaTime) 
		{
			//�R���g���[���[���󂯎��
			auto inputDevice = App::GetApp()->GetInputDevice();
			m_controller = inputDevice.GetControlerVec()[0];
		};
		virtual void Exit() {};
	};

	//�k���X�e�[�g
	class PlayerWalkState :public PlayerStateBase
	{
	private:
		//shared_ptr<Player> m_player;
	public:
		PlayerWalkState(shared_ptr<GameObject>& obj) :
			PlayerStateBase(obj)
			//m_player(dynamic_pointer_cast<Player>(obj))
		{
		}

		~PlayerWalkState()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();
	};

	//����X�e�[�g
	class PlayerDodgeState : public PlayerStateBase
	{
	private:
	public:
		PlayerDodgeState(shared_ptr<GameObject>& obj) :
			PlayerStateBase(obj)
		{

		}
		~PlayerDodgeState()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit(int a);//�Ȃ���Exit�ň��������Ȃ��ƃG���[�f���܂�
	};

	//�_�b�V���X�e�[�g
	class PlayerDashState : public PlayerStateBase
	{
	private:
		shared_ptr<Player> m_player;
	public:
		PlayerDashState(shared_ptr<GameObject>& obj) :
			PlayerStateBase(obj),
			m_player(dynamic_pointer_cast<Player>(obj))
		{

		}
		~PlayerDashState()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();
	};

	//�U���X�e�[�g(��ԍŏ��ɏo�Ă���U��)
	class PlayerAttack1State :public PlayerStateBase
	{
	public:
		PlayerAttack1State(shared_ptr<GameObject>& obj) :
			PlayerStateBase(obj)
		{

		}
		~PlayerAttack1State()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();
	};

	//�v���C���[�X�e�[�g�}�V��
	class PlayerStateMachine : public StateMachineBase
	{
	public:
		PlayerStateMachine(shared_ptr<GameObject>& obj)
		{
			//�X�e�[�g�̓o�^
			AddState(L"PlayerWalk", shared_ptr<PlayerWalkState>(new PlayerWalkState(obj)));
			AddState(L"Dodge", shared_ptr<PlayerDodgeState>(new PlayerDodgeState(obj)));
			AddState(L"Dash", shared_ptr<PlayerDashState>(new PlayerDashState(obj)));

			//�ŏ��̃X�e�[�g��Walk�������炢���ȃX�e�[�g�ɕύX���� �C�[�u�C�݂����Ȃ���
			ChangeState(L"PlayerWalk");
		}
	};

}
//end basecross
