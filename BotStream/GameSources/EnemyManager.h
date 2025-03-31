/*!
@file EnemyManager.h
@brief �G�Ǘ��p�I�u�W�F�N�g�H
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
#include "Enemy.h"

namespace basecross {
	class MyGameObject;
	class EnemyManager : public MyGameObject
	{
		//�v�[��(10��)
		shared_ptr<Enemy> m_enemies[10];
	public:
		EnemyManager(const shared_ptr<Stage>& stagePtr) : 
			MyGameObject(stagePtr)
		{
			//���炩���ߐ���
			for (auto& e : m_enemies) {
				e = GetStage()->AddGameObject<Enemy>(Vec3(0), Vec3(0), Vec3(0));
			}
		}
		~EnemyManager() {}

		//�G�̔z���Ԃ�(true�Ȃ�A�N�e�B�u�Ȃ��̂̂�)
		vector<shared_ptr<Enemy>> GetEnemyVec(bool onlyActives) {
			vector<shared_ptr<Enemy>> ret;
			for (auto e : m_enemies) {
				if (e->GetUsed() || !onlyActives) {
					ret.push_back(e);
				}
			}
			return ret;
		}

		//�A�N�e�B�u�ȓG�̒��ōł��������̎Ⴂ���̂�Ԃ�
		shared_ptr<Enemy> GetActiveEnemy() {
			for (auto e : m_enemies) {
				if (e->GetUsed() == false) {
					return e;
				}
			}
		}

		//�ԍ��Ŏw��
		shared_ptr<Enemy> GetActiveEnemy(int element) {
			for (auto e : m_enemies) {
				if (e->GetUsed() == false) {
					return e;
				}
			}
		}
		
		//�G�̏�����͂��ēG��1�̏o��
		void InstEnemy(Vec3 pos, Vec3 rot, Vec3 scale);

		void OnCreate() override;
	};

}
//end basecross

