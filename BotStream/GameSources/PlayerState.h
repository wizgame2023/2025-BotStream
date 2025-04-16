/*!
@file PlayerStaet.h
@brief �v���C���[�X�e�[�g
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class Player;
	class PlayerWalkState :public StateBase
	{
	private:
		shared_ptr<Player> m_player;
	public:
		PlayerWalkState(shared_ptr<GameObject>& obj) :
			StateBase(obj),
			m_player(dynamic_pointer_cast<Player>(obj))
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
	class PlayerDodgeState : public StateBase
	{
	private:
		shared_ptr<Player> m_player;
		CONTROLER_STATE m_controller;
	public:
		PlayerDodgeState(shared_ptr<GameObject>& obj) :
			StateBase(obj),
			m_player(dynamic_pointer_cast<Player>(obj))
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
	class PlayerDashState : public StateBase
	{
	private:
		shared_ptr<Player> m_player;
	public:
		PlayerDashState(shared_ptr<GameObject>& obj) :
			StateBase(obj),
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
