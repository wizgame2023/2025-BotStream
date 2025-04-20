#pragma once
/*!
@file GameClearScene.h
@brief クリア画面
*/

#include "stdafx.h"

namespace basecross {
	class GameClear : public Stage
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
		GameClear() :Stage() {}
		virtual ~GameClear() {}

		void CreateSprite();
		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate()override;

	};
}