#pragma once
/*!
@file TitleStage.h
@brief タイトル画面
*/

#include "stdafx.h"

namespace basecross {
	class TitleStage : public Stage
	{
		//ビューの作成
		void CreateViewLight();
		shared_ptr<Sprite> m_titleSprite;
		shared_ptr<Sprite> m_titleBackSprite;
		shared_ptr<Sprite> m_textSprite;
		shared_ptr<Sprite> m_textSpriteKeyBoard;

		shared_ptr<SoundItem> m_BGM;

		Vec3 m_selectPos;
		float m_time = 0;
	public:
		//構築と破棄
		TitleStage() :Stage() {}
		virtual ~TitleStage() {}

		void CreateSprite();
		void CreateBGM();
		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate()override;

	};
}