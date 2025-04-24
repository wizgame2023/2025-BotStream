/*!
@file Actor.h
@brief �v���C���[�Ȃ�
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
#include "LandDetect.h"
//#include "MyGameObject.h"

namespace basecross {
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
		//�����x(Friction�֐��Ŏg�p����̂�velocity��ϓ�������ꍇ�͂��̕ϐ����g���Ă�������)
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
		float m_disableLandDetect = 0.0f;
		//�n��ɂ��邩�ۂ�
		bool m_isLand = false;
		//�����Ă���p�x
		float m_angle;

		//��炢���[�V��������
		float m_hitbacktime = 0;
		//�U�����󂯂�����
		Vec3 m_hitDirection = Vec3(0);

		//�{�[�Y��Ԃ��ǂ���
		bool m_poseFlag = false;

		//�U������
		shared_ptr<AttackCollision> m_AttackCol;
		//���n����
		shared_ptr<LandDetect> m_LandDetect;
		//�󂯂��U���̏��
		HitInfo m_GetHitInfo;

		//���C
		void Friction();
		//���x����
		void SpeedLimit(float multiply);
		//�d��
		void Gravity();

		//�U�����󂯂����̏���(�p���p)
		virtual void OnDamaged() { }

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

		// �G�t�F�N�g�̍Đ�
		void EfkPlaying(const wstring efkKey, const float rad, const Vec3 rotate,Vec3 pushPos = Vec3(0.0f));
		void EfkPlaying(const wstring efkKey, const float rad, const Vec3 rotate, Col4 changeColor, Vec3 pushPos = Vec3(0.0f));
		// �n�ʒ��n
		void OnLanding();

		//OnCollisionEnter�ɒu��
		void DetectBeingAttacked(shared_ptr<GameObject>& other);
	public:
		Actor(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale);
		~Actor();

		void OnCreate() override;
		void OnUpdate() override;

		//�G�t�F�N�g���o������
		virtual void AddEffect(int addEffect);

		//�|�[�Y�̃t���O���I���I�t����֐�
		void PoseSwitch(bool onOff);

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
			m_hitbacktime = m_GetHitInfo.HitTime_Stand;

			Vec3 nrm = m_hitDirection.normalize();
			float dir = atan2f(nrm.z, nrm.x);

			Vec3 vel = m_GetHitInfo.HitVel_Stand;
			m_velocity.x = (cosf(dir) * vel.x) - (sinf(dir) * vel.z);
			m_velocity.y = vel.y;
			m_velocity.z = (cosf(dir) * vel.z) + (sinf(dir) * vel.x);
		}

		//�U������̃|�C���^���擾
		shared_ptr<AttackCollision> GetAttackPtr() {
			return m_AttackCol;
		}

		//�U������̓��e���X�V����
		void DefAttack(float activetime, HitInfo info) {
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

		Vec3 GetForward() {
			Vec3 vec = GetComponent<Transform>()->GetForward();

			const float rotate = -XM_PIDIV2;

			Vec3 fixedVec;
			fixedVec.x = (cosf(rotate) * vec.x) - (sinf(rotate) * vec.z);
			fixedVec.y = vec.y;
			fixedVec.z = (cosf(rotate) * vec.z) + (sinf(rotate) * vec.x);

			return fixedVec;
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
	};
	
}
//end basecross
