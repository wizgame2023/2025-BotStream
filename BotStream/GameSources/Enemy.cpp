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
		const float playerDir = GetPlayerSubDirection();
		float rotate = m_rotateSpeed * multiply * _delta * (playerDir < 0 ? -1 : 1);

		if (abs(playerDir) > threshold) {
			Quat q = GetQuaternion();

			if (abs(playerDir) < abs(rotate)) {
				rotate = playerDir;
			}

			q = RotateQuat(q, Vec3(0, 1, 0), rotate);
			SetQuaternion(q);
		}
	}

	void EnemyBase::RotateToPlayer(const float multiply) {
		RotateToPlayer(multiply, m_rotateThreshold);
	}

	//--------------------------------------------------------------------------

	void BossFirst::RegisterAnim() {
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

		//叩きつけ予備動作
		ptrDraw->AddAnimation(L"AttackSlamStart", 1010, 65, false, 60.0f);
		//叩きつけ1発目
		ptrDraw->AddAnimation(L"AttackSlam1", 1075, 12, false, 60.0f);
		//叩きつけ2発目
		ptrDraw->AddAnimation(L"AttackSlam2", 1088, 22, false, 60.0f);
		//叩きつけ終了
		ptrDraw->AddAnimation(L"AttackSlamEnd", 1110, 90, false, 60.0f);

		//飛び道具予備動作
		ptrDraw->AddAnimation(L"AttackSphereStart", 1210, 15, false, 60.0f);
		//飛び道具1発目
		ptrDraw->AddAnimation(L"AttackSphere1", 1225, 25, false, 30.0f);
		//飛び道具2発目
		ptrDraw->AddAnimation(L"AttackSphere2", 1275, 55, false, 60.0f);
		//飛び道具終了
		ptrDraw->AddAnimation(L"AttackSphereEnd", 1330, 45, false, 60.0f);

		//極太ビーム用意
		ptrDraw->AddAnimation(L"AttackSPBeam1", 1500, 75, false, 60.0f);
		//極太ビーム展開
		ptrDraw->AddAnimation(L"AttackSPBeam2", 1601, 49, false, 60.0f);
		//極太ビーム終了
		ptrDraw->AddAnimation(L"AttackSPBeam3", 1651, 69, false, 60.0f);
	}

	void BossFirst::OnCreate() {
		Actor::OnCreate();
		//Transform設定
		m_trans = GetComponent<Transform>();
		SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

		m_HPMax = 450;
		m_attack = 10;
		m_defense = 1;
		m_HPCurrent = m_HPMax;

		m_armorMax = 150.0f;
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

		//アーマーブレイク回復を監視
		if (m_armor > 0 && m_prevArmor <= 0) {
			m_isRecoveredFromArmorBreak = true;
		}
		m_prevArmor = m_armor;

		//アニメーション再生
		GetComponent<PNTBoneModelDraw>()->UpdateAnimation(_delta);

		UpdatePosition();
	}

	void BossFirst::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		//無敵
		if (m_HPCurrent <= 0) return;
		if (GetBoneModelDraw()->GetCurrentAnimation() == L"AttackSpin2") return;

		DetectBeingAttacked(Other);
	}

	void BossFirst::OnDamaged() {
		int armorDamage = m_getHitInfo.Damage;
		bool isArmorBreak = m_armor > 0;

		//アーマーへのダメージ2倍
		if (GetBoneModelDraw()->GetCurrentAnimation() == L"Bonus") {
			armorDamage *= 2;
		}
		m_armor -= CalculateDamage(armorDamage);

		//非アーマー
		if (m_armor <= 0) {
			//ブレイク時の演出
			if (isArmorBreak) {
				AddEffect(EnemyEffect_ArmorBreak);
				App::GetApp()->GetXAudio2Manager()->Start(L"Beam", 0, 0.9f);

				m_state->ChangeState(L"Stun");
			}
			else {
				m_state->ChangeState(L"Hit");
			}

			m_HPCurrent -= CalculateDamage(m_getHitInfo.Damage);
		}
		//アーマー
		else {
			m_HPCurrent -= CalculateDamage(m_getHitInfo.Damage) / 5.0f;
			m_armorFlash = m_armorFlashMax;
		}

		//死亡
		if (GetHPCurrent() <= 0) {
			m_state->ChangeState(L"Dead");
		}
	}

	/// <summary>
	/// ボス1の衝撃波
	/// </summary>
	void BossFirstShockwave::OnCreate()
	{
		Actor::OnCreate();

		m_speed = 0;
		m_originAngle = 0;
		m_canMoveDistance = 0;

		//Transform設定
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);
		
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

		m_innerCollision = AddComponent<CollisionCapsule>();
		m_innerCollision->SetAfterCollision(AfterCollision::None);
		//m_innerCollision->SetDrawActive(true);
		//GetAttackPtr()->GetCollisionPtr()->SetDrawActive(true);

		HitInfoInit();
		DrawInit();

		m_doPhysics = false;
	}

	void BossFirstShockwave::OnUpdate()
	{
		//もしポーズフラグがオンであればアップデート処理は出来なくなる
		if (m_poseFlag)
		{
			return;
		}
		_delta = App::GetApp()->GetElapsedTime();

		if (m_radius >= m_radiusMax || m_AttackCol->GetMoveContact()) {
			RemoveSelf();
			return;
		}

		//拡大
		m_radius += m_radiateSpeed * _delta;
		SetScale(Vec3(m_radius - m_widthCircle, m_height, m_radius - m_widthCircle));
		m_AttackCol->SetScale(Vec3(m_radius, m_height, m_radius));
		
		if (m_radius >= m_radiusStartFade) {
			Col4 tmp = Col4(1, 1, 1, 1 - (m_radius - m_radiusStartFade) / (m_radiusMax - m_radiusStartFade));
			for (auto& e : m_vertices) {
				e.color = tmp;
			}
		}

		//プレイヤーが内側にいたら攻撃判定を消す
		float playerInside = m_isPlayerInsideCnt <= 0 ? 1 : 0;
		m_AttackCol->ActivateCollision(playerInside);
		//プレイヤーが外側にいるときの判定用
		if (m_isPlayerInsideCnt > 0) {
			m_isPlayerInsideCnt--;
		}

		m_ptrDraw->UpdateVertices(m_vertices);
	}

	//描画コンポーネントの情報
	void BossFirstShockwave::DrawInit() {
		m_ptrDraw = AddComponent<BcPCTStaticDraw>();

		m_vertices.clear();
		m_vertices.reserve((m_numOfVertices + 1) * 2);

		for (int i = 0; i <= m_numOfVertices; i++) {
			Vec3 vtxPos;
			VertexPositionColorTexture tmp;

			float rad = XMConvertToRadians(360.0f / m_numOfVertices * i);
			float u = m_loop.x * static_cast<float>(i) / static_cast<float>(m_numOfVertices);
			float v = m_loop.y;
			
			//↑
			const float scale = GetScale().x;
			float plus = (m_topRadiusPlus / scale);
			vtxPos = Vec3(cosf(rad) / 2 * plus, m_meshHeight, sinf(rad) / 2 * plus);
			tmp = VertexPositionColorTexture(vtxPos, m_topColor, Vec2(u, 0));
			m_vertices.push_back(tmp);

			//↓
			plus = (m_btmRadiusPlus / scale);
			vtxPos = Vec3(cosf(rad) / 2 * plus, 0, sinf(rad) / 2 * plus);
			tmp = VertexPositionColorTexture(vtxPos, m_btmColor, Vec2(u, v));
			m_vertices.push_back(tmp);
		}

		const vector<uint16_t> baseIndex = {
			2, 1, 0,
			3, 1, 2
		};

		m_indices.clear();
		m_indices.reserve(m_numOfVertices * baseIndex.size());
		//各頂点の上下に対してループ
		for (int i = 0; i < m_numOfVertices; i++) {
			for (auto e : baseIndex) {
				m_indices.push_back(e + (2 * i));
			}
		}

		m_ptrDraw->SetOriginalMeshUse(true);
		m_ptrDraw->CreateOriginalMesh(m_vertices, m_indices);
		m_ptrDraw->SetSamplerState(SamplerState::LinearWrap);
		m_ptrDraw->SetDepthStencilState(DepthStencilState::Read);
		m_ptrDraw->SetBlendState(BlendState::Additive);
		m_ptrDraw->SetTextureResource(m_texKey);
		SetAlphaActive(true);
	}

	//当たり判定の情報
	void BossFirstShockwave::HitInfoInit() {

		//攻撃判定の定義
		auto tmp = GetAttackPtr()->GetHitInfo();
		tmp.HitOnce = true;
		tmp.Type = AttackType::Enemy;
		tmp.Damage = 8;
		tmp.HitVel_Stand = Vec3(-10, 40, 0);
		tmp.HitVel_Air = Vec3(-8, 20, 0);
		tmp.HitTime_Stand = 1.5f;
		tmp.HitTime_Air = 1.0f;

		DefAttack(5, tmp);
		GetAttackPtr()->SetCollScale(.5f);
		GetAttackPtr()->SetCollHeight(m_height);
	}

	void BossFirstShockwave::OnCollisionExcute(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"Player")) {
			m_isPlayerInsideCnt = m_isPlayerInsideCntMax;
		}
	}

	/// <summary>
	/// ボス1のビーム判定
	/// </summary>
	void BossFirstBeam::OnCreate()
	{
		Actor::OnCreate();

		m_speed = 300.0f;
		m_originAngle = 0.0f;
		m_canMoveDistance = 100.0f;

		//Transform設定
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

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

	void BossFirstBeam::HitInfoInit() {
		float velX = m_isFinalBlow ? -50.0f : 0.0f;

		//攻撃判定の定義
		auto tmp = GetAttackPtr()->GetHitInfo();
		tmp.HitOnce = true;
		tmp.Type = AttackType::Enemy;
		tmp.Damage = 3;
		tmp.HitVel_Stand = Vec3(velX, m_hitBeamVel, 0);
		tmp.HitVel_Air = Vec3(velX, m_hitBeamVel, 0);
		tmp.HitTime_Stand = 1.2f;
		tmp.HitTime_Air = 1.2f;

		DefAttack(5.0f, tmp);
		GetAttackPtr()->SetCollScale(4.0f);
	}

	/// <summary>
	/// ボス1のエネルギー弾
	/// </summary>
	void BossFirstSphere::OnCreate()
	{
		Actor::OnCreate();

		//Transform設定
		SetPosition(m_pos);
		SetQuaternion(m_face);
		SetScale(m_scale);

		m_player = dynamic_pointer_cast<Player>(GetStage()->GetSharedObject(L"Player"));

		m_doPhysics = false;

		m_effect = AddEffect(EnemyEffect_Sphere);

		//仮
		//auto ptrDraw = AddComponent<PNTStaticDraw>();
		//ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		//ptrDraw->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 1.0f));
		//ptrDraw->SetOwnShadowActive(false);//影は消す
		//ptrDraw->SetDrawActive(true);
		//ptrDraw->SetEmissive(Col4(1.0f, 1.0f, 1.0f, 1.0f)); // 自己発光カラー（ライティングによる陰影を消す効果がある）
		//ptrDraw->SetOwnShadowActive(true); // 影の映り込みを反映させる

		//攻撃判定の定義
		auto tmp = GetAttackPtr()->GetHitInfo();
		tmp.HitOnce = true;
		tmp.Type = AttackType::Enemy;
		tmp.Damage = 3;
		tmp.HitVel_Stand = Vec3(-8, 5, 0);
		tmp.HitVel_Air = Vec3(-6, 3, 0);
		tmp.HitTime_Stand = .4f;
		tmp.HitTime_Air = .4f;

		DefAttack(100.0f, tmp);
		GetAttackPtr()->GetComponent<Transform>()->SetScale(Vec3(.5f));
		GetAttackPtr()->SetCollScale(8.0f);
		GetAttackPtr()->SetCollHeight(0.1f);

		SetVelocity(GetForward() * m_firstMoveSpeed);
	}

	void BossFirstSphere::OnUpdate() {
		//もしポーズフラグがオンであればアップデート処理は出来なくなる
		if (m_poseFlag)
		{
			return;
		}
		Actor::OnUpdate();

		//消滅する条件
		bool doDisappear = m_AttackCol->GetMoveContact() || m_disappear;
		doDisappear = doDisappear && m_disappearTime >= m_disappearTimeMax;

		if (doDisappear) {
			EffectManager::Instance().StopEffect(m_effect);
			RemoveSelf();
		}

		//消滅するまでに少し時間を設ける
		if (m_disappear) {
			m_disappearTime += _delta;
			return;
		}

		m_time += _delta;

		//最初にfirstの設定で宙に浮く
		//次にプレイヤーの位置を取ってその方向に加速
		if (!m_towardPlayer) {
			SetVelocity(GetVelocity() - (GetVelocity() * m_speedDown * (1000.0f / 60.0f) * _delta));
			if (m_time >= m_firstMoveTime) {
				//プレイヤーの胸の高さ分。ここをどうするか少し考え
				Vec3 v = m_player.lock()->GetPosition() + Vec3(0, 4, 0) - GetPosition();
				m_secondMoveAngle = v.normalize();
				m_towardPlayer = true;
			}
		}
		else {
			AddVelocity(m_secondMoveAngle * m_secondMoveSpeed * _delta);
		}

		//エフェクトを本体に追従させる
		EffectManager::Instance().SetPosition(m_effect, GetPosition());

		UpdatePosition();
	}

	void BossFirstSphere::CollidedWithTerrain() {
		if (m_disappear) return;

		//エフェクト再生
		EffectManager::Instance().StopEffect(m_effect);
		m_effect = AddEffect(EnemyEffect_SphereEnd);

		m_disappear = true;
	}

	void BossFirstSphere::CreateChildObjects() {
		auto stage = GetStage();

		//着地判定の生成
		m_LandDetect = nullptr;

		//攻撃判定の生成
		m_AttackCol = stage->AddGameObject<BossFirstSphereCollision>(GetThis<BossFirstSphere>());
		m_AttackCol->GetComponent<Transform>()->SetParent(dynamic_pointer_cast<GameObject>(GetThis<Actor>()));
	}

	void BossFirstSphereCollision::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"Player") || Other->FindTag(L"Terrain")) {
			m_obj->CollidedWithTerrain();
		}
	}
}
//end basecross
