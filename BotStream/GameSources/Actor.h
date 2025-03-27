/*!
@file Actor.h
@brief ÉvÉåÉCÉÑÅ[Ç»Ç«
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
//#include "MyGameObject.h"

namespace basecross {
	class Actor : public ObjectMove
	{

	private:
		//shared_ptr<AttackCollision> m_AttackCol;
		//shared_ptr<State> m_State;
		//shared_ptr<LandDetect> m_LandDetect;
	public:
		Actor(const shared_ptr<Stage>& stagePtr);
		~Actor();

	};
	
}
//end basecross
