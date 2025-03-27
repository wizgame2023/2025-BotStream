/*!
@file Actor.cpp
@brief PlayerやEnemyなどのキャラクター用のクラス
*/

#include "stdafx.h"
#include "Project.h"
#include "Actor.h"

namespace basecross {
	Actor::Actor(const shared_ptr<Stage>& stagePtr) :
		ObjectMove(stagePtr)
	{

	}
	Actor::~Actor()
	{

	}


}
//end basecross
