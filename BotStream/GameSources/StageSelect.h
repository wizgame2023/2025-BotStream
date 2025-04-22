#pragma once
/*!
@file StageSelect.h
@brief ステージセレクト画面
*/

#include "stdafx.h"

namespace basecross {
	class StageSelect : public Stage
	{
		//ビューの作成
		void CreateViewLight();
		shared_ptr<Sprite> m_selectSprite;
		shared_ptr<Sprite> m_selectBackSprite;
		shared_ptr<Sprite> m_textSprite;
		shared_ptr<Sprite> m_stageNum;
		shared_ptr<SoundItem> m_BGM;

		Vec3 m_selectPos;

	public:
		//構築と破棄
		StageSelect() :Stage() {}
		virtual ~StageSelect() {}

		void CreateSprite();
		void CreateBGM();

		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate()override;

	};
}