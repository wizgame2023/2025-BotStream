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

		if (m_time >= m_startAttack && rnd() % 1000 <= m_startAttackRand) {
			auto boss = dynamic_pointer_cast<BossFirst>(_obj);
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

	}
	void BossFirstAttackState::Update(float deltatime) {

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
