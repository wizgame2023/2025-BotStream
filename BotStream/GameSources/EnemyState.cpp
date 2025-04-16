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

	}
	void EnemyWalkState::Exit() {

	}

	void EnemyAttackState::Enter() {

	}
	void EnemyAttackState::Update(float deltatime) {
		auto p = dynamic_pointer_cast<EnemyBase>(_obj);

	}
	void EnemyAttackState::Exit() {

	}
}
//end basecross
