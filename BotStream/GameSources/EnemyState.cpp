/*!
@file EnemyState.cpp
@brief 敵のステート
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	void EnemyWalkState::Enter() {

	}
	void EnemyWalkState::Update(float deltatime) {
		auto p = dynamic_pointer_cast<EnemyBase>(_obj);
		p->ChangeAnim(L"Idle");

	}
	void EnemyWalkState::Exit() {

	}

	void EnemyAttackState::Enter() {

	}
	void EnemyAttackState::Update(float deltatime) {

	}
	void EnemyAttackState::Exit() {

	}

	void EnemyHitState::Enter() {
		auto p = dynamic_pointer_cast<EnemyBase>(_obj);
		p->ChangeAnim(L"HitBack", true);
		p->HitBack();
	}
	void EnemyHitState::Update(float deltatime) {
		auto p = dynamic_pointer_cast<EnemyBase>(_obj);
		p->HitBackStandBehavior();
	}
	void EnemyHitState::Exit() {

	}

	//------------------------------------------------------------------------------------------
	// BOSS1
	//------------------------------------------------------------------------------------------

	void BossFirstStandState::Enter() {
		m_time = 0;
		auto boss = dynamic_pointer_cast<BossFirst>(_obj);
		boss->ChangeAnim(L"Idle");
	}
	void BossFirstStandState::Update(float deltatime) {
		m_time += deltatime;

		auto boss = dynamic_pointer_cast<BossFirst>(_obj);
		float playerDir = boss->GetPlayerSubDirection();

		if (abs(playerDir) > m_rotateSpeed) {
			Quat q = boss->GetQuaternion();
			q = boss->RotateQuat(q, Vec3(0, 1, 0), m_rotateSpeed * deltatime * (playerDir < 0 ? -1 : 1));
			boss->SetQuaternion(q);
		}

		if (m_time >= m_startAttack && rnd() % 1000 <= m_startAttackRand) {
			const float dist = boss->GetPlayerDist();

			if (dist < m_farDist) {
				//近い！
				boss->ChangeState(L"Attack");
			}
			else {
				//遠い！
				boss->ChangeState(L"Chase");
			}
		}
	}
	void BossFirstStandState::Exit() {

	}


	void BossFirstChaseState::Enter() {

	}
	void BossFirstChaseState::Update(float deltatime) {

	}
	void BossFirstChaseState::Exit() {

	}


	void BossFirstAttackState::Enter() {
		auto boss = dynamic_pointer_cast<EnemyBase>(_obj);
		boss->ChangeAnim(L"AttackClose1", true);
		m_time = 0;
		m_attacked = false;
	}
	void BossFirstAttackState::Update(float deltatime) {
		m_time += deltatime;
		
		auto boss = dynamic_pointer_cast<EnemyBase>(_obj);
		//攻撃判定の定義
		if (m_time >= 0.3f && !m_attacked) {
			m_attacked = !m_attacked;

			auto tmp = boss->GetAttackPtr()->GetHitInfo();
			tmp.HitOnce = true;
			tmp.Type = AttackType::Enemy;
			tmp.Damage = 10;
			tmp.HitVel_Stand = Vec3(-5, 0, 0);
			tmp.HitTime_Stand = 1.2f;
			boss->DefAttack(.3f, tmp);
			boss->GetAttackPtr()->SetPos(Vec3(1, 1, 0));
		}

		if (m_time >= m_end) {
			boss->ChangeState(L"Stand");
		}
	}
	void BossFirstAttackState::Exit() {

	}

	void BossFirstHitState::Enter() {
		auto p = dynamic_pointer_cast<BossFirst>(_obj);
		p->ChangeAnim(L"HitBack");
		p->HitBack();
	}
	void BossFirstHitState::Update(float deltatime) {
		auto p = dynamic_pointer_cast<BossFirst>(_obj);
		p->HitBackStandBehavior();
	}
	void BossFirstHitState::Exit() {

	}
}
//end basecross
