/*!
@file ObjectNotMove.h
@brief 動かないオブジェクト
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	class ObjectNotMove : public MyGameObject
	{
	private:
		bool m_pose;//ポーズフラグ
	public:
		ObjectNotMove(const shared_ptr<Stage>& stagePtr);
		~ObjectNotMove();
	protected:
		//void OnCreate()override;
		//void OnUpdate()override;
	};

}
//end basecross