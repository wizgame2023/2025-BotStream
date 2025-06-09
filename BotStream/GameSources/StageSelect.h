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
		shared_ptr<Sprite> m_stagePhoto[2];
		shared_ptr<SoundItem> m_BGM;

		shared_ptr<Sprite> m_stageNum[3];

		Vec3 m_selectPos;

		// 切り替えの時に一気に切り替わらないようにするためのフラグ
		bool m_selectOnceFlag = false;

		// 選んでいるステージ数
		short int m_selectStageNum = 0;

		bool m_stageFlag = false; // ステージ選択フラグ

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