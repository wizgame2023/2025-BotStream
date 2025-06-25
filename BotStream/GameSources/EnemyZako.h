/*!
@file EnemyZako.h
@brief 雑魚敵
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class EnemyZako : public EnemyBase
	{
	protected:
		void OnDamaged() override;

		//HPバー用のビルボード
		shared_ptr<BillBoard> m_HPFrame = nullptr;
		shared_ptr<BillBoardGauge> m_HPBer = nullptr;

		//攻撃のタイプ　テスト用に近距離にしたいのでそうする
		int m_AttackType = Zako_Melee;

		//アニメーションの更新時間
		float m_addTimeAnimation = 0.0f;

		//攻撃のクールダウン関係
		bool m_attackFlag = false;
		float m_timeOfAttackCool = 5.0f;
		float m_timeCountOfAttackCool = 3.0f;

		bool m_beforUsed = false;

	public:
		// 雑魚敵の種類を追加するときはここに書く
		enum EnemyZakoAttackType
		{
			Zako_Melee,//近距離型
			Zako_Long,//遠距離型
			Zako_Flying,//滞空型
		};

		EnemyZako(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, bool used = false, int attackType = Zako_Melee) :
			EnemyBase(stagePtr, pos, rot, scale, used),
			m_AttackType(attackType)
		{

		}
		~EnemyZako() {}

		//受けた攻撃の情報を渡すゲッター
		HitInfo EnemyZako::GetHitInfo()
		{
			return m_getHitInfo;
		}

		void OnCreate() override;
		void OnUpdate() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
		void OnDestroy()override;

		//ダメージビルボード
		void CreateDamageBill(
			shared_ptr<GameObject> actorPtr,
			int damage,
			float pushY = 2.0f,
			float scale = 1.0f,
			float displayTime = 0.5f
		);

		//ビルボードの処理
		virtual void UpdateHPBer();

		//攻撃のクールタイム
		virtual void TimeOfAttackCool();

		//ダメージを受けたときにStandステートに戻る処理
		virtual void HitBackStandBehavior()override;

		//攻撃のタイプのゲッタ
		virtual int GetAttackType()
		{
			return m_AttackType;
		}

		//攻撃フラグのゲッタセッタ
		bool GetAttackFlag()
		{
			return m_attackFlag;
		}
		void SetAttackFlag(bool attackFlag)
		{
			m_attackFlag = attackFlag;
		}

		//アニメーションの追加時間のゲッタセッタ
		float GetAddTimeAnimation()
		{
			return m_addTimeAnimation;
		}
		void SetAddTimeAnimation(float addTimeAnimation)
		{
			m_addTimeAnimation = addTimeAnimation;
		}
	};

	//遠距離の雑魚
	class EnemyZakoLong :public EnemyZako
	{
	private:

	public:
		EnemyZakoLong(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, bool used = false) :
			EnemyZako(stagePtr, pos, rot, scale, used, Zako_Long)
		{

		}
		~EnemyZakoLong() {};

		void OnCreate()override;

	};

	class EnemyZakoFlying : public EnemyZako
	{
	public:
		enum EnemyZakoFlyingAttackType
		{
			Zako_Melee,//近距離型
			Zako_Long//遠距離型
		};

		EnemyZakoFlying(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, bool used = false) :
			EnemyZako(stagePtr, pos, rot, scale, used)
		{

		}
		~EnemyZakoFlying() {}

		void OnCreate() override;
		void OnUpdate() override;

	};

}