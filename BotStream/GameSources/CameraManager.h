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

}
//end basecross

