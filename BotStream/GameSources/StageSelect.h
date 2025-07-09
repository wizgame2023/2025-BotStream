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
		shared_ptr<Sprite> m_stagePhoto[6];
		shared_ptr<Sprite> m_tutorialSprite;

		// 決定とキャンセル達
		shared_ptr<Sprite> m_AButtonSp;
		shared_ptr<Sprite> m_BButtonSp;
		shared_ptr<Sprite> m_enterKeySp;
		shared_ptr<Sprite> m_backSpaceKeySp;
		shared_ptr<Sprite> m_textSp[2];


		shared_ptr<SoundItem> m_BGM;
		shared_ptr<SoundItem> m_SE;

		shared_ptr<Sprite> m_stageNum[3];

		shared_ptr<XAudio2Manager> m_BGMManager = nullptr;	// BGMを再生するためのマネージャ
		shared_ptr<XAudio2Manager> m_SEManager = nullptr;	// SEなどを再生するためのマネージャ

		weak_ptr<Scene> m_scene;

		Vec3 m_selectPos;

		// 切り替えの時に一気に切り替わらないようにするためのフラグ1
		bool m_selectOnceFlag1 = false;
		// 切り替えの時に一気に切り替わらないようにするためのフラグ2
		bool m_selectOnceFlag2 = false;

		// チュートリアルを選択しているときのフラグ
		bool m_tutorialFlag = false;

		// チュートリアルで一回だけ反応させるフラグ
		bool m_tutorialOnceFlag = false;

		// 選んでいるステージ数
		short int m_selectStageNum = 0;
		 // ステージ選択フラグ
		bool m_stageFlag = false;
		// time切り替えフラグ
		bool m_timeFlag = false;

		float m_time = 0.0f;

		// BGMのボリューム
		float m_BGMVol;
		// SEのボリューム
		float m_SEVol;
	public:
		//構築と破棄
		StageSelect() :Stage() {}
		virtual ~StageSelect() {}

		void CreateSprite();
		void CreateBGM();

		void IsContorollerConnect();

		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate()override;

	};
}