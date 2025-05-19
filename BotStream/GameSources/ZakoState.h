#pragma once
/*!
@file ZakoState.h
@brief �G���̃X�e�[�g
*/

#include "stdafx.h"

namespace basecross {
	/*

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

	//�ڋߐ������Ƃ��̏����X�e�[�g
	class EnemyZakoPreparationforMeleeState :public EnemyZakoStateBase
	{
	private:
		float m_timeOfShot = 0.0f;//�ł��Ԍo�߂𑪂�ϐ�
		float m_timeMaxOfShot = 4.0f;//�ł��Ԃ̕ۑ��p�ϐ�
		float m_speed = 1.0f;//���̑���
	public:
		EnemyZakoPreparationforMeleeState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();

		void SppedChange();//�r�̃X�s�[�h��ς��鏈��
	};

	//�U��������Ƃ��̃X�e�[�g(�ߋ���)
	class EnemyZakoMeleeState :public EnemyZakoStateBase
	{
	private:
		float m_timeOfAttack = 0.0f;//�U�����Ԍo�߂𑪂�ϐ�
		float m_timeMaxOfAttack = 2.0f;//�U�����Ԃ̕ۑ��p�ϐ�

		float m_timeOfAttackAdd = 1.2f;//�U������̔�������

		bool m_Attack = true;//�U��������o�������̃t���O
	public:
		EnemyZakoMeleeState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();

	};

	//����ł��O�̎����킹�̂Ƃ��̃X�e�[�g
	class EnemyZakoAlignmentState :public EnemyZakoStateBase
	{
	private:
		float m_timeOfShot = 0.0f;//�ł��Ԍo�߂𑪂�ϐ�
		float m_timeMaxOfShot = 3.0f;//�ł��Ԃ̕ۑ��p�ϐ�

		float m_speed = 1.0f;//���̑��x
	public:
		EnemyZakoAlignmentState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	//�U��������Ƃ��̃X�e�[�g(������)
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
			AddState(L"Alignment", shared_ptr<EnemyZakoAlignmentState>(new EnemyZakoAlignmentState(obj)));
			AddState(L"PreparationforMelee", shared_ptr<EnemyZakoPreparationforMeleeState>(new EnemyZakoPreparationforMeleeState(obj)));
			AddState(L"Melee", shared_ptr<EnemyZakoMeleeState>(new EnemyZakoMeleeState(obj)));
			AddState(L"Hit", shared_ptr<EnemyZakoHitState>(new EnemyZakoHitState(obj)));

			ChangeState(L"Stand");
		}
	};

	// ��ԃU�R�̃X�e�[�g----------------------------------------------------------

	//�����Ȃ��Ƃ��̃X�e�[�g
	class EnemyZakoFlyingStandState :public EnemyZakoStateBase
	{
	private:
		float m_timeOfShot = 0.0f;//�ł��Ԍo�߂𑪂�ϐ�
		float m_timeMaxOfShot = 4.0f;//�ł��Ԃ̕ۑ��p�ϐ�
	public:
		EnemyZakoFlyingStandState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	//�ڋߐ������Ƃ��̏����X�e�[�g
	class EnemyZakoFlyingPreparationforMeleeState :public EnemyZakoStateBase
	{
	private:
		float m_timeOfShot = 0.0f;//�ł��Ԍo�߂𑪂�ϐ�
		float m_timeMaxOfShot = 4.0f;//�ł��Ԃ̕ۑ��p�ϐ�
	public:
		EnemyZakoFlyingPreparationforMeleeState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	//�U��������Ƃ��̃X�e�[�g(�ߋ���)
	class EnemyZakoFlyingMeleeState :public EnemyZakoStateBase
	{
	private:
		float m_timeOfAttack = 0.0f;//�U�����Ԍo�߂𑪂�ϐ�
		float m_timeMaxOfAttack = 2.0f;//�U�����Ԃ̕ۑ��p�ϐ�

		float m_timeOfAttackAdd = 1.2f;//�U������̔�������

		bool m_Attack = true;//�U��������o�������̃t���O
	public:
		EnemyZakoFlyingMeleeState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();

	};

	//����ł��O�̎����킹�̂Ƃ��̃X�e�[�g
	class EnemyZakoFlyingAlignmentState :public EnemyZakoStateBase
	{
	private:
		float m_timeOfShot = 0.0f;//�ł��Ԍo�߂𑪂�ϐ�
		float m_timeMaxOfShot = 4.0f;//�ł��Ԃ̕ۑ��p�ϐ�
	public:
		EnemyZakoFlyingAlignmentState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	//�U��������Ƃ��̃X�e�[�g(������)
	class EnemyZakoFlyingShotState :public EnemyZakoStateBase
	{
	private:
		float m_timeOfAttack = 0.0f;//�ł��Ԍo�߂𑪂�ϐ�
		float m_timeMaxOfAttack = 1.0f;//�ł��Ԃ̕ۑ��p�ϐ�
	public:
		EnemyZakoFlyingShotState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();

	};

	//�_���[�W���󂯂��Ƃ�
	class EnemyZakoFlyingHitState :public EnemyZakoStateBase
	{
	private:

	public:
		EnemyZakoFlyingHitState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};


	// ��ԓG�̃X�e�[�g�}�V��
	class EnemyZakoFlyingStateMachine :public StateMachineBase
	{
	public:
		EnemyZakoFlyingStateMachine(shared_ptr<GameObject>& obj)
		{
			AddState(L"Stand", shared_ptr<EnemyZakoFlyingStandState>(new EnemyZakoFlyingStandState(obj)));
			AddState(L"Shot", shared_ptr<EnemyZakoFlyingShotState>(new EnemyZakoFlyingShotState(obj)));
			AddState(L"Alignment", shared_ptr<EnemyZakoFlyingAlignmentState>(new EnemyZakoFlyingAlignmentState(obj)));
			AddState(L"PreparationforMelee", shared_ptr<EnemyZakoFlyingPreparationforMeleeState>(new EnemyZakoFlyingPreparationforMeleeState(obj)));
			AddState(L"Melee", shared_ptr<EnemyZakoFlyingMeleeState>(new EnemyZakoFlyingMeleeState(obj)));
			AddState(L"Hit", shared_ptr<EnemyZakoFlyingHitState>(new EnemyZakoFlyingHitState(obj)));

			ChangeState(L"Stand");
		}
	};
	*/
}