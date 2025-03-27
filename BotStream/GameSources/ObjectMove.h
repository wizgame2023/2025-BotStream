/*!
@file ObjectMove.h
@brief 動くオブジェクト
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	class ObjectMove : public MyGameObject
	{
	private:
		bool m_pose;//ポーズフラグ
	public:
		ObjectMove(const shared_ptr<Stage>& stagePtr);
		~ObjectMove();
	protected:
		//void OnCreate()override;
		//void OnUpdate()override;
	};

}
//end basecross#pragma once
