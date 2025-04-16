/*!
@file AttackCollision.h
@brief 攻撃判定
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {

	enum class AttackType {
		Player,
		Enemy
	};

	//攻撃判定から相手に伝える情報一覧
	struct HitInfo {
	public:
		AttackType Type;
		//ダメージ
		int Damage;
		//ヒットストップ時間
		float PauseTime;
		//地上相手ののけぞり時間
		float HitTime_Stand;
		//地上相手ののけぞり移動距離
		Vec3 HitVel_Stand;
		//空中相手ののけぞり時間
		float HitTime_Air;
		//空中相手の吹き飛ばし距離
		Vec3 HitVel_Air;
		//ダウンする
		bool DoFall;
		//ダウンする(空中相手のみ)
		bool DoFall_Air;
		//ダウンさせない
		bool ForceRecover;
		//一度しかヒットしない
		bool HitOnce;

		//エフェクト・音のキー
		wstring HitEffect;
		wstring GuardEffect;
		wstring HitSound;
		wstring GuardSound;
		
		//コンストラクタ
		HitInfo::HitInfo(AttackType attacktype, int damage, float pausetime,
			float hittime_stand, Vec3 hitvel_stand,
			float hittime_air, Vec3 hitvel_air,
			bool dofall, bool dofall_air,
			bool forcerecover, bool hitonce,
			wstring hiteffect, wstring guardeffect,
			wstring hitsound, wstring guardsound
		)
		{
			this->Type = attacktype;
			this->Damage = damage;
			this->PauseTime = pausetime;
			this->HitTime_Stand = hittime_stand;
			this->HitVel_Stand = hitvel_stand;
			this->HitTime_Air = hittime_air;
			this->HitVel_Air = hitvel_air;
			this->DoFall = dofall;
			this->DoFall_Air = dofall_air;
			this->ForceRecover = forcerecover;
			this->HitOnce = hitonce;
			this->HitEffect = hiteffect;
			this->GuardEffect = guardeffect;
			this->HitSound = hitsound;
			this->GuardSound = guardeffect;
		}
		HitInfo::HitInfo() {
			this->Type = AttackType::Player;
			this->Damage = 0;
			this->PauseTime = 0;
			this->HitTime_Stand = 0;
			this->HitVel_Stand = Vec3(0);
			this->HitTime_Air = 0;
			this->HitVel_Air = Vec3(0);
			this->DoFall = false;
			this->DoFall_Air = false;
			this->ForceRecover = false;
			this->HitOnce = false;
			this->HitEffect = L"";
			this->GuardEffect = L"";
			this->HitSound = L"";
			this->GuardSound = L"";
		}
	};

	class AttackCollision : public MyGameObject
	{
		shared_ptr<CollisionSphere> m_collision;

		//攻撃のスペック
		HitInfo m_info;
		//持続時間
		float m_ActiveTime = 0.0f;
	public:
		AttackCollision(const shared_ptr<Stage>& stagePtr);
		~AttackCollision();

		void OnCreate() override;
		void OnUpdate() override;

		shared_ptr<CollisionSphere> GetCollisionPtr() {
			return m_collision;
		}

		//Positionゲッタ・セッタ
		void SetPos(Vec3 pos) {
			GetComponent<Transform>()->SetPosition(pos);
		}
		Vec3 GetPos() {
			return GetComponent<Transform>()->GetPosition();
		}

		//判定の半径を変える
		void SetCollScale(float scale) {
			m_collision->SetMakedRadius(scale);
		}
		//判定の持続時間を設定
		void ActiveCollision(float activetime) {
			m_ActiveTime = activetime;
		}

		//攻撃判定を相手に当てた時に呼ばれる関数
		HitInfo GetHitInfo() {
			return m_info;
		}
		//攻撃判定を更新する
		void SetHitInfo(HitInfo info) {
			this->m_info = info;
		}
	};
}
//end basecross

