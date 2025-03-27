/*!
@file Enemy.h
@brief “G‚È‚Ç
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross{
	class Enemy : public Actor
	{
	public:
		Enemy(const shared_ptr<Stage>& stagePtr);
		~Enemy();
	};

}
//end basecross

