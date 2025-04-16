/*!
@file EnemyManager.h
@brief 敵管理用オブジェクト？
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
#include "Enemy.h"

namespace basecross {
	class MyGameObject;
	class EnemyManager : public MyGameObject
	{
		//プール(10体)
		shared_ptr<EnemyBase> m_enemies[10];
		//ボス
		shared_ptr<EnemyBase> m_boss;
	public:
		EnemyManager(const shared_ptr<Stage>& stagePtr) : 
			MyGameObject(stagePtr)
		{
			//あらかじめ生成
			for (auto& e : m_enemies) {
				e = GetStage()->AddGameObject<EnemyBase>(Vec3(0), Vec3(0), Vec3(0));
			}
		}
		~EnemyManager() {}

		//敵の配列を返す(trueならアクティブなもののみ)
		vector<shared_ptr<EnemyBase>> GetEnemyVec(bool onlyActives) {
			vector<shared_ptr<EnemyBase>> ret;
			for (auto e : m_enemies) {
				if (e->GetUsed() || !onlyActives) {
					ret.push_back(e);
				}
			}
			return ret;
		}

		//アクティブな敵の中で最も生成順の若いものを返す
		shared_ptr<EnemyBase> GetActiveEnemy() {
			for (auto e : m_enemies) {
				if (e->GetUsed() == false) {
					return e;
				}
			}
		}

		//番号で指定
		shared_ptr<EnemyBase> GetActiveEnemy(int element) {
			for (auto e : m_enemies) {
				if (e->GetUsed() == false) {
					return e;
				}
			}
		}
		
		//敵の情報を入力して敵を1体出す
		void InstEnemy(Vec3 pos, Vec3 rot, Vec3 scale);

		void OnCreate() override;
	};

}
//end basecross

