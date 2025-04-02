/*!
@file Ground.h
@brief 地面オブジェクト
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
#include "ObjectNotMove.h"

namespace basecross {
	class Ground : public ObjectNotMove
	{
	public:
		Ground(const shared_ptr<Stage>& stagePtr);
		~Ground();
		virtual void OnCreate() override;
	};

}
//end basecross

