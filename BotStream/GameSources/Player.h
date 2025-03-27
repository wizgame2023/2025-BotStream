/*!
@file Player.h
@brief プレイヤーなど
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
#include "Actor.h"

namespace basecross{
	class Player : public Actor
	{
	private:
		shared_ptr<Transform> m_trans;
		Vec3 m_pos;//位置
		Vec3 m_rot;//回転
		Vec3 m_scale;//大きさ

	public:
		Player(const shared_ptr<Stage>& stagePtr,Vec3 pos,Vec3 rot,Vec3 scale);
		~Player();

		void OnCreate()override;//作成
		void OnUpdate()override;//更新

		//動く処理
		void PlayerMove();

		//デバック用の文字列
		void DebagLog();

	};

}
//end basecross

