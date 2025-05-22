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
	class BillBoard;
	class BillBoardGauge;
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
		PlayerEffect_Beam,
		EnemyEffect_ArmorBreak,
		EnemyEffect_Beam,
		EnemyEffect_Sphere,
		EnemyEffect_SphereEnd
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
		//回避できるかのフラグ
		bool m_dodgeFlag = true;
		//回避処理を終えたかのフラグ
		bool m_endDodgeFlag = false;
		//回避計測時間
		float m_dodgeTime = 0.0f;
		//回避のクールタイムを測る変数
		float m_dodgeCoolTime = 0.0f;
		//回避のクールタイムの時間
		float m_maxDodgeCoolTime = 0.3f;
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
		int m_bulletNumMax = 10;
		//現在の球数
		int m_bulletNum = 10;

		//リロードしている時間計測
		float m_reloadTimeCount = 0.0f;

		// ほぼごり押しだからメンバー要確認
		//shared_ptr<GameStage> m_Stage;
		shared_ptr<StageSato> m_Stage;

		//アニメーションの更新時間
		float m_addTimeAnimation = 1.0f * _delta;

		//ステートマシン
		shared_ptr<PlayerStateMachine> m_stateMachine;

		//UI関係
		shared_ptr<PlayerBulletUI> m_playerBulletUI = nullptr;//現在の球数を表示するUI

		//コントローラー関係
		CONTROLER_STATE m_controller;
		Vec3 m_stickL;

		void Jump();

		//ダッシュ処理
		void Dash();

		//回避処理
		void Dodge();

		//エフェクトテスト用
		Vec3 m_EfkPos;
		Handle m_testEffect;

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
		//移動方向の処理
		float MoveAngle(Vec3 stickL);

		//球がなくなった時のリロード処理
		void ReloadBullet(float ReloadTime);
		//アニメーションの更新
		void UpdateAnimation(float addTime);

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
		
		//DodgeFlagのセッター
		void SetDodgeFlag(bool setDodgeFlag);
		//DodgeFlagのゲッター
		bool GetDodgeFlag();

		//endDodgeFlagのセッター
		void SetEndDodgeFlag(bool setDodgeFlag);
		//endDodgeFlagのゲッター
		bool GetEndDodgeFlag();


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
		//スティックの傾きを受け取るセッター
		void Player::SetStickL(Vec3 SetStickL)
		{
			m_stickL = SetStickL;
		}

		//アニメーションの追加時間のゲッタセッタ
		float GetAddTimeAnimation()
		{
			return m_addTimeAnimation;
		}
		void SetAddTimeAnimation(float addTimeAnimation)
		{
			m_addTimeAnimation = addTimeAnimation;
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
		float m_AngleYAxis = 0.0f;//y軸の角度
		float m_angleXAxis = 0.0f;//x軸の角度
		float m_canMoveDistance = 0.0f;//移動できる長さ

		//攻撃しているアクター
		int m_actorType;

		weak_ptr<Actor> m_originObj;//自分を生成したオブジェクト

		shared_ptr<Transform> m_trans;

	public:
		Bullet(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale,float speed,shared_ptr<Actor> originObj,float canMoveDistance = 30.0f,int actorType = ActorName_Player):
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

		void OnCollisionEnter(shared_ptr<GameObject>& obj)override;

		//攻撃しているアクターを指定する
		void SetAttackActor(int actorName);
	};


	//雑魚敵の処理をいったんここに書きますマージ終わったらEnemy.cpp.hに戻す
	class EnemyZako : public EnemyBase
	{
	private:
		void OnDamaged() override;

		//HPバー用のビルボード
		shared_ptr<BillBoard> m_HPFrame = nullptr;
		shared_ptr<BillBoardGauge> m_HPBer = nullptr;

		shared_ptr<BillBoard> m_damageBill = nullptr;

		//攻撃のタイプ　テスト用に近距離にしたいのでそうする
		int m_AttackType = Zako_Melee;

		//アニメーションの更新時間
		float m_addTimeAnimation = 0.0f;
		//shared_ptr<EnemyDamageBill> m_damageBill = nullptr;

		//攻撃のクールダウン関係
		bool m_attackFlag = false;
		float m_timeOfAttackCool = 5.0f;
		float m_timeCountOfAttackCool = 3.0f;

	public:
		// 雑魚敵の種類を追加するときはここに書く
		enum EnemyZakoAttackType
		{
			Zako_Melee,//近距離型
			Zako_Long,//遠距離型
			Zako_Flying,//滞空型
		};

		EnemyZako(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, bool used = false, int attackType = Zako_Melee):
			EnemyBase(stagePtr,pos,rot,scale,used),
			m_AttackType(attackType)
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

		//ビルボードの処理
		void UpdateHPBer();

		//攻撃のクールタイム
		void TimeOfAttackCool();

		//攻撃のタイプのゲッタ
		int GetAttackType()
		{
			return m_AttackType;
		}

		//攻撃フラグのゲッタセッタ
		bool GetAttackFlag()
		{
			return m_attackFlag;
		}
		void SetAttackFlag(bool attackFlag)
		{
			m_attackFlag = attackFlag;
		}

		//アニメーションの追加時間のゲッタセッタ
		float GetAddTimeAnimation()
		{
			return m_addTimeAnimation;
		}
		void SetAddTimeAnimation(float addTimeAnimation)
		{
			m_addTimeAnimation = addTimeAnimation;
		}
	};
	
	//class EnemyZakoFlying : public EnemyBase
	//{
	//private:
	//	void OnDamaged() override;

	//	//HPバー用のビルボード
	//	shared_ptr<BillBoard> m_HPFrame = nullptr;
	//	shared_ptr<BillBoardGauge> m_HPBer = nullptr;

	//	shared_ptr<BillBoard> m_damageBill = nullptr;

	//	//攻撃のタイプ　テスト用に近距離にしたいのでそうする
	//	int m_AttackType = Zako_Melee;

	//	//アニメーションの更新時間
	//	float m_addTimeAnimation = 0.0f;
	//	//shared_ptr<EnemyDamageBill> m_damageBill = nullptr;

	//	//攻撃のクールダウン関係
	//	bool m_attackFlag = true;
	//	float m_timeOfAttackCool = 5.0f;
	//	float m_timeCountOfAttackCool = 0.0f;

	//public:
	//	enum EnemyZakoAttackType
	//	{
	//		Zako_Melee,//近距離型
	//		Zako_Long//遠距離型
	//	};

	//	EnemyZakoFlying(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, bool used = false):
	//		EnemyBase(stagePtr,pos,rot,scale,used)
	//	{

	//	}
	//	~EnemyZakoFlying() {}

	//	//受けた攻撃の情報を渡すゲッター
	//	HitInfo EnemyZakoFlying::GetHitInfo()
	//	{
	//		return m_GetHitInfo;
	//	}

	//	void OnCreate() override;
	//	void OnUpdate() override;
	//	virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;

	//	//ビルボードの処理
	//	void UpdateHPBer();

	//	//攻撃のクールタイム
	//	void TimeOfAttackCool();

	//	//攻撃のタイプのゲッタ
	//	int GetAttackType()
	//	{
	//		return m_AttackType;
	//	}

	//	//攻撃フラグのゲッタセッタ
	//	bool GetAttackFlag()
	//	{
	//		return m_attackFlag;
	//	}
	//	void SetAttackFlag(bool attackFlag)
	//	{
	//		m_attackFlag = attackFlag;
	//	}

	//	//アニメーションの追加時間のゲッタセッタ
	//	float GetAddTimeAnimation()
	//	{
	//		return m_addTimeAnimation;
	//	}
	//	void SetAddTimeAnimation(float addTimeAnimation)
	//	{
	//		m_addTimeAnimation = addTimeAnimation;
	//	}
	//};
}
//end basecross

