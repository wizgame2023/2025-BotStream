/*!
@file StageSanpei.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class StageSanpei : public Stage {
		//ビューの作成
		void CreateViewLight();
	public:
		//構築と破棄
		StageSanpei() :Stage() {}
		virtual ~StageSanpei() {}
		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual void OnDraw()override;
	};


}
//end basecross

