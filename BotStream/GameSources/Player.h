/*!
@file Player.h
@brief プレイヤーなど
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
#include "Actor.h"
#include "Enemy.h"

namespace basecross{
	class EfkEffect;
	class StageSato;
	class PlayerBulletUI;
	class EnemyBase;//EnemyZakoが居なくなったら消す
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

	enum ActorName
	{
		ActorName_Player,
		ActorName_Enemy
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
		int m_bulletNumMax = 3;
		//現在の球数
		int m_bulletNum = 3;

		//リロードしている時間計測
		float m_reloadTimeCount = 0.0f;

		// ほぼごり押しだからメンバー要確認
		//shared_ptr<GameStage> m_Stage;
		shared_ptr<StageSato> m_Stage;

		//ステートマシン
		shared_ptr<PlayerStateMachine> m_stateMachine;

		//SE関係
		shared_ptr<SoundItem> m_SE = nullptr;//再生しているSE
		shared_ptr<XAudio2Manager> m_SEManager = nullptr;//SEなどを再生するためのマネージャ

		//UI関係
		shared_ptr<PlayerBulletUI> m_playerBulletUI = nullptr;//現在の球数を表示するUI


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
		//void AddEffect(int addEffect)override;//エフェクトを出す処理

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
		//HPのセッター
		void SetHP(int setHP);
		//SPのセッター
		void SetSP(int setSP);
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
		//受けた攻撃の情報を渡すゲッター
		HitInfo Player::GetHitInfo()
		{
			return m_GetHitInfo;
		}

		void OnCollisionEnter(shared_ptr<GameObject>& Other)override;

		void OnDamaged()override;

		void hitbackMove();



		//デバック用の文字列
		void DebugLog();
	};

	class Bullet :public Actor
	{
	private:
		float m_speed = 1.0f;
		float m_playerAngle = 0.0f;
		float m_canMoveDistance;//移動できる長さ

		//攻撃しているアクター
		int m_actorType;

		weak_ptr<Actor> m_originObj;//自分を生成したオブジェクト

		shared_ptr<Transform> m_trans;
	public:
		Bullet(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale,float speed,shared_ptr<Actor> originObj,float canMoveDistance = 10.0f,int actorType = ActorName_Player):
			Actor(stagePtr,pos,rot,scale),
			m_speed(speed),
			m_originObj(originObj),
			m_canMoveDistance(canMoveDistance),
			m_actorType(actorType)
		{

		}
		~Bullet()
		{
		}

		void OnCreate()override;
		void OnUpdate()override;

		//攻撃しているアクターを指定する
		void SetAttackActor(int actorName);
	};


	//雑魚敵の処理をいったんここに書きますマージ終わったらEnemy.cpp.hに戻す
	class EnemyZako : public EnemyBase
	{
	private:
		void OnDamaged() override;

	public:
		EnemyZako(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, bool used = false):
			EnemyBase(stagePtr,pos,rot,scale,used)
		{

		}
		~EnemyZako() {}

		//受けた攻撃の情報を渡すゲッター
		HitInfo EnemyZako::GetHitInfo()
		{
			return m_GetHitInfo;
		}

		void OnCreate() override;
		void OnUpdate() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
	};
	
}
//end basecross

