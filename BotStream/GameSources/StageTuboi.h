/*!
@file GameTuboi.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class StageTuboi : public Stage {
		//ビューの作成
		void CreateViewLight();
		void CreateSprite();
		//--------写すときはここをコピペすればいいと思われる--------
		shared_ptr<Sprite> m_plHPSprite;
		shared_ptr<Sprite> m_plSPSprite;
		shared_ptr<Sprite> m_gaugeFrameSprite;
		shared_ptr<Sprite> m_katanaSprite;
		shared_ptr<Sprite> m_gunSprite;

		float m_playerHP = 1000.0f;
		float m_playerMaxHP = 1000.0f;

		float m_playerSP = 0.0f;        // 初期値SP
		float m_playerMaxSP = 100.0f;   // SPの最大値

		bool m_weaponSwitchFlag = true;
		//----------------------------------------------------------

	public:
		//構築と破棄
		StageTuboi() :Stage() {}
		virtual ~StageTuboi() {}
		//初期化
		virtual void OnCreate()override;
		
	};


}
//end basecross

