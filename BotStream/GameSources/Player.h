/*!
@file Player.h
@brief �v���C���[�Ȃ�
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
#include "Actor.h"

namespace basecross{
	class Player : public Actor
	{
	private:
		//�o�ߎ���
		float _delta;
		//���x
		Vec3 m_velocity;
		Vec3 m_accel;
		//�W�����v�͂��c
		const float m_jumpPower = 18.0f;
		//�ō���
		const float m_speedMax = 10;
		//�����x
		const float m_baseAccel = 400.0f;
		//�d��
		const float m_gravity = -32.0f;
		//�������̏I�[���x
		const float m_fallTerminal = -120.0f;
		//���C�W��(��/��/���S��~)
		const float m_friction = .75f;
		const float m_frictionDynamic = .5f;
		const float m_frictionThreshold = .5f;
		//�X�e�B�b�N�̃f�b�h�]�[��(���Ƃ�manager�n�ɒu������)
		float m_stickDeadZone = .1f;
		//�n��ɂ��邩�ۂ�
		bool m_isLand = false;
		//���n����𖳌������鎞��
		float m_disableLandDetect = 0.0f;

		float m_angle;//Player�������Ă���p�x

		//��������
		void PlayerMove();
		//�X�e�B�b�N����
		Vec3 GetMoveVector();
		//���C
		void Friction();
		//���x����
		void SpeedLimit(float multiply);
		//�d��
		void Gravity();

		void Jump();

	public:
		Player(const shared_ptr<Stage>& stagePtr,Vec3 pos,Vec3 rot,Vec3 scale);
		~Player();

		void OnCreate()override;//�쐬
		void OnUpdate()override;//�X�V

		float GetAngle();   //���v���C���[�������Ă�������̃Q�b�^�[
		void SetAngle(float angle);	//�v���C���[�̌����Ă�������̃Z�b�^�[

		shared_ptr<PNTBoneModelDraw> GetBoneModelDraw() {
			return GetComponent<PNTBoneModelDraw>();
		}
		shared_ptr<Transform> GetTransform() {
			return GetComponent<Transform>();
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

		//�f�o�b�N�p�̕�����
		void DebugLog();

	};

}
//end basecross

