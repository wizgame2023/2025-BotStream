/*!
@file Actor.cpp
@brief PlayerやEnemyなどのキャラクター用のクラス
*/

#include "stdafx.h"
#include "Project.h"
#include "Actor.h"

namespace basecross {
	Actor::Actor(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale) :
		ObjectMove(stagePtr),
		m_pos(pos),
		m_rot(rot),
		m_scale(scale)
	{

	}
	Actor::~Actor()
	{

	}

	void Actor::OnCreate() {
		auto stage = GetStage();

		//Actorグループに登録する
		auto group = stage->GetSharedObjectGroup(L"Actor");
		if (group) {
			group->IntoGroup(GetThis<GameObject>());
		}

		//描画コンポーネントの追加
		AddComponent<PNTBoneModelDraw>();

		CreateChildObjects();

		//オーディオマネージャーの取得
		m_SEManager = App::GetApp()->GetXAudio2Manager();
	}

	void Actor::OnUpdate() {
		//もしポーズフラグがオンであればアップデート処理は出来なくなる
		if (m_poseFlag)
		{
			return;
		}
		_delta = App::GetApp()->GetElapsedTime();

		//着地判定(無効化時間中ならそれを減算する)
		if (m_LandDetect) {
			OnLanding();
		}

		//物理的な処理
		if (m_doPhysics) {
			if (m_isLand) {
				Friction();
			}
			else {
				Gravity();
			}
		}
	}

	//最高速度
	void Actor::SpeedLimit(float multiply) {
		float limit = m_speedMax * multiply;
		auto angle = m_velocity;
		angle.y = 0;
		if (angle.length() > 0) {
			angle.normalize();
			if (angle.x > 0) {
				if (m_velocity.x > angle.x * limit) m_velocity.x = angle.x * limit;
			}
			else {
				if (m_velocity.x < angle.x * limit) m_velocity.x = angle.x * limit;
			}
			if (angle.z > 0) {
				if (m_velocity.z > angle.z * limit) m_velocity.z = angle.z * limit;
			}
			else {
				if (m_velocity.z < angle.z * limit) m_velocity.z = angle.z * limit;
			}
		}
		//落下の終端速度
		if (m_velocity.y < m_fallTerminal) m_velocity.y = m_fallTerminal;
	}

	//摩擦(地上のみ)
	void Actor::Friction() {

		//静摩擦
		if (m_accel == Vec3(0)) {
			m_velocity.x -= m_velocity.x * m_friction * (1000.0f / 60.0f) * _delta;
			m_velocity.z -= m_velocity.z * m_friction * (1000.0f / 60.0f) * _delta;
			if (m_velocity.length() < m_frictionThreshold) m_velocity.x = 0;
			return;
		}
		//動摩擦
		if (m_accel != Vec3(0)) {
			m_velocity.x -= m_velocity.x * m_frictionDynamic * (1000.0f / 60.0f) * _delta;
			m_velocity.z -= m_velocity.z * m_frictionDynamic * (1000.0f / 60.0f) * _delta;

			//加速度リセット
			m_accel = Vec3(0);
		}
	}

	//重力
	void Actor::Gravity() {
		m_velocity.y += m_gravity * _delta;
	}

	//OnCollisionEnterに置く
	void Actor::DetectBeingAttacked(shared_ptr<GameObject>& other) {
		auto atk = dynamic_pointer_cast<AttackCollision>(other);
		if (!atk) return;

		m_hitDirection = other->GetComponent<Transform>()->GetWorldPosition();
		m_hitDirection -= GetComponent<Transform>()->GetWorldPosition();

		bool isAttacked = false;
		HitInfo info = atk->GetHitInfo();

		isAttacked = isAttacked || (FindTag(L"Enemy") && info.Type == AttackType::Player);
		isAttacked = isAttacked || (FindTag(L"Player") && info.Type == AttackType::Enemy);

		//攻撃を受けたら
		if (isAttacked) {
			atk->SetMoveContact(true);
			Vec3 v = GetForward();
			float fwd = atan2(v.z, v.x);
			EfkPlaying(info.HitEffect, fwd, Vec3(0, 1, 0),Vec3(1), Vec3(0));
			PlaySnd(info.HitSound, 1.0f, 0);

			//攻撃判定から攻撃のデータを取得
			m_getHitInfo = info;

			if (info.HitOnce == true) {
				//攻撃判定を消す
				atk->ActivateCollision(0);
			}

			//被弾処理へ
			OnDamaged();
		}
	}

