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
		const float m_rotateSpeed = 1.2f;
		const float m_rotateThreshold = .3f;

		float m_armorMax = 0;
		float m_armor = 0;
		float m_armorRecover = 0;
		float m_armorRecoverTime = 0;

		float m_armorFlash = 0;
		const float m_armorFlashMax = .1f;
		Col4 m_armorFlashFX = Col4(2.0f, .5f, .5f, 0);

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
		//����������(��������)
		void Initialize() {
			m_used = true;
			SetDrawActive(true);
			SetUpdateActive(true);
			m_LandDetect->SetUpdateActive(true);
		}

		bool GetUsed() {
			return m_used;
		}

		//�v���C���[�Ƃ̋���(Vec3)
		Vec3 GetPlayerDistInVec3();

		//�v���C���[�Ƃ̋����𑪂�
		float GetPlayerDist();

		//�v���C���[�Ƃ̊p�x�̍��𑪂�
		float GetPlayerSubDirection();

		//��]
		void RotateToPlayer(const float multiply, const float threshold);
		void RotateToPlayer(const float multiply);

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

