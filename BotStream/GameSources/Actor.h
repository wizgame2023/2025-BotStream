/*!
@file Actor.h
@brief �v���C���[�Ȃ�
*/

#pragma once
#include "stdafx.h"
//#include "Project.h"
#include "LandDetect.h"
//#include "MyGameObject.h"

namespace basecross {
	enum ActorEffect
	{
		PlayerEffect_Attack1,
		PlayerEffect_Attack2,
		PlayerEffect_Attack3,
		PlayerEffect_AttackEx,
		PlayerEffect_Beam,
		PlayerEffect_Dodge,
		PlayerEffect_Dash,
		PlayerEffect_DashRipple,
		EnemyEffect_Stun,
		EnemyEffect_ArmorBreak,
		EnemyEffect_Beam,
		EnemyEffect_Sphere,
		EnemyEffect_SphereEnd,
		EnemyEffect_Spin,
		EnemyEffect_Wave
	};

	class Actor : public ObjectMove
	{

	protected:
		shared_ptr<Transform> m_trans;
		Vec3 m_pos;//�ʒu
		Vec3 m_rot;//��]
		Vec3 m_scale;//�傫��

		//�ő�HP
		int m_HPMax;
		//�U����
		int m_attack;
		//�h���
		int m_defense;
		//����HP
		int m_HPCurrent;

		//�o�ߎ���
		float _delta;
		//���x
		Vec3 m_velocity;
		//�����x(Friction�֐��Ŏg�p)
		Vec3 m_accel;
		//�ō���
		float m_speedMax = 10;
		//�����x�ɂ����鐔
		float m_baseAccel = 400.0f;
		//�d��
		float m_gravity = -32.0f;
		//�������̏I�[���x
		float m_fallTerminal = -120.0f;
		//���C�W��(��/��/���S��~)
		float m_friction = .75f;
		float m_frictionDynamic = .5f;
		float m_frictionThreshold = .5f;

		//���n����𖳌������鎞��
		float m_landDetectDisableTime = 0.0f;
		//�n��ɂ��邩�ۂ�
		bool m_isLand = false;
		//�����Ă���p�x
		float m_angle;
		//�d�́E���C�����̗L��
		bool m_doPhysics = true;

		//��炢���[�V��������
		float m_hitbacktime = 0;
		//�U�����󂯂�����
		Vec3 m_hitDirection = Vec3(0);

		//SE�֌W
		shared_ptr<SoundItem> m_SE = nullptr;//�Đ����Ă���SE
		shared_ptr<XAudio2Manager> m_SEManager = nullptr;//SE�Ȃǂ��Đ����邽�߂̃}�l�[�W��

		//�{�[�Y��Ԃ��ǂ���
		bool m_PauseFlag = false;

		//�U������
		shared_ptr<AttackCollision> m_AttackCol;
		//���n����
		shared_ptr<LandDetect> m_LandDetect;
		//�󂯂��U���̏��
		HitInfo m_getHitInfo;

		//���C
		void Friction();
		//���x����
		void SpeedLimit(float multiply);
		//�d��
		void Gravity();

		//�U�����󂯂����̏���(�p���p)
		virtual void OnDamaged() {}

		//�_���[�W�v�Z��
		int CalculateDamage(int damage) {
			int ret = damage - m_defense;
			return ret <= 0 ? 1 : ret;
		}

		shared_ptr<PNTBoneModelDraw> GetBoneModelDraw() {
			return GetComponent<PNTBoneModelDraw>();
		}

		shared_ptr<Transform> GetTransform() {
			return GetComponent<Transform>();
		}

		//velocity��Transform��Position�ɂԂ����ލ��
		void UpdatePosition() {
			GetTransform()->SetPosition((m_velocity * _delta) + GetTransform()->GetPosition());
		}

		// �G�t�F�N�g�̍Đ�
		Effekseer::Handle EfkPlaying(const wstring efkKey, const float rad, const Vec3 rotate, const Vec3 scale = Vec3(1.0f), Vec3 pushPos = Vec3(0.0f));
		Effekseer::Handle EfkPlaying(const wstring efkKey, const float rad, const Vec3 rotate, Col4 changeColor, Vec3 pushPos = Vec3(0.0f));
		//void EfkPlaying(const wstring efkKey, const float rad, const Vec3 rotate, const Vec3 scale = Vec3(1.0f), Vec3 pushPos = Vec3(0.0f));
		
		// �n�ʒ��n
		void OnLanding();

		//OnCollisionEnter�ɒu��
		void DetectBeingAttacked(shared_ptr<GameObject>& other);

		//OnCreate�Ŏ��s
		virtual void CreateChildObjects() {
			auto stage = GetStage();

			//���n����̐���
			m_LandDetect = stage->AddGameObject<LandDetect>();
			m_LandDetect->GetComponent<Transform>()->SetParent(dynamic_pointer_cast<GameObject>(GetThis<Actor>()));

			//�U������̐���
			m_AttackCol = stage->AddGameObject<AttackCollision>();
			m_AttackCol->GetComponent<Transform>()->SetParent(dynamic_pointer_cast<GameObject>(GetThis<Actor>()));
		}

