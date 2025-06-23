#pragma once
/*!
@file PlayerUI.h
@brief プレイヤーに関するUI
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {

	class PlayerGaugeUI : public MyGameObject {

		shared_ptr<Sprite> m_gaugeFrameSprite;
		shared_ptr<Sprite> m_plHPSprite;
		shared_ptr<Sprite> m_plSPSprite;
		shared_ptr<Stage> m_stage;

		weak_ptr<Player> m_player;

		float m_playerMaxHP = 1000.0f;		// HPの最大値
		float m_playerHP = m_playerMaxHP;	// HPの初期値

		float m_playerMaxSP = 100.0f;   // SPの最大値
		float m_playerSP = 0.0f;        // 初期値SP

		void AllClear(bool clear);
	public:
		PlayerGaugeUI(const shared_ptr<Stage>& stagePtr,shared_ptr<Player> player) :
			MyGameObject(stagePtr),
			m_player(player)
		{
		}
		virtual ~PlayerGaugeUI() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

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

		// playerのHPの更新(sprite)
		void SetPLHPSprite(float value)
		{
			m_playerHP = value;
		}

		// playerの最大SP(必殺技ゲージ)を設定(sprite)
		void SetPLMaxSPSprite(float value)
		{
			m_playerMaxSP = value;
		}

		// playerの最大SP(必殺技ゲージ)を設定(sprite)
		void SetPLSPSprite(float value)
		{
			m_playerSP = value;
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

	class PlayerBulletUI : public MyGameObject
	{
		// スプライトのリスト
		vector<std::shared_ptr<Sprite>> m_bulletDigits;

		shared_ptr<Stage> m_stage;
		weak_ptr<Player> m_player;

		// 弾数
		int m_bulletNum = 90;
		// 弾数の数字の大きさ
		float m_digitSize = 40;
		// 文字の位置
		Vec2 m_digitPos;

	public:
		// digitPos:位置設定
		// bulletNum:最大装填数
		// digitSize:文字の大きさ
		PlayerBulletUI(const shared_ptr<Stage>& stagePtr, shared_ptr<Player> player, Vec2 digitPos, float digitSize = 40.0f) :
			MyGameObject(stagePtr),
			m_digitPos(digitPos),
			m_digitSize(digitSize),
			m_player(player)
		{
		}
		virtual ~PlayerBulletUI() {}

		//弾の数のセッター
		void SetBullet(int setBullet)
		{
			m_bulletNum = setBullet;
		}
		//弾の数のゲッター
		int GetBullet()
		{
			return m_bulletNum;
		}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

	};


	//==============================================================================
	// コントローラボタンアイコン表示UI
	//==============================================================================
	class PlayerButtonUI : public MyGameObject {
		Vec2 m_buttonPos;
		Vec2 m_buttonSize;
		//int  m_buttonSwitch;
		shared_ptr<Sprite> m_button;

		shared_ptr<Stage> m_stage;
	public:
		// stagePtr: ステージ, buttonPos: アイコン位置, buttonSize: サイズ, switch: 選択インデックス
		PlayerButtonUI(const std::shared_ptr<Stage>& stagePtr,
			const Vec2& buttonPos,
			const Vec2& buttonSize/*,
			int buttonSwitch*/
		) :
			MyGameObject(stagePtr),
			m_buttonPos(buttonPos),
			m_buttonSize(buttonSize)//,
			//m_buttonSwitch(buttonSwitch)
		{
		}

		virtual ~PlayerButtonUI() {}

		virtual void OnCreate() override;

	};

	//==============================================================================
	// コントローラボタンのテキスト表示UI
	//==============================================================================
	class PlayerButtonText : public MyGameObject {
		Vec2 m_textPos;
		Vec2 m_textSize;
		int  m_textSwitch;
		shared_ptr<Sprite> m_buttonText;

		shared_ptr<Stage> m_stage;

	public:
		// stagePtr: ステージ, textPos: 文字位置, textSize: サイズ, switch: 選択インデックス
		PlayerButtonText(const std::shared_ptr<Stage>& stagePtr,
			const Vec2& textPos,
			const Vec2& textSize,
			int textSwitch
		) :
			MyGameObject(stagePtr),
			m_textPos(textPos),
			m_textSize(textSize),
			m_textSwitch(textSwitch)
		{
		}
		virtual ~PlayerButtonText() {}

		virtual void OnCreate() override;
	};

	//==============================================================================
	// 武器切替UI
	//==============================================================================
	class PlayerWeaponUI : public MyGameObject {

		shared_ptr<Stage> m_stage;

		shared_ptr<Sprite> m_fightSprite[8];

	public:
		// stagePtr: ステージ
		PlayerWeaponUI(const std::shared_ptr<Stage>& stagePtr) :
			MyGameObject(stagePtr)
		{
		}

		virtual ~PlayerWeaponUI() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void AllFightSpriteClear(bool clear);
	};

	//==============================================================================
	// 装備パーツUI
	//==============================================================================
	class PartsTextChange : public MyGameObject {
		shared_ptr<Stage> m_stage;
		shared_ptr<Sprite> m_partsTextSprite[3];
		shared_ptr<Sprite> m_num[3];
	public:
		PartsTextChange(const std::shared_ptr<Stage>& stagePtr) :
			MyGameObject(stagePtr)
		{
		}

		virtual ~PartsTextChange() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void AllClear(bool clear);
	};
}
// namespace basecross
