/*!
@file Enemy.cpp
@brief 敵の処理
*/

#include "stdafx.h"
#include "Project.h"
#include "Actor.h"
#include "Enemy.h"

namespace basecross {
	EnemyBase::EnemyBase(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale) :
		Actor(stagePtr, pos, rot, scale),
		m_used(false)
	{

	}

	EnemyBase::EnemyBase(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, bool use) :
		Actor(stagePtr, pos, rot, scale),
		m_used(use)
	{

	}

	void EnemyBase::HitBackStandBehavior() {
		m_hitbacktime -= _delta;
		if (m_hitbacktime <= 0) {
			m_state->ChangeState(L"Stand");
		}
	}

	void EnemyBase::OnCreate() {
		Actor::OnCreate();
		//Transform設定
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(.5f, .5f, .5f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//ドローメッシュの設定
		auto ptrDraw = GetComponent<PNTBoneModelDraw>();
		ptrDraw->SetMeshResource(L"Boss1");
		ptrDraw->SetDiffuse(Col4(0));
		ptrDraw->SetSamplerState(SamplerState::LinearWrap);
		ptrDraw->SetMeshToTransformMatrix(spanMat);
		ptrDraw->SetTextureResource(L"Tx_Boss1");

		RegisterAnim();
		ChangeAnim(L"Idle");

		//コリジョン作成
		auto ptrColl = AddComponent<CollisionSphere>();//コリジョンスフィアの方が壁にぶつかる判定に違和感がない
		ptrColl->SetAfterCollision(AfterCollision::Auto);
		AddTag(L"Enemy");

		m_player = dynamic_pointer_cast<Player>(GetStage()->GetSharedObject(L"Player"));

		m_state = shared_ptr<EnemyStateMachine>(new EnemyStateMachine(GetThis<GameObject>()));
	}

	void EnemyBase::OnUpdate() {
		//もしポーズフラグがオンであればアップデート処理は出来なくなる
		if (m_poseFlag)
		{
			return;
		}
		if (GetDrawActive() != m_used) {
			SetDrawActive(m_used);
		}
		if (m_used == false) {
			m_LandDetect->SetDrawActive(false);
			m_LandDetect->SetUpdateActive(false);
			SetUpdateActive(m_used);
			return;
		}

		Actor::OnUpdate();

		//アーマー回復
		if (m_armorMax != 0 && m_armor <= 0) {
			m_armorRecover += _delta;
			if (m_armorRecoverTime <= m_armorRecover) {
				m_armor = m_armorMax;
				m_armorRecover = 0;
			}
		}

		if (m_armorFlash > 0) {
			GetBoneModelDraw()->SetDiffuse(m_armorFlashFX);
			m_armorFlash -= _delta;
			if (m_armorFlash <= 0) {
				GetBoneModelDraw()->SetDiffuse(Col4(0));
			}
		}

		//なんやかんや
		m_state->Update();

	}

	void EnemyBase::RegisterAnim() {
		auto ptrDraw = GetComponent<PNTBoneModelDraw>();
		//立
		ptrDraw->AddAnimation(L"Idle", 0, 25, true, 30.0f);
		//回転
		ptrDraw->AddAnimation(L"Rotate", 26, 154, true, 30.0f);
		//歩き
		ptrDraw->AddAnimation(L"Walk", 181, 169, true, 60.0f);
		//のけぞり
		ptrDraw->AddAnimation(L"HitBack", 488, 52, false, 60.0f);
		//ダウン
		ptrDraw->AddAnimation(L"KnockedDown", 351, 79, false, 60.0f);
		//ダウン復帰	
		ptrDraw->AddAnimation(L"WakeUp", 431, 56, false, 60.0f);
		//ボーナス行動
		ptrDraw->AddAnimation(L"Bonus", 541, 99, false, 90.0f);

		//近接1
		ptrDraw->AddAnimation(L"AttackClose1", 651, 67, false, 60.0f);
		//近接2
		ptrDraw->AddAnimation(L"AttackClose2", 719, 80, false, 60.0f);
		//回転発生
		ptrDraw->AddAnimation(L"AttackSpin1", 800, 49, false, 60.0f);
		//回転持続
		ptrDraw->AddAnimation(L"AttackSpin2", 850, 75, false, 30.0f);
		//回転硬直
		ptrDraw->AddAnimation(L"AttackSpin3", 926, 84, false, 60.0f);
	}

	//XZ平面におけるプレイヤーとの距離
	float EnemyBase::GetPlayerDist() {
		auto p = (m_player.lock());
		if (p == nullptr) return 0;

		return (p->GetPosition() - GetPosition()).length();
	}
	//単純にVec3で返す版
	Vec3 EnemyBase::GetPlayerDistInVec3() {
		auto p = (m_player.lock());
		if (p == nullptr) return Vec3(0);

		return (p->GetPosition() - GetPosition());
	}
	//XZ平面におけるプレイヤーの方向
	float EnemyBase::GetPlayerSubDirection() {
		auto p = (m_player.lock());
		if (p == nullptr) return 0;

		//プレイヤーの向きベクトルを取得
		auto fwd = GetComponent<Transform>()->GetForward();
		float selfAngle = atan2f(fwd.z, fwd.x);
		fwd = (p->GetPosition() - GetPosition());
		fwd.normalize();
		float playerAngle = atan2f(fwd.z, fwd.x);

		float ret = selfAngle - playerAngle - XM_PIDIV2;

		//180°より大きい角度が入っている場合
		if (ret > XM_PI) {
			ret = -XM_2PI + ret;
		}
		if (ret < -XM_PI) {
			ret = XM_2PI + ret;
		}
		return ret;
	}
	
	void EnemyBase::RotateToPlayer(const float multiply, const float threshold) {
		float playerDir = GetPlayerSubDirection();

		if (abs(playerDir) > threshold) {
			Quat q = GetQuaternion();
			q = RotateQuat(q, Vec3(0, 1, 0), m_rotateSpeed * multiply * _delta * (playerDir < 0 ? -1 : 1));
			SetQuaternion(q);
		}
	}

	void EnemyBase::RotateToPlayer(const float multiply) {
		RotateToPlayer(multiply, m_rotateThreshold);
	}

	//--------------------------------------------------------------------------

	void BossFirst::OnCreate() {
		Actor::OnCreate();
		//Transform設定
		m_trans = GetComponent<Transform>();
		SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

		m_HPMax = 300;
		m_attack = 10;
		m_defense = 1;
		m_HPCurrent = m_HPMax;

		m_armorMax = 100.0f;
		m_armor = m_armorMax;
		m_armorRecoverTime = 6.0f;

		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f),
			Vec3(0.0f, -3.25f, 0.0f)
		);

