/*!
@file Player.h
@brief プレイヤーなど
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
#include "Actor.h"
#include "Enemy.h"

namespace basecross {
	class EfkEffect;
	class StageSato;
	class PlayerBulletUI;
	class EnemyBase;//EnemyZakoが居なくなったら消す
	class BillBoard;
	class BillBoardGauge;
	class Sprite;

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


	enum ActorName
	{
		ActorName_Player,
		ActorName_Enemy
	};

	////装備しているパーツのステータス合計一覧
	//struct EquippedParts
	//{
	//	//攻撃力
	//	int addAttack;
	//	//速度
	//	int addSpeed;
	//	//善か悪
	//	int addAttribute;
	//	//追加HP
	//	int addHP;
	//};

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
		//移動していいかのフラグ
		bool m_walkFlag = true;
		//攻撃していいかのフラグ
		bool m_attackFlag = true;
		//ダッシュのフラグ
		bool m_dashFlag = false;


		// ジャスト回避したかのフラグ
		bool m_jastDodge = false;
		// スローモーションをどれくらい継続するかの数値
		float m_timeOfJastDodgeCount = 0.0f;
		float m_timeOfJastDodgeMax = 1.0f;

		//地面にめり込んでしまっている時間
		float m_immersedInTime = 0.0f;

		//元々の最大HP
		int m_originallyHPMax = 100;
		//最大SP
		int m_SPMax = 100;
		//現在SP 
		int m_SPCurrent = 0;
		//最大の球数
		int m_bulletNumMax = 10;
		//現在の球数
		int m_bulletNum = 10;

		//現在の装備しているパーツのステータス
		PartsStatus m_equippedParts;

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
		shared_ptr<PlayerBulletUI> m_playerBulletUI = nullptr;// 現在の球数を表示するUI
		shared_ptr<Sprite> m_JastDodgeSprite = nullptr;// ジャスト回避用の演出UI
		float m_alphaColorjastDodge = 0.0f;

		//コントローラー関係
		CONTROLER_STATE m_controller;
		Vec3 m_stickL;

		void Jump();

		//ダッシュ処理
		void Dash();

		//回避処理
		void Dodge();

	public:
		Player(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, int HP = 100, int attack = 10, int defense = 1);
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

		//ジャスト回避時の処理
		void JastDodge(float deltaScale,float slowTime);

		//地面めり込み回避処理
		void ImmersedInCheck();

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

		//m_equippedPartsのセッタ
		void SetEquippedParts(PartsStatus parts);
		//m_equippedPartsのゲッタ
		PartsStatus GetEquippedParts();

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
			return m_getHitInfo;
		}
		//スティックの傾きを受け取るセッター
		void Player::SetStickL(Vec3 SetStickL)
		{
			m_stickL = SetStickL;
		}

		//攻撃フラグのゲッタセッタ
		bool Player::GetAttackFlag()
		{
			return m_attackFlag;
		}
		void Player::SetAttackFlag(bool onOff)
		{
			m_attackFlag = onOff;
		}

		//移動フラグのゲッタセッタ
		bool Player::GetwalkFlag()
		{
			return m_walkFlag;
		}
		void Player::SetWalkFlag(bool onOff)
		{
			m_walkFlag = onOff;
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

		//ジャスト回避しているかのゲッタ
		bool GetDodge()
		{
			return m_jastDodge;
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
		float m_speed = 1.5f;
		float m_AngleYAxis = 0.0f;//y軸の角度
		float m_angleXAxis = 0.0f;//x軸の角度
		float m_canMoveDistance = 0.0f;//移動できる長さ

		Handle m_gunLine;

		//攻撃しているアクター
		int m_actorType;

		weak_ptr<Actor> m_originObj;//自分を生成したオブジェクト

		shared_ptr<Transform> m_trans;

	public:
		Bullet(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, float speed, shared_ptr<Actor> originObj, float canMoveDistance = 30.0f, int actorType = ActorName_Player) :
			Actor(stagePtr, pos, rot, scale),
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
}
//end basecross

