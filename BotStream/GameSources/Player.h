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
		shared_ptr<Transform> m_trans;
		Vec3 m_pos;//�ʒu
		Vec3 m_rot;//��]
		Vec3 m_scale;//�傫��

		float m_angle;//Player�������Ă���p�x

	public:
		Player(const shared_ptr<Stage>& stagePtr,Vec3 pos,Vec3 rot,Vec3 scale);
		~Player();

		void OnCreate()override;//�쐬
		void OnUpdate()override;//�X�V

		//��������
		void PlayerMove();

		float GetAngle();   //���v���C���[�������Ă�������̃Q�b�^�[
		void SetAngle(float angle);	//�v���C���[�̌����Ă�������̃Z�b�^�[


		//�f�o�b�N�p�̕�����
		void DebagLog();

	};

}
//end basecross

