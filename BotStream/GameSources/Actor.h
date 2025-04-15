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

		shared_ptr<AttackCollision> m_AttackCol;
		//shared_ptr<State> m_State;

		shared_ptr<LandDetect> m_LandDetect;

		//���C
		void Friction();
		//���x����
		void SpeedLimit(float multiply);
		//�d��
		void Gravity();

		//�U�����󂯂����̏���(�p���p)
		virtual void OnDamaged() {

		}
	public:
		Actor(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale);
		~Actor();

		void OnCreate() override;
		void OnUpdate() override;
	};
	
}
//end basecross
