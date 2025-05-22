/*!
@file ZakoState.cpp
@brief ŽG‹›‚ĚXe[g
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//何もないときのステート
	void EnemyZakoStandState::Enter()
	{
		m_enemyZako->ChangeAnim(L"Stand");//立つアニメーションに変更
	}
	void EnemyZakoStandState::Update(float deltaTime)
	{
		auto stage = m_enemyZako->GetStage();
		auto attackType = m_enemyZako->GetAttackType();

		auto isLand = m_enemyZako->GetLand();//着地しているかのフラグ

		//雑魚敵のタイプによって攻撃の方法が変わる
		//遠距離
		m_timeOfShot += deltaTime;
		if (!(m_enemyZako->GetAttackFlag()))return;//攻撃フラグが立ってなかったら攻撃動作はできない
		if (attackType == m_enemyZako->Zako_Long && isLand)
		{
			m_enemyZako->ChangeState(L"PreparationforLong");//軸合わせから始まる
		}
		//近距離
		if (attackType == m_enemyZako->Zako_Melee && isLand)
		{
			m_enemyZako->ChangeState(L"PreparationforCharge");//接近して攻撃する
		}
	}
	void EnemyZakoStandState::Exit()
	{
		//打つカウントダウンリセット
		m_timeOfShot = 0.0f;
	}

	//接近戦をするときのステート
	void EnemyZakoMeleeState::Enter()
	{
		auto LandFlag = m_enemyZako->GetLand();
		auto testVector = m_enemyZako->GetVelocity();

		//攻撃っぽいアニメーションにしてみる
		m_enemyZako->ChangeAnim(L"Down");
		//m_enemyZako->ChangeAnim(L"Walk");//歩くアニメーションに変更

		//攻撃しているタグ追加
		m_enemyZako->AddTag(L"AttackNow");
	}
	void EnemyZakoMeleeState::Update(float deltaTime)
	{
		auto stage = m_enemyZako->GetStage();

		m_timeOfAttack += deltaTime;

		//アニメーション更新時間設定
		m_enemyZako->SetAddTimeAnimation(deltaTime * 2.5f);

		//攻撃しているときも少しだけ進んでいる
		auto m_speed = 1.0f;//足の速さ
		auto move = m_enemyZako->GetForward() * -(m_speed * 0.8);


		//攻撃判定の生成
		if (m_timeOfAttack >= m_timeOfAttackAdd && m_Attack)
		{
			auto tmp = m_enemyZako->GetAttackPtr()->GetHitInfo();
			tmp.HitOnce = true;
			tmp.Damage = 5;
			tmp.HitVel_Stand = Vec3(-3, 5, 0);
			tmp.HitTime_Stand = .8f;
			tmp.Type = AttackType::Enemy;
			//tmp.ForceRecover = false;//ノックバックする
			m_enemyZako->DefAttack(.5f, tmp);
			m_enemyZako->GetAttackPtr()->SetPos(Vec3(3, 1, 0));
			auto AttackPtr = m_enemyZako->GetAttackPtr();
			AttackPtr->GetComponent<Transform>()->SetScale(Vec3(3.7f, 3.0f, 3.0f));
			AttackPtr->SetCollScale(1.0f);

			m_enemyZako->SetAttackFlag(false);//攻撃判定が複数発生させないようにする
			m_Attack = false;//攻撃判定が複数発生させないようにする
		}

		//一定時間たったら攻撃ステートをやめる
		if (m_timeOfAttack >= m_timeMaxOfAttack)
		{
			m_enemyZako->ChangeState(L"Stand");
		}
	}
	void EnemyZakoMeleeState::Exit()
	{
		m_Attack = true;
		m_timeOfAttack = 0.0f;

		//攻撃しているタグ削除
		m_enemyZako->RemoveTag(L"AttackNow");
	}

	//突進の準備ステート(Playerのいる方向に回転する)
	void EnemyZakoPreparationforChargeState::Enter()
	{

	}
	void EnemyZakoPreparationforChargeState::Update(float deltaTime)
	{
		//移動中なのでそれに合わせたアニメーション
		m_enemyZako->ChangeAnim(L"Walk");

		//プレイヤーのいる方向に回転する
		auto PushAngle = 0.0f;//回転のずれ
		m_enemyZako->RotateToPlayer(1.0f, PushAngle);

		auto angleForPlayer = m_enemyZako->GetPlayerSubDirection();//雑魚敵から見てプレイヤーのいる方向

		//ある程度プレイヤーのいる方向に回転出来たら突進攻撃する
		float forgiveAngle = XMConvertToRadians(3.0f);
		if (angleForPlayer <= forgiveAngle)
		{
			m_enemyZako->ChangeState(L"Charge");//突進ステートに遷移
		}
	}
	void EnemyZakoPreparationforChargeState::Exit()
	{

	}

	//突進ステート
	void EnemyZakoChargeState::Enter()
	{
		//まず、プレイヤーとの距離を計算して、どれくらい突進するか決める
		m_playerdistance = m_enemyZako->GetPlayerDist() * 1.1f;
		m_playerStartdistance = m_playerdistance;
		//そのあとプレイヤーのいる方向に回転する
		//auto PushAngle = XM_PIDIV4 / 4;//回転のずれ
		//m_enemyZako->RotateToPlayer(1.0f, PushAngle);
		m_moveAngle = m_enemyZako->GetPlayerSubDirection();
		auto playerQt = m_enemyZako->GetComponent<Transform>()->GetQuaternion();
		m_enemyZako->GetComponent<Transform>()->SetQuaternion(playerQt * Quat(0.0f, sin(m_moveAngle / 2), 0.0f, cos(m_moveAngle / 2)));

		auto test = 0;
	}
	void EnemyZakoChargeState::Update(float deltaTime)
	{
		//突進(移動兼攻撃)処理
		auto stage = m_enemyZako->GetStage();
		auto meleeRange = 5.0f;//接近攻撃有効範囲


		auto attackFlag = m_enemyZako->GetAttackFlag();//攻撃フラグを受け取る

		auto m_speed = 40.0f;
		//if (m_enemyZako->GetPlayerDist() < meleeRange)
		if (m_playerdistance >= 0)
		{
			//移動中なのでそれに合わせたアニメーション
			m_enemyZako->ChangeAnim(L"Walk");

			//進む距離を決める
			auto move = m_enemyZako->GetForward() * m_speed;
			m_playerdistance -= move.length()*deltaTime;//進んでいる距離分引く

			auto LandFlag = m_enemyZako->GetLand();
			//if (LandFlag)
			//{
			//	move.y = 0.0f;
			//}
			m_enemyZako->SetVelocity(move);
			//アニメーション更新時間設定
			m_enemyZako->SetAddTimeAnimation(deltaTime * 2.5f);
		}
		else
		{//ステート変更処理

			m_enemyZako->ChangeState(L"Stand");
		}

		//攻撃コリジョン発生処理
		//少し進んだらコリジョンを発生させる
		if ((m_playerStartdistance *0.9f) >= m_playerdistance)
		{
			auto tmp = m_enemyZako->GetAttackPtr()->GetHitInfo();
			tmp.HitOnce = true;
			tmp.Damage = 5;
			tmp.HitVel_Stand = Vec3(-3, 5, 0);
			tmp.HitTime_Stand = .8f;
			tmp.Type = AttackType::Enemy;
			//tmp.ForceRecover = false;//ノックバックする
			m_enemyZako->DefAttack(.5f, tmp);
			m_enemyZako->GetAttackPtr()->SetPos(Vec3(0, 0, 0));
			auto AttackPtr = m_enemyZako->GetAttackPtr();
			AttackPtr->GetComponent<Transform>()->SetScale(Vec3(3.0f, 3.0f, 3.0f));
			AttackPtr->SetCollScale(1.0f);

			m_enemyZako->SetAttackFlag(false);//攻撃判定が複数発生させないようにする
			m_Attack = false;//攻撃判定が複数発生させないようにする
		}

	}
	void EnemyZakoChargeState::Exit()
	{
		m_playerdistance = 0.0f;
		m_enemyZako->SetAttackFlag(false);//攻撃が終わったのでクールタイムを入れる
	}


	//接近戦をするときの準備ステート(攻撃できる距離になるまで近づく)
	void EnemyZakoPreparationforMeleeState::Enter()
	{
		m_enemyZako->ChangeAnim(L"Walk");//歩くアニメーションに変更

		//初期のプレイヤーとの距離によって足の速さを変える
		auto playerdist = m_enemyZako->GetPlayerDist();
		SppedChange();

		//デバック用
		auto test = m_speed;
		auto a = 0;
	}
	void EnemyZakoPreparationforMeleeState::Update(float deltaTime)
	{
		//プレイヤとの距離によってスピードを変える
		SppedChange();

		auto stage = m_enemyZako->GetStage();
		auto meleeRange = 5.0f;//接近攻撃有効範囲

		//Playerの方向に回転する
		auto PushAngle = XM_PIDIV4 / 4;//回転のずれ
		m_enemyZako->RotateToPlayer(1.0f, PushAngle);

		auto attackFlag = m_enemyZako->GetAttackFlag();//攻撃フラグを受け取る

		//攻撃のクールタイムを過ぎていれば接近そうでなければ離れる
		if (attackFlag)
		{
			//有効範囲まで近づけたら近接攻撃をするそうでなければ、そこまで移動
			if (m_enemyZako->GetPlayerDist() < meleeRange)
			{
				//攻撃のために立ち止まるので立つアニメーションに変更
				m_enemyZako->ChangeAnim(L"Stand");

				//攻撃フラグがオンなら攻撃できる
				if (!attackFlag) return;
				m_enemyZako->ChangeState(L"Melee");
			}
			else if (m_enemyZako->GetPlayerDist() >= meleeRange)
			{
				//移動中なのでそれに合わせたアニメーション
				m_enemyZako->ChangeAnim(L"Walk");

				//進む距離を決める
				auto move = m_enemyZako->GetForward() * m_speed;

				auto LandFlag = m_enemyZako->GetLand();
				if (LandFlag)
				{
					move.y = 0.0f;
				}

				m_enemyZako->SetVelocity(move);
				//アニメーション更新時間設定
				m_enemyZako->SetAddTimeAnimation(deltaTime * 2.5f);
			}
		}
		else if (!attackFlag)//プレイヤーから距離を取る
		{
			if (m_enemyZako->GetPlayerDist() < meleeRange * 3.0f)
			{
				//移動中なのでそれに合わせたアニメーション
				m_enemyZako->ChangeAnim(L"Walk");

				//進む距離を決める
				auto move = m_enemyZako->GetForward() * -(m_speed * 0.8f);

				auto LandFlag = m_enemyZako->GetLand();
				if (LandFlag)
				{
					move.y = 0.0f;
				}

				m_enemyZako->SetVelocity(move);
				//アニメーション更新時間設定
				m_enemyZako->SetAddTimeAnimation(deltaTime * 2.5f);
			}

		}


	}
	void EnemyZakoPreparationforMeleeState::Exit()
	{

	}

	//プレイヤーとの距離によって脚のスピードを変える処理
	void EnemyZakoPreparationforMeleeState::SppedChange()
	{
		//距離によってスピードを変える
		auto playerdist = m_enemyZako->GetPlayerDist();
		if (playerdist > 50.0f)//遠
		{
			m_speed = 15.0f;
		}
		else if (playerdist > 30.0f)//中
		{
			m_speed = 7.0f;
		}
		else//近い
		{
			m_speed = 10.0f;
		}
	}


	//球を打つ直前の準備ステート
	void EnemyZakoPreparationforLongState::Enter()
	{

	}
	void EnemyZakoPreparationforLongState::Update(float deltaTime)
	{
		auto stage = m_enemyZako->GetStage();

		//Playerの方向に回転する
		auto PushAngle = XM_PIDIV4 / 4;//回転のずれ
		m_enemyZako->RotateToPlayer(1.0f, PushAngle);

		//有効範囲まで接近する、プレイヤーにある程度近づかれたら距離を取る
		auto Range = 25.0f;
		if (m_enemyZako->GetPlayerDist() >= Range)//近づく
		{
			//移動中なのでそれに合わせたアニメーション
			m_enemyZako->ChangeAnim(L"Walk");

			//進む距離を決める
			m_speed = 10.0f;
			auto move = m_enemyZako->GetForward() * m_speed;

			auto LandFlag = m_enemyZako->GetLand();
			if (LandFlag)
			{
				move.y = 0.0f;
			}

			m_enemyZako->SetVelocity(move);
			//アニメーション更新時間設定
			m_enemyZako->SetAddTimeAnimation(deltaTime * 2.5f);
		}
		if (m_enemyZako->GetPlayerDist() <= Range - 5.0f)//離れる
		{
			//移動中なのでそれに合わせたアニメーション
			m_enemyZako->ChangeAnim(L"Walk");

			//進む距離を決める
			m_speed = -7.0f;
			auto move = m_enemyZako->GetForward() * m_speed;

			auto LandFlag = m_enemyZako->GetLand();
			if (LandFlag)
			{
				move.y = 0.0f;
			}

			m_enemyZako->SetVelocity(move);
			//アニメーション更新時間設定(後ろに進むので逆再生にしたい)
			m_enemyZako->SetAddTimeAnimation(deltaTime * 2.5f);
		}

		//一定時間たったら攻撃する
		m_timeOfShot += deltaTime;
		if (m_timeOfShot >= m_timeMaxOfShot)
		{
			m_timeOfShot = 0.0f;//リセット
			m_enemyZako->ChangeState(L"Shot");//打つステートがないのでコメントアウト
		}
	}
	void EnemyZakoPreparationforLongState::Exit()
	{
		//打つカウントダウンリセット
		m_timeOfShot = 0.0f;
	}

	//攻撃をするステート(遠距離)
	void EnemyZakoShotState::Enter()
	{
		auto stage = m_enemyZako->GetStage();
		auto posEnemy = m_enemyZako->GetPosition();
		//弾生成
		auto bullet = stage->AddGameObject<Bullet>(posEnemy, Vec3(0.0f), Vec3(0.4f), 30.0f,
			dynamic_pointer_cast<Actor>(m_enemyZako), 30.0f, ActorName_Enemy);

		m_enemyZako->ChangeAnim(L"Shot");//撃つアニメーションに変更
	}
	void EnemyZakoShotState::Update(float deltaTime)
	{
		auto stage = m_enemyZako->GetStage();

		////目標となる角度取得
		auto angleTarget = m_enemyZako->GetPlayerSubDirection();

		//アニメーション更新時間設定
		m_enemyZako->SetAddTimeAnimation(deltaTime);

		//一定時間たったらStandステートに戻る
		m_timeOfAttack += deltaTime;
		if (m_timeOfAttack >= m_timeMaxOfAttack)
		{
			m_enemyZako->ChangeState(L"Stand");
		}
	}
	void EnemyZakoShotState::Exit()
	{
		//打つカウントダウンリセット
		m_timeOfAttack = 0.0f;
	}


	//ダメージを受けたステート
	void EnemyZakoHitState::Enter()
	{
		auto hitInfo = m_enemyZako->GetHitInfo();
		auto HPNow = m_enemyZako->GetHPCurrent();
		//攻撃を受けたのでヒットバックする
		m_enemyZako->HitBack();
		//ダメージ処理
		m_enemyZako->SetHPCurrent(HPNow - hitInfo.Damage);

		m_enemyZako->ChangeAnim(L"Stand");//ダメージを受けたアニメーションに変更
	}
	void EnemyZakoHitState::Update(float deltaTime)
	{
		//一定時間たったらStandステートに戻る
		m_enemyZako->HitBackStandBehavior();

		//アニメーション更新時間設定
		m_enemyZako->SetAddTimeAnimation(deltaTime);
	}
	void EnemyZakoHitState::Exit()
	{

	}


	//--------------------------------------------
	// 飛ぶザコのステート
	// -------------------------------------------

	// 何もないときのステート----------------------
	void EnemyZakoFlyingStandState::Enter()
	{
		m_enemyZako->ChangeAnim(L"Stand");//待機アニメーションに変更
	}

	void EnemyZakoFlyingStandState::Update(float deltatime)
	{
		auto stage = m_enemyZako->GetStage();
		auto attackType = m_enemyZako->GetAttackType();

		auto isLand = m_enemyZako->GetLand();//着地(滞空？)しているかのフラグ

	}

	void EnemyZakoFlyingStandState::Exit()
	{

	}
	// END-----------------------------------------

	//接近戦をするときの準備ステート-----------------
	void EnemyZakoFlyingPreparationforMeleeState::Enter()
	{
		m_enemyZako->ChangeAnim(L"Walk");//歩くアニメーションに変更
	}

	void EnemyZakoFlyingPreparationforMeleeState::Update(float deltaTime)
	{
		auto stage = m_enemyZako->GetStage();


		auto meleeRange = 10.0f;//接近攻撃有効範囲

		//Playerの方向に回転する
		auto PushAngle = XM_PIDIV4 / 4;//回転のずれ
		m_enemyZako->RotateToPlayer(1.0f, PushAngle);

		auto attackFlag = m_enemyZako->GetAttackFlag();
		//有効範囲まで近づけたら近接攻撃をするそうでなければ、そこまで移動
		if (m_enemyZako->GetPlayerDist() < meleeRange)
		{
			//攻撃のために立ち止まるので立つアニメーションに変更
			m_enemyZako->ChangeAnim(L"Stand");

			//攻撃フラグがオンなら攻撃できる
			if (!attackFlag) return;
			m_enemyZako->ChangeState(L"Melee");
		}
		else if (m_enemyZako->GetPlayerDist() >= meleeRange)
		{
			//移動中なのでそれに合わせたアニメーション
			m_enemyZako->ChangeAnim(L"Walk");

			//進む距離を決める
			auto move = m_enemyZako->GetForward() * 10.0f;

			auto LandFlag = m_enemyZako->GetLand();
			if (LandFlag)
			{
				move.y = 0.0f;
			}

			m_enemyZako->SetVelocity(move);
			//アニメーション更新時間設定
			m_enemyZako->SetAddTimeAnimation(deltaTime * 2.5f);
		}
	}

	void EnemyZakoFlyingPreparationforMeleeState::Exit()
	{

	}
	// END--------------------------------------------

	// 攻撃をするときのステート(近距離)-----
	void EnemyZakoFlyingMeleeState::Enter()
	{
		auto LandFlag = m_enemyZako->GetLand();
		auto testVector = m_enemyZako->GetVelocity();

		//攻撃っぽいアニメーションにしてみる
		m_enemyZako->ChangeAnim(L"Down");
		//m_enemyZako->ChangeAnim(L"Walk");//歩くアニメーションに変更

	}

	void EnemyZakoFlyingMeleeState::Update(float deltaTime)
	{
		auto stage = m_enemyZako->GetStage();

		m_timeOfAttack += deltaTime;

		//アニメーション更新時間設定
		m_enemyZako->SetAddTimeAnimation(deltaTime * 2.5f);

		//攻撃判定の生成
		if (m_timeOfAttack >= m_timeOfAttackAdd && m_Attack)
		{
			auto tmp = m_enemyZako->GetAttackPtr()->GetHitInfo();
			tmp.HitOnce = true;
			tmp.Damage = 5;
			tmp.HitVel_Stand = Vec3(-3, 5, 0);
			tmp.HitTime_Stand = .8f;
			tmp.Type = AttackType::Enemy;
			//tmp.ForceRecover = false;//ノックバックする
			m_enemyZako->DefAttack(.5f, tmp);
			m_enemyZako->GetAttackPtr()->SetPos(Vec3(3, 1, 0));
			auto AttackPtr = m_enemyZako->GetAttackPtr();
			AttackPtr->GetComponent<Transform>()->SetScale(Vec3(3.7f, 3.0f, 3.0f));
			AttackPtr->SetCollScale(1.0f);

			m_enemyZako->SetAttackFlag(false);//攻撃判定が複数発生させないようにする
			m_Attack = false;//攻撃判定が複数発生させないようにする
		}

		//一定時間たったら攻撃ステートをやめる
		if (m_timeOfAttack >= m_timeMaxOfAttack)
		{
			m_enemyZako->ChangeState(L"Stand");
		}

	}

	void EnemyZakoFlyingMeleeState::Exit()
	{

	}
	// END-----------------------------------

	// 遠距離の直前の軸合わせの時のステート-----
	void EnemyZakoFlyingAlignmentState::Enter()
	{

	}

	void EnemyZakoFlyingAlignmentState::Update(float deltaTime)
	{
		auto stage = m_enemyZako->GetStage();

		//目標となる角度取得
		//auto angleTarget = m_enemyZako->GetPlayerSubDirection();
		//auto angleTarget = atan2f(targetVec.z, targetVec.x);


		//Playerのいる方向を計算
		auto playerPos = stage->GetSharedGameObject<Player>(L"Player")->GetPosition();
		auto EnemyPos = m_enemyZako->GetPosition();
		auto targetVec = playerPos - EnemyPos;
		auto angleTarget = (atan2f(targetVec.z, -targetVec.x) / 2);
		angleTarget += XMConvertToRadians(90.0f);

		//大体その方向に向いたらPlayerの方向に向いているとみなす
		if (abs(angleTarget) < XMConvertToRadians(3.0f))
		{
			angleTarget = 0.0f;
		}
		//回転処理
		if (angleTarget != 0.0f)
		{
			auto qt = m_enemyZako->GetComponent<Transform>()->GetBeforeQuaternion();
			qt.y = 0.0f;
			////qt.y += angleTarget;
			qt = qt * Quat(0.0f, sin(angleTarget) / 2.0f, 0.0f, cos(angleTarget) / 2.0f);
			m_enemyZako->GetComponent<Transform>()->SetQuaternion(qt);
		}


		//一定時間たったら攻撃する
		m_timeOfShot += deltaTime;
		if (m_timeOfShot >= m_timeMaxOfShot)
		{
			m_timeOfShot = 0.0f;//リセット
			m_enemyZako->ChangeState(L"Shot");//打つステートがないのでコメントアウト
		}

	}

	void EnemyZakoFlyingAlignmentState::Exit()
	{
		//打つカウントダウンリセット
		m_timeOfShot = 0.0f;

	}
	// END--------------------------------------

	// 攻撃をするときのステート(遠距離)-----
	void EnemyZakoFlyingShotState::Enter()
	{

	}

	void EnemyZakoFlyingShotState::Update(float deltatime)
	{

	}

	void EnemyZakoFlyingShotState::Exit()
	{

	}
	// END-----------------------------------

	//ダメージを受けたとき-------------------
	void EnemyZakoFlyingHitState::Enter()
	{
		auto hitInfo = m_enemyZako->GetHitInfo();
		auto HPNow = m_enemyZako->GetHPCurrent();

		// ダメージ処理
		m_enemyZako->SetHPCurrent(HPNow - hitInfo.Damage);

		// ダメージを受けたアニメーションに変更
		m_enemyZako->ChangeAnim(L"Stand");
	}

	void EnemyZakoFlyingHitState::Update(float deltaTime)
	{
		//一定時間たったらStandステートに戻る
		m_enemyZako->HitBackStandBehavior();

		//アニメーション更新時間設定
		m_enemyZako->SetAddTimeAnimation(deltaTime);

	}

	void EnemyZakoFlyingHitState::Exit()
	{

	}
	// END------------------------------------

	//-------------------------------------------------------
	// 飛ぶザコのステート終端
	//-------------------------------------------------------

}