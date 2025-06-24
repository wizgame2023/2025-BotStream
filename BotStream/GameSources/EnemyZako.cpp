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

		m_HPMax = 40.0f;
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

		//攻撃タイプによって見た目が変わる
		if (m_AttackType == Zako_Long)
		{
			ptrDraw->SetMeshResource(L"Enemy_A");
		}
		if (m_AttackType == Zako_Melee)
		{
			ptrDraw->SetMeshResource(L"Enemy_C");
		}
		ptrDraw->SetDiffuse(Col4(0.5f));
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
		m_LandDetect->SetBindPos(Vec3(0, -2.5f, 0));
		m_LandDetect->GetComponent<Transform>()->SetScale(Vec3(2.0f, 2.0f, 2.0f));
		//m_LandDetect->SetCollScale(3.0f);

		//ステートマシン生成
		m_state = shared_ptr<EnemyZakoStateMachine>(new EnemyZakoStateMachine(GetThis<GameObject>()));

		//頭上にHPバーを表示させる		
		m_HPBer = GetStage()->AddGameObject<BillBoardGauge>(GetThis<GameObject>(), L"ZakoHPMater", 3, 5.0f, Vec3(2.0f, 0.5f, 5.0f));
		m_HPFrame = GetStage()->AddGameObject<BillBoard>(GetThis<GameObject>(), L"BossGaugeFrame", 3, 5.0f, Vec3(2.0f, 0.5f, 5.0f));
		m_HPBer->SetPercent(1.0f);
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
				m_HPCurrent = m_HPMax;
				m_attackFlag = false;
				m_timeCountOfAttackCool = 3.0f;
				//初期ステートに戻す
				ChangeState(L"Stand");
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

		//スタン値追加
		m_stun += m_getHitInfo.StunDamage;

		//攻撃を受けたときの処理
		if (m_hitbacktime <= 0) 
		{
			if (m_HPCurrent <= 0)
			{
				ChangeState(L"Die");
			}
			else
			{
				//スタン値が一定を過ぎたらスタン状態になる
				if (m_stun >= m_stunMax)
				{
					ChangeState(L"Stun");
					AddEffect(EnemyEffect_Stun);
					App::GetApp()->GetXAudio2Manager()->Start(L"ArmorBreak", 0, 0.9f);
					m_stun = 0;
				}
				else
				{
					ChangeState(L"Stand");
				}
			}
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
		auto stage = GetStage();
		stage->GetSharedGameObject<PartsManager>(L"PartsManager")->PartsDrop(m_pos);
	}

	//ダメージを受けた際の処理
	void EnemyZako::OnDamaged()
	{
		//hpがあるならダメージ処理する
		if (m_HPCurrent > 0)
		{
			m_state->ChangeState(L"Hit");
		}
	}

	//遠距離の雑魚敵
	void EnemyZakoLong::OnCreate()
	{
		Actor::OnCreate();

		//いったん雑魚敵のHPは50とする
		m_HPMax = 25.0f;
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
		ptrDraw->SetDiffuse(Col4(0.5f));
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
		m_LandDetect->SetBindPos(Vec3(0, -2.5f, 0));
		m_LandDetect->GetComponent<Transform>()->SetScale(Vec3(1.0f, 1.0f, 1.0f));

		//ステートマシン生成
		m_state = shared_ptr<EnemyZakoStateMachine>(new EnemyZakoStateMachine(GetThis<GameObject>()));

		//頭上にHPバーを表示させる
		m_HPFrame = GetStage()->AddGameObject<BillBoard>(GetThis<GameObject>(), L"BossGaugeFrame", 4, 5.0f, Vec3(2.0f, 0.5f, 5.0f));
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
		ptrDraw->AddAnimation(L"Down", 637, 88, 60.0f);

		//コリジョン作成
		auto ptrColl = AddComponent<CollisionSphere>();//コリジョンスフィアの方が壁にぶつかる判定に違和感がない
		ptrColl->SetAfterCollision(AfterCollision::Auto);
		ptrColl->SetDrawActive(true);//デバック用

		AddTag(L"Enemy");
		AddTag(L"EnemyZako");

		m_player = GetStage()->GetSharedGameObject<Player>(L"Player");

		//接地判定の設定
		m_LandDetect->SetBindPos(Vec3(0, -1.0f, 0));
		m_LandDetect->GetComponent<Transform>()->SetScale(Vec3(7.0f, 7.0f, 7.0f));
		//m_LandDetect->SetCollScale(3.0f);

		//ステートマシン生成
		m_state = shared_ptr<EnemyZakoFlyingStateMachine>(new EnemyZakoFlyingStateMachine(GetThis<GameObject>()));

		//頭上にHPバーを表示させる
		m_HPFrame = GetStage()->AddGameObject<BillBoard>(GetThis<GameObject>(), L"BossGaugeFrame", 4, 5.0f, Vec3(2.0f, 0.5f, 5.0f));
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

	//HPバーの処理
	void EnemyZakoFlying::UpdateHPBer()
	{
		//ビルボードの処理
		if (!m_used)
		{
			m_HPFrame->SetScale(Vec3(0.0f));
			m_HPBer->SetScale(Vec3(0.0f));
		}
		if (m_used)
		{
			m_HPFrame->SetScale(Vec3(2.0f, 0.5f, 5.0f));
			m_HPBer->SetScale(Vec3(2.0f, 0.5f, 5.0f));

			//HPの割合によってゲージが減る
			float HPPercent = (float)m_HPCurrent / (float)m_HPMax;
			m_HPBer->SetPercent(HPPercent);

		}


	}

	//攻撃のクールタイム
	void EnemyZakoFlying::TimeOfAttackCool()
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

	//コリジョン判定
	void EnemyZakoFlying::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		DetectBeingAttacked(Other);
	}

	//ダメージを受けた際の処理
	void EnemyZakoFlying::OnDamaged()
	{
		////攻撃時はノックバックしないようにする(実験)(強すぎるので別の方向性で強くする)
		//if (!FindTag(L"AttackNow"))
		//{
		//	m_state->ChangeState(L"Hit");
		//}
		//else if (FindTag(L"AttackNow"))
		//{
		//	m_HPCurrent -= CalculateDamage(m_getHitInfo.Damage);
		//}

		m_state->ChangeState(L"Hit");
	}

}