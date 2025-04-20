#pragma once
/*!
@file GameOverScene.cpp
@brief ゲームオーバー画面
*/

#include "stdafx.h"

namespace basecross {
	class GameOver : public Stage
	{
		//ビューの作成
		void CreateViewLight();
		shared_ptr<Sprite> m_selectSprite[2];

		// 選択切り替えフラグ
		bool m_selectFlag = false;
		int m_select = 0;

		Vec3 m_selectPos;

	public:
		//構築と破棄
		GameOver() :Stage() {}
		virtual ~GameOver() {}

		void CreateSprite();

		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate()override;

	};
}