/*!
@file StageAbe.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class StageAbe : public Stage {
		//ビューの作成
		void CreateViewLight();
	public:
		//構築と破棄
		StageAbe() :Stage() {}
		virtual ~StageAbe() {}
		//初期化
		virtual void OnCreate()override;
	};


}
//end basecross