		//�폜����
		void RemoveSelf() {
			GetStage()->RemoveGameObject<LandDetect>(m_LandDetect);
			GetStage()->RemoveGameObject<AttackCollision>(m_AttackCol);
			GetStage()->RemoveGameObject<Actor>(GetThis<Actor>());
		}

	public:
		Actor(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale);
		~Actor();

		void OnCreate() override;
		void OnUpdate() override;

		//�G�t�F�N�g���o������
		virtual Effekseer::Handle AddEffect(int addEffect);

		//�|�[�Y�̃t���O���I���I�t����֐�
		void SetPause(bool onOff);

		//HP�֌W�̃Q�b�^�Z�b�^
		int GetHPCurrent() {
			return m_HPCurrent;
		}
		void SetHPCurrent(int setHP) {
			m_HPCurrent = setHP;
		}
		int GetHPMax() {
			return m_HPMax;
		}

		//�������U���̐�����΂���������(����n��̂��̂̂�)
		void HitBack() {
			m_hitbacktime = m_getHitInfo.HitTime_Stand;

			//�ǂ��炩��U�����ꂽ�����v�Z
			Vec3 nrm = m_hitDirection.normalize();
			float dir = atan2f(nrm.z, nrm.x);

			Vec3 vel = (m_isLand) ? m_getHitInfo.HitVel_Stand : m_getHitInfo.HitVel_Air;

			Vec3 accel;
			accel.x = (cosf(dir) * vel.x) - (sinf(dir) * vel.z);
			accel.y = vel.y;
			accel.z = (cosf(dir) * vel.z) + (sinf(dir) * vel.x);
			
			SetVelocity(accel);
		}

		//�󂯂��U���̏����擾
		HitInfo GetHitInfo() {
			return m_getHitInfo;
		}

		//�U������̃|�C���^���擾
		shared_ptr<AttackCollision> GetAttackPtr() {
			return m_AttackCol;
		}

		//�U������̓��e���X�V����
		void DefAttack(float activetime, HitInfo info) {
			m_AttackCol->SetMoveContact(false);
			m_AttackCol->SetHitInfo(info);
			m_AttackCol->ActivateCollision(activetime);
		}

		float GetAngle();   //�������Ă�������̃Q�b�^�[
		void SetAngle(float angle);	//�����Ă�������̃Z�b�^�[

		Vec3 GetVelocity() {
			return m_velocity;
		}
		void SetVelocity(Vec3 vel) {
			m_velocity = vel;
		}
		void AddVelocity(Vec3 vel) {
			m_accel = vel;
			m_velocity += vel;
		}

		//�O���x�N�g���̎擾
		Vec3 GetForward() {
			Vec3 vec = GetComponent<Transform>()->GetForward();

			const float rotate = -XM_PIDIV2;

			Vec3 fixedVec;
			fixedVec.x = (cosf(rotate) * vec.x) - (sinf(rotate) * vec.z);
			fixedVec.y = vec.y;
			fixedVec.z = (cosf(rotate) * vec.z) + (sinf(rotate) * vec.x);

			return fixedVec;
		}

		//�n�ʂ̏�ɂ��邩�ۂ��̃Q�b�^�[
		bool GetLand() {
			return m_isLand;
		}

		//�����I�����̃Z�b�^
		void SetPhysics(bool doPhysics) {
			m_doPhysics = doPhysics;
		}
		//�����I�����̃Q�b�^
		bool GetPhysics() {
			return m_doPhysics;
		}

		//�A�j���[�V�����ύX(���������ꍇtrue��Ԃ�)
		bool ChangeAnim(wstring anim, bool forceChange = false) {
			auto drawPtr = GetBoneModelDraw();
			//���ɍĐ����Ȃ�ύX���Ȃ��@forceChange�̏ꍇ�͗�O
			if (drawPtr->GetCurrentAnimation() != anim || forceChange)
			{
				drawPtr->ChangeCurrentAnimation(anim);
				return true;
			}
			else {
				return false;
			}
		}

		//SE�̍Đ�
		void PlaySnd(wstring sndname, float volume, float loopcount) {
			m_SE = m_SEManager->Start(sndname, loopcount, volume);
		}

		//SE�̒�~
		void StopSnd() {
			if (m_SE == nullptr) {
				return;
			}
			m_SEManager->Stop(m_SE);
		}
	};

	/// <summary>
	/// ��ѓ���̐e�N���X
	/// </summary>
	class ProjectileBase :public Actor
	{
	protected:
		float m_speed = 1.0f;
		float m_originAngle = 0.0f;
		float m_canMoveDistance;//�ړ��ł��钷��

		weak_ptr<Actor> m_originObj;//�����𐶐������I�u�W�F�N�g

		//�U�������ݒ肷�邽�߂̊֐��BOnCreate�ɒu��
		virtual void HitInfoInit() { }
	public:
		ProjectileBase(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, shared_ptr<Actor> originObj) :
			Actor(stagePtr, pos, rot, scale),
			m_originObj(originObj)
		{

		}
		~ProjectileBase()
		{
		}

		void OnCreate()override;
		void OnUpdate()override;


	};

}
//end basecross
