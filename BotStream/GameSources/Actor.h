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

		shared_ptr<AttackCollision> m_AttackCol;
		//shared_ptr<State> m_State;

		shared_ptr<LandDetect> m_LandDetect;

		//摩擦
		void Friction();
		//速度制限
		void SpeedLimit(float multiply);
		//重力
		void Gravity();

		//攻撃を受けた時の処理(継承用)
		virtual void OnDamaged() {

		}
	public:
		Actor(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale);
		~Actor();

		void OnCreate() override;
		void OnUpdate() override;
	};
	
}
//end basecross
