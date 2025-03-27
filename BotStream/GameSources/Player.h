/*!
@file Player.h
@brief ƒvƒŒƒCƒ„[‚È‚Ç
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
#include "Actor.h"

namespace basecross{
	class Player : public Actor
	{
	public:
		Player(const shared_ptr<Stage>& stagePtr);
		~Player();
	};

}
//end basecross