		//ドローメッシュの設定
		auto ptrDraw = GetComponent<PNTBoneModelDraw>();
		ptrDraw->SetMeshResource(L"Boss1");
		ptrDraw->SetDiffuse(Col4(0));
		ptrDraw->SetSamplerState(SamplerState::LinearWrap);
		ptrDraw->SetMeshToTransformMatrix(spanMat);
		ptrDraw->SetTextureResource(L"Tx_Boss1");

		RegisterAnim();
		ChangeAnim(L"Walk");

		//コリジョン作成
		auto ptrColl = AddComponent<CollisionSphere>();//コリジョンスフィアの方が壁にぶつかる判定に違和感がない
		ptrColl->SetAfterCollision(AfterCollision::Auto);

		ptrColl->SetMakedRadius(3);
		ptrColl->SetDrawActive(false);//debug

		m_LandDetect->SetBindPos(Vec3(0, -2.7f, 0));
		m_LandDetect->SetCollScale(1.0f);

		AddTag(L"Enemy");

		m_player = dynamic_pointer_cast<Player>(GetStage()->GetSharedObject(L"Player"));

		m_state = shared_ptr<BossFirstStateMachine>(new BossFirstStateMachine(GetThis<GameObject>()));
	}

	void BossFirst::OnUpdate() {
		//もしポーズフラグがオンであればアップデート処理は出来なくなる
		if (m_poseFlag)
		{
			return;
		}

		EnemyBase::OnUpdate();

		//アニメーション再生
		GetComponent<PNTBoneModelDraw>()->UpdateAnimation(_delta);

		GetComponent<Transform>()->SetPosition((m_velocity * _delta) + GetComponent<Transform>()->GetPosition());

		//////デバック用
		//wstringstream wss(L"");
		//auto scene = App::GetApp()->GetScene<Scene>();
		//auto quat = GetComponent<Transform>()->GetQuaternion();
		//wss /* << L"デバッグ用文字列 "*/
		//	<< L"\n Pos.x " << m_pos.x << " Pos.z " << m_pos.z
		//	<< L" Vel.x " << m_velocity.x << L"\ Vel.y " << m_velocity.y << L" Vel.z " << m_velocity.z
		//	<< endl << "onLand: " << m_isLand << " LandDetect: " << m_LandDetect->GetLand()
		//	<< L"\nQuat : (" << L"\n" << quat.x << L"\n" << quat.y << L"\n" << quat.z << L"\n" << quat.w
		//	<< L"\nAngle : " << GetPlayerSubDirection()
		//	<< L"\nHP : " << m_HPCurrent << " / " << m_armor << " / " << m_armorRecoverTime - m_armorRecover << endl;

		//scene->SetDebugString(wss.str());
	}

	void BossFirst::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		//無敵
		if (m_HPCurrent <= 0) return;
		if (GetBoneModelDraw()->GetCurrentAnimation() == L"AttackSpin2") return;

		DetectBeingAttacked(Other);
	}

	void BossFirst::OnDamaged() {
		int armorDamage = m_GetHitInfo.Damage;
		bool armorEffect = m_armor > 0;

		//アーマーへのダメージ2倍
		if (GetBoneModelDraw()->GetCurrentAnimation() == L"Bonus") {
			armorDamage *= 2;
		}
		m_armor -= CalculateDamage(armorDamage);

		if (m_armor <= 0) {
			if (armorEffect) {
				AddEffect(EnemyEffect_ArmorBreak);

				//SE
				App::GetApp()->GetXAudio2Manager()->Start(L"Beam", 0, 0.9f);
			}

			//非アーマー
			m_HPCurrent -= CalculateDamage(m_GetHitInfo.Damage);

			m_state->ChangeState(L"Hit");

		}
		else {
			//アーマー
			m_HPCurrent -= CalculateDamage(m_GetHitInfo.Damage) / 5.0f;
			m_armorFlash = m_armorFlashMax;
		}

		//死亡
		if (GetHPCurrent() <= 0) {
			m_state->ChangeState(L"Dead");
		}
	}
}
//end basecross