	//これはOnUpdateに置く
	void Actor::OnLanding() {
		if (m_landDetectDisableTime > 0) {
			m_landDetectDisableTime -= _delta;
		}
		else {
			if (m_LandDetect->GetLand() != m_isLand) {
				//���n��������
				if (!m_isLand)
				{
					m_velocity.y = 0;
					EfkPlaying(L"Landing", GetAngle(), Vec3(0, 1, 0));

				}

				m_isLand = !m_isLand;
			}
		}
	}


	//向いている方向のゲッター
	float Actor::GetAngle()
	{
		return -m_angle;
	}

	//向いている方向のセッター
	void Actor::SetAngle(float angle)
	{
		m_angle = angle;
	}

	//エフェクトを出す処理
	Effekseer::Handle Actor::AddEffect(int addEffect)
	{
		Handle ret = -1;

		Vec3 fwd = GetForward();
		float angle = -atan2(fwd.z, fwd.x) + XM_PIDIV2;
		switch (addEffect)
		{
		case PlayerEffect_Attack1:
			ret = EfkPlaying(L"Sword", GetAngle() + XM_PI, Vec3(0, 1, 0));
			ret = EfkPlaying(L"Slash01Efk", GetAngle() + XM_PI, Vec3(0, 1, 0));
			break;
		case PlayerEffect_Attack2:
			ret = EfkPlaying(L"Sword", GetAngle() + XM_PI, Vec3(0, 1, 0), Col4(0.22f, 1.0f, 0.48f, 1.0f));
			ret = EfkPlaying(L"Slash02Efk", GetAngle() + XM_PI, Vec3(0, 1, 0), Col4(0.22f, 1.0f, 0.48f, 1.0f));
			break;
		case PlayerEffect_Attack3:
			ret = EfkPlaying(L"Sword", GetAngle() + XM_PI, Vec3(0, 1, 0), Col4(1.0f, 0.94f, 0.45f, 1.0f));
			ret = EfkPlaying(L"Slash03Efk", GetAngle() + XM_PI, Vec3(0, 1, 0), Col4(1.0f, 0.94f, 0.45f, 1.0f));
			break;
		case PlayerEffect_AttackEx:
			ret = EfkPlaying(L"SpinAttack", GetAngle() + XM_PI, Vec3(0, 1, 0), Col4(0.22f, 1.0f, 0.48f, 1.0f), Vec3(0.0f, 2.0f, 0.0f));
			ret = EfkPlaying(L"Slash04Efk", GetAngle() + XM_PI, Vec3(0, 1, 0), Col4(0.22f, 1.0f, 0.48f, 1.0f), Vec3(0.0f, 2.0f, 0.0f));
			break;
		case PlayerEffect_Dodge:
			ret = EfkPlaying(L"Dodge", GetAngle() + XM_PI, Vec3(0, 1, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec3(0.0f, 2.0f, 0.0f));
			break;
		case PlayerEffect_Dash:
			ret = EfkPlaying(L"Dash", GetAngle() + XM_PIDIV2, Vec3(0, 1, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f));
			break;
		case PlayerEffect_DashRipple:
			ret = EfkPlaying(L"Landing", GetAngle(), Vec3(0, 1, 0));
			break;
		case PlayerEffect_Beam:
			ret = EfkPlaying(L"Laser", GetAngle() + XM_PIDIV2, Vec3(0, 1, 0));
			break;
		case EnemyEffect_ArmorBreak:
			ret = EfkPlaying(L"ArmorBreak", GetAngle() + XM_PIDIV2, Vec3(0, 1, 0));
			break;
		case EnemyEffect_Beam:
			ret = EfkPlaying(L"Beam", angle, Vec3(0, 1, 0));
			break;
		case EnemyEffect_Sphere:
			ret = EfkPlaying(L"EnergySphere", angle, Vec3(0, 1, 0));
			break;
		case EnemyEffect_SphereEnd:
			ret = EfkPlaying(L"EnergySphereEnd", angle, Vec3(0, 1, 0));
			break;
		case EnemyEffect_Wave:
			ret = EfkPlaying(L"EnemyWave", angle, Vec3(0, 1, 0), Vec3(1), Vec3(0, -2, 0));
			break;
		default:
			break;
		}
		return ret;
	}

	// エフェクトのプレイ(大きさを変えることが出来る)
	Effekseer::Handle Actor::EfkPlaying(wstring EfkKey, float rad, Vec3 rotate, Vec3 scale,Vec3 pushPos)
	{
		rotate.normalize();
		auto trans = GetComponent<Transform>();
		auto plPos = trans->GetPosition()+ pushPos;
		auto plRot = trans->GetRotation();
		auto efkHandler = EffectManager::Instance().PlayEffect(EfkKey, plPos);
		//EffectManager::Instance().SetRotate(efkHandler,XMConvertToRadians(45.0f),-plRot.y,0.0f);
		EffectManager::Instance().SetRotation(efkHandler, rotate, rad);
		EffectManager::Instance().SetScale(efkHandler, Vec3(scale.x, scale.y, scale.z));
		return efkHandler;
	}

	// エフェクトのプレイ(色が変えることができる)
	Effekseer::Handle Actor::EfkPlaying(wstring EfkKey, float rad, Vec3 rotate, Col4 changeColor, Vec3 pushPos)
	{
		rotate.normalize();
		auto trans = GetComponent<Transform>();
		auto plPos = trans->GetPosition() + pushPos;

		auto efkHandler = EffectManager::Instance().PlayEffect(EfkKey, plPos);
		EffectManager::Instance().SetAllColor(efkHandler, changeColor);//エフェクトの色を変える
		EffectManager::Instance().SetRotation(efkHandler, Vec3(rotate.x, rotate.y, rotate.z), rad);
		return efkHandler;
	}

	//// エフェクトのプレイ(大きさを変えることが出来る)
	//// ここに新しいEfkPlaying作ろうと思ったけど、なんか変なエラー出たし、一番上のやつを改造しました。
	//void Actor::EfkPlaying(wstring EfkKey, float rad, Vec3 rotate, Vec3 scale, Vec3 pushPos)
	//{
	//	rotate.normalize();
	//	auto trans = GetComponent<Transform>();
	//	auto plPos = trans->GetPosition() + pushPos;

	//	auto efkHandler = EffectManager::Instance().PlayEffect(EfkKey, plPos);
	//	EffectManager::Instance().SetRotation(efkHandler, Vec3(rotate.x, rotate.y, rotate.z), rad);
	//	EffectManager::Instance().SetScale(efkHandler, Vec3(scale.x, scale.y, scale.z));
	//}

	//ポーズのフラグをオンオフする
	void Actor::SetPose(bool onOff)
	{
		m_poseFlag = onOff;
	}
	
	/// <summary>
	/// 飛び道具の親クラス
	/// </summary>

	//球のクラス
	void ProjectileBase::OnCreate()
	{
		Actor::OnCreate();

		//Transform設定
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

		//ドローメッシュの設定
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDraw->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 1.0f));
		ptrDraw->SetOwnShadowActive(false);//影は消す
		ptrDraw->SetDrawActive(true);
		ptrDraw->SetEmissive(Col4(1.0f, 1.0f, 1.0f, 1.0f)); // 自己発光カラー（ライティングによる陰影を消す効果がある）
		ptrDraw->SetOwnShadowActive(true); // 影の映り込みを反映させる

