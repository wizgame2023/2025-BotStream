#pragma once
/*!
@file PauseUI.h
@brief ポーズ画面
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {

	class PauseSprite :public MyGameObject
	{
	private:
		// ポーズ画面のメイン
		shared_ptr<Sprite> m_selectSprite;	// 選択してるところ
		shared_ptr<Sprite> m_pauseBack;		// ポーズの背景
		shared_ptr<Sprite> m_pauseTextSprite[6];	// ポーズ画面のテキスト(0:Pause 1:再開 2:ステージセレクト 3:オーディオ 4:BGM 5:SE)

		// オーディオ設定画面
		shared_ptr<Sprite> m_speaker[2];	// スピーカーの図
		shared_ptr<Sprite> m_BGMMater[10];	// BGMのメーター
		shared_ptr<Sprite> m_SEMater[10];	// SEのメーター
		shared_ptr<Sprite> m_audioSelect[2];	// オーディオ設定の選択しているところ

		// ポーズ中のボタンUI
		shared_ptr<Sprite> m_pauseAButton;
		// ボタンの説明
		shared_ptr<Sprite> m_buttonText;	

		shared_ptr<Stage> m_stage;
		
		// フラグ -------------------------------------------------
		bool m_selectFlag = false; // 選択切り替えフラグ
		bool m_audioSelectFlag = false;// Audio用の選択切り替えフラグ
		
		bool m_pauseFlag = false;		// ポーズのフラグ
		bool m_pauseAudioFlag = false;	// オーディオ設定のフラグ
		// -------------------------------------------------

		float m_audioMax[2] = { 1.0f , 1.0f };	// 音量の実数値(0:BGM 1:SE)
		int m_audioMaxSetCol[2] = { 10, 10 };	// 音量の色変えるためだけに存在するもの

		int m_mainSelect = 0;	// 選択しているところの実数値
		bool m_audioFlag = false;

		// メインの選択しているところのポジション
		Vec3 m_selectPos;

	public:
		PauseSprite(shared_ptr<Stage>& StagePtr) :
			MyGameObject(StagePtr)
		{

		}
		~PauseSprite()
		{
		}
		void CreateSprite();

		// BGM,SE関係の表示非表示の制御
		void AudioUIClear(bool clear);

		// メインのUIを消す
		void MainUIClear(bool clear);
	
		// 全てのUIを消す
		void AllUIClear(bool clear);

		//Actorを止める処理
		void MoveSwitchActor(bool OnOff);

		template <typename T>
		T clamp(T value, T minValue, T maxValue)
		{
			if (value < minValue) return minValue;
			if (value > maxValue) return maxValue;
			return value;
		}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		// オーディオの現在値を取得
		// retun: 0=BGM, 1=SE
		float GetAudioMax(int BGMorSE)
		{
			return m_audioMax[BGMorSE];
		}
	};

}