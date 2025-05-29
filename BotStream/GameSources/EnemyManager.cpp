/*!
@file EnemyManager.cpp
@brief 敵管理用オブジェクト？
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
#include "Enemy.h"

namespace basecross {
	
	//型名を指定する方
	void EnemyManager::InstEnemy(Vec3 pos, Vec3 rot, Vec3 scale) {
		for (auto& e : m_enemies) {

			if (e == nullptr) continue;
			if (e->GetUsed() == false) {
				e->Initialize(pos, rot, scale);
				return;
			}

		}
	}

	void EnemyManager::OnCreate() {
	}

}
//end basecross

