/*!
@file State.h
@brief ��ԂȂ�
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross{
	class StateBase;

	/// <summary>
	/// �X�e�[�g���܂Ƃ߂邽�߂̃I�u�W�F�N�g
	/// </summary>
	class StateMachineBase : public MyGameObject
	{


	public:
		StateMachineBase(const shared_ptr<Stage>& stagePtr);
		~StateMachineBase();
	};

}
//end basecross

