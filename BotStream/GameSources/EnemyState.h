/*!
@file EnemyState.h
@brief 敵のステート
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	class GameObject;
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

	//------------------------------------------------------------------------------------------

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

}
//end basecross
