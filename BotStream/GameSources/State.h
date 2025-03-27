/*!
@file State.h
@brief ó‘Ô‚È‚Ç
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross{
	class State : public ObjectMove
	{
	public:
		State(const shared_ptr<Stage>& stagePtr);
		~State();
	};

}
//end basecross

