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
		//ダッシュ時の最高速
		const float m_speedMaxDash = 20.0f;
		//スティックのデッドゾーン(あとでmanager系に置くかも)
		float m_stickDeadZone = .1f;
		//回避したかのフラグ
		bool m_dodgeFlag = false;
		//回避時間を測る変数
		float m_dodgeTime;
		//ダッシュのフラグ
		bool m_dashFlag = false;

		////最大HP
		//int m_HPMax = 100.0f;
		////攻撃力
		//int m_attack;
		////防御力
		//int m_defense;
		////現在HP
		//int m_HPCurrent;
		//最大SP
		int m_SPMax = 100;
		//現在SP 
		int m_SPCurrent = 0;
		//最大の球数
		int m_bulletNumMax = 20;
		//現在の球数
		int m_bulletNum = 20;

		//リロードしている時間計測
		float m_reloadTimeCount = 0.0f;

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

	public:
		Player(const shared_ptr<Stage>& stagePtr,Vec3 pos,Vec3 rot,Vec3 scale,int HP = 100,int attack = 10,int defense = 1);
		~Player();

		void OnCreate()override;//作成
		void OnUpdate()override;//更新

		void ChangeState(wstring stateName);//ステート変更
		void AddEffect(int addEffect);//エフェクトを出す処理

		//プレイヤーの移動処理
		void PlayerMove(int playerState);
		//移動ベクトルの計算処理
		Vec3 GetMoveVector(int playerState);

		//球がなくなった時のリロード処理
		void ReloadBullet(float ReloadTime);

		//回避フラグのゲッター
		bool GetDodgeFlag();

		//HPのゲッター
		int GetHP();
		//HPのゲッター
		int GetMaxHP();
		//SPのゲッター
		int GetSP();
		//SPMaxのゲッター
		int GetMaxSP();
		//現在の球数を受け取る
		int Player::GetBulletNum()
		{
			return m_bulletNum;
		}
		//現在の球数を変更する
		void Player::SetBulletNum(int BulletNum)
		{
			m_bulletNum = BulletNum;
		}
		//最大の球数を受け取る
		int Player::GetBulletMaxNum()
		{
			return m_bulletNumMax;
		}



		//デバック用の文字列
		void DebugLog();


	};

}
//end basecross

