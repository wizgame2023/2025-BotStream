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
		Enemy(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, bool use);
		~Enemy();

		void OnCreate() override;
		void OnUpdate() override;

		//初期化処理(敵をスポーンさせるとき使う？)
		void Initialize(Vec3 pos, Vec3 rot, Vec3 scale) {
			m_used = true;
			auto ptr = GetComponent<Transform>();
			ptr->SetPosition(pos);
			ptr->SetRotation(rot);
			ptr->SetScale(scale);
			SetDrawActive(true);
			SetUpdateActive(true);
		}

		bool GetUsed() {
			return m_used;
		}
	};


}
//end basecross

