/*!
@file Enemy.h
@brief 敵など
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
#include "Actor.h"

namespace basecross{
	class Enemy : public Actor
	{
		bool m_used = false;

	public:
		Enemy(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale);
		~Enemy();

		void OnCreate() override;
		void OnUpdate() override;

		//初期化処理(敵をスポーンさせるとき使う？)
		void Initialize(Vec3 pos, Vec3 rot, Vec3 scale) {
			m_used = true;
			m_pos = pos;
			m_rot = rot;
			m_scale = scale;
		}

		bool GetUsed() {
			return m_used;
		}
	};


}
//end basecross

