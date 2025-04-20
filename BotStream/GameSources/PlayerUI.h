#pragma once
/*!
@file PlayerUI.h
@brief プレイヤーに関するUI
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {

	class PlayerUI : public MyGameObject {

		shared_ptr<Sprite> m_gaugeFrameSprite;
		shared_ptr<Sprite> m_plHPSprite;
		shared_ptr<Sprite> m_plSPSprite;
		shared_ptr<Stage> m_stage;

		float m_playerMaxHP = 1000.0f;		// HPの最大値
		float m_playerHP = m_playerMaxHP;	// HPの初期値

		float m_playerMaxSP = 100.0f;   // SPの最大値
		float m_playerSP = 0.0f;        // 初期値SP

	public:
		PlayerUI(const std::shared_ptr<Stage>& stagePtr,int HPMax = 1000.0f,int SPMax = 100.0f):
			MyGameObject(stagePtr),
			m_playerMaxHP(HPMax),
			m_playerHP(HPMax),
			m_playerSP(0.0f),
			m_playerMaxSP(SPMax)
		{}
		virtual ~PlayerUI() {}

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

} // namespace basecross
