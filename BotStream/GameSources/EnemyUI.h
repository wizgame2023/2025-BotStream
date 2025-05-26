#pragma once
/*!
@file EnemyUI.h
@brief ポーズ画面
*/

#pragma once
#include "stdafx.h"

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

	class BossGaugeUI : public MyGameObject
	{
	private:
		shared_ptr<Sprite> m_gaugeFrameSp;
		shared_ptr<Sprite> m_hitPointSp;
		shared_ptr<Sprite> m_armorPointSp;
		shared_ptr<Stage> m_stage;

		weak_ptr<EnemyBase> m_boss;

		float m_hitPointMax = 1000;			// HPの最大値
		float m_hitPoint = m_hitPointMax;	// HPの初期値

		float m_armorPointMax = 100;		// アーマーポイントの最大値
		float m_armorPoint = m_armorPointMax;// アーマー

	public:
		BossGaugeUI(const shared_ptr<Stage>& stagePtr, float hitMax = 300.0f, float armMax = 100.0f) :
			MyGameObject(stagePtr),
			m_hitPointMax(hitMax),
			m_armorPointMax(armMax)
		{
		}
		virtual ~BossGaugeUI() {}

		void CreateSprite();

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		float GetBossMaxHPSprite()
		{
			return m_hitPointMax;
		}

		void SetBossMaxHPSprite(float maxHP)
		{
			m_hitPointMax = maxHP;
		}

		// ボスのゲージ表示非表示
		void ClearBossGaugeUI(bool clear)
		{
			m_hitPointSp->OnClear(clear);
			m_gaugeFrameSp->OnClear(clear);
		}

		// 比率みたいなやつをあーだこーだするやつ
		template <typename T>
		T clamp(T value, T minValue, T maxValue)
		{
			if (value < minValue) return minValue;
			if (value > maxValue) return maxValue;
			return value;
		}

	};

	class EnemyDamageBill : public BillBoard
	{
		vector<uint16_t> m_indices;//インデックス情報

	public:
		EnemyDamageBill(
			const shared_ptr<Stage>& StagePtr,
			shared_ptr<GameObject>& actorPtr,
			wstring spriteName,
			int layer = 2,
			float pushY = 18.0f,
			Vec3 scale = Vec3(3.0f, 3.0f, 3.0f)
		) :
			BillBoard(StagePtr, actorPtr, spriteName, layer, pushY, scale),
			m_indices(vector<uint16_t>())
		{
		}

		virtual ~EnemyDamageBill() {}

		void ShowDamage(int value, float digitSize/*, BillBoard bill*/);

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

	};
}