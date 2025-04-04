/*!
@file State.cpp
@brief ��Ԃ̏���
*/

#include "stdafx.h"
#include "Project.h"
#include "State.h"

namespace basecross {
	StateMachineBase::StateMachineBase(const shared_ptr<Stage>& stagePtr) :
		MyGameObject(stagePtr)
	{

	}
	StateMachineBase::~StateMachineBase()
	{

	}

}
//end basecross
