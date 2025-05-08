#pragma once
/*!
@file EnemyUI.h
@brief ポーズ画面
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	class BillBoardGauge :public BillBoard
	{
	private:
		float m_parsecond;//全体の何パーセント出すか決める

		vector<uint16_t> m_indices;//インデックス情報

	public:
		BillBoardGauge(const shared_ptr<Stage>& StagePtr,
			shared_ptr<GameObject>& actorPtr, wstring spriteName, int layer = 2, float pushY = 18.0f, Vec3 scale = Vec3(3.0f, 3.0f, 3.0f));
		~BillBoardGauge();

		void OnCreate()override;
		void OnUpdate()override;

		void SetPercent(float parcent);//頂点の更新
	};


}