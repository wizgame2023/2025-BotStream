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

	/// <summary>
	/// �e�N���X
	/// </summary>
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
			m_landDetectDisableTime = 0;
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

		//�A�[�}�[�̏󋵂�0.0f�`1.0f�ŕԂ�
		float GetArmorePercentage() {
			if (m_armor > 0) {
				return m_armor / m_armorMax;
			}
			else {
				return m_armorRecover / m_armorRecoverTime;
			}
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

	/// <summary>
	/// �X�e�[�W1�̃{�X
	/// </summary>
	class BossFirst : public EnemyBase {
		void OnDamaged() override;
		void RegisterAnim();

		//�r�[���p
		bool m_isRecoveredFromArmorBreak = false;
		float m_prevArmor = 0.0f;
	public:
		BossFirst(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, bool used = false) :
			EnemyBase(stagePtr, pos, rot, scale, used)
		{

		}
		~BossFirst() {}

		//�A�[�}�[�u���C�N����񕜂����Ƃ�true�ɂȂ�
		bool IsRecoveredFromArmorBreak() {
			bool ret = m_isRecoveredFromArmorBreak;
			m_isRecoveredFromArmorBreak = false;
			return ret;
		}

		void OnCreate() override;
		void OnUpdate() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
	};

	/// <summary>
	/// �{�X1�̏Ռ��g
	/// </summary>
	class BossFirstShockwave : public ProjectileBase {
	protected:
		shared_ptr<CollisionCapsule> m_innerCollision;

		//���݂̔��a
		float m_radius = 0;
		//�L���鑬�x
		float m_radiateSpeed = 54.0f;
		//���a�̍ő�
		float m_radiusMax = 108.0f;
		//�O�~�Ɠ��~�̍�
		float m_widthCircle = .5f;

		const float m_height = 2.0f;

		int m_isPlayerInsideCnt = 0;
		const int m_isPlayerInsideCntMax = 1;

		void HitInfoInit() override;
	public:
		BossFirstShockwave(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, shared_ptr<Actor> originObj) :
			ProjectileBase(stagePtr, pos, rot, scale, originObj)
		{

		}
		~BossFirstShockwave() {}

		void OnCreate() override;
		void OnUpdate() override;

		void OnCollisionExcute(shared_ptr<GameObject>& Other) override;
	};

	/// <summary>
	/// �{�X1�̃r�[������
	/// </summary>
	class BossFirstBeam : public ProjectileBase {
	protected:
		float m_hitBeamVel = 12.0f;
		bool m_isFinalBlow = false;

		void HitInfoInit() override;
	public:
		BossFirstBeam(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, shared_ptr<Actor> originObj, float hitVel, bool final) :
			ProjectileBase(stagePtr, pos, rot, scale, originObj),
			m_hitBeamVel(hitVel),
			m_isFinalBlow(final)
		{

		}
		~BossFirstBeam() {}

		void OnCreate() override;

	};
}
//end basecross

