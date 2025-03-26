/*!
@file MyGameObject.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class MyGameObject : public GameObject
	{
	private:
		bool m_pose;//ポーズフラグ
	public:
		MyGameObject(const shared_ptr<Stage>& stagePtr);
		~MyGameObject();
	protected:
		 //void OnCreate()override;
		 //void OnUpdate()override;
	};

}
//end basecross