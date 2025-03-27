/*!
@file CameraManager.h
@brief カメラの管理
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross{
	class CameraManager : public MyGameObject
	{
	private:
		weak_ptr<Camera> m_stageCamera;//ステージ上のカメラ

		float m_cameraAngle;  //Playerから見てカメラのいる角度
		float m_range;//Playerからどのくらい離れるか

	public:
		CameraManager(const shared_ptr<Stage>& stagePtr);
		~CameraManager();

		void OnCreate()override;//作成
		void OnUpdate()override;//更新

	};

}
//end basecross

