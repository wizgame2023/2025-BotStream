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
		shared_ptr<EfkEffect> m_EfkLaser;
		shared_ptr<EfkEffect> m_EfkSword;
		shared_ptr<EfkEffect> m_EfkLanding;

		shared_ptr<EfkPlay> m_EfkPlay;
		shared_ptr<Player> m_Player;

		shared_ptr<EfkInterface> m_ShEfkInterface;

	public:
		//構築と破棄
		StageSato() :Stage() {}
		virtual ~StageSato() {}
		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual void OnDraw()override;

		shared_ptr<EfkInterface> GetEfkInterface()
		{
			return m_ShEfkInterface;
		}

	};


}
//end basecross

