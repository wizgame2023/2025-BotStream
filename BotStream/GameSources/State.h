/*!
@file State.h
@brief 状態など
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross{
	class StateBase;

	/// <summary>
	/// ステートをまとめるためのオブジェクト
	/// </summary>
	class StateMachineBase : public MyGameObject
	{


	public:
		StateMachineBase(const shared_ptr<Stage>& stagePtr);
		~StateMachineBase();
	};

}
//end basecross

