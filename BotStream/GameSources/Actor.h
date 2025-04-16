/*!
@file Actor.h
@brief プレイヤーなど
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
#include "LandDetect.h"
//#include "MyGameObject.h"

namespace basecross {
	class Actor : public ObjectMove
	{

	protected:
		shared_ptr<Transform> m_trans;
		Vec3 m_pos;//位置
		Vec3 m_rot;//回転
		Vec3 m_scale;//大きさ

		//最大HP
		int m_HPMax;
		//攻撃力
		int attack;
		//防御力
		int defense;
		//現在HP
		int m_HPCurrent;

		//経過時間
		float _delta;
		//速度
		Vec3 m_velocity;
		//加速度(Friction関数で使用するのでvelocityを変動させる場合はこの変数を使ってください)
		Vec3 m_accel;
		//最高速
		float m_speedMax = 10;
		//加速度にかける数
		float m_baseAccel = 400.0f;
		//重力
		float m_gravity = -32.0f;
		//落下時の終端速度
		float m_fallTerminal = -120.0f;
		//摩擦係数(静/動/完全停止)
		float m_friction = .75f;
		float m_frictionDynamic = .5f;
		float m_frictionThreshold = .5f;

		//攻撃判定
		shared_ptr<AttackCollision> m_AttackCol;
		//着地判定
		shared_ptr<LandDetect> m_LandDetect;
		//受けた攻撃の情報
		HitInfo m_GetHitInfo;

		//摩擦
		void Friction();
		//速度制限
		void SpeedLimit(float multiply);
		//重力
		void Gravity();

		//攻撃を受けた時の処理(継承用)
		virtual void OnDamaged() { }

		shared_ptr<PNTBoneModelDraw> GetBoneModelDraw() {
			return GetComponent<PNTBoneModelDraw>();
		}

		shared_ptr<Transform> GetTransform() {
			return GetComponent<Transform>();
		}

		//攻撃判定の内容を更新する
		void DefAttack(float activetime, HitInfo info) {
			m_AttackCol->SetHitInfo(info);
			m_AttackCol->ActiveCollision(activetime);
		}

		//アニメーション変更(成功した場合trueを返す)
		bool ChangeAnim(wstring anim, bool forceChange = false) {
			auto drawPtr = GetBoneModelDraw();
			//既に再生中なら変更しない　forceChangeの場合は例外
			if (drawPtr->GetCurrentAnimation() != anim || forceChange)
			{
				drawPtr->ChangeCurrentAnimation(anim);
				return true;
			}
			else {
				return false;
			}
		}

		//OnCollisionEnterに置く
		void DetectBeingAttacked(shared_ptr<GameObject>& other);
	public:
		Actor(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale);
		~Actor();

		void OnCreate() override;
		void OnUpdate() override;

	};
	
}
//end basecross
