/*!
@file Player.h
@brief プレイヤーなど
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
#include "Actor.h"

namespace basecross{
	class Player : public Actor
	{
	private:
		//経過時間
		float _delta;
		//速度
		Vec3 m_velocity;
		Vec3 m_accel;
		//最高速
		const float m_speedMax = 100.0f;
		//加速度
		const float m_baseAccel = 400.0f;
		//摩擦係数(静/動/完全停止)
		const float m_friction = .75f;
		const float m_frictionDynamic = .5f;
		const float m_frictionThreshold = .5f;
		//スティックのデッドゾーン(あとでmanager系に置くかも)
		float m_stickDeadZone = .1f;


		float m_angle;//Playerが向いている角度

		//動く処理
		void PlayerMove();
		//スティック操作
		Vec3 GetMoveVector();
		//摩擦
		void Friction();
		//速度制限
		void SpeedLimit(float multiply);

	public:
		Player(const shared_ptr<Stage>& stagePtr,Vec3 pos,Vec3 rot,Vec3 scale);
		~Player();

		void OnCreate()override;//作成
		void OnUpdate()override;//更新

		float GetAngle();   //今プレイヤーが向いている方向のゲッター
		void SetAngle(float angle);	//プレイヤーの向いている方向のセッター

		shared_ptr<PNTBoneModelDraw> GetBoneModelDraw() {
			return GetComponent<PNTBoneModelDraw>();
		}
		shared_ptr<Transform> GetTransform() {
			return GetComponent<Transform>();
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

		//デバック用の文字列
		void DebugLog();

	};

}
//end basecross