		//原点オブジェクトが消えていたら自分も消える
		auto originLock = m_originObj.lock();
		if (!originLock)
		{
			GetStage()->RemoveGameObject<ProjectileBase>(GetThis<ProjectileBase>());
			return;
		}
		auto cameraManager = GetStage()->GetSharedGameObject<CameraManager>(L"CameraManager");

		if (originLock->FindTag(L"Player"))
		{
			//Y軸のカメラの角度を受け取る
			m_originAngle = -(cameraManager->GetAngle(L"Y")) - XM_PI;
		}
		else if (originLock->FindTag(L"Enemy"))
		{
			auto playerAngleVec = originLock->GetComponent<Transform>()->GetForward();
			m_originAngle = atan2f(playerAngleVec.z, -playerAngleVec.x);
			m_originAngle -= XM_PIDIV2;
		}

		HitInfoInit();
	}

	void ProjectileBase::OnUpdate()
	{
		//もしポーズフラグがオンであればアップデート処理は出来なくなる
		if (m_poseFlag)
		{
			return;
		}

		Actor::OnUpdate();

		//移動距離を計算する
		Vec3 moveVec;

		//原点オブジェクトを受け取る
		auto originLock = m_originObj.lock();

		moveVec.x = m_speed * cos(m_originAngle) * _delta;
		moveVec.z = m_speed * sin(-m_originAngle) * _delta;

		//プレイヤーの位置を取得して移動する
		SetPosition(GetPosition() + moveVec);

		m_canMoveDistance -= m_speed * _delta;
		//一定時間移動したら消える
		if (m_canMoveDistance <= 0)
		{
			GetStage()->RemoveGameObject<GameObject>(GetThis<GameObject>());
			GetStage()->RemoveGameObject<LandDetect>(m_LandDetect);
			GetStage()->RemoveGameObject<AttackCollision>(m_AttackCol);
		}
	}

}
//end basecross
