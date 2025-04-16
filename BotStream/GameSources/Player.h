/*!
@file Player.h
@brief �v���C���[�Ȃ�
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
#include "Actor.h"

namespace basecross{
	class EfkEffect;
	class StageSato;
	class Player : public Actor
	{
	private:
		//�W�����v�͂��c
		const float m_jumpPower = 18.0f;
		//�ō���
		const float m_speedMax = 10;
		//�_�b�V�����̍ō���
		const float m_speedMaxDash = 20.0f;
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
		//����������̃t���O
		bool m_dodgeFlag = false;
		//������Ԃ𑪂�ϐ�
		float m_dodgeTime;
		//�_�b�V���̃t���O
		bool m_dashFlag = false;
		//���n����𖳌������鎞��
		float m_disableLandDetect = 0.0f;

		float m_angle;//Player�������Ă���p�x

		// �قڂ��艟�������烁���o�[�v�m�F
		//shared_ptr<GameStage> m_Stage;
		shared_ptr<StageSato> m_Stage;

		//��������
		void PlayerMove();
		//�X�e�B�b�N����
		Vec3 GetMoveVector();

		void Jump();

		//�_�b�V������
		void Dash();

		//�������
		void Dodge();

		// �G�t�F�N�g�̍Đ�
		void EfkPlaying(const wstring efkKey,const float rad, const Vec3 rotate);
		// �n�ʒ��n
		void OnLanding();
	public:
		Player(const shared_ptr<Stage>& stagePtr,Vec3 pos,Vec3 rot,Vec3 scale);
		~Player();

		void OnCreate()override;//�쐬
		void OnUpdate()override;//�X�V

		float GetAngle();   //���v���C���[�������Ă�������̃Q�b�^�[
		void SetAngle(float angle);	//�v���C���[�̌����Ă�������̃Z�b�^�[


		//�f�o�b�N�p�̕�����
		void DebugLog();

	};

}
//end basecross

