/*!
@file EnemyManager.h
@brief 敵管理用オブジェクト？
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
#include "Enemy.h"

//そのうち、m_enemies配列の特定の要素を、EnemyBaseを継承した別種類の敵で上書きする機能を追加するかも？

namespace basecross {
	class MyGameObject;
	class EnemyManager : public MyGameObject
	{
		//プール(10体 [0]はボス向け)
		shared_ptr<EnemyBase> m_enemies[23];
	public:
		EnemyManager(const shared_ptr<Stage>& stagePtr) : 
			MyGameObject(stagePtr)
		{
			bool isFirstofVector = true;

			//あらかじめ生成
			for (auto& e : m_enemies) {
				if (isFirstofVector) {
					isFirstofVector = !isFirstofVector;
					continue;
				}
				auto test = 0.0f;
				e = GetStage()->AddGameObject<EnemyZako>(Vec3(0), Vec3(0), Vec3(0));//ここを雑魚敵のクラスに変える
			}
		}
		~EnemyManager() {}

		//敵の配列を返す(trueならアクティブなもののみ)
		vector<shared_ptr<EnemyBase>> GetEnemyVec(bool onlyActives) {
			vector<shared_ptr<EnemyBase>> ret;
			for (auto e : m_enemies) {
				if (e == nullptr) continue;
				if (e->GetUsed() || !onlyActives) {
					ret.push_back(e);
				}
			}
			return ret;
		}

		//アクティブな敵の中で最も生成順の若いものを返す
		shared_ptr<EnemyBase> GetActiveEnemy() {
			for (auto e : m_enemies) {
				if (e == nullptr) continue;
				if (e->GetUsed() == false) {
					return e;
				}
			}
		}

		//番号で指定
		shared_ptr<EnemyBase> GetActiveEnemy(int element) {
			return m_enemies[element];
		}
		
		//敵の情報を入力して敵を1体出す
		void InstEnemy(Vec3 pos, Vec3 rot, Vec3 scale);

		//ボス敵のポインタを渡す用の変数
		void InstBoss(shared_ptr<EnemyBase> boss) {
			m_enemies[0] = boss;
			m_enemies[0]->Initialize();
		}
		//ボス敵のポインタを渡す用の変数
		void InstBoss(shared_ptr<EnemyBase> boss, Vec3 pos, Vec3 rot, Vec3 scale) {
			m_enemies[0] = boss;
			m_enemies[0]->Initialize(pos, rot, scale);
		}

		//ボス取得(いなかったらnullptrが返る)
		shared_ptr<EnemyBase> GetBoss() {
			return m_enemies[0];
		}

		void OnCreate() override;
	};

}
//end basecross

