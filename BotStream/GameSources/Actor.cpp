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

		//着地判定の生成
		m_LandDetect = stage->AddGameObject<LandDetect>();
		m_LandDetect->GetComponent<Transform>()->SetParent(dynamic_pointer_cast<GameObject>(GetThis<Actor>()));

		//攻撃判定の生成
		m_AttackCol = GetStage()->AddGameObject<AttackCollision>();
		m_AttackCol->GetComponent<Transform>()->SetParent(dynamic_pointer_cast<GameObject>(GetThis<Actor>()));


	}

	void Actor::OnUpdate() {
		//もしポーズフラグがオンであればアップデート処理は出来なくなる
		if (m_poseFlag)
		{
			return;
		}
		_delta = App::GetApp()->GetElapsedTime();
	}

	//着地判定(無効化時間中ならそれを減算する)
	OnLanding();

	//物理的な処理
	if (m_doPhysics) {
		if (!m_isLand) {
			Gravity();
		}
		else {
			Friction();
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
			//攻撃判定から攻撃のデータを取得
			m_GetHitInfo = info;

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
	void Actor::AddEffect(int addEffect)
	{
		switch (addEffect)
		{
		case PlayerEffect_Attack1:
			EfkPlaying(L"Sword", GetAngle() + XM_PI, Vec3(0, 1, 0));
			break;
		case PlayerEffect_Attack2:
			EfkPlaying(L"Sword", GetAngle() + XM_PI, Vec3(0, 1, 0), Col4(0.22f, 1.0f, 0.48f, 1.0f));
			break;
		case PlayerEffect_Attack3:
			EfkPlaying(L"Sword", GetAngle() + XM_PI, Vec3(0, 1, 0), Col4(1.0f, 0.94f, 0.45f, 1.0f));
			break;
		case PlayerEffect_AttackEx:
			EfkPlaying(L"SpinAttack", GetAngle() + XM_PI, Vec3(0, 1, 0), Col4(0.22f, 1.0f, 0.48f, 1.0f),Vec3(0.0f,2.0f,0.0f));
			//EfkPlaying(L"Attack", GetAngle(), Vec3(0, 1, 0));
			break;
		case PlayerEffect_Beam:
			EfkPlaying(L"Laser", GetAngle() + XM_PIDIV2, Vec3(0, 1, 0));
			break;
		case EnemyEffect_ArmorBreak:
			EfkPlaying(L"ArmorBreak", GetAngle() + XM_PIDIV2, Vec3(0, 1, 0));
			break;
		default:
			break;
		}
	}

	// エフェクトのプレイ
	void Actor::EfkPlaying(wstring EfkKey, float rad, Vec3 rotate,Vec3 pushPos)
	{
		rotate.normalize();
		auto trans = GetComponent<Transform>();
		auto plPos = trans->GetPosition()+ pushPos;

		auto efkHandler = EffectManager::Instance().PlayEffect(EfkKey, plPos);
		EffectManager::Instance().SetRotation(efkHandler, Vec3(rotate.x, rotate.y, rotate.z), rad);
	}

	void Actor::EfkPlaying(wstring EfkKey, float rad, Vec3 rotate, Col4 changeColor, Vec3 pushPos)
	{
		rotate.normalize();
		auto trans = GetComponent<Transform>();
		auto plPos = trans->GetPosition() + pushPos;

		auto efkHandler = EffectManager::Instance().PlayEffect(EfkKey, plPos);
		EffectManager::Instance().SetAllColor(efkHandler, changeColor);//エフェクトの色を変える
		EffectManager::Instance().SetRotation(efkHandler, Vec3(rotate.x, rotate.y, rotate.z), rad);
	}

	//ポーズのフラグをオンオフする
	void Actor::PoseSwitch(bool onOff)
	{
		m_poseFlag = onOff;
	}
}
//end basecross
