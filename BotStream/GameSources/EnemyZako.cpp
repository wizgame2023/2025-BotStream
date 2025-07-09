/*!
@file EnemyZako.h
@brief 雑魚敵
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void EnemyZako::OnCreate()
	{
		Actor::OnCreate();

		//ステータス初期化
		m_HPMax = 130.0f;
		m_HPCurrent = m_HPMax;
		m_armorMax = 30.0f;
		m_armor = m_armorMax;
		m_armorRecoverTime = 10.0f;
		m_armorRecover = 0.0f;
		m_stunMax = 1;

		//Transform設定
		m_trans = GetComponent<Transform>();
		SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(1.0f / 5, 1.0f / 5, 1.0f / 5),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		//ドローメッシュの設定
		auto ptrDraw = GetComponent<PNTBoneModelDraw>();

		//攻撃タイプによって見た目が変わる
		if (m_AttackType == Zako_Long)
		{
			ptrDraw->SetMeshResource(L"Enemy_A");
		}
		if (m_AttackType == Zako_Melee)
		{
			ptrDraw->SetMeshResource(L"Enemy_C");
		}
		ptrDraw->SetDiffuse(Col4(1.0f));
		ptrDraw->SetSamplerState(SamplerState::LinearWrap);
		ptrDraw->SetMeshToTransformMatrix(spanMat);

		//アニメーション追加(攻撃タイプによって追加アニメーションが変わる)
		ptrDraw->AddAnimation(L"Stand", 0, 1, 24.0f);
		ptrDraw->AddAnimation(L"Walk", 0, 224, 24.0f);
		ptrDraw->AddAnimation(L"Shot", 225, 136, false, 24.0f);
		ptrDraw->AddAnimation(L"Down", 362, 62, false, 24.0f);
		ptrDraw->AddAnimation(L"Hit", 350, 11, false, 24.0f);
		ptrDraw->AddAnimation(L"Stan", 320, 19, false, 24.0f);
		if (m_AttackType == Zako_Melee)
		{
			ptrDraw->AddAnimation(L"Melee_Jamp", 625, 74, false, 24.0f);
			ptrDraw->AddAnimation(L"Melee_Scratch", 700, 22, false, 24.0f);
		}

		//影をつける（シャドウマップを描画する）
		auto shadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		shadowPtr->SetMeshResource(L"Enemy_A");
		shadowPtr->SetMeshToTransformMatrix(spanMat);


		//コリジョン作成
		auto ptrColl = AddComponent<CollisionSphere>();//コリジョンスフィアの方が壁にぶつかる判定に違和感がない
		ptrColl->SetAfterCollision(AfterCollision::Auto);
		ptrColl->SetDrawActive(false);//デバック用

		AddTag(L"Enemy");
		AddTag(L"EnemyZako");

		m_player = GetStage()->GetSharedGameObject<Player>(L"Player");

		//接地判定の設定
		m_LandDetect->SetBindPos(Vec3(0, -2.0f, 0));
		m_LandDetect->GetComponent<CollisionSphere>()->SetMakedRadius(1.0f);
		m_LandDetect->GetComponent<Transform>()->SetScale(Vec3(1.0f, 1.0f, 1.0f));
		//m_LandDetect->SetCollScale(3.0f);

		//ステートマシン生成
		m_state = shared_ptr<EnemyZakoStateMachine>(new EnemyZakoStateMachine(GetThis<GameObject>()));

		//頭上にHPバーを表示させる		
		m_HPBer = GetStage()->AddGameObject<BillBoardGauge>(GetThis<GameObject>(), L"ZakoHPMater", 3, 5.0f, Vec3(2.0f, 0.5f, 5.0f));
		m_HPFrame = GetStage()->AddGameObject<BillBoard>(GetThis<GameObject>(), L"ZakoGaugeFrame", 3, 5.0f, Vec3(2.0f, 0.5f, 5.0f));
		m_HPBer->SetPercent(1.0f);

		SetAlphaActive(true);
	}

	void EnemyZako::OnUpdate()
	{
		//もしポーズフラグがオンであればアップデート処理は出来なくなる
		if (m_PauseFlag)
		{
			return;
		}
		//もし、使わない状態から使う状態に変更された時メンバ変数の初期化をする	
		if (!m_beforUsed)
		{
			if (m_used)
			{
				// 初期化
				Initialize();	
				GetComponent<PNTBoneModelDraw>()->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 1.0f));
				GetComponent<Shadowmap>()->SetDrawActive(true);
			}
		}
		if (m_beforUsed)
		{
			if (!m_used)
			{
				auto stage = GetStage();
				auto pos = GetComponent<Transform>()->GetPosition();
				stage->GetSharedGameObject<PartsManager>(L"PartsManager")->PartsDrop(pos);
			}
		}
		//現在の使用状況と見比べて変わっていないか見る
		m_beforUsed = m_used;

		////アーマー回復
		//if (m_armorMax != 0 && m_armor <= 0) 
		//{
		//	m_armorRecoverCountTime += _delta;
		//	if (m_armorRecoverTime <= m_armorRecoverCountTime) 
		//	{
		//		m_armor = m_armorMax;
		//		m_armorRecoverCountTime = 0;
		//	}
		//}

		EnemyBase::OnUpdate();

		//着地判定(無効化時間中ならそれを減算する)
		OnLanding();
		//HPバーの処理
		UpdateHPBer();
		//攻撃のクールタイム
		TimeOfAttackCool();

		//HPがゼロになったら消えるための準備をする
		if (m_HPCurrent <= 0)
		{
			RemoveTag(L"LockOnCan");
			RemoveTag(L"LockOnTarget");

			//m_used = false;
		}

		//アニメーション更新
		GetComponent<PNTBoneModelDraw>()->UpdateAnimation(m_addTimeAnimation);
		//位置更新
		//SpeedLimit(3.0f); //スピードリミット
		UpdatePosition();
	}

	//初期化処理
	void EnemyZako::Initialize()
	{
		m_HPCurrent = m_HPMax;
		m_attackFlag = false;
		m_timeCountOfAttackCool = 3.0f;
		//初期ステートに戻す
		ChangeState(L"Stand");
	}

	void EnemyZako::CreateDamageBill(shared_ptr<GameObject> actorPtr, int damage, float pushY, float scale, float displayTime)
	{
		string dmgStr = to_string(damage);                    // 受け取ったダメージの文字数(30であれば「3」と「0」で2文字)
		int len = (int)dmgStr.size();                        // ダメージの桁数
		float charWidth = scale;                             // 各スプライト幅
		float totalW = charWidth * len;                        // 全体幅
		float startX = -totalW / 2.0f + charWidth / 2.0f;   // 左端基準
		for (int i = 0; i < len; i++)
		{
			float offsetX = startX + i * charWidth;

			auto damageBill = GetStage()->AddGameObject<DamageBill>(
				actorPtr,                    // 各ビルボードは敵にくっつける
				L"DamageNumbers",                  // テクスチャ名
				3,                           // レイヤー
				pushY,                       // Y軸オフセット
				Vec3(scale),				 // サイズ
				Col4(1, 1, 1, 1),            // 色 (黄色など)
				offsetX,                     // 左右オフセット
				displayTime					 // 表示時間
			);

			// ダメージの文字を設定
			damageBill->SetBillUV(
				Vec2((dmgStr[i] - '0') * 0.1f, 0.0f), // UVの左上
				Vec2((dmgStr[i] - '0' + 1) * 0.1f, 1.0f) // UVの右下
			);
		}
	}

	//HPバーの処理
	void EnemyZako::UpdateHPBer()
	{
		//ビルボードの処理 自分が使用されているかどうかでビルボードが出るかでないか決める
		if (!m_used)
		{
			m_HPFrame->SetScale(Vec3(0.0f));
			m_HPBer->SetScale(Vec3(0.0f));
		}
		if (m_used)
		{
			m_HPFrame->SetScale(Vec3(2.0f, 0.5f, 5.0f));
			m_HPBer->SetScale(Vec3(2.0f, 0.5f, 5.0f));

			// 撮影用
			//m_HPFrame->SetScale(Vec3(0.0f));
			//m_HPBer->SetScale(Vec3(0.0f));

			//HPの割合によってゲージが減る
			float HPPercent = (float)m_HPCurrent / (float)m_HPMax;
			m_HPBer->SetPercent(HPPercent);
		}
	}

	//攻撃のクールタイム
	void EnemyZako::TimeOfAttackCool()
	{
		//攻撃のクールタイム
		if (!m_attackFlag)
		{
			m_timeCountOfAttackCool += _delta;
			//クールタイム過ぎたら攻撃できるようになる
			if (m_timeCountOfAttackCool >= m_timeOfAttackCool)
			{
				m_timeCountOfAttackCool = 0.0f;//リセット
				m_attackFlag = true;
			}
		}
	}

	//ダメージを受けた際の処理
	void EnemyZako::HitBackStandBehavior()
	{
		//ダメージを受けた後のHPによってステートの遷移を変える
		m_hitbacktime -= _delta;

		//攻撃を受けたときの処理
		if (m_hitbacktime <= 0) 
		{
			ChangeState(L"Stand");
		}
	}

	//コリジョン判定
	void EnemyZako::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		DetectBeingAttacked(Other);
	}

	//削除時処理
	void EnemyZako::OnDestroy()
	{
		Actor::OnDestroy();

		auto stage = GetStage();
		stage->GetSharedGameObject<PartsManager>(L"PartsManager")->PartsDrop(m_pos);
	}

	//ダメージを受けた際の処理
	void EnemyZako::OnDamaged()
	{
		float damage = m_getHitInfo.Damage;
		float pushY = 2.5f;
		float scale = 0.7f;
		float displayTime = 0.5f;

		bool isArmorBreak = m_armor > 0;

		//スタン値へのダメージ
		m_stun += m_getHitInfo.StunDamage;
		bool isStun = m_stun >= m_stunMax;

		m_armor -= CalculateDamage(m_getHitInfo.Damage);

		//hpがあるならダメージ処理する
		if (m_HPCurrent > 0)
		{
			//アーマーがあるかないかでダメージ時処理が変わる
			if (m_armor <= 0)
			{
				//アーマー耐久値がなくなったら
				if (isArmorBreak)
				{
					AddEffect(EnemyZakoEffect_ArmorBreak);
					//PlaySnd(L"ZakoArmorBreak", 1.0f, 0);
					m_SEManager->Start(L"ZakoArmorBreak", false, 1.0f * m_SEVol);
				}

				CreateDamageBill(GetThis<GameObject>(), damage, pushY, scale, displayTime);
				m_state->ChangeState(L"Hit");
			}
			else
			{
				CreateDamageBill(GetThis<GameObject>(), damage, pushY, scale, displayTime);
				m_HPCurrent -= CalculateDamage(m_getHitInfo.Damage) / 5.0f;
			}

			//スタン時の演出
			if (isStun) {
				AddEffect(EnemyEffect_Stun);
				App::GetApp()->GetXAudio2Manager()->Start(L"ArmorBreak", 0, 0.9f);
				m_stun = 0;

				m_state->ChangeState(L"Stun");
			}

			//やられ処理移行
			if (m_HPCurrent <= 0)
			{
				m_state->ChangeState(L"Die");
			}

		}
	}

	//遠距離の雑魚敵
	void EnemyZakoLong::OnCreate()
	{
		Actor::OnCreate();

		//いったん雑魚敵のHPは50とする
		m_HPMax = 50.0f;
		m_HPCurrent = m_HPMax;

		//Transform設定
		m_trans = GetComponent<Transform>();
		SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(1.0f / 5, 1.0f / 5, 1.0f / 5),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		//ドローメッシュの設定
		auto ptrDraw = GetComponent<PNTBoneModelDraw>();
		ptrDraw->SetMeshResource(L"Enemy_A");
		ptrDraw->SetDiffuse(Col4(1.0f));
		ptrDraw->SetSamplerState(SamplerState::LinearWrap);
		ptrDraw->SetMeshToTransformMatrix(spanMat);

		//影をつける（シャドウマップを描画する）
		auto shadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		shadowPtr->SetMeshResource(L"Enemy_A");
		shadowPtr->SetMeshToTransformMatrix(spanMat);

		//アニメーション追加(攻撃タイプによって追加アニメーションが変わる)
		ptrDraw->AddAnimation(L"Stand", 0, 1, 24.0f);
		ptrDraw->AddAnimation(L"Walk", 0, 224, 24.0f);
		ptrDraw->AddAnimation(L"Shot", 225, 136, 24.0f);
		ptrDraw->AddAnimation(L"Down", 362, 424, 24.0f);
		ptrDraw->AddAnimation(L"Hit", 543, 29, false, 24.0f);
		ptrDraw->AddAnimation(L"Stan", 463, 78, false, 24.0f);

		//コリジョン作成
		auto ptrColl = AddComponent<CollisionSphere>();//コリジョンスフィアの方が壁にぶつかる判定に違和感がない
		ptrColl->SetAfterCollision(AfterCollision::Auto);
		ptrColl->SetDrawActive(false);//デバック用

		AddTag(L"Enemy");
		AddTag(L"EnemyZako");

		m_player = GetStage()->GetSharedGameObject<Player>(L"Player");

		//接地判定の設定
		m_LandDetect->SetBindPos(Vec3(0, -2.0f, 0));
		m_LandDetect->GetComponent<CollisionSphere>()->SetMakedRadius(1.0f);
		m_LandDetect->GetComponent<Transform>()->SetScale(Vec3(1.0f, 1.0f, 1.0f));

		//ステートマシン生成
		m_state = shared_ptr<EnemyZakoStateMachine>(new EnemyZakoStateMachine(GetThis<GameObject>()));

		//頭上にHPバーを表示させる
		m_HPFrame = GetStage()->AddGameObject<BillBoard>(GetThis<GameObject>(), L"ZakoGaugeFrame", 4, 5.0f, Vec3(2.0f, 0.5f, 5.0f));
		m_HPBer = GetStage()->AddGameObject<BillBoardGauge>(GetThis<GameObject>(), L"BossHPMater", 3, 5.0f, Vec3(2.0f, 0.5f, 5.0f));
		m_HPBer->SetPercent(1.0f);
	}


	void EnemyZakoFlying::OnCreate()
	{
		Actor::OnCreate();

		//いったん雑魚敵のHPは50とする
		m_HPMax = 50.0f;
		m_HPCurrent = m_HPMax;

		//Transform設定
		m_trans = GetComponent<Transform>();
		SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

		// 重力OFF
		m_doPhysics = false;


		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(1.0f / 5, 1.0f / 5, 1.0f / 5),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		//ドローメッシュの設定
		auto ptrDraw = GetComponent<PNTBoneModelDraw>();
		//攻撃タイプによって見た目が変わる
		if (m_AttackType == Zako_Long)
		{
			ptrDraw->SetMeshResource(L"Enemy_A");
		}
		if (m_AttackType == Zako_Melee)
		{
			ptrDraw->SetMultiMeshResource(L"Enemy_B");
		}
		ptrDraw->SetDiffuse(Col4(0.5f));
		//ptrDraw->SetEmissive(Col4(1));
		ptrDraw->SetSamplerState(SamplerState::LinearWrap);
		ptrDraw->SetMeshToTransformMatrix(spanMat);
		//ptrDraw->SetTextureResource(L"Tx_Boss1");

		//影をつける（シャドウマップを描画する）
		auto shadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		shadowPtr->SetMeshResource(L"Enemy_C");
		shadowPtr->SetMeshToTransformMatrix(spanMat);


		//アニメーション追加(攻撃タイプによって追加アニメーションが変わる)
		ptrDraw->AddAnimation(L"Stand", 0, 1, 60.0f);
		ptrDraw->AddAnimation(L"Walk", 126, 49, 60.0f);
		ptrDraw->AddAnimation(L"Charge", 0, 125, 60.0f);
		ptrDraw->AddAnimation(L"Down", 637, 88, false, 60.0f);
		ptrDraw->AddAnimation(L"Hit", 637, 25, false, 60.0f);
		ptrDraw->AddAnimation(L"Stan", 637, 88, false, 60.0f);
	
		//コリジョン作成
		auto ptrColl = AddComponent<CollisionSphere>();//コリジョンスフィアの方が壁にぶつかる判定に違和感がない
		ptrColl->SetAfterCollision(AfterCollision::Auto);
		ptrColl->SetDrawActive(false);//デバック用

		AddTag(L"Enemy");
		AddTag(L"EnemyZako");

		m_player = GetStage()->GetSharedGameObject<Player>(L"Player");

		//接地判定の設定
		m_LandDetect->SetBindPos(Vec3(0, -1.0f, 0));
		m_LandDetect->GetComponent<Transform>()->SetScale(Vec3(2.0f, 2.0f, 2.0f));
		//m_LandDetect->SetCollScale(3.0f);

		//ステートマシン生成
		m_state = shared_ptr<EnemyZakoFlyingStateMachine>(new EnemyZakoFlyingStateMachine(GetThis<GameObject>()));

		//頭上にHPバーを表示させる
		m_HPFrame = GetStage()->AddGameObject<BillBoard>(GetThis<GameObject>(), L"ZakoGaugeFrame", 4, 5.0f, Vec3(2.0f, 0.5f, 5.0f));
		m_HPBer = GetStage()->AddGameObject<BillBoardGauge>(GetThis<GameObject>(), L"BossHPMater", 3, 5.0f, Vec3(2.0f, 0.5f, 5.0f));
		m_HPBer->SetPercent(1.0f);

		//m_damageBill = GetStage()->AddGameObject<EnemyDamageBill>(GetThis<GameObject>(), L"Numbers", 2, 7.0f, Vec3(0.5f, 2.0f, 1.0f));

		//auto m_billBoard2 = GetStage()->AddGameObject<BillBoard>(GetThis<GameObject>(), L"BossHPMater", 3, 5.0f, Vec3(2.0f, 0.5f, 5.0f));
	}

	void EnemyZakoFlying::OnUpdate()
	{
		//もしポーズフラグがオンであればアップデート処理は出来なくなる
		if (m_PauseFlag)
		{
			return;
		}
		if (!m_beforUsed)
		{
			if (m_used)
			{
				m_HPCurrent = m_HPMax;
				m_attackFlag = false;
				m_timeCountOfAttackCool = 3.0f;
				//初期ステートに戻す
				ChangeState(L"Stand");
				GetComponent<PNTBoneModelDraw>()->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
		if (m_beforUsed)
		{
			if (!m_used)
			{
				auto stage = GetStage();
				auto pos = GetComponent<Transform>()->GetPosition();
				stage->GetSharedGameObject<PartsManager>(L"PartsManager")->PartsDrop(pos);
			}
		}
		//現在の使用状況と見比べて変わっていないか見る
		m_beforUsed = m_used;

		EnemyBase::OnUpdate();

		//着地判定(無効化時間中ならそれを減算する)
		OnLanding();

		//物理的な処理
		if (m_doPhysics) {
			if (!m_isLand) {
				Gravity();
			}
			else {
				//Friction();
			}
		}


		//HPバーの処理
		UpdateHPBer();
		//攻撃のクールタイム
		TimeOfAttackCool();

		//HPがゼロになったら消える
		if (m_HPCurrent <= 0)
		{
			RemoveTag(L"LockOnCan");
			RemoveTag(L"LockOnTarget");

			m_used = false;

			//GetStage()->RemoveGameObject<EnemyZako>(GetThis<EnemyZako>());
			//GetStage()->RemoveGameObject<LandDetect>(m_LandDetect);
			//GetStage()->RemoveGameObject<AttackCollision>(m_AttackCol);
		}

		//アニメーション更新
		GetComponent<PNTBoneModelDraw>()->UpdateAnimation(m_addTimeAnimation);

		GetComponent<Transform>()->SetPosition((m_velocity * _delta) + GetComponent<Transform>()->GetPosition());
	}

}