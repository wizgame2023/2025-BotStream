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
		//銃怯み
		int StunDamage;
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
		//やられ中無敵
		bool InvincibleOnHit;
		//一度しかヒットしない
		bool HitOnce;

		//エフェクト・音のキー
		wstring HitEffect;
		wstring HitSound;

		//コンストラクタ
		HitInfo::HitInfo(AttackType attacktype, 
			int damage, int stundamage, float pausetime,
			float hittime_stand, Vec3 hitvel_stand,
			float hittime_air, Vec3 hitvel_air,
			bool invincibleonhit, bool hitonce,
			wstring hiteffect, wstring hitsound
		)
		{
			this->Type = attacktype;
			this->Damage = damage;
			this->StunDamage = stundamage;
			this->PauseTime = pausetime;
			this->HitTime_Stand = hittime_stand;
			this->HitVel_Stand = hitvel_stand;
			this->HitTime_Air = hittime_air;
			this->HitVel_Air = hitvel_air;
			this->InvincibleOnHit = invincibleonhit;
			this->HitOnce = hitonce;
			this->HitEffect = hiteffect;
			this->HitSound = hitsound;
		}
		HitInfo::HitInfo() {
			this->Type = AttackType::Player;
			this->Damage = 0;
			this->StunDamage = 0;
			this->PauseTime = 0;
			this->HitTime_Stand = 0;
			this->HitVel_Stand = Vec3(0);
			this->HitTime_Air = 0;
			this->HitVel_Air = Vec3(0);
			this->InvincibleOnHit = false;
			this->HitOnce = false;
			this->HitEffect = L"SlashHitEfk";
			this->HitSound = L"Damage";
		}
	};

	class AttackCollision : public MyGameObject
	{
		shared_ptr<CollisionCapsule> m_collision;

		//攻撃のスペック
		HitInfo m_info;

		//当たったらtrueになるやつ
		bool m_moveContact = false;
		//持続時間
		float m_ActiveTime = 0.0f;
	public:
		AttackCollision(const shared_ptr<Stage>& stagePtr);
		~AttackCollision();

		void OnCreate() override;
		void OnUpdate() override;

		shared_ptr<CollisionCapsule> GetCollisionPtr() {
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
		//判定の高さを変える
		void SetCollHeight(float scale) {
			m_collision->SetMakedHeight(scale);
		}

		//判定の持続時間を設定
		void ActivateCollision(float activetime) {
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

		bool GetMoveContact();
		void SetMoveContact(bool mc);

	};
}
//end basecross

