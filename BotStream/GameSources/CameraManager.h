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
	public:
		CameraManager(const shared_ptr<Stage>& stagePtr);
		~CameraManager();
	};

}
//end basecross

