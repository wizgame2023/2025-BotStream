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
		if (attackType == m_enemyZako->Zako_Long && isLand)
		{
			m_enemyZako->ChangeState(L"PreparationforLong");//軸合わせから始まる
		}
		//近距離
		if (attackType == m_enemyZako->Zako_Melee && isLand)
		{
			if (m_enemyZako->GetAttackFlag())//攻撃フラグが立ってなかったら攻撃動作はできない
			{
				//ランダムに攻撃する
				auto par = rand() % 3;
				switch (par)
				{
				case 0:
					m_enemyZako->ChangeState(L"PreparationforCharge");//突進
					break;
				case 1:
					m_enemyZako->ChangeState(L"PreparationforMelee");//接近して攻撃する
					break;
				case 2:
					m_enemyZako->ChangeState(L"PreparationforMelee");//接近して攻撃する
					break;
				default:
					break;
				}
			}
			if (!(m_enemyZako->GetAttackFlag()))//攻撃できる状態でないので逃げる
			{
				//逃げるステート
				m_enemyZako->ChangeState(L"Escape");//接近して攻撃する
			}

		}
	}
	void EnemyZakoStandState::Exit()
	{
		//打つカウントダウンリセット
		m_timeOfShot = 0.0f;
	}

	//プレイヤーから距離を取るステート
	void EnemyZakoEscapeState::Enter()
	{

	}
	void EnemyZakoEscapeState::Update(float deltaTime)
	{
		//Playerの方向に回転する
		auto PushAngle = XM_PIDIV4 / 4;//回転のずれ
		m_enemyZako->RotateToPlayer(1.0f, PushAngle);

		float distance = 10.0f;
		float m_speed = 1.0f;
		//Playerから一定距離離れる
		if (m_enemyZako->GetPlayerDist() < distance)
		{
			//移動中なのでそれに合わせたアニメーション
			m_enemyZako->ChangeAnim(L"Walk");

			//進む距離を決める
			auto move = m_enemyZako->GetForward() * -m_speed;

			m_enemyZako->AddVelocity(move);
			//アニメーション更新時間設定
			m_enemyZako->SetAddTimeAnimation(deltaTime * 2.5f);
		}

		//攻撃できる状態ならこの距離を取る状態をやめる
		bool attackFlag = m_enemyZako->GetAttackFlag();
		if (attackFlag)
		{
			m_enemyZako->ChangeState(L"Stand");
		}
	}
	void EnemyZakoEscapeState::Exit()
	{

	}

	//接近戦をするときのステート
	void EnemyZakoMeleeState::Enter()
	{
		EnemyZakoStateBase::Enter();

		auto LandFlag = m_enemyZako->GetLand();
		auto testVector = m_enemyZako->GetVelocity();


		//攻撃っぽいアニメーションにしてみる
		m_enemyZako->ChangeAnim(L"Melee_Jamp");
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
			tmp.InvincibleOnHit = true;
			tmp.Damage = 5;
			tmp.HitVel_Stand = Vec3(-3, 5, 0);
			tmp.HitTime_Stand = .3f;
			tmp.Type = AttackType::Enemy;
			//tmp.ForceRecover = false;//ノックバックする
			m_enemyZako->DefAttack(.5f, tmp);
			m_enemyZako->GetAttackPtr()->SetPos(Vec3(3, 1, 0));
			auto AttackPtr = m_enemyZako->GetAttackPtr();
			AttackPtr->GetComponent<Transform>()->SetScale(Vec3(3.7f, 3.0f, 3.0f));
			AttackPtr->SetCollScale(1.0f);
			AttackPtr->ActivateCollision(0.1f);//攻撃発生時間

			m_enemyZako->SetAttackFlag(false);//攻撃判定が複数発生させないようにする
			m_Attack = false;//攻撃判定が複数発生させないようにする

			//攻撃用SE再生
			m_SE = m_SEManager->Start(L"Enemy_Slash", 0, 0.4f);
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
		//後ろに進む距離とスピードを決める(初期化)
		m_backDistance = 10.0f;//後ろに進む距離
		m_speed = 2.0f;
	}
	void EnemyZakoPreparationforChargeState::Update(float deltaTime)
	{
		//Playerの方向に回転する
		auto PushAngle = XM_PIDIV4 / 4;//回転のずれ
		m_enemyZako->RotateToPlayer(1.0f, PushAngle);

		//少し後ろに進んでから一定時間立ったら突進に移行する
		if (m_backDistance > 0.0f)
		{
			//移動中なのでそれに合わせたアニメーション
			m_enemyZako->ChangeAnim(L"Walk");

			//進む距離を決める
			auto move = m_enemyZako->GetForward() * -m_speed;

			m_enemyZako->AddVelocity(move);
			//アニメーション更新時間設定
			m_enemyZako->SetAddTimeAnimation(deltaTime * 2.5f);

			//バックした距離分引く
			m_backDistance -= m_enemyZako->GetVelocity().length() * deltaTime;
		}

		//後ろに進み終わったら突進する
		if (m_backDistance < 0.0f)
		{
			m_enemyZako->ChangeState(L"Charge");//突進ステートに遷移
		}


		////移動中なのでそれに合わせたアニメーション
		//m_enemyZako->ChangeAnim(L"Walk");

		////プレイヤーのいる方向に回転する
		//auto PushAngle = 0.0f;//回転のずれ
		//m_enemyZako->RotateToPlayer(1.0f, PushAngle);

		//auto angleForPlayer = m_enemyZako->GetPlayerSubDirection();//雑魚敵から見てプレイヤーのいる方向

		////ある程度プレイヤーのいる方向に回転出来たら突進攻撃する
		//float forgiveAngle = XMConvertToRadians(3.0f);
		//if (angleForPlayer <= forgiveAngle)
		//{
		//	m_enemyZako->ChangeState(L"Charge");//突進ステートに遷移
		//}
	}
	void EnemyZakoPreparationforChargeState::Exit()
	{

	}

	//突進ステート
	void EnemyZakoChargeState::Enter()
	{
		EnemyZakoStateBase::Enter();
		
		//スピードを決める
		m_speed = 5.0f;

		//まず、プレイヤーとの距離を計算して、どれくらい突進するか決める
		m_playerdistance = (m_enemyZako->GetPlayerDist() * 1.1f);
		m_playerStartdistance = m_playerdistance;
		m_moveAngle = m_enemyZako->GetPlayerSubDirection();
		auto playerQt = m_enemyZako->GetComponent<Transform>()->GetQuaternion();
		m_enemyZako->GetComponent<Transform>()->SetQuaternion(playerQt * Quat(0.0f, sin(m_moveAngle / 2), 0.0f, cos(m_moveAngle / 2)));

		m_enemyZako->ChangeAnim(L"Walk");
		//SE再生
		m_SE = m_SEManager->Start(L"EnemyZako_Charge", 0, 0.4f);
	}
	void EnemyZakoChargeState::Update(float deltaTime)
	{
		//突進していい距離を過ぎるまで突進する
		if (m_playerdistance >= 0)
		{
			//移動中なのでそれに合わせたアニメーション
			m_enemyZako->ChangeAnim(L"Walk");

			//進む距離を決める
			auto move = m_enemyZako->GetForward() * m_speed;

			auto LandFlag = m_enemyZako->GetLand();
			m_enemyZako->AddVelocity(move);
			//アニメーション更新時間設定(突進なので早めにする)
			m_enemyZako->SetAddTimeAnimation(deltaTime * 5.5f);	
			
			m_playerdistance -= m_enemyZako->GetVelocity().length() * deltaTime;//進んでいる距離分引く
		}
		else
		{//ステート変更処理
			m_enemyZako->ChangeState(L"Stand");
		}

		//攻撃コリジョン発生処理
		//少し進んだらコリジョンを発生させる
		if ((m_playerStartdistance * 0.9f) >= m_playerdistance)
		{
			auto tmp = m_enemyZako->GetAttackPtr()->GetHitInfo();
			tmp.HitOnce = true;
			tmp.InvincibleOnHit = true;
			tmp.Damage = 5;
			tmp.HitVel_Stand = Vec3(-8, 15, 0);
			tmp.HitTime_Stand = .5f;
			tmp.Type = AttackType::Enemy;
			//tmp.ForceRecover = false;//ノックバックする
			m_enemyZako->DefAttack(.5f, tmp);
			m_enemyZako->GetAttackPtr()->SetPos(Vec3(0, 0, 0));
			auto AttackPtr = m_enemyZako->GetAttackPtr();
			AttackPtr->GetComponent<Transform>()->SetScale(Vec3(3.5f, 3.5f, 3.5f));
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
		m_speed;

		//初期のプレイヤーとの距離によって足の速さを変える
		auto playerdist = m_enemyZako->GetPlayerDist();
		SppedChange();
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
				auto move = m_enemyZako->GetForward().normalize() * m_speed;

				m_enemyZako->AddVelocity(move);
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
		if (playerdist > 30.0f)//中
		{
			m_speed = 3.0f;
		}
		else//近い
		{
			m_speed = 2.0f;
		}
	}


	//球を打つ直前の準備ステート
	void EnemyZakoPreparationforLongState::Enter()
	{
		//弾を発射する時間を決める4~5秒の間で発射する
		float minTime = 5.0f;
		m_timeMaxOfShot = (rand() % 10) * 0.1f + minTime;
	}
	void EnemyZakoPreparationforLongState::Update(float deltaTime)
	{
		auto stage = m_enemyZako->GetStage();

		//Playerの方向に回転する
		auto PushAngle = 0.0f;//回転のずれ
		m_enemyZako->RotateToPlayer(1.0f, PushAngle);

		//有効範囲まで接近する、プレイヤーにある程度近づかれたら距離を取る
		auto Range = 25.0f;
		if (m_enemyZako->GetPlayerDist() >= Range)//近づく
		{
			//移動中なのでそれに合わせたアニメーション
			m_enemyZako->ChangeAnim(L"Walk");

			//進む距離を決める
			m_speed = 3.0f;
			auto move = m_enemyZako->GetForward() * m_speed;

			m_enemyZako->SetVelocity(move);
			//アニメーション更新時間設定
			m_enemyZako->SetAddTimeAnimation(deltaTime * 2.5f);
		}
		if (m_enemyZako->GetPlayerDist() <= Range - 10.0f)//離れる
		{
			//移動中なのでそれに合わせたアニメーション
			m_enemyZako->ChangeAnim(L"Walk");

			//進む距離を決める
			m_speed = -1.0f;
			auto move = m_enemyZako->GetForward() * m_speed;

			m_enemyZako->AddVelocity(move);
			//アニメーション更新時間設定(後ろに進むので逆再生にしたい)
			m_enemyZako->SetAddTimeAnimation(deltaTime * 2.5f);
		}

		//時間計測
		m_timeOfShot += deltaTime;
		//射程内なら打つ
		if (m_enemyZako->GetPlayerDist() <= Range)
		{
			if (m_timeOfShot >= m_timeMaxOfShot)
			{
				m_timeOfShot = 0.0f;//リセット
				m_enemyZako->ChangeState(L"Shot");//打つステートがないのでコメントアウト
			}
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
		EnemyZakoStateBase::Enter();

		//撃つアニメーションに変更
		m_enemyZako->ChangeAnim(L"Shot");
		//SE再生
		m_SE = m_SEManager->Start(L"EnemyZako_Shot", 0, 0.4f);
	}
	void EnemyZakoShotState::Update(float deltaTime)
	{
		auto stage = m_enemyZako->GetStage();

		//目標となる角度取得
		auto angleTarget = m_enemyZako->GetPlayerSubDirection();

		//アニメーション更新時間設定
		m_enemyZako->SetAddTimeAnimation(deltaTime*2.5f);

		//一定時間たったら弾が発射される
		if (m_timeOfAttack >= m_timeOfStartAttack && m_attackFlag)
		{
			//弾の生成位置がちょうどいい位置に出現できるように調整する
			auto posEnemy = m_enemyZako->GetPosition();
			auto pushPos = Vec3(0.0f, 0.8f, 0.0f);

			//弾生成
			auto bullet = stage->AddGameObject<Bullet>(posEnemy+pushPos, Vec3(0.0f), Vec3(0.4f), 30.0f,
				dynamic_pointer_cast<Actor>(m_enemyZako), 30.0f, ActorName_Enemy);

			//アップデートで何発も弾が出ないようにfalseにする
			m_attackFlag = false;
		}

		//一定時間たったらStandステートに戻る
		m_timeOfAttack += deltaTime;
		if (m_timeOfAttack >= m_timeMaxOfAttack)
		{
			m_enemyZako->ChangeState(L"Stand");
		}
	}
	void EnemyZakoShotState::Exit()
	{
		//リセット
		m_timeOfAttack = 0.0f;
		m_attackFlag = true;
	}


	//ダメージを受けたステート
	void EnemyZakoHitState::Enter()
	{
		auto hitInfo = m_enemyZako->GetHitInfo();
		auto HPNow = m_enemyZako->GetHPCurrent();

		//アニメーションをダメージを受けたものにする
		m_enemyZako->ChangeAnim(L"Hit");

		//攻撃を受けたのでヒットバックする
		m_enemyZako->HitBack();
		//ダメージ処理
		m_enemyZako->SetHPCurrent(HPNow - hitInfo.Damage);
	}
	void EnemyZakoHitState::Update(float deltaTime)
	{
		//一定時間たったらStandステートに戻る
		m_enemyZako->HitBackStandBehavior();

		//遠距離タイプの雑魚敵か確認する
		auto enemyShotType = dynamic_pointer_cast<EnemyZakoLong>(_obj.lock());

		//雑魚敵のタイプによってくらうアニメーションの長さが違うので調整する
		if (enemyShotType)
		{
			//アニメーション更新時間設定
			m_enemyZako->SetAddTimeAnimation(deltaTime * 3.5f);
		}
		else
		{
			//アニメーション更新時間設定
			m_enemyZako->SetAddTimeAnimation(deltaTime*2.5f);
		}
	}
	void EnemyZakoHitState::Exit()
	{
		//アニメーションをリセットして次のステートがまたHitでもアニメーションが流れるようにする
		m_enemyZako->ChangeAnim(L"Stand");
	}

	//やられたときのステート
	void EnemyZakoDieState::Enter()
	{
		EnemyZakoStateBase::Enter();
		//やられたときのSE再生
		m_SEManager->Start(L"Enemy_Defeat", 0, 0.4f);
		//やられたとき用のアニメーションに変更
		m_enemyZako->ChangeAnim(L"Down");	

	}
	void EnemyZakoDieState::Update(float deltaTime)
	{
		//アニメーション更新時間
		m_enemyZako->SetAddTimeAnimation(deltaTime * 1.5f);
		//時間計測
		m_timeOfState += deltaTime;

		//一定時間過ぎたら(やられる演出)消える
		if (m_timeOfState >= m_timeMaxOfState)
		{		
			m_enemyZako->SetUsed(false);
		}
	}
	void EnemyZakoDieState::Exit()
	{
		//リセット
		m_timeOfState = 0.0f;
	}



	//--------------------------------------------
	// 飛ぶザコのステート
	// -------------------------------------------


	//何もないときのステート
	void EnemyZakoFlyingStandState::Enter()
	{
		m_enemyZako->ChangeAnim(L"Stand");//立つアニメーションに変更
	}
	void EnemyZakoFlyingStandState::Update(float deltaTime)
	{
		auto stage = m_enemyZako->GetStage();
		auto attackType = m_enemyZako->GetAttackType();
		auto pos = m_enemyZako->GetPosition();
		auto isLand = m_enemyZako->GetLand();//着地しているかのフラグ

		if (pos.y <= 10.0)
		{
			m_enemyZako->SetVelocity(Vec3(0, 10.0f, 0));
		}
		else
		{
			//雑魚敵のタイプによって攻撃の方法が変わる
			//遠距離
			m_timeOfShot += deltaTime;
			if (attackType == m_enemyZako->Zako_Long/* && isLand*/)
			{
				m_enemyZako->ChangeState(L"Alignment");//軸合わせから始まる
			}
			//近距離
			if (attackType == m_enemyZako->Zako_Melee/* && isLand*/)
			{
				m_enemyZako->ChangeState(L"PreparationforMelee");//接近して攻撃する
			}

		}
	}
	void EnemyZakoFlyingStandState::Exit()
	{
		//打つカウントダウンリセット
		m_timeOfShot = 0.0f;
	}
	// END-----------------------------------------


	//接近戦をするときの準備ステート(攻撃できる距離になるまで近づく)
	void EnemyZakoFlyingPreparationforMeleeState::Enter()
	{
		m_enemyZako->ChangeAnim(L"Walk");//歩くアニメーションに変更

		//初期のプレイヤーとの距離によって足の速さを変える
		auto playerdist = m_enemyZako->GetPlayerDist();
		SpeedChange();

		//デバック用
		auto test = m_speed;
		auto a = 0;
	}
	void EnemyZakoFlyingPreparationforMeleeState::Update(float deltaTime)
	{
		//プレイヤとの距離によってスピードを変える
		SpeedChange();

		auto stage = m_enemyZako->GetStage();
		auto meleeRange = 25.0f;//接近攻撃有効範囲

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
				m_enemyZako->ChangeState(L"Charge");
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
				auto move = m_enemyZako->GetForward() * -(m_speed * 0.8);

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
	void EnemyZakoFlyingPreparationforMeleeState::Exit()
	{

	}
	// END--------------------------------------------

	// 突進攻撃をするときのステート---------------------
	void EnemyZakoFlyingChargeState::Enter()
	{
		m_chargeTime = 0.0f;
		// プレイヤーの方向ベクトルをキャッシュ
		Vec3 toPlayer = m_enemyZako->GetPlayerDistInVec3();
		toPlayer.normalize();
		m_chargeDir = toPlayer;
		// アニメーション切り替え（例）
		m_enemyZako->ChangeAnim(L"Charge");
		//攻撃しているタグ追加
		m_enemyZako->AddTag(L"AttackNow");
	}

	void EnemyZakoFlyingChargeState::Update(float deltaTime)
	{
		
		// 1) 突進移動
		Vec3 v = m_chargeDir * m_chargeSpeed;

		m_enemyZako->SetVelocity(v);
		m_chargeTime += deltaTime;

		////攻撃判定の生成
		if (m_chargeTime <= m_maxChargeTime)
		{
			auto tmp = m_enemyZako->GetAttackPtr()->GetHitInfo();
			tmp.HitOnce = true;
			tmp.Damage = 5;
			tmp.HitVel_Stand = Vec3(-3, 5, 0);
			tmp.HitTime_Stand = .8f;
			tmp.Type = AttackType::Enemy;
			//tmp.ForceRecover = false;//ノックバックする
			m_enemyZako->DefAttack(.5f, tmp);
			m_enemyZako->GetAttackPtr()->SetPos(Vec3(2, 1, 0));
			auto AttackPtr = m_enemyZako->GetAttackPtr();
			AttackPtr->GetComponent<Transform>()->SetScale(Vec3(3.7f, 3.0f, 3.0f));
			AttackPtr->SetCollScale(1.0f);

			m_enemyZako->SetAttackFlag(false);//攻撃判定が複数発生させないようにする
			m_Attack = false;//攻撃判定が複数発生させないようにする


		}
		//アニメーション更新時間設定
		m_enemyZako->SetAddTimeAnimation(deltaTime * 2.5f);
		

		// 2) 突進終了判定
		// →時間制限を超える or 壁・プレイヤーへの衝突を検知したら止める
		if (m_chargeTime >= m_maxChargeTime || (m_enemyZako->GetAttackPtr()->GetMoveContact() && m_Attack))
		{

			m_enemyZako->SetVelocity(Vec3(0, 0, 0));      // 移動停止
			m_enemyZako->ChangeState(L"Stand");         // 次のステートへ
		}

	}

	void EnemyZakoFlyingChargeState::Exit()
	{
		m_Attack = true;
		m_enemyZako->SetVelocity(Vec3(0, 0, 0));
	}
	// END----------------------------------------------

	//接近戦をするときのステート----------------------
	void EnemyZakoFlyingMeleeState::Enter()
	{
		auto LandFlag = m_enemyZako->GetLand();
		auto testVector = m_enemyZako->GetVelocity();

		//攻撃っぽいアニメーションにしてみる
		m_enemyZako->ChangeAnim(L"Down");
		//m_enemyZako->ChangeAnim(L"Walk");//歩くアニメーションに変更

		//攻撃しているタグ追加
		m_enemyZako->AddTag(L"AttackNow");
	}

	void EnemyZakoFlyingMeleeState::Update(float deltaTime)
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
			m_enemyZako->GetAttackPtr()->SetPos(Vec3(2, 1, 0));
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
		m_Attack = true;
		m_timeOfAttack = 0.0f;

		//攻撃しているタグ削除
		m_enemyZako->RemoveTag(L"AttackNow");
	}
	// END-----------------------------------
	
	//プレイヤーとの距離によって脚のスピードを変える処理
	void EnemyZakoFlyingPreparationforMeleeState::SpeedChange()
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
	// END-----------------------------------------------

	// 遠距離の直前の軸合わせの時のステート-----
	void EnemyZakoFlyingAlignmentState::Enter()
	{

	}
	void EnemyZakoFlyingAlignmentState::Update(float deltaTime)
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
	void EnemyZakoFlyingAlignmentState::Exit()
	{
		//打つカウントダウンリセット
		m_timeOfShot = 0.0f;
	}

	// END--------------------------------------

	// 攻撃をするときのステート(遠距離)-----
	void EnemyZakoFlyingShotState::Enter()
	{
		auto stage = m_enemyZako->GetStage();
		auto posEnemy = m_enemyZako->GetPosition();
		//弾生成
		auto bullet = stage->AddGameObject<Bullet>(posEnemy, Vec3(0.0f), Vec3(0.4f), 30.0f,
			dynamic_pointer_cast<Actor>(m_enemyZako), 30.0f, ActorName_Enemy);

		m_enemyZako->ChangeAnim(L"Shot");//撃つアニメーションに変更
	}

	void EnemyZakoFlyingShotState::Update(float deltaTime)
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

	void EnemyZakoFlyingShotState::Exit()
	{
		//打つカウントダウンリセット
		m_timeOfAttack = 0.0f;
	}
	// END-----------------------------------

	//ダメージを受けたとき-------------------
	void EnemyZakoFlyingHitState::Enter()
	{
		auto hitInfo = m_enemyZako->GetHitInfo();
		auto HPNow = m_enemyZako->GetHPCurrent();
		//攻撃を受けたのでヒットバックする
		m_enemyZako->HitBack();
		//ダメージ処理
		m_enemyZako->SetHPCurrent(HPNow - hitInfo.Damage);

		m_enemyZako->ChangeAnim(L"Stand");//ダメージを受けたアニメーションに変更
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