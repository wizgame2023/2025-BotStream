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

		int m_stun = 0;
		int m_stunMax = 3;

		float m_armorMax = 0;
		float m_armor = 0;
		float m_armorRecover = 0;
		float m_armorRecoverTime = 0;

		float m_armorFlash = 0;
		const float m_armorFlashMax = .1f;
		Col4 m_armorFlashFX = Col4(5.0f, .5f, .5f, 0);

		weak_ptr<Player> m_player;


	public:
		EnemyBase(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale);
		EnemyBase(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, bool use);
		~EnemyBase() {}

		virtual void HitBackStandBehavior();

		void DetectBeingAttacked(shared_ptr<GameObject>& other)override;

		void ChangeState(wstring stateName) {
			m_state->ChangeState(stateName);
		}
		wstring GetStateName() {
			return m_state->GetStateName();
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
		void SetUsed(bool used) {
			m_used = used;
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

		/// <summary>
		/// HP�̗ʂ�ς���(�X�e�[�W���̍��ق��o������)
		/// </summary>
		/// <param name="max">HP�l</param>
		void ChangeHPMax(int max) {
			m_HPMax = max;
			m_HPCurrent = max;
		}
		/// <summary>
		/// �A�[�}�[�ʂ�ς���(�X�e�[�W���̍��ق��o������)
		/// </summary>
		/// <param name="max">�A�[�}�[�ϋv�l</param>
		void ChangeArmorMax(float max) {
			m_armorMax = max;
			m_armor = max;
		}
		void ChangeArmorMax(float max, float recovertime) {
			m_armorMax = max;
			m_armor = max;
			m_armorRecoverTime = recovertime;
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
		
		//�Ǝ������蔻��
		shared_ptr<CollisionCapsule> m_innerCollision;

		//���݂̔��a
		float m_radius = 0;
		//�L���鑬�x
		const float m_radiateSpeed = 72.0f;
		//���߂��n�܂锼�a
		const float m_radiusStartFade = 45.0f;
		//���a�̍ő�
		const float m_radiusMax = 180.0f;
		//�O�~�Ɠ��~�̍�
		const float m_widthCircle = .5f;

		const float m_height = 4.0f;

		int m_isPlayerInsideCnt = 0;
		const int m_isPlayerInsideCntMax = 1;

		//�����ڊ֌W
		shared_ptr<BcPCTStaticDraw> m_ptrDraw;
		vector<uint16_t> m_indices;
		vector<VertexPositionColorTexture> m_vertices;

		Vec2 m_loop = Vec2(1.0f, 1.0f);
		int m_numOfVertices = 12;
		float m_meshHeight = 1.5f;
		float m_topRadiusPlus = 1.2f;
		float m_btmRadiusPlus = 1.0f;
		Col4 m_topColor = Col4(1, 1, 1, 1);
		Col4 m_btmColor = Col4(1, 1, 1, 1);
		wstring m_texKey = L"Tex_Shockwave";

		void HitInfoInit() override;

		void DrawInit();
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

	/// <summary>
	/// �{�X1�̃G�l���M�[�e
	/// </summary>
	class BossFirstSphere : public Actor {
	protected:
		float m_time = 0;
		Quat m_face;

		const float m_firstMoveSpeed = 120.0f;
		float m_firstMoveTime = .8f;
		const float m_speedDown = .65f;

		bool m_towardPlayer = false;
		Vec3 m_secondMoveAngle;
		const float m_secondMoveSpeed = 300.0f;

		bool m_disappear = false;
		float m_disappearTime = 0;
		const float m_disappearTimeMax = 1.0f;

		weak_ptr<Player> m_player;

		//�G�t�F�N�g
		Effekseer::Handle m_effect;

		void CreateChildObjects() override;
	public:
		BossFirstSphere(const shared_ptr<Stage>& stagePtr, Vec3 pos, Quat rot, Vec3 scale, float towardPlayerTime) :
			Actor(stagePtr, pos, Vec3(0), scale),
			m_face(rot),
			m_firstMoveTime(towardPlayerTime)
		{

		}
		~BossFirstSphere() {}

		void OnCreate() override;
		void OnUpdate() override;

		void CollidedWithTerrain();
	};

	/// <summary>
	/// �G�l���M�[�e��p�̓����蔻��I�u�W�F�N�g
	/// </summary>
	class BossFirstSphereCollision : public AttackCollision {
		shared_ptr<BossFirstSphere> m_obj;

	public:
		BossFirstSphereCollision(const shared_ptr<Stage>& stagePtr, const shared_ptr<BossFirstSphere>& obj) :
			AttackCollision(stagePtr),
			m_obj(obj)
		{

		}
		~BossFirstSphereCollision() {

		}
		void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
	};
}
//end basecross

