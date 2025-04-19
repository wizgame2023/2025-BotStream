/*!
@file GameTuboi.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class StageTuboi : public Stage {
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
		StageTuboi() :Stage() {}
		virtual ~StageTuboi() {}
		//������
		virtual void OnCreate()override;
		
	};


}
//end basecross

