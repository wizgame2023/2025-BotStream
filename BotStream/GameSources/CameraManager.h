/*!
@file CameraManager.h
@brief �J�����̊Ǘ�
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross{
	class CameraManager : public MyGameObject
	{
	private:
		weak_ptr<Camera> m_stageCamera;//�X�e�[�W��̃J����

		float m_cameraAngle;  //Player���猩�ăJ�����̂���p�x
		float m_range;//Player����ǂ̂��炢����邩

	public:
		CameraManager(const shared_ptr<Stage>& stagePtr);
		~CameraManager();

		void OnCreate()override;//�쐬
		void OnUpdate()override;//�X�V

	};

	//�f�o�b�N�p�̂����̎l�p�����
	class Cube : public ObjectNotMove
	{
	private:
		Vec3 m_pos;//�ʒu
		Vec3 m_rot;//��]
		Vec3 m_scale;//�傫��
		Col4 m_color;//�F
	public:
		Cube(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, Col4 color = Col4(1.0f, 1.0f, 1.0f, 1.0f));
		~Cube();
		void OnCreate()override;//�쐬
		//void OnUpdate()override;
	};

}
//end basecross

