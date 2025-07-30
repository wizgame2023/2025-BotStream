#pragma once

/*!
@file StageCollisionController.h
@brief 当たり判定の操作用
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	class StageCollisionController :public MyGameObject
	{
	private:
		bool m_CollisionSwhich;

		void CollisionActive(bool On);//ステージ上の有効なコリジョン範囲を決める

	public:
		StageCollisionController(const shared_ptr<Stage>& stagePtr);
		~StageCollisionController();

		void OnCreate()override;
		void OnUpdate()override;
		void SetCollisionSwhich(bool On);//コリジョン範囲を決めるためのフラグを取得する関数

	};

}