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
		PlayerUI(const std::shared_ptr<Stage>& stagePtr)
			: MyGameObject(stagePtr) {}
		virtual ~PlayerUI() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

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
