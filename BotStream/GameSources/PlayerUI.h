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

		// player�̍ő�HP���擾(sprite)
		float GetPLMaxHPSprite()
		{
			return m_playerMaxHP;
		}

		// player�̌��݂�HP���擾(sprite)
		float GetPLHPSprite()
		{
			return m_playerHP;
		}

		// player�̍ő�SP(�K�E�Z�Q�[�W)���擾(sprite)
		float GetPLMaxSPSprite()
		{
			return m_playerMaxSP;
		}

		// player�̌��݂�SP(�K�E�Z�Q�[�W)���擾(sprite)
		float GetPLSPSprite()
		{
			return m_playerSP;
		}

		// player�̍ő�HP��ݒ�(sprite)
		void SetPLMaxHPSprite(float value)
		{
			m_playerMaxHP = value;
		}

		// player��HP�̍X�V(sprite)
		void SetPLHPSprite(float value)
		{
			m_playerHP = value;
		}

		// player�̍ő�SP(�K�E�Z�Q�[�W)��ݒ�(sprite)
		void SetPLMaxSPSprite(float value)
		{
			m_playerMaxSP = value;
		}

		// player�̍ő�SP(�K�E�Z�Q�[�W)��ݒ�(sprite)
		void SetPLSPSprite(float value)
		{
			m_playerSP = value;
		}

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
