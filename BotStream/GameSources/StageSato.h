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

		// 性格のステータス
		struct PersonalState
		{
			int Good = 0;
			int Evil = 0;
			int Lawful = 0;
			int Chaos = 0;
		};


		enum PlayerType
		{
			Type_Speed,
			Type_Power,
			Type_Balance,
		};

		//shared_ptr<Sprite> m_plHPSprite;
		//shared_ptr<Sprite> m_plSPSprite;
		//shared_ptr<Sprite> m_gaugeFrameSprite;
		//shared_ptr<Sprite> m_katanaSprite;
		//shared_ptr<Sprite> m_gunSprite;
		//shared_ptr<Sprite> m_questionSprite[5];
		//shared_ptr<Sprite> m_answerSprite[5][3];
		//shared_ptr<Sprite> m_resultSprite;

		shared_ptr<Sprite> m_selectSprite;

		shared_ptr<Sprite> m_personalSelect;
		shared_ptr<Sprite> m_pauseBack;
		shared_ptr<Sprite> m_pauseTextSprite[6];
		shared_ptr<Sprite> m_speaker[2];
		shared_ptr<Sprite> m_audioMater[2];
		shared_ptr<Sprite> m_audioSelect[2];

		// スプライトのリスト
		std::vector<std::shared_ptr<Sprite>> m_bulletDigits;
		// 質問の内容を入れておくためのリスト
		std::vector<int> m_questionOrder;

		float m_playerMaxHP = 1000.0f;		// HPの最大値
		float m_playerHP = m_playerMaxHP;	// HPの初期値

		float m_playerMaxSP = 100.0f;   // SPの最大値
		float m_playerSP = 0.0f;        // 初期値SP

		// 武器切り替えのフラグ
		bool m_weaponSwitchFlag = true;

		// 選択切り替えフラグ
		bool m_selectFlag = false;
		bool m_questionEndFlag = false;
		// Audio用の選択切り替えフラグ
		bool m_audioSelectFlag = false;
		int m_select = 0;
		Vec3 m_selectPos;

		bool m_pauseFlag = false;
		bool m_pauseAudioFlag = false;
		float m_audioMax[2] = { 1.0f , 1.0f };

		int m_select2 = 0;

		PlayerType m_type = PlayerType::Type_Speed;

		// リザルト表示フラグ
		bool m_resultFlag = false;

		// 弾数
		int m_bulletNum = 90;
		// 弾数の数字の大きさ
		float m_digitSize = 40;

		// もう知らない！ローカル変数のバカ！
		int m_switchQues = 0;
		int m_questionNum = 0;

		PersonalState m_personality;
		//----------------------------------------------------------

	public:
		//構築と破棄
		StageSato() :Stage() {}
		virtual ~StageSato() {}
		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual void OnDraw()override;

		// デバッグ用文字列
		void DebugLog();

		// 軸の正規化
		float NormalizeAxis(float minusSide, float plusSide);

		// 性格診断のステータス変動
		//void PersonalStateChange(int questionID, int answerIndex);

		//void StateResult(int LawCha, int EvilGood);

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

		// playerの初期性格(タイプ)を取得
		PlayerType GetPLType()
		{
			return m_type;
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

