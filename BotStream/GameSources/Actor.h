/*!
@file Actor.h
@brief プレイヤーなど
*/

#pragma once
#include "stdafx.h"
//#include "Project.h"
#include "LandDetect.h"
//#include "MyGameObject.h"

namespace basecross {
	enum ActorEffect
	{
		PlayerEffect_Attack1,
		PlayerEffect_Attack2,
		PlayerEffect_Attack3,
		PlayerEffect_AttackEx,
		PlayerEffect_Beam,
		PlayerEffect_Dodge,
		PlayerEffect_Dash,
		PlayerEffect_DashRipple,
		EnemyEffect_Stun,
		EnemyEffect_ArmorBreak,
		EnemyEffect_Beam,
		EnemyEffect_Sphere,
		EnemyEffect_SphereEnd,
		EnemyEffect_Spin,
		EnemyEffect_Wave
	};

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
		//加速度(Friction関数で使用)
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
		float m_landDetectDisableTime = 0.0f;
		//地上にいるか否か
		bool m_isLand = false;
		//向いている角度
		float m_angle;
		//重力・摩擦処理の有無
		bool m_doPhysics = true;

		//喰らいモーション時間
		float m_hitbacktime = 0;
		//攻撃を受けた方向
		Vec3 m_hitDirection = Vec3(0);

		//SE関係
		shared_ptr<SoundItem> m_SE = nullptr;//再生しているSE
		shared_ptr<XAudio2Manager> m_SEManager = nullptr;//SEなどを再生するためのマネージャ

		//ボーズ状態かどうか
		bool m_PauseFlag = false;

		//攻撃判定
		shared_ptr<AttackCollision> m_AttackCol;
		//着地判定
		shared_ptr<LandDetect> m_LandDetect;
		//受けた攻撃の情報
		HitInfo m_getHitInfo;

		//摩擦
		void Friction();
		//速度制限
		void SpeedLimit(float multiply);
		//重力
		void Gravity();

		//攻撃を受けた時の処理(継承用)
		virtual void OnDamaged() {}

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

		//velocityをTransformのPositionにぶち込む作業
		void UpdatePosition() {
			GetTransform()->SetPosition((m_velocity * _delta) + GetTransform()->GetPosition());
		}

		// エフェクトの再生
		Effekseer::Handle EfkPlaying(const wstring efkKey, const float rad, const Vec3 rotate, const Vec3 scale = Vec3(1.0f), Vec3 pushPos = Vec3(0.0f));
		Effekseer::Handle EfkPlaying(const wstring efkKey, const float rad, const Vec3 rotate, Col4 changeColor, Vec3 pushPos = Vec3(0.0f));
		//void EfkPlaying(const wstring efkKey, const float rad, const Vec3 rotate, const Vec3 scale = Vec3(1.0f), Vec3 pushPos = Vec3(0.0f));
		
		// 地面着地
		void OnLanding();

		//OnCollisionEnterに置く
		void DetectBeingAttacked(shared_ptr<GameObject>& other);

		//OnCreateで実行
		virtual void CreateChildObjects() {
			auto stage = GetStage();

			//着地判定の生成
			m_LandDetect = stage->AddGameObject<LandDetect>();
			m_LandDetect->GetComponent<Transform>()->SetParent(dynamic_pointer_cast<GameObject>(GetThis<Actor>()));

			//攻撃判定の生成
			m_AttackCol = stage->AddGameObject<AttackCollision>();
			m_AttackCol->GetComponent<Transform>()->SetParent(dynamic_pointer_cast<GameObject>(GetThis<Actor>()));
		}

		//削除処理
		void RemoveSelf() {
			GetStage()->RemoveGameObject<LandDetect>(m_LandDetect);
			GetStage()->RemoveGameObject<AttackCollision>(m_AttackCol);
			GetStage()->RemoveGameObject<Actor>(GetThis<Actor>());
		}

	public:
		Actor(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale);
		~Actor();

		void OnCreate() override;
		void OnUpdate() override;

		//エフェクトを出す処理
		virtual Effekseer::Handle AddEffect(int addEffect);

		//ポーズのフラグをオンオフする関数
		void SetPause(bool onOff);

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
			m_hitbacktime = m_getHitInfo.HitTime_Stand;

			//どちらから攻撃されたかを計算
			Vec3 nrm = m_hitDirection.normalize();
			float dir = atan2f(nrm.z, nrm.x);

			Vec3 vel = (m_isLand) ? m_getHitInfo.HitVel_Stand : m_getHitInfo.HitVel_Air;

			Vec3 accel;
			accel.x = (cosf(dir) * vel.x) - (sinf(dir) * vel.z);
			accel.y = vel.y;
			accel.z = (cosf(dir) * vel.z) + (sinf(dir) * vel.x);
			
			SetVelocity(accel);
		}

		//受けた攻撃の情報を取得
		HitInfo GetHitInfo() {
			return m_getHitInfo;
		}

		//攻撃判定のポインタを取得
		shared_ptr<AttackCollision> GetAttackPtr() {
			return m_AttackCol;
		}

		//攻撃判定の内容を更新する
		void DefAttack(float activetime, HitInfo info) {
			m_AttackCol->SetMoveContact(false);
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
		void AddVelocity(Vec3 vel) {
			m_accel = vel;
			m_velocity += vel;
		}

		//前方ベクトルの取得
		Vec3 GetForward() {
			Vec3 vec = GetComponent<Transform>()->GetForward();

			const float rotate = -XM_PIDIV2;

			Vec3 fixedVec;
			fixedVec.x = (cosf(rotate) * vec.x) - (sinf(rotate) * vec.z);
			fixedVec.y = vec.y;
			fixedVec.z = (cosf(rotate) * vec.z) + (sinf(rotate) * vec.x);

			return fixedVec;
		}

		//地面の上にいるか否かのゲッター
		bool GetLand() {
			return m_isLand;
		}

		//物理的処理のセッタ
		void SetPhysics(bool doPhysics) {
			m_doPhysics = doPhysics;
		}
		//物理的処理のゲッタ
		bool GetPhysics() {
			return m_doPhysics;
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

		//SEの再生
		void PlaySnd(wstring sndname, float volume, float loopcount) {
			m_SE = m_SEManager->Start(sndname, loopcount, volume);
		}

		//SEの停止
		void StopSnd() {
			if (m_SE == nullptr) {
				return;
			}
			m_SEManager->Stop(m_SE);
		}
	};

	/// <summary>
	/// 飛び道具の親クラス
	/// </summary>
	class ProjectileBase :public Actor
	{
	protected:
		float m_speed = 1.0f;
		float m_originAngle = 0.0f;
		float m_canMoveDistance;//移動できる長さ

		weak_ptr<Actor> m_originObj;//自分を生成したオブジェクト

		//攻撃判定を設定するための関数。OnCreateに置く
		virtual void HitInfoInit() { }
	public:
		ProjectileBase(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, shared_ptr<Actor> originObj) :
			Actor(stagePtr, pos, rot, scale),
			m_originObj(originObj)
		{

		}
		~ProjectileBase()
		{
		}

		void OnCreate()override;
		void OnUpdate()override;


	};

}
//end basecross
