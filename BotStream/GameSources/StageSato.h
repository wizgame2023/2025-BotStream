/*!
@file StageSato.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class StageSato : public Stage {
		//ビューの作成
		void CreateViewLight();
		void CreateSprite();

		//--------写すときはここをコピペすればいいと思われる--------
		shared_ptr<Sprite> m_plHPSprite;
		shared_ptr<Sprite> m_plSPSprite;
		shared_ptr<Sprite> m_gaugeFrameSprite;
		shared_ptr<Sprite> m_katanaSprite;
		shared_ptr<Sprite> m_gunSprite;
		shared_ptr<Sprite> m_questionSprite[5];
		shared_ptr<Sprite> m_answerSprite[5][3];
		// スプライトのリスト
		std::vector<std::shared_ptr<Sprite>> m_bulletDigits;

		float m_playerMaxHP = 1000.0f;		// HPの最大値
		float m_playerHP = m_playerMaxHP;	// HPの初期値

		float m_playerMaxSP = 100.0f;   // SPの最大値
		float m_playerSP = 0.0f;        // 初期値SP

		// 武器切り替えのフラグ
		bool m_weaponSwitchFlag = true;

		// 弾数
		int m_bulletNum = 90;
		// 弾数の数字の大きさ
		float m_digitSize = 40;
		
		// もう知らない！ローカル変数のバカ！
		int m_switchQues = 0;
		int m_questionNum = 0;

		//----------------------------------------------------------

	public:
		//構築と破棄
		StageSato() :Stage() {}
		virtual ~StageSato() {}
		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual void OnDraw()override;
		
		// 数字を画面上に表示する関数
		// value     : 表示する数値
		// pos       : 表示開始位置
		// digitSize : 各桁の幅と高さ
		void ShowNumber(int value, Vec2 pos, float digitSize);

		// playerの最大HPを取得(sprite)
		float GetPLMaxHPSprite()
		{
			return m_playerMaxHP;
		}

		// playerの現在のHPを取得(sprite)
		float GetPLHPSprite()
		{
			return m_playerHP;
		}

		// playerの最大SP(必殺技ゲージ)を取得(sprite)
		float GetPLMaxSPSprite()
		{
			return m_playerMaxSP;
		}

		// playerの現在のSP(必殺技ゲージ)を取得(sprite)
		float GetPLSPSprite()
		{
			return m_playerSP;
		}

		// playerの最大HPを設定(sprite)
		void SetPLMaxHPSprite(float value)
		{
			m_playerMaxHP = value;
		}

		// playerの最大SP(必殺技ゲージ)を設定(sprite)
		void SetPLMaxSPSprite(float value)
		{
			m_playerMaxSP = value;
		}

		// playerの最大装填数の設定(sprite)
		void SetPLBulletNumSprite(float value)
		{
			m_bulletNum = value;
		}

		// 比率みたいなやつをあーだこーだするやつ
		template <typename T>
		T clamp(T value, T minValue, T maxValue)
		{
			if (value < minValue) return minValue;
			if (value > maxValue) return maxValue;
			return value;
		}

	};


}
//end basecross

