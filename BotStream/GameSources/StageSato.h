/*!
@file StageSato.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class StageSato : public Stage {
		//�r���[�̍쐬
		void CreateViewLight();
		void CreateSprite();

		//--------�ʂ��Ƃ��͂������R�s�y����΂����Ǝv����--------
		shared_ptr<Sprite> m_plHPSprite;
		shared_ptr<Sprite> m_plSPSprite;
		shared_ptr<Sprite> m_gaugeFrameSprite;
		shared_ptr<Sprite> m_katanaSprite;
		shared_ptr<Sprite> m_gunSprite;

		float m_playerHP = 1000.0f;
		float m_playerMaxHP = 1000.0f;

		float m_playerSP = 0.0f;        // �����lSP
		float m_playerMaxSP = 100.0f;   // SP�̍ő�l

		bool m_weaponSwitchFlag = true;
		//----------------------------------------------------------

	public:
		//�\�z�Ɣj��
		StageSato() :Stage() {}
		virtual ~StageSato() {}
		//������
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual void OnDraw()override;

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

