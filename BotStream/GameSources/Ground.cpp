/*!
@file Ground.cpp
@brief 地面オブジェクト実体
*/

#include "stdafx.h"
#include "Project.h"
#include "Ground.h"

namespace basecross {
	Ground::Ground(const shared_ptr<Stage>& stagePtr) :
		ObjectNotMove(stagePtr)
	{

	}
	Ground::~Ground()
	{

	}
}
//end basecross

