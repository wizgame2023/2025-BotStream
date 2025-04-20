/*!
@file Enemy.h
@brief �G�Ȃ�
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
#include "Actor.h"
#include "EnemyState.h"

namespace basecross{
	class EnemyStateMachine;
	class EnemyBase : public Actor
	{
	protected:
		bool m_used = false;
		shared_ptr<StateMachineBase> m_state;

		int m_armor;

		weak_ptr<Player> m_player;

		void RegisterAnim();

	public:
		EnemyBase(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale);
		EnemyBase(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, bool use);
		~EnemyBase() {}

		void HitBackStandBehavior();

		void ChangeState(wstring stateName) {
			m_state->ChangeState(stateName);
		}

		void OnCreate() override;
		void OnUpdate() override;

		//����������(�G���X�|�[��������Ƃ��g���H)
		void Initialize(Vec3 pos, Vec3 rot, Vec3 scale) {
			m_used = true;
			auto ptr = GetComponent<Transform>();
			ptr->SetPosition(pos);
			ptr->SetRotation(rot);
			ptr->SetScale(scale);
			SetDrawActive(true);
			SetUpdateActive(true);
			m_LandDetect->SetUpdateActive(true);
		}

		bool GetUsed() {
			return m_used;
		}

		//�v���C���[�Ƃ̋����𑪂�
		float GetPlayerDist();

		//�v���C���[�Ƃ̊p�x�̍��𑪂�
		float GetPlayerSubDirection();
	};

	class BossFirst : public EnemyBase {
		void OnDamaged() override;

	public:
		BossFirst(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, bool used = false) :
			EnemyBase(stagePtr, pos, rot, scale, used)
		{

		}
		~BossFirst() {}

		void OnCreate() override;
		void OnUpdate() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
	};
}
//end basecross

