/*!
@file EnemyState.h
@brief �G�̃X�e�[�g
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
#include <random>

namespace basecross {
	class GameObject;

	//�O�i
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

	//����
	class BossFirstStandState : public StateBase {
		float m_time = 0;
		const float m_startAttack = 3.0f;
		const float m_startAttackRand = 10;
		const float m_spinRand = 400;
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

	//�O�i
	class BossFirstChaseState : public StateBase {
		const float m_rotateThreshold = XM_PIDIV4 / 4;
		const float m_closeDist = 8;
	public:
		BossFirstChaseState(shared_ptr<GameObject>& obj) :
			StateBase(obj) {
		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	class BossFirstAttackState : public StateBase {
		float m_time = 0;
		float m_startup = .6f;
		float m_attackTime = .3f;
		const float m_end = 1.15f;

		bool m_attacked = false;
	public:
		BossFirstAttackState(shared_ptr<GameObject>& obj) :
			StateBase(obj) {
		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};
	class BossFirstAttack2State : public StateBase {
		float m_time = 0;
		const float m_startup = .5f;
		const float m_attackTime = .3f;
		const float m_end = 1.35f;

		bool m_attacked = false;
	public:
		BossFirstAttack2State(shared_ptr<GameObject>& obj) :
			StateBase(obj) {
		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	class BossFirstSpinStartState : public StateBase {
		float m_time = 0;
		const float m_startup = .9f;

	public:
		BossFirstSpinStartState(shared_ptr<GameObject>& obj) :
			StateBase(obj) {
		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};
	class BossFirstSpinState : public StateBase {
		bool m_attacked = false;

		float m_time = 0;
		float m_totalTime = 0;
		const float m_attackTime = .15f;
		const float m_allFrame = 2.0f;
		const float m_allFramePlus = 1.2f;
		float m_end = 0;

		float m_spinSpeed = 0;
		const float m_spinAccel = XM_2PI;
		const float m_spinSpeedMax = XM_2PI * 2;

		float m_speed = 0;
		const float m_accel = 10.0f;
		const float m_speedMax = 30.0f;
	public:
		BossFirstSpinState(shared_ptr<GameObject>& obj) :
			StateBase(obj) {
		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};
	class BossFirstSpinOverState : public StateBase {
		float m_time = 0;
		const float m_end = 1.4f;

		float m_spinSpeed = 0;
		const float m_spinMul = .8f;
		const float m_spinSpeedMax = XM_2PI * 2;

	public:
		BossFirstSpinOverState(shared_ptr<GameObject>& obj) :
			StateBase(obj) {
		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	class BossFirstBeamStartState : public StateBase {
		float m_time = 0;
		const float m_startup = 1.8f;

		const float m_rotateThreshold = XM_PIDIV4 / 8;
	public:
		BossFirstBeamStartState(shared_ptr<GameObject>& obj) :
			StateBase(obj) {
		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};
	class BossFirstBeamFireState : public StateBase {
		float m_time = 0;
		const float m_activeTime = 1.5f;
		int m_beamCnt = 0;
		bool m_isFinalBlow = false;

		const float m_beamHitVelBase = 4.0f;
		const float m_beamHitVelAdd = 0.5f;

		float m_attackTime = 0;
		const float m_attackTimeMax = .05f;
		const float m_attackTimeEnd = .75f;
	public:
		BossFirstBeamFireState(shared_ptr<GameObject>& obj) :
			StateBase(obj) {
		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};
	class BossFirstBeamEndState : public StateBase {
		float m_time = 0;
		const float m_recovery = 1.3f;

	public:
		BossFirstBeamEndState(shared_ptr<GameObject>& obj) :
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

	class BossFirstStunState : public StateBase {
		float m_time = 0;
		const float m_downTime = 2.0f;
	public:
		BossFirstStunState(shared_ptr<GameObject>& obj) :
			StateBase(obj) {
		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	class BossFirstStunRecoverState : public StateBase {
		float m_time = 0;
		const float m_recoverTime = 1.0f;
	public:
		BossFirstStunRecoverState(shared_ptr<GameObject>& obj) :
			StateBase(obj) {
		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	class BossFirstDeadState : public StateBase {
	public:
		BossFirstDeadState(shared_ptr<GameObject>& obj) :
			StateBase(obj) {
		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	class BossFirstBonusState : public StateBase {
		const float m_end = 1.2f;
		float m_time = 0;
	public:
		BossFirstBonusState(shared_ptr<GameObject>& obj) :
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
			AddState(L"Attack2", shared_ptr<BossFirstAttack2State>(new BossFirstAttack2State(obj)));
			
			AddState(L"SpinStart", shared_ptr<BossFirstSpinStartState>(new BossFirstSpinStartState(obj)));
			AddState(L"Spin", shared_ptr<BossFirstSpinState>(new BossFirstSpinState(obj)));
			AddState(L"SpinOver", shared_ptr<BossFirstSpinOverState>(new BossFirstSpinOverState(obj)));

			AddState(L"BeamStart", shared_ptr<BossFirstBeamStartState>(new BossFirstBeamStartState(obj)));
			AddState(L"BeamFire", shared_ptr<BossFirstBeamFireState>(new BossFirstBeamFireState(obj)));
			AddState(L"BeamEnd", shared_ptr<BossFirstBeamEndState>(new BossFirstBeamEndState(obj)));

			AddState(L"Bonus", shared_ptr<BossFirstBonusState>(new BossFirstBonusState(obj)));
			AddState(L"Hit", shared_ptr<BossFirstHitState>(new BossFirstHitState(obj)));
			AddState(L"Stun", shared_ptr<BossFirstStunState>(new BossFirstStunState(obj)));
			AddState(L"StunRecover", shared_ptr<BossFirstStunRecoverState>(new BossFirstStunRecoverState(obj)));
			AddState(L"Dead", shared_ptr<BossFirstDeadState>(new BossFirstDeadState(obj)));

			ChangeState(L"Stand");
		}

	};

}
//end basecross
