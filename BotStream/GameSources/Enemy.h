/*!
@file Enemy.h
@brief 敵など
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
#include "Actor.h"
#include "EnemyState.h"

namespace basecross{

	/// <summary>
	/// 親クラス
	/// </summary>
	class EnemyBase : public Actor
	{
	protected:
		bool m_used = false;
		shared_ptr<StateMachineBase> m_state;
		const float m_rotateSpeed = 1.2f;
		const float m_rotateThreshold = .3f;

		int m_stun = 0;
		int m_stunMax = 3;

		float m_armorMax = 0;
		float m_armor = 0;
		float m_armorRecover = 0;
		float m_armorRecoverTime = 0;

		float m_armorFlash = 0;
		const float m_armorFlashMax = .1f;
		Col4 m_armorFlashFX = Col4(5.0f, .5f, .5f, 0);

		weak_ptr<Player> m_player;


	public:
		EnemyBase(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale);
		EnemyBase(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, bool use);
		~EnemyBase() {}

		virtual void HitBackStandBehavior();

		void DetectBeingAttacked(shared_ptr<GameObject>& other)override;

		void ChangeState(wstring stateName) {
			m_state->ChangeState(stateName);
		}
		wstring GetStateName() {
			return m_state->GetStateName();
		}

		void OnCreate() override;
		void OnUpdate() override;

		//初期化処理(敵をスポーンさせるとき使う？)
		void Initialize(Vec3 pos, Vec3 rot, Vec3 scale) {
			m_used = true;

			auto ptr = GetComponent<Transform>();
			ptr->SetPosition(pos);
			ptr->SetRotation(rot);
			ptr->SetScale(scale);

			SetDrawActive(true);
			SetUpdateActive(true);
			m_LandDetect->SetUpdateActive(true);
			m_landDetectDisableTime = 0;
		}
		//初期化処理(引数無し)
		void Initialize() {
			m_used = true;
			SetDrawActive(true);
			SetUpdateActive(true);
			m_LandDetect->SetUpdateActive(true);
		}

		bool GetUsed() {
			return m_used;
		}
		void SetUsed(bool used) {
			m_used = used;
		}

		//アーマーの状況を0.0f～1.0fで返す
		float GetArmorePercentage() {
			if (m_armor > 0) {
				return m_armor / m_armorMax;
			}
			else {
				return m_armorRecover / m_armorRecoverTime;
			}
		}

		//プレイヤーとの距離(Vec3)
		Vec3 GetPlayerDistInVec3();

		//プレイヤーとの距離を測る
		float GetPlayerDist();

		//プレイヤーとの角度の差を測る
		float GetPlayerSubDirection();

		//回転
		void RotateToPlayer(const float multiply, const float threshold);
		void RotateToPlayer(const float multiply);


	};

	/// <summary>
	/// ステージ1のボス
	/// </summary>
	class BossFirst : public EnemyBase {
		void OnDamaged() override;
		void RegisterAnim();

		//ビーム用
		bool m_isRecoveredFromArmorBreak = false;
		float m_prevArmor = 0.0f;
	public:
		BossFirst(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, bool used = false) :
			EnemyBase(stagePtr, pos, rot, scale, used)
		{

		}
		~BossFirst() {}

		//アーマーブレイクから回復したときtrueになる
		bool IsRecoveredFromArmorBreak() {
			bool ret = m_isRecoveredFromArmorBreak;
			m_isRecoveredFromArmorBreak = false;
			return ret;
		}

		/// <summary>
		/// HPの量を変える(ステージ毎の差異を出すため)
		/// </summary>
		/// <param name="max">HP値</param>
		void ChangeHPMax(int max) {
			m_HPMax = max;
			m_HPCurrent = max;
		}
		/// <summary>
		/// アーマー量を変える(ステージ毎の差異を出すため)
		/// </summary>
		/// <param name="max">アーマー耐久値</param>
		void ChangeArmorMax(float max) {
			m_armorMax = max;
			m_armor = max;
		}
		void ChangeArmorMax(float max, float recovertime) {
			m_armorMax = max;
			m_armor = max;
			m_armorRecoverTime = recovertime;
		}

		void OnCreate() override;
		void OnUpdate() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
	};

	/// <summary>
	/// ボス1の衝撃波
	/// </summary>
	class BossFirstShockwave : public ProjectileBase {
	protected:
		
		//独自当たり判定
		shared_ptr<CollisionCapsule> m_innerCollision;

		//現在の半径
		float m_radius = 0;
		//広がる速度
		const float m_radiateSpeed = 72.0f;
		//透過が始まる半径
		const float m_radiusStartFade = 45.0f;
		//半径の最大
		const float m_radiusMax = 180.0f;
		//外円と内円の差
		const float m_widthCircle = .5f;

		const float m_height = 4.0f;

		int m_isPlayerInsideCnt = 0;
		const int m_isPlayerInsideCntMax = 1;

		//見た目関係
		shared_ptr<BcPCTStaticDraw> m_ptrDraw;
		vector<uint16_t> m_indices;
		vector<VertexPositionColorTexture> m_vertices;

		Vec2 m_loop = Vec2(1.0f, 1.0f);
		int m_numOfVertices = 12;
		float m_meshHeight = 1.5f;
		float m_topRadiusPlus = 1.2f;
		float m_btmRadiusPlus = 1.0f;
		Col4 m_topColor = Col4(1, 1, 1, 1);
		Col4 m_btmColor = Col4(1, 1, 1, 1);
		wstring m_texKey = L"Tex_Shockwave";

		void HitInfoInit() override;

		void DrawInit();
	public:
		BossFirstShockwave(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, shared_ptr<Actor> originObj) :
			ProjectileBase(stagePtr, pos, rot, scale, originObj)
		{

		}
		~BossFirstShockwave() {}

		void OnCreate() override;
		void OnUpdate() override;

		void OnCollisionExcute(shared_ptr<GameObject>& Other) override;
	};

	/// <summary>
	/// ボス1のビーム判定
	/// </summary>
	class BossFirstBeam : public ProjectileBase {
	protected:
		float m_hitBeamVel = 12.0f;
		bool m_isFinalBlow = false;

		void HitInfoInit() override;
	public:
		BossFirstBeam(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, shared_ptr<Actor> originObj, float hitVel, bool final) :
			ProjectileBase(stagePtr, pos, rot, scale, originObj),
			m_hitBeamVel(hitVel),
			m_isFinalBlow(final)
		{

		}
		~BossFirstBeam() {}

		void OnCreate() override;

	};

	/// <summary>
	/// ボス1のエネルギー弾
	/// </summary>
	class BossFirstSphere : public Actor {
	protected:
		float m_time = 0;
		Quat m_face;

		const float m_firstMoveSpeed = 120.0f;
		float m_firstMoveTime = .8f;
		const float m_speedDown = .65f;

		bool m_towardPlayer = false;
		Vec3 m_secondMoveAngle;
		const float m_secondMoveSpeed = 300.0f;

		bool m_disappear = false;
		float m_disappearTime = 0;
		const float m_disappearTimeMax = 1.0f;

		weak_ptr<Player> m_player;

		//エフェクト
		Effekseer::Handle m_effect;

		void CreateChildObjects() override;
	public:
		BossFirstSphere(const shared_ptr<Stage>& stagePtr, Vec3 pos, Quat rot, Vec3 scale, float towardPlayerTime) :
			Actor(stagePtr, pos, Vec3(0), scale),
			m_face(rot),
			m_firstMoveTime(towardPlayerTime)
		{

		}
		~BossFirstSphere() {}

		void OnCreate() override;
		void OnUpdate() override;

		void CollidedWithTerrain();
	};

	/// <summary>
	/// エネルギー弾専用の当たり判定オブジェクト
	/// </summary>
	class BossFirstSphereCollision : public AttackCollision {
		shared_ptr<BossFirstSphere> m_obj;

	public:
		BossFirstSphereCollision(const shared_ptr<Stage>& stagePtr, const shared_ptr<BossFirstSphere>& obj) :
			AttackCollision(stagePtr),
			m_obj(obj)
		{

		}
		~BossFirstSphereCollision() {

		}
		void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
	};
}
//end basecross

