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

	enum EnemyVariation {
		EVar_Normal,
		EVar_Projectile,
		EVar_Aerial
	};

	class EnemyManager : public MyGameObject
	{
		//プール(10体 [0]はボス向け)
		shared_ptr<EnemyBase> m_enemies[30];
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

				e = GetStage()->AddGameObject<EnemyZako>(Vec3(0), Vec3(0), Vec3(0));

			}
		}

		//vectorを渡すことで生成する敵の種類を変えられたらいいねという話
		EnemyManager(const shared_ptr<Stage>& stagePtr, vector<EnemyVariation>& enemyVariation) :
			MyGameObject(stagePtr)
		{
			bool isFirstofVector = true;

			int cnt = 0;
			//あらかじめ生成
			for (auto& e : m_enemies) {
				if (isFirstofVector) {
					isFirstofVector = !isFirstofVector;
					continue;
				}
				
				//バグ避け
				if (cnt >= enemyVariation.max_size()) {
					e = GetStage()->AddGameObject<EnemyZako>(Vec3(0), Vec3(0), Vec3(0));
					continue;
				}

				switch (enemyVariation[cnt]) 
				{
					//必要に応じてクラスを変える
				case EVar_Normal:
					e = GetStage()->AddGameObject<EnemyZako>(Vec3(0), Vec3(0), Vec3(0));
					break;

				case EVar_Projectile:
					e = GetStage()->AddGameObject<EnemyZakoLong>(Vec3(0), Vec3(0), Vec3(0));
					break;

				case EVar_Aerial:
					e = GetStage()->AddGameObject<EnemyZakoFlying>(Vec3(0), Vec3(0), Vec3(0));
					break;

				default:
					e = GetStage()->AddGameObject<EnemyZako>(Vec3(0), Vec3(0), Vec3(0));
					break;
				}

				cnt++;
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

		template<typename T>
		void InstEnemy(Vec3 pos, Vec3 rot, Vec3 scale) {
			for (auto& e : m_enemies) {

				shared_ptr<T> tmp = dynamic_pointer_cast<T>(e);

				if (tmp == nullptr) continue;
				if (e->GetUsed() == false) {
					e->Initialize(pos, rot, scale);
					return;
				}
			}
			return;
		}

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

