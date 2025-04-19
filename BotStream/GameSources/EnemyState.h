/*!
@file EnemyState.h
@brief 敵のステート
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
#include <random>

namespace basecross {
	class GameObject;

	//前進
	class EnemyWalkState : public StateBase {
	public:
		EnemyWalkState(shared_ptr<GameObject>& obj) :
			StateBase(obj) {
		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	class EnemyAttackState : public StateBase {
	public:
		EnemyAttackState(shared_ptr<GameObject>& obj) :
			StateBase(obj) {
		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	class EnemyHitState : public StateBase {
	public:
		EnemyHitState(shared_ptr<GameObject>& obj) :
			StateBase(obj) {
		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	class EnemyStateMachine : public StateMachineBase {
	public:
		EnemyStateMachine(shared_ptr<GameObject>& obj)
		{
			AddState(L"Walk", shared_ptr<EnemyWalkState>(new EnemyWalkState(obj)));
			AddState(L"Attack", shared_ptr<EnemyAttackState>(new EnemyAttackState(obj)));
			AddState(L"Hit", shared_ptr<EnemyHitState>(new EnemyHitState(obj)));

			ChangeState(L"Walk");
		}

	};

	//------------------------------------------------------------------------------------------
	// BOSS1
	//------------------------------------------------------------------------------------------

	//立ち
	class BossFirstStandState : public StateBase {
		float m_time = 0;
		const float m_startAttack = 3.0f;
		const float m_startAttackRand = 10;
		const float m_farDist = 10;
		random_device rnd;
	public:
		BossFirstStandState(shared_ptr<GameObject>& obj) :
			StateBase(obj) {
		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	//前進
	class BossFirstChaseState : public StateBase {
	public:
		BossFirstChaseState(shared_ptr<GameObject>& obj) :
			StateBase(obj) {
		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	class BossFirstAttackState : public StateBase {
	public:
		BossFirstAttackState(shared_ptr<GameObject>& obj) :
			StateBase(obj) {
		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	class BossFirstHitState : public StateBase {
	public:
		BossFirstHitState(shared_ptr<GameObject>& obj) :
			StateBase(obj) {
		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	class BossFirstStateMachine : public StateMachineBase {
	public:
		BossFirstStateMachine(shared_ptr<GameObject>& obj)
		{
			AddState(L"Stand", shared_ptr<BossFirstStandState>(new BossFirstStandState(obj)));
			AddState(L"Chase", shared_ptr<BossFirstChaseState>(new BossFirstChaseState(obj)));
			AddState(L"Attack", shared_ptr<BossFirstAttackState>(new BossFirstAttackState(obj)));
			AddState(L"Hit", shared_ptr<BossFirstHitState>(new BossFirstHitState(obj)));

			ChangeState(L"Stand");
		}

	};

}
//end basecross
