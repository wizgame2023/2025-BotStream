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
		shared_ptr<Sprite> m_fadeInSprite;
		shared_ptr<Sprite> m_noiseSprite;

		shared_ptr<SoundItem> m_BGM;
		shared_ptr<SoundItem> m_SE;

		shared_ptr<XAudio2Manager> m_BGMMana;
		shared_ptr<XAudio2Manager> m_SEMana;

		weak_ptr<Scene> m_scene;

		// 選択切り替えフラグ
		bool m_selectFlag = false;
		// フェードインフラグ
		bool m_fadeInFlag = false;

		int m_select = 0;

		int m_frameCount[2] = { 0 };

		Vec3 m_selectPos;
		int m_stageNum;

		float m_time;

		float m_SEVol;

		wstring m_strStage;
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