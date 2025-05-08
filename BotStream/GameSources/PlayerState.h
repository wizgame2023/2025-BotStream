/*!
@file PlayerStaet.h
@brief �v���C���[�X�e�[�g
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class Player;	
	class Actor;
	class Cube;
	class EnemyZako;
	//�v���C���[�X�e�[�g�̌��ƂȂ�N���X
	class PlayerStateBase :public StateBase
	{
	protected:
		shared_ptr<Player> m_player;//�v���C���[�̎���
		CONTROLER_STATE m_controller;//�R���g���[���[
		float m_timeOfPushAttackButton = 0.0f;//�U���{�^���������Ă��鎞��
		shared_ptr<Actor> m_targetObj = nullptr;//���b�N�I�����̑Ώ�
		float m_targetDistance;//�^�[�Q�b�g�ΏۂƂ̋���

		bool m_dodgeFlag = true;//����ł��邩�̃t���O
		bool m_meleeFlag = true;//�ڋߐ�ł��邩�̃t���O

		shared_ptr<SoundItem> m_SE = nullptr;//�Đ����Ă���SE
		shared_ptr<XAudio2Manager> m_SEManager = nullptr;//SE�Ȃǂ��Đ����邽�߂̃}�l�[�W��

	public:
		PlayerStateBase(shared_ptr<GameObject>& obj) :
			StateBase(obj),
			m_player(dynamic_pointer_cast<Player>(obj))
		{

		}
		~PlayerStateBase()
		{
		}

		
		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit() {};

		float GetTargetDistance();
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
		//������Ă������̃t���O
		bool m_playerDodgeFlag = false;
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
		virtual void Exit();

	};

	//�_�b�V���X�e�[�g
	class PlayerDashState : public PlayerStateBase
	{
	private:
		shared_ptr<Player> m_player;
		float m_timeOfPushAttackButton = 0.0f;//�U���{�^���������Ă��鎞��
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
	private:
		//�U������
		float m_timeMaxOfAttack = 0.5f;
		//�U�����Ԍv��
		float m_timeOfAttack;
		//���̍U���̗P�\����
		float m_graceTimeOfNextAttack = 0.3f;
		//���̍U�������邩�̃t���O
		float m_nestAttackFlag = false;

		shared_ptr<Cube> m_AttackObj = nullptr;

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

	//�U���X�e�[�g(�Q�Ԗڂɏo��U��)
	class PlayerAttack2State :public PlayerStateBase
	{
	private:
		//�U������
		float m_timeMaxOfAttack = 0.7f;
		//�U�����Ԍv��
		float m_timeOfAttack;
		//���̍U���̗P�\����
		float m_graceTimeOfNextAttack = 0.5f;
		//���̍U�������邩�̃t���O
		float m_nestAttackFlag = false;

		shared_ptr<Cube> m_AttackObj = nullptr;

	public:
		PlayerAttack2State(shared_ptr<GameObject>& obj) :
			PlayerStateBase(obj)
		{

		}
		~PlayerAttack2State()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();
	};

	//�U���X�e�[�g(3�Ԗڂɏo��U��)
	class PlayerAttack3State :public PlayerStateBase
	{
	private:
		//�U������
		float m_timeMaxOfAttack = 1.0f;
		//�U�����Ԍv��
		float m_timeOfAttack;
		//���̍U���̗P�\����
		float m_graceTimeOfNextAttack = 0.9f;
		//���̍U�������邩�̃t���O
		float m_nestAttackFlag = false;

		shared_ptr<Cube> m_AttackObj = nullptr;

	public:
		PlayerAttack3State(shared_ptr<GameObject>& obj) :
			PlayerStateBase(obj)
		{

		}
		~PlayerAttack3State()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();
	};

	//�U���X�e�[�g(�Ō�ɏo��U��)
	class PlayerAttackExState :public PlayerStateBase
	{
	private:
		//�U������
		float m_timeMaxOfAttack = 1.2f;
		//�U�����Ԍv��
		float m_timeOfAttack;
		////���̍U���̗P�\����
		//float m_graceTimeOfNextAttack = 0.9f;
		////���̍U�������邩�̃t���O
		//float m_nestAttackFlag = false;

		shared_ptr<Cube> m_AttackObj = nullptr;

	public:
		PlayerAttackExState(shared_ptr<GameObject>& obj) :
			PlayerStateBase(obj)
		{

		}
		~PlayerAttackExState()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();
	};

	//�U���X�e�[�g(�K�E�Z)
	class PlayerAttackSpecialState :public PlayerStateBase
	{
	private:
		//�U������
		float m_timeMaxOfAttack = 2.0f;
		//�U�����Ԍv��
		float m_timeOfAttack;
		////���̍U���̗P�\����
		//float m_graceTimeOfNextAttack = 0.9f;
		////���̍U�������邩�̃t���O
		//float m_nestAttackFlag = false;

		shared_ptr<Cube> m_AttackObj = nullptr;

	public:
		PlayerAttackSpecialState(shared_ptr<GameObject>& obj) :
			PlayerStateBase(obj)
		{

		}
		~PlayerAttackSpecialState()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();
	};


	//�U���X�e�[�g(�Ō�ɏo��U��)
	class PlayerAttackLongState :public PlayerStateBase
	{
	private:
		//�U������
		float m_timeMaxOfAttack = 0.5f;
		//�U�����Ԍv��
		float m_timeOfAttack;
		////���̍U���̗P�\����
		//float m_graceTimeOfNextAttack = 0.9f;
		////���̍U�������邩�̃t���O
		//float m_nestAttackFlag = false;

		shared_ptr<Cube> m_AttackObj = nullptr;

	public:
		PlayerAttackLongState(shared_ptr<GameObject>& obj) :
			PlayerStateBase(obj)
		{

		}
		~PlayerAttackLongState()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();
	};

	//�U���������������̃X�e�[�g
	class PlayerHitState :public PlayerStateBase
	{
	private:
		//�U������
		float m_timeMaxOfHitBack = 1.2f;
		//�U�����Ԍv��
		float m_timeOfHitBack = 0.0f;
		////���̍U���̗P�\����
		//float m_graceTimeOfNextAttack = 0.9f;
		////���̍U�������邩�̃t���O
		//float m_nestAttackFlag = false;

		shared_ptr<Cube> m_AttackObj = nullptr;

	public:
		PlayerHitState(shared_ptr<GameObject>& obj) :
			PlayerStateBase(obj)
		{

		}
		~PlayerHitState()
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
			AddState(L"Attack1", shared_ptr<PlayerAttack1State>(new PlayerAttack1State(obj)));
			AddState(L"Attack2", shared_ptr<PlayerAttack2State>(new PlayerAttack2State(obj)));
			AddState(L"Attack3", shared_ptr<PlayerAttack3State>(new PlayerAttack3State(obj)));
			AddState(L"AttackEx", shared_ptr<PlayerAttackExState>(new PlayerAttackExState(obj)));
			AddState(L"AttackLong", shared_ptr<PlayerAttackLongState>(new PlayerAttackLongState(obj)));
			AddState(L"AttackSpecial", shared_ptr<PlayerAttackSpecialState>(new PlayerAttackSpecialState(obj)));
			AddState(L"Hit", shared_ptr<PlayerHitState>(new PlayerHitState(obj)));


			//�ŏ��̃X�e�[�g��Walk�������炢���ȃX�e�[�g�ɕύX���� �C�[�u�C�݂����Ȃ���
			ChangeState(L"PlayerWalk");
		}
		
	};


	//�G���G�̃X�e�[�g�֌W

	//�G���G�̃X�e�[�g�̌��ƂȂ�N���X
	class EnemyZakoStateBase :public StateBase
	{
	protected:
		shared_ptr<EnemyZako> m_enemyZako;
	public:
		EnemyZakoStateBase(shared_ptr<GameObject>& obj) :
			StateBase(obj),
			m_enemyZako(dynamic_pointer_cast<EnemyZako>(obj))
		{

		}

		virtual void Enter() {}
		virtual void Update(float deltatime) {}
		virtual void Exit() {}
	};
	
	//�����Ȃ��Ƃ��̃X�e�[�g
	class EnemyZakoStandState :public EnemyZakoStateBase
	{
	private:
		float m_timeOfShot = 0.0f;//�ł��Ԍo�߂𑪂�ϐ�
		float m_timeMaxOfShot = 4.0f;//�ł��Ԃ̕ۑ��p�ϐ�
	public:
		EnemyZakoStandState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	//�U��������Ƃ��̃X�e�[�g
	class EnemyZakoShotState :public EnemyZakoStateBase
	{
	private:
		float m_timeOfAttack = 0.0f;//�ł��Ԍo�߂𑪂�ϐ�
		float m_timeMaxOfAttack = 1.0f;//�ł��Ԃ̕ۑ��p�ϐ�
	public:
		EnemyZakoShotState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();

	};
	
	//�_���[�W���󂯂��G���G
	class EnemyZakoHitState :public EnemyZakoStateBase
	{
	private:

	public:
		EnemyZakoHitState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	class EnemyZakoStateMachine :public StateMachineBase
	{
	public:
		EnemyZakoStateMachine(shared_ptr<GameObject>& obj)
		{
			AddState(L"Stand", shared_ptr<EnemyZakoStandState>(new EnemyZakoStandState(obj)));
			AddState(L"Shot", shared_ptr<EnemyZakoShotState>(new EnemyZakoShotState(obj)));
			AddState(L"Hit", shared_ptr<EnemyZakoHitState>(new EnemyZakoHitState(obj)));
		
			ChangeState(L"Stand");
		}
	};

}
//end basecross
