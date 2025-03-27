/*!
@file AttackCollision.h
@brief UŒ‚”»’è
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross{
	class AttackCollision : public ObjectMove
	{
	public:
		AttackCollision(const shared_ptr<Stage>& stagePtr);
		~AttackCollision();
	};

}
//end basecross

