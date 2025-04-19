#pragma once
/*!
@file PlayerUI.h
@brief �v���C���[�Ɋւ���UI
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

		float m_playerMaxHP = 1000.0f;		// HP�̍ő�l
		float m_playerHP = m_playerMaxHP;	// HP�̏����l

		float m_playerMaxSP = 100.0f;   // SP�̍ő�l
		float m_playerSP = 0.0f;        // �����lSP

	public:
		PlayerUI(const std::shared_ptr<Stage>& stagePtr)
			: MyGameObject(stagePtr) {}
		virtual ~PlayerUI() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		// �䗦�݂����Ȃ�����[�����[��������
		template <typename T>
		T clamp(T value, T minValue, T maxValue)
		{
			if (value < minValue) return minValue;
			if (value > maxValue) return maxValue;
			return value;
		}

	};

} // namespace basecross
