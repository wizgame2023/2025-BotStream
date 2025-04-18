/*!
@file MyGameObject.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	class MyGameObject : public GameObject
	{
	private:
		bool m_pose;//ポーズフラグ
	public:
		MyGameObject(const shared_ptr<Stage>& stagePtr);
		~MyGameObject();

		Vec3 GetPosition() {
			return GetComponent<Transform>()->GetPosition();
		}
		void SetPosition(Vec3 pos) {
			GetComponent<Transform>()->SetPosition(pos);
		}
	protected:
		 //void OnCreate()override;
		 //void OnUpdate()override;
	};

}
//end basecross