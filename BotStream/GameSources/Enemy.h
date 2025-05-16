/*!
@file Enemy.h
@brief 敵など
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
#include "Actor.h"
#include "EnemyState.h"

namespace basecross{
	class EnemyStateMachine;

	/// <summary>
	/// 親クラス
	/// </summary>
	class EnemyBase : public Actor
	{
	protected:
		bool m_used = false;
		shared_ptr<StateMachineBase> m_state;
		const float m_rotateSpeed = 1.2f;
		const float m_rotateThreshold = .3f;

		float m_armorMax = 0;
		float m_armor = 0;
		float m_armorRecover = 0;
		float m_armorRecoverTime = 0;

		float m_armorFlash = 0;
		const float m_armorFlashMax = .1f;
		Col4 m_armorFlashFX = Col4(2.0f, .5f, .5f, 0);

		weak_ptr<Player> m_player;


	public:
		EnemyBase(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale);
		EnemyBase(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, bool use);
		~EnemyBase() {}

		void HitBackStandBehavior();

		void ChangeState(wstring stateName) {
			m_state->ChangeState(stateName);
		}

		void OnCreate() override;
		void OnUpdate() override;

		//初期化処理(敵をスポーンさせるとき使う？)
		void Initialize(Vec3 pos, Vec3 rot, Vec3 scale) {
			m_used = true;

			auto ptr = GetComponent<Transform>();
			ptr->SetPosition(pos);
			ptr->SetRotation(rot);
			ptr->SetScale(scale);

			SetDrawActive(true);
			SetUpdateActive(true);
			m_LandDetect->SetUpdateActive(true);
			m_landDetectDisableTime = 0;
		}
		//初期化処理(引数無し)
		void Initialize() {
			m_used = true;
			SetDrawActive(true);
			SetUpdateActive(true);
			m_LandDetect->SetUpdateActive(true);
		}

		bool GetUsed() {
			return m_used;
		}

		//アーマーの状況を0.0f～1.0fで返す
		float GetArmorePercentage() {
			if (m_armor > 0) {
				return m_armor / m_armorMax;
			}
			else {
				return m_armorRecover / m_armorRecoverTime;
			}
		}

		//プレイヤーとの距離(Vec3)
		Vec3 GetPlayerDistInVec3();

		//プレイヤーとの距離を測る
		float GetPlayerDist();

		//プレイヤーとの角度の差を測る
		float GetPlayerSubDirection();

		//回転
		void RotateToPlayer(const float multiply, const float threshold);
		void RotateToPlayer(const float multiply);


	};

	/// <summary>
	/// ステージ1のボス
	/// </summary>
	class BossFirst : public EnemyBase {
		void OnDamaged() override;
		void RegisterAnim();

		//ビーム用
		bool m_isRecoveredFromArmorBreak = false;
		float m_prevArmor = 0.0f;
	public:
		BossFirst(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, bool used = false) :
			EnemyBase(stagePtr, pos, rot, scale, used)
		{

		}
		~BossFirst() {}

		//アーマーブレイクから回復したときtrueになる
		bool IsRecoveredFromArmorBreak() {
			bool ret = m_isRecoveredFromArmorBreak;
			m_isRecoveredFromArmorBreak = false;
			return ret;
		}

		void OnCreate() override;
		void OnUpdate() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
	};

	/// <summary>
	/// ボス1の衝撃波
	/// </summary>
	class BossFirstShockwave : public ProjectileBase {
	protected:
		shared_ptr<CollisionCapsule> m_innerCollision;

		//現在の半径
		float m_radius = 0;
		//広がる速度
		float m_radiateSpeed = 54.0f;
		//半径の最大
		float m_radiusMax = 108.0f;
		//外円と内円の差
		float m_widthCircle = .5f;

		const float m_height = 2.0f;

		int m_isPlayerInsideCnt = 0;
		const int m_isPlayerInsideCntMax = 1;

		void HitInfoInit() override;
	public:
		BossFirstShockwave(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, shared_ptr<Actor> originObj) :
			ProjectileBase(stagePtr, pos, rot, scale, originObj)
		{

		}
		~BossFirstShockwave() {}

		void OnCreate() override;
		void OnUpdate() override;

		void OnCollisionExcute(shared_ptr<GameObject>& Other) override;
	};

	/// <summary>
	/// ボス1のビーム判定
	/// </summary>
	class BossFirstBeam : public ProjectileBase {
	protected:
		float m_hitBeamVel = 12.0f;
		bool m_isFinalBlow = false;

		void HitInfoInit() override;
	public:
		BossFirstBeam(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, shared_ptr<Actor> originObj, float hitVel, bool final) :
			ProjectileBase(stagePtr, pos, rot, scale, originObj),
			m_hitBeamVel(hitVel),
			m_isFinalBlow(final)
		{

		}
		~BossFirstBeam() {}

		void OnCreate() override;

	};
}
//end basecross

