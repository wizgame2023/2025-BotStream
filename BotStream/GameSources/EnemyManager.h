/*!
@file EnemyManager.h
@brief �G�Ǘ��p�I�u�W�F�N�g�H
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
#include "Enemy.h"

//���̂����Am_enemies�z��̓���̗v�f���AEnemyBase���p�������ʎ�ނ̓G�ŏ㏑������@�\��ǉ����邩���H

namespace basecross {
	class MyGameObject;

	enum EnemyVariation {
		EVar_Normal,
		EVar_Projectile,
		EVar_Aerial
	};

	class EnemyManager : public MyGameObject
	{
		//�v�[��(10�� [0]�̓{�X����)
		shared_ptr<EnemyBase> m_enemies[30];
	public:
		EnemyManager(const shared_ptr<Stage>& stagePtr) :
			MyGameObject(stagePtr)
		{
			bool isFirstofVector = true;

			//���炩���ߐ���
			for (auto& e : m_enemies) {
				if (isFirstofVector) {
					isFirstofVector = !isFirstofVector;
					continue;
				}

				e = GetStage()->AddGameObject<EnemyZako>(Vec3(0), Vec3(0), Vec3(0));

			}
		}

		//vector��n�����ƂŐ�������G�̎�ނ�ς���ꂽ�炢���˂Ƃ����b
		EnemyManager(const shared_ptr<Stage>& stagePtr, vector<EnemyVariation>& enemyVariation) :
			MyGameObject(stagePtr)
		{
			bool isFirstofVector = true;

			int cnt = 0;
			//���炩���ߐ���
			for (auto& e : m_enemies) {
				if (isFirstofVector) {
					isFirstofVector = !isFirstofVector;
					continue;
				}
				
				//�o�O����
				if (cnt >= enemyVariation.max_size()) {
					e = GetStage()->AddGameObject<EnemyZako>(Vec3(0), Vec3(0), Vec3(0));
					continue;
				}

				switch (enemyVariation[cnt]) 
				{
					//�K�v�ɉ����ăN���X��ς���
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

		//�G�̔z���Ԃ�(true�Ȃ�A�N�e�B�u�Ȃ��̂̂�)
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

		//�A�N�e�B�u�ȓG�̒��ōł��������̎Ⴂ���̂�Ԃ�
		shared_ptr<EnemyBase> GetActiveEnemy() {
			for (auto e : m_enemies) {
				if (e == nullptr) continue;
				if (e->GetUsed() == false) {
					return e;
				}
			}
		}

		//�ԍ��Ŏw��
		shared_ptr<EnemyBase> GetActiveEnemy(int element) {
			return m_enemies[element];
		}
		
		//�G�̏�����͂��ēG��1�̏o��
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

		//�{�X�G�̃|�C���^��n���p�̕ϐ�
		void InstBoss(shared_ptr<EnemyBase> boss) {
			m_enemies[0] = boss;
			m_enemies[0]->Initialize();
		}
		//�{�X�G�̃|�C���^��n���p�̕ϐ�
		void InstBoss(shared_ptr<EnemyBase> boss, Vec3 pos, Vec3 rot, Vec3 scale) {
			m_enemies[0] = boss;
			m_enemies[0]->Initialize(pos, rot, scale);
		}

		//�{�X�擾(���Ȃ�������nullptr���Ԃ�)
		shared_ptr<EnemyBase> GetBoss() {
			return m_enemies[0];
		}

		void OnCreate() override;
	};

}
//end basecross

