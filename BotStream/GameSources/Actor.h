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
		int m_attack;
		//防御力
		int m_defense;
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

		//着地判定を無効化する時間
		float m_disableLandDetect = 0.0f;
		//地上にいるか否か
		bool m_isLand = false;
		//向いている角度
		float m_angle;

		//喰らいモーション時間
		float m_hitbacktime = 0;
		//攻撃を受けた方向
		Vec3 m_hitDirection = Vec3(0);

		//ボーズ状態かどうか
		bool m_poseFlag = false;

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

		//ダメージ計算式
		int CalculateDamage(int damage) {
			int ret = damage - m_defense;
			return ret <= 0 ? 1 : ret;
		}

		shared_ptr<PNTBoneModelDraw> GetBoneModelDraw() {
			return GetComponent<PNTBoneModelDraw>();
		}

		shared_ptr<Transform> GetTransform() {
			return GetComponent<Transform>();
		}

		// エフェクトの再生
		void EfkPlaying(const wstring efkKey, const float rad, const Vec3 rotate,Vec3 pushPos = Vec3(0.0f));
		void EfkPlaying(const wstring efkKey, const float rad, const Vec3 rotate, Col4 changeColor, Vec3 pushPos = Vec3(0.0f));
		// 地面着地
		void OnLanding();

		//OnCollisionEnterに置く
		void DetectBeingAttacked(shared_ptr<GameObject>& other);
	public:
		Actor(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale);
		~Actor();

		void OnCreate() override;
		void OnUpdate() override;

		//エフェクトを出す処理
		virtual void AddEffect(int addEffect);

		//ポーズのフラグをオンオフする関数
		void PoseSwitch(bool onOff);

		//HP関係のゲッタセッタ
		int GetHPCurrent() {
			return m_HPCurrent;
		}
		void SetHPCurrent(int setHP) {
			m_HPCurrent = setHP;
		}
		int GetHPMax() {
			return m_HPMax;
		}

		//喰らった攻撃の吹き飛ばし距離を代入(現状地上のもののみ)
		void HitBack() {
			m_hitbacktime = m_GetHitInfo.HitTime_Stand;

			Vec3 nrm = m_hitDirection.normalize();
			float dir = atan2f(nrm.z, nrm.x);

			Vec3 vel = m_GetHitInfo.HitVel_Stand;
			m_velocity.x = (cosf(dir) * vel.x) - (sinf(dir) * vel.z);
			m_velocity.y = vel.y;
			m_velocity.z = (cosf(dir) * vel.z) + (sinf(dir) * vel.x);
		}

		//攻撃判定のポインタを取得
		shared_ptr<AttackCollision> GetAttackPtr() {
			return m_AttackCol;
		}

		//攻撃判定の内容を更新する
		void DefAttack(float activetime, HitInfo info) {
			m_AttackCol->SetHitInfo(info);
			m_AttackCol->ActivateCollision(activetime);
		}

		float GetAngle();   //今向いている方向のゲッター
		void SetAngle(float angle);	//向いている方向のセッター

		Vec3 GetVelocity() {
			return m_velocity;
		}
		void SetVelocity(Vec3 vel) {
			m_velocity = vel;
		}

		Vec3 GetForward() {
			Vec3 vec = GetComponent<Transform>()->GetForward();

			const float rotate = -XM_PIDIV2;

			Vec3 fixedVec;
			fixedVec.x = (cosf(rotate) * vec.x) - (sinf(rotate) * vec.z);
			fixedVec.y = vec.y;
			fixedVec.z = (cosf(rotate) * vec.z) + (sinf(rotate) * vec.x);

			return fixedVec;
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
	};
	
}
//end basecross
