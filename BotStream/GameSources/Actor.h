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
		int attack;
		//�h���
		int defense;
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

		shared_ptr<PNTBoneModelDraw> GetBoneModelDraw() {
			return GetComponent<PNTBoneModelDraw>();
		}

		shared_ptr<Transform> GetTransform() {
			return GetComponent<Transform>();
		}

		//�U������̓��e���X�V����
		void DefAttack(float activetime, HitInfo info) {
			m_AttackCol->SetHitInfo(info);
			m_AttackCol->ActiveCollision(activetime);
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

		//OnCollisionEnter�ɒu��
		void DetectBeingAttacked(shared_ptr<GameObject>& other);
	public:
		Actor(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale);
		~Actor();

		void OnCreate() override;
		void OnUpdate() override;

	};
	
}
//end basecross
