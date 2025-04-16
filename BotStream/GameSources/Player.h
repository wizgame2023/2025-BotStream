/*!
@file Player.h
@brief プレイヤーなど
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
#include "Actor.h"

namespace basecross{
	class EfkEffect;
	class StageSato;
	enum PlayerState
	{
		PlayerState_Walk,
		PlayerState_Dodge,
		PlayerState_Dash,
		PlayerState_Attack1,
		PlayerState_Attack2,
		PlayerState_Attack3,
		PlayerState_AttackEx
	};

	enum PlayerEffect
	{
		PlayerEffect_Attack1,
		PlayerEffect_Attack2,
		PlayerEffect_Attack3,
		PlayerEffect_AttackEx,
		PlayerEffect_Beam
	};

	class Player : public Actor
	{
	private:
		//ジャンプ力ぅ…
		const float m_jumpPower = 18.0f;
		//最高速
		const float m_speedMax = 10;
		//ダッシュ時の最高速
		const float m_speedMaxDash = 20.0f;
		//加速度
		const float m_baseAccel = 400.0f;
		//重力
		const float m_gravity = -32.0f;
		//落下時の終端速度
		const float m_fallTerminal = -120.0f;
		//摩擦係数(静/動/完全停止)
		const float m_friction = .75f;
		const float m_frictionDynamic = .5f;
		const float m_frictionThreshold = .5f;
		//スティックのデッドゾーン(あとでmanager系に置くかも)
		float m_stickDeadZone = .1f;
		//地上にいるか否か
		bool m_isLand = false;
		//回避したかのフラグ
		bool m_dodgeFlag = false;
		//回避時間を測る変数
		float m_dodgeTime;
		//ダッシュのフラグ
		bool m_dashFlag = false;
		//着地判定を無効化する時間
		float m_disableLandDetect = 0.0f;

		float m_angle;//Playerが向いている角度

		// ほぼごり押しだからメンバー要確認
		//shared_ptr<GameStage> m_Stage;
		shared_ptr<StageSato> m_Stage;

		//ステートマシン
		shared_ptr<PlayerStateMachine> m_stateMachine;


		void Jump();

		//ダッシュ処理
		void Dash();

		//回避処理
		void Dodge();

		// エフェクトの再生
		void EfkPlaying(const wstring efkKey, const float rad, const Vec3 rotate);
		void EfkPlaying(const wstring efkKey, const float rad, const Vec3 rotate, Col4 changeColor);
		// 地面着地
		void OnLanding();
	public:
		Player(const shared_ptr<Stage>& stagePtr,Vec3 pos,Vec3 rot,Vec3 scale);
		~Player();

		void OnCreate()override;//作成
		void OnUpdate()override;//更新

		void ChangeState(wstring stateName);//ステート変更
		void AddEffect(int addEffect);//エフェクトを出す処理

		float GetAngle();   //今プレイヤーが向いている方向のゲッター
		void SetAngle(float angle);	//プレイヤーの向いている方向のセッター

		//プレイヤーの移動処理
		void PlayerMove(int playerState);
		//移動ベクトルの計算処理
		Vec3 GetMoveVector(int playerState);

		//回避フラグのゲッター
		bool GetDodgeFlag();


		//デバック用の文字列
		void DebugLog();


	};

}
//end basecross

