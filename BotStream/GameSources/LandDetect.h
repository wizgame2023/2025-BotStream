/*!
@file LandDetect.h
@brief �n�ʐڐG
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross{
	class LandDetect : public ObjectMove
	{
	public:
		LandDetect(const shared_ptr<Stage>& stagePtr);
		~LandDetect();
	};

}
//end basecross

