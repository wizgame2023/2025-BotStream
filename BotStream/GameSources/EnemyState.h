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

	// 立ち
	class BossFirstStandState : public StateBase {
		float m_time = 0;
		const float m_startAttack = 3.0f;
		const float m_startAttackRand = 10;
		const float m_spinRand = 400;
		const float m_slamRand = 250;
		const float m_sphereRand = 200;
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

	// プレイヤー追跡
	class BossFirstChaseState : public StateBase {
		float m_time = 0.0f;

		const float m_rotateThreshold = XM_PIDIV4 / 4;
		const float m_closeDist = 8;
		const float m_chaseSpeed = 20;
		const float m_sphereSwitchTime = 1.0f;

		random_device rnd;
		const float m_slamRand = 500;
		const float m_sphereRand = 2;
	public:
		BossFirstChaseState(shared_ptr<GameObject>& obj) :
			StateBase(obj) {
		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	// 近接1
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

	// 近接2
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

	// 回転開始
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

	// 回転
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

	// 回転終わり
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

	// エネルギー弾開始
	class BossFirstSphereStartState : public StateBase {
		float m_time = 0;
		const float m_startup = .5f;
		bool m_finishRotate = false;

		const float m_rotateSpeed = 2.0f;
		const float m_rotateThreshold = XM_PIDIV4 / 8;
	public:
		BossFirstSphereStartState(shared_ptr<GameObject>& obj) :
			StateBase(obj) {
		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	// エネルギー弾1
	class BossFirstSphere1State : public StateBase {
		float m_time = 0;
		const float m_fireTime[2] = { .25f, .3f };
		const float m_endTime = 1.0f;
		bool m_attacked[2] = { false };

		const Vec3 m_firePos[4] = {
			Vec3(3, 0, -6),
			Vec3(3, 0, 6),
			Vec3(3, 0, -3),
			Vec3(3, 0, 3)
		};
		const float m_towardPlayerTime[4] = {
			.8f,
			1.7f,
			1.1f,
			1.4f
		};
		const Vec3 m_fireHeight = Vec3(0, 7, 0);

		Vec3 FirePosAdjust(Vec3 fwd, Vec3 pos) {
			Vec3 ret;

			auto face = atan2f(fwd.z, fwd.x);

			ret.x = (cosf(face) * pos.x) - (sinf(face) * pos.z);
			ret.y = pos.y;
			ret.z = (cosf(face) * pos.z) + (sinf(face) * pos.x);

			return ret;
		}
	public:
		BossFirstSphere1State(shared_ptr<GameObject>& obj) :
			StateBase(obj) {
		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	// エネルギー弾終わり
	class BossFirstSphereEndState : public StateBase {
		float m_time = 0;
		const float m_recoveryTime = .75f;

	public:
		BossFirstSphereEndState(shared_ptr<GameObject>& obj) :
			StateBase(obj) {
		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	// ビーム開始
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

	// ビーム発射
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

	// ビーム終了
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

	// 叩きつけ初動
	class BossFirstSlamStartState : public StateBase {
		float m_time = 0;
		const float m_endTime = 1.1f;
		const float m_rotateThreshold = XM_PIDIV4 / 4;

	public:
		BossFirstSlamStartState(shared_ptr<GameObject>& obj) :
			StateBase(obj) {
		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	// 叩きつけ1段目
	class BossFirstSlam1State : public StateBase {
		float m_time = 0;
		const float m_attackTime = .2f;
		const float m_endTime = .5f;

		bool m_attacked = false;

	public:
		BossFirstSlam1State(shared_ptr<GameObject>& obj) :
			StateBase(obj) {
		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	// 叩きつけ2段目
	class BossFirstSlam2State : public StateBase {
		float m_time = 0;
		const float m_attackTime = .35f;
		const float m_endTime = .4f;

		bool m_attacked = false;

	public:
		BossFirstSlam2State(shared_ptr<GameObject>& obj) :
			StateBase(obj) {
		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	// 叩きつけ終了
	class BossFirstSlamEndState : public StateBase {
		float m_time = 0;
		const float m_endTime = 1.5f;

	public:
		BossFirstSlamEndState(shared_ptr<GameObject>& obj) :
			StateBase(obj) {
		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	// 被弾
	class BossFirstHitState : public StateBase {
	public:
		BossFirstHitState(shared_ptr<GameObject>& obj) :
			StateBase(obj) {
		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	// アマブレ
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

	// アマブレ復帰
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

	// 死亡
	class BossFirstDeadState : public StateBase {
	public:
		BossFirstDeadState(shared_ptr<GameObject>& obj) :
			StateBase(obj) {
		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	// ボーナス行動(何の意味もない威嚇)
	class BossFirstBonusState : public StateBase {
		const float m_end = 1.2f;
		const float m_sndTime = .4f;
		bool m_sndPlayed = false;

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

			AddState(L"SphereStart", shared_ptr<BossFirstSphereStartState>(new BossFirstSphereStartState(obj)));
			AddState(L"Sphere1", shared_ptr<BossFirstSphere1State>(new BossFirstSphere1State(obj)));

			AddState(L"BeamStart", shared_ptr<BossFirstBeamStartState>(new BossFirstBeamStartState(obj)));
			AddState(L"BeamFire", shared_ptr<BossFirstBeamFireState>(new BossFirstBeamFireState(obj)));
			AddState(L"BeamEnd", shared_ptr<BossFirstBeamEndState>(new BossFirstBeamEndState(obj)));

			AddState(L"SlamStart", shared_ptr<BossFirstSlamStartState>(new BossFirstSlamStartState(obj)));
			AddState(L"Slam1", shared_ptr<BossFirstSlam1State>(new BossFirstSlam1State(obj)));
			AddState(L"Slam2", shared_ptr<BossFirstSlam2State>(new BossFirstSlam2State(obj)));
			AddState(L"SlamEnd", shared_ptr<BossFirstSlamEndState>(new BossFirstSlamEndState(obj)));

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
