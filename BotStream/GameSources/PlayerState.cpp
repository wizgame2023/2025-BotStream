/*!
@file PlayerState.cpp
@brief プレイヤーのステート
*/

#include "stdafx.h"
#include "Project.h"

#define AttackButton m_controller.wReleasedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER
#define DodgeButton m_controller.wReleasedButtons & XINPUT_GAMEPAD_A

namespace basecross {
	void PlayerStateBase::Enter()
	{
		m_SEManager = App::GetApp()->GetXAudio2Manager();

		//コントローラーを受け取る
		auto inputDevice = App::GetApp()->GetInputDevice();
		m_controller = inputDevice.GetControlerVec()[0];
	}
	void PlayerStateBase::Update(float deltaTime)
	{

		//コントローラーを受け取る
		auto inputDevice = App::GetApp()->GetInputDevice();
		m_controller = inputDevice.GetControlerVec()[0];

		//カメラマネージャ取得
		auto cameraManager = m_player->GetStage()->GetSharedGameObject<CameraManager>(L"CameraManager");
		

		//回避してよいかフラグを受け取る
		m_dodgeFlag = m_player->GetDodgeFlag();
		//接近戦指定以下のフラグ受け取る
		m_meleeFlag = cameraManager->GetMeleeFlag();

		////カメラマネージャからLockOnTargetの位置を取得する
		//shared_ptr<CameraManager> camera = nullptr;
		//camera = m_player->GetStage()->GetSharedGameObject<CameraManager>(L"CameraManager");
		//shared_ptr<Actor> targetObj = nullptr;
		//targetObj = camera->GetTargetObj();
		////ターゲット対象がいる時対象との距離を測る
		//if (targetObj)
		//{
		//	//Vec3 targetPos = Vec3(0.0f);
		//	//Vec3 playerPos = Vec3(0.0f);
		//	//targetPos = targetObj->GetComponent<Transform>()->GetPosition();
		//	//playerPos = m_player->GetComponent<Transform>()->GetPosition();
		//	////LockOnTargetとの距離を計算する
		//	m_targetDistance = camera->GetTargetDis();
		//}
		//else if (!targetObj)
		//{
		//	m_targetDistance = 0.0f;
		//}


	};
	//ターゲット対象との距離を取得する
	float PlayerStateBase::GetTargetDistance()
	{
		return m_targetDistance;
	}

	//Playerの歩くモーション
	void PlayerWalkState::Enter()
	{
		PlayerStateBase::Enter();
	}
	void PlayerWalkState::Update(float deltaTime)
	{
		PlayerStateBase::Update(deltaTime);
		Vec3 stick = Vec3(m_controller.fThumbLX, 0, m_controller.fThumbLY);
		
		//移動処理
		Vec3 move = m_player->GetMoveVector(PlayerState_Walk);
		m_player->PlayerMove(PlayerState_Walk);

		//歩きアニメーション再生
		if (move.length() != 0)
		{
			m_player->ChangeAnim(L"Walk");
		}
		else 
		{
			m_player->ChangeAnim(L"Idle");
		}

		//歩きステートのアニメーション再生
		m_player->SetAddTimeAnimation(deltaTime * 1.5f);
	
		//回避していいフラグ状態だったら回避ステートに変更
		if (m_controller.wPressedButtons & XINPUT_GAMEPAD_A)
		{
			if (m_dodgeFlag)
			{
				m_player->ChangeState(L"Dodge");
			}
		}
		//攻撃ステートに変更する　長押しだったら回転攻撃そうでなければ通常攻撃
		if (m_controller.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			m_timeOfPushAttackButton += deltaTime;//押している時間を測る
		}
		//攻撃するときの処理(刀か銃にするか)
		if (AttackButton)
		{
			if (m_meleeFlag)
			{//近距離
				if (m_timeOfPushAttackButton >= 1.5f)
				{//長押しなら最終段の技が出る
					m_player->ChangeState(L"AttackEx");
					m_player->AddEffect(PlayerEffect_AttackEx);//攻撃エフェクトを出す
				}
				if (m_timeOfPushAttackButton < 1.5f)
				{
					m_player->ChangeState(L"Attack1");
					m_player->AddEffect(PlayerEffect_Attack1);//攻撃エフェクトを出す
				}
			}
			else if (!m_meleeFlag)
			{//遠距離	
				auto BulletNumNow = m_player->GetBulletNum();
				//弾が残っていれば打てる
				if (BulletNumNow > 0)
				{
					m_player->ChangeState(L"AttackLong");
					m_player->SetBulletNum(--BulletNumNow);
				}
				if (BulletNumNow <= 0)
				{
					m_SEManager->Start(L"CantShotSE", 0, 0.9f);
				}
			}
			m_timeOfPushAttackButton = 0.0f;//押した時間リセット
		}
		//もしSPゲージがMAXであれば必殺技が打てる
		auto SPCurrent = m_player->GetSP();
		auto SPMAX = m_player->GetMaxSP();
		if (SPCurrent >= SPMAX)
		{
			if (m_controller.wPressedButtons & XINPUT_GAMEPAD_X && m_controller.wPressedButtons & XINPUT_GAMEPAD_A)
			{
				m_player->ChangeState(L"AttackSpecial");
			}
		}

	}
	void PlayerWalkState::Exit()
	{
		
	}

	//回避ステート
	void PlayerDodgeState::Enter()
	{
		PlayerStateBase::Enter();
		//PlayerStateBase::Update(0.0f);

		//回避する瞬間にスティックを傾けていたらその方向に進む
		auto m_stickL = Vec3(m_controller.fThumbLX,0,m_controller.fThumbLY);
		//m_player->SetStickL(m_stickL);
		if (m_stickL != Vec3(0.0f, 0.0f, 0.0f))
		{
			m_player->MoveAngle(m_stickL);
		}

		//回避SE
		m_SEManager->Start(L"Dash", 0, 0.9f);
		//回避アニメーション
		m_player->ChangeAnim(L"Dodge");


		//回避していいか確認する
		//m_playerDodgeFlag = m_player->GetDodgeFlag();
		//回避タグ追加
		m_player->AddTag(L"DodgeNow");

		//回避判定が開始されたことを伝える
		m_player->SetEndDodgeFlag(true);
	}
	void PlayerDodgeState::Update(float deltaTime)
	{
		// 入力デバイス取得
		PlayerStateBase::Update(deltaTime);

		//移動処理
		Vec3 move = m_player->GetMoveVector(PlayerState_Dodge);
		m_player->PlayerMove(PlayerState_Dodge);

		//アニメーション更新
		m_player->SetAddTimeAnimation(deltaTime * 1.7f);

		//回避した後の処理(どっちのステートに行くかの処理)
		bool endDodgeFlag = m_player->GetEndDodgeFlag();
		if (!endDodgeFlag)
		{
			//ダッシュステートにするか歩くステートにするか
			if (m_controller.wButtons & XINPUT_GAMEPAD_A)
			{
				m_player->ChangeState(L"Dash");
			}
			else
			{
				m_player->ChangeState(L"PlayerWalk");
			}
			//回避終了判定リセット
			m_player->SetEndDodgeFlag(true);
			//回避したので回避のクールタイムを入れる
			m_player->SetDodgeFlag(false);

		}

	}
	void PlayerDodgeState::Exit()//終了処理
	{
		//回避タグ削除
		m_player->RemoveTag(L"DodgeNow");
	}


	//ダッシュステート
	void PlayerDashState::Enter()
	{
		PlayerStateBase::Enter();
		//ダッシュ用SEを再生
		m_SE = m_SEManager->Start(L"Landing", XAUDIO2_LOOP_INFINITE, 0.9f);
	}
	void PlayerDashState::Update(float deltaTime)
	{
		// 入力デバイス取得
		PlayerStateBase::Update(deltaTime);


		//移動処理
		Vec3 move = m_player->GetMoveVector(PlayerState_Dash);
		m_player->PlayerMove(PlayerState_Dash);

		//歩きアニメーション再生
		if (move.length() != 0)
		{
			m_player->ChangeAnim(L"Dash");
		}
		else
		{
			m_player->ChangeAnim(L"Idle");
		}

		//ダッシュステートのアニメーション再生
		m_player->SetAddTimeAnimation(deltaTime * 1.5f);

		//Aボタン離したらorスティックを離したら歩くステートに変更する
		if (m_controller.wReleasedButtons & XINPUT_GAMEPAD_A)
		{
			m_player->ChangeState(L"PlayerWalk");
		}
		Vec3 stickVec = Vec3(m_controller.fThumbLX, 0, m_controller.fThumbLY);
		if (stickVec == Vec3(0.0f))
		{
			m_player->ChangeState(L"PlayerWalk");
		}

		//攻撃ステートに変更する　長押しだったら回転攻撃そうでなければ通常攻撃
		if (m_controller.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			m_timeOfPushAttackButton += deltaTime;//押している時間を測る
		}
		if (AttackButton)
		{
			//攻撃する際状況によってステートの遷移が変わる
			if (m_meleeFlag)
			{//近距離
				if (m_timeOfPushAttackButton >= 1.5f)
				{//長押しなら最終段の技が出る
					m_player->ChangeState(L"AttackEx");
					m_player->AddEffect(PlayerEffect_AttackEx);//攻撃エフェクトを出す
				}
				if (m_timeOfPushAttackButton < 1.5f)
				{
					m_player->ChangeState(L"Attack1");
					m_player->AddEffect(PlayerEffect_Attack1);//攻撃エフェクトを出す
				}
			}
			else if (!m_meleeFlag)
			{//遠距離	
				auto BulletNumNow = m_player->GetBulletNum();
				//弾が残っていれば打てる
				if (BulletNumNow > 0)
				{
					m_player->ChangeState(L"AttackLong");
					m_player->SetBulletNum(--BulletNumNow);
				}
				if (BulletNumNow <= 0)
				{
					m_SEManager->Start(L"CantShotSE", 0, 0.9f);
				}
			}
			m_timeOfPushAttackButton = 0.0f;//押した時間リセット
		}
		//もしSPゲージがMAXであれば必殺技が打てる
		auto SPCurrent = m_player->GetSP();
		auto SPMAX = m_player->GetMaxSP();
		if (SPCurrent >= SPMAX)
		{
			if (m_controller.wPressedButtons & XINPUT_GAMEPAD_X && m_controller.wPressedButtons & XINPUT_GAMEPAD_A)
			{
				m_player->ChangeState(L"AttackSpecial");
			}
		}
	}
	void PlayerDashState::Exit()
	{
		//ダッシュSEを止める
		m_SEManager->Stop(m_SE);
	}


	//攻撃ステート(一番最初に出てくる攻撃)
	void PlayerAttack1State::Enter()
	{
		PlayerStateBase::Enter();

		m_SE = m_SEManager->Start(L"Attack1", 0, 0.9f);//SE再生

		//Attack1アニメーションに変更
		m_player->ChangeAnim(L"Attack1");
	}
	void PlayerAttack1State::Update(float deltaTime)
	{
		// 入力デバイス取得
		PlayerStateBase::Update(deltaTime);

		//アニメーションの更新
		m_player->SetAddTimeAnimation(deltaTime * 1.5f);
		//移動処理
		m_player->PlayerMove(PlayerState_Attack1);
    
		//攻撃判定の定義
		if (m_timeOfAttack >= m_timeOfStartAttack && AttackCollisionFlag)
		{
			auto tmp = m_player->GetAttackPtr()->GetHitInfo();
			//tmp.HitSound = L"Attack1";

			tmp.HitOnce = true;//一回しかヒットしないか
			tmp.Damage = 10;//ダメージ
			tmp.HitVel_Stand = Vec3(-2, 1, 0);//ヒットバック距離 本来のヒットバックはVec3(-2,5,0)
			tmp.HitTime_Stand = .5f;//のけぞり時間なし
			//tmp.ForceRecover = true;
			m_player->DefAttack(.5f, tmp);
			m_player->GetAttackPtr()->SetPos(Vec3(3, 1, 0));
			auto AttackPtr = m_player->GetAttackPtr();
			AttackPtr->GetComponent<Transform>()->SetScale(6.0f, 5.0f, 3.0f);
			AttackPtr->SetCollScale(1.0f);
			AttackPtr->ActivateCollision(m_timeMaxOfAttackFirst);

			//攻撃判定はもう出ない
			AttackCollisionFlag = false;
		}

		//攻撃の時間計測
		m_timeOfAttack += deltaTime;
		//猶予時間以内に攻撃ボタンを押せたら次の攻撃ステートに遷移できる
		//if (m_timeOfAttack < m_graceTimeOfNextAttack)
		//{
			//攻撃ステートに変更する
			if (AttackButton)
			{			
				m_nestAttackFlag = true;	
			}
		//}

		//一定時間たったら回避行動ができる(一段目と二段目の攻撃のみ)
		if (m_timeOfAttack > m_timeOfStartDodge)
		{
			if (DodgeButton)
			{
				m_player->ChangeState(L"Dodge");
			}
		}//デバック用にどれぐらい時間がたっているのか確認する
		if (DodgeButton)
		{
			auto test = m_timeOfAttack;
			auto a = 0;
		}


		//攻撃の時間を越えたら別のステートに移動する
		if (m_timeOfAttack >= m_timeMaxOfAttack)
		{	
			auto stage = m_player->GetStage();

			//ステート遷移
			if (m_nestAttackFlag)
			{
				m_player->ChangeState(L"Attack2");////次の攻撃ステートに移動
				m_player->AddEffect(PlayerEffect_Attack2);//攻撃エフェクトを出す
			}
			else
			{
				//stage->RemoveGameObject<Cube>(m_AttackObj);//攻撃判定削除
				m_player->ChangeState(L"PlayerWalk");
			}
		}
		
	}
	void PlayerAttack1State::Exit()
	{
		auto stage = m_player->GetStage();
		//stage->RemoveGameObject<Cube>(m_AttackObj);//攻撃判定削除
		m_timeOfAttack = 0.0f;//リセット
		m_nestAttackFlag = false;
		AttackCollisionFlag = true;//リセット
	}


	//攻撃ステート(２番目に出る攻撃)
	void PlayerAttack2State::Enter()
	{
		PlayerStateBase::Enter();
		m_SE = m_SEManager->Start(L"Attack1", 0, 0.9f);//SE再生

		//Attack2アニメーションに変更
		m_player->ChangeAnim(L"Attack2");
	

	}
	void PlayerAttack2State::Update(float deltaTime)
	{
		// 入力デバイス取得
		PlayerStateBase::Update(deltaTime);

		//アニメーションの更新
		m_player->SetAddTimeAnimation(deltaTime * 1.9f);
		//移動処理
		m_player->PlayerMove(PlayerState_Attack2);

		//攻撃の時間計測
		m_timeOfAttack += deltaTime;

		//攻撃判定の定義(一撃目)
		if (m_timeOfAttack >= m_timeOfStartAttackFirst && m_attackCollisionFlag == 0)
		{
			auto tmp = m_player->GetAttackPtr()->GetHitInfo();
			tmp.HitOnce = true;
			tmp.Damage = 12;
			tmp.HitVel_Stand = Vec3(-2, 5, 0);//ヒットバック距離 本来のヒットバックはVec3(-2,5,0)
			tmp.HitTime_Stand = .5f;//のけぞり時間なし
			m_player->DefAttack(.5f, tmp);
			m_player->GetAttackPtr()->SetPos(Vec3(3, 1, 0));
			auto AttackPtr = m_player->GetAttackPtr();
			AttackPtr->GetComponent<Transform>()->SetScale(3.0f, 5.0f, 5.0f);//横長の当たり判定
			AttackPtr->SetCollScale(1.0f);
			AttackPtr->ActivateCollision(m_timeMaxOfAttackFirst);

			//一段目の攻撃はもう出ない
			m_attackCollisionFlag = 1;
		}
		//攻撃判定の定義(2撃目)
		if (m_timeOfAttack >= m_timeOfStartAttackSecond && m_attackCollisionFlag == 1)
		{
			auto tmp = m_player->GetAttackPtr()->GetHitInfo();
			tmp.HitOnce = true;
			tmp.Damage = 12;
			tmp.HitVel_Stand = Vec3(-2, 5, 0);//ヒットバック距離 本来のヒットバックはVec3(-2,5,0)
			tmp.HitTime_Stand = .5f;//のけぞり時間なし
			m_player->DefAttack(.5f, tmp);
			m_player->GetAttackPtr()->SetPos(Vec3(3, 1, 0));
			auto AttackPtr = m_player->GetAttackPtr();
			AttackPtr->GetComponent<Transform>()->SetScale(6.0f, 5.0f, 3.0f);//縦長の当たり判定
			AttackPtr->SetCollScale(1.0f);
			AttackPtr->ActivateCollision(m_timeMaxOfAttackSecond);

			//２段目の攻撃はもうない
			m_attackCollisionFlag = 2;
		}

		//猶予時間以内に攻撃ボタンを押せたら次の攻撃ステートに遷移できる
		//if (m_timeOfAttack < m_graceTimeOfNextAttack)
		//{
			//攻撃ステートに変更する
			if (AttackButton)
			{
				m_nestAttackFlag = true;
			}
		//}
		//一定時間たったら回避行動ができる(一段目と二段目の攻撃のみ)
		if (m_timeOfAttack > m_timeOfStartDodge)
		{
			if (DodgeButton)
			{
				m_player->ChangeState(L"Dodge");
			}
		}


		//攻撃の時間を越えたら別のステートに移動する
		if (m_timeOfAttack >= m_timeMaxOfAttackTotal)
		{
			auto stage = m_player->GetStage();
			m_timeOfAttack = 0.0f;//リセット

			//ステート遷移
			if (m_nestAttackFlag)
			{
				//stage->RemoveGameObject<Cube>(m_AttackObj);//攻撃判定削除
				m_player->ChangeState(L"Attack3");//次の攻撃ステートに移動
				m_player->AddEffect(PlayerEffect_Attack3);//攻撃エフェクトを出す
			}
			else
			{
				//stage->RemoveGameObject<Cube>(m_AttackObj);//攻撃判定削除
				m_player->ChangeState(L"PlayerWalk");
			}
		}
	}
	void PlayerAttack2State::Exit()
	{
		auto stage = m_player->GetStage();
		//stage->RemoveGameObject<Cube>(m_AttackObj);//攻撃判定削除
		m_timeOfAttack = 0.0f;//リセット
		m_nestAttackFlag = false;
		m_attackCollisionFlag = 0;//リセット
	}


	//攻撃ステート(3番目に出る攻撃)
	void PlayerAttack3State::Enter()
	{
		PlayerStateBase::Enter();
		m_SE = m_SEManager->Start(L"Attack2", 0, 0.9f);//SE再生

		//Attack3アニメーションに変更
		m_player->ChangeAnim(L"Attack3");

	}
	void PlayerAttack3State::Update(float deltaTime)
	{
		// 入力デバイス取得
		PlayerStateBase::Update(deltaTime);

		//アニメーションの更新
		m_player->SetAddTimeAnimation(deltaTime * 1.8f);
		//移動処理
		m_player->PlayerMove(PlayerState_Attack3);

		//攻撃の時間計測
		m_timeOfAttack += deltaTime;

		//攻撃判定の定義(１撃目)
		if (m_timeOfAttack >= m_timeOfStartAttackFirst && m_attackCollisionFlag == 0)
		{
			auto tmp = m_player->GetAttackPtr()->GetHitInfo();
			tmp.HitOnce = true;
			tmp.Damage = 18;
			tmp.HitVel_Stand = Vec3(-10, 5, 0);//ヒットバック距離
			tmp.HitTime_Stand = .5f;
			m_player->DefAttack(.5f, tmp);
			m_player->GetAttackPtr()->SetPos(Vec3(3, 1, 0));
			auto AttackPtr = m_player->GetAttackPtr();
			AttackPtr->GetComponent<Transform>()->SetScale(5.0f, 5.0f, 7.0f);
			AttackPtr->SetCollScale(1.0f);
			AttackPtr->ActivateCollision(m_timeMaxOfAttackFirst);

			//攻撃判定はもう出ない(１撃目)
			m_attackCollisionFlag = 1;
		}
		//攻撃判定の定義(２撃目)
		if (m_timeOfAttack >= m_timeOfStartAttackSecond && m_attackCollisionFlag == 1)
		{
			auto tmp = m_player->GetAttackPtr()->GetHitInfo();
			tmp.HitOnce = true;
			tmp.Damage = 18;
			tmp.HitVel_Stand = Vec3(-10, 5, 0);//ヒットバック距離
			tmp.HitTime_Stand = .5f;
			m_player->DefAttack(.5f, tmp);
			m_player->GetAttackPtr()->SetPos(Vec3(3, 1, 0));
			auto AttackPtr = m_player->GetAttackPtr();
			AttackPtr->GetComponent<Transform>()->SetScale(5.0f, 5.0f, 7.0f);
			AttackPtr->SetCollScale(1.0f);
			AttackPtr->ActivateCollision(m_timeMaxOfAttackSecond);

			//攻撃判定はもう出ない(１撃目)
			m_attackCollisionFlag = 2;
		}

		//猶予時間以内に攻撃ボタンを押せたら次の攻撃ステートに遷移できる
		//if (m_timeOfAttack < m_graceTimeOfNextAttack)
		//{
			//攻撃ステートに変更する
			if (AttackButton)
			{
				m_nestAttackFlag = true;
			}
		//}
		//攻撃の時間を越えたら別のステートに移動する
		if (m_timeOfAttack >= m_timeMaxOfAttackTotal)
		{
			auto stage = m_player->GetStage();
			m_timeOfAttack = 0.0f;//リセット

			//ステート遷移
			if (m_nestAttackFlag)
			{
				m_player->ChangeState(L"AttackEx");//次の攻撃ステートに移動
				m_player->AddEffect(PlayerEffect_AttackEx);//攻撃エフェクトを出す
			}
			else
			{
				//stage->RemoveGameObject<Cube>(m_AttackObj);//攻撃判定削除
				m_player->ChangeState(L"PlayerWalk");
			}
		}
	}
	void PlayerAttack3State::Exit()
	{
		auto stage = m_player->GetStage();
		m_timeOfAttack = 0.0f;//リセット
		m_nestAttackFlag = false;
		m_attackCollisionFlag = 0;//リセット
	}


	//攻撃ステート(4番目に出る攻撃)
	void PlayerAttackExState::Enter()
	{
		PlayerStateBase::Enter();
		m_SE = m_SEManager->Start(L"Attack3", 0, 0.9f);//SE再生

		//AttackExアニメーションに変更
		m_player->ChangeAnim(L"AttackEx");


	}
	void PlayerAttackExState::Update(float deltaTime)
	{
		//攻撃の時間計測
		m_timeOfAttack += deltaTime;

		//アニメーションの更新
		m_player->SetAddTimeAnimation(deltaTime * 2.2f);

		//攻撃判定の定義
		if (m_timeOfAttack >= m_timeOfStartAttack && AttackCollisionFlag)
		{
			auto tmp = m_player->GetAttackPtr()->GetHitInfo();
			tmp.HitOnce = true;
			tmp.Damage = 25;
			tmp.HitVel_Stand = Vec3(-20, 5, 0);//ヒットバック距離
			tmp.HitTime_Stand = .8f;
			//tmp.ForceRecover = false;//ノックバックする
			m_player->DefAttack(.5f, tmp);
			m_player->GetAttackPtr()->SetPos(Vec3(0, 1, 0));
			auto AttackPtr = m_player->GetAttackPtr();
			AttackPtr->GetComponent<Transform>()->SetScale(10.0f, 5.0f, 10.0f);
			AttackPtr->SetCollScale(1.0f);
			AttackPtr->ActivateCollision(m_timeMaxOfAttack);

			//攻撃判定はもう出ない
			AttackCollisionFlag = false;
		}

		//攻撃の時間を越えたら別のステートに移動する
		if (m_timeOfAttack >= m_timeMaxOfAttack)
		{
			auto stage = m_player->GetStage();
			m_timeOfAttack = 0.0f;//リセット

			m_player->ChangeState(L"PlayerWalk");
		}
	}
	void PlayerAttackExState::Exit()
	{
		auto stage = m_player->GetStage();
		m_timeOfAttack = 0.0f;//リセット
		AttackCollisionFlag = true;//リセット
	}


	//必殺技
	void PlayerAttackSpecialState::Enter()
	{
		PlayerStateBase::Enter();
		//m_SE = m_SEManager->Start(L"Attack3", 0, 0.9f);//SE再生

		//攻撃の当たり判定を出す
		auto stage = m_player->GetStage();
		//攻撃の当たり判定(仮)
		m_AttackObj = stage->AddGameObject<Cube>(Vec3(0.0f, 2.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Col4(0.0f, 0.0f, 0.0f, 1.0f));
		m_player->SetSP(0);//SPゲージをリセットする

		//攻撃判定の定義
		if (m_timeOfAttack <= 0) {
			auto tmp = m_player->GetAttackPtr()->GetHitInfo();
			tmp.HitOnce = true;
			tmp.Damage = 100;
			tmp.HitVel_Stand = Vec3(-1, 5, 0);
			tmp.HitTime_Stand = .8f;
			//tmp.ForceRecover = false;//ノックバックする
			m_player->DefAttack(.5f, tmp);
			m_player->GetAttackPtr()->SetPos(Vec3(1, 1, 0));
			auto AttackPtr = m_player->GetAttackPtr();
			AttackPtr->SetCollScale(12.0f);
		}

	}
	void PlayerAttackSpecialState::Update(float deltaTime)
	{
		//攻撃の時間計測
		m_timeOfAttack += deltaTime;

		m_player->AddEffect(PlayerEffect_Attack3);//攻撃エフェクトを出す

		//攻撃の時間を越えたら別のステートに移動する
		if (m_timeOfAttack >= m_timeMaxOfAttack)
		{
			auto stage = m_player->GetStage();
			m_timeOfAttack = 0.0f;//リセット

			m_player->ChangeState(L"PlayerWalk");
		}

	}
	void PlayerAttackSpecialState::Exit()
	{
		auto stage = m_player->GetStage();
		stage->RemoveGameObject<Cube>(m_AttackObj);//攻撃判定削除
		m_timeOfAttack = 0.0f;//リセット
	}



	void PlayerAttackLongState::Enter()
	{
		PlayerStateBase::Enter();

		//球を出す
		auto stage = m_player->GetStage();
		auto playerPos = m_player->GetPosition();
		m_SEManager->Start(L"HandGun", 0, 0.9f);//銃SE再生
		auto bullet = stage->AddGameObject<Bullet>(playerPos+Vec3(0.0f,2.0f,0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), 30.0f, m_player, 30.0f);
		//m_player->AddEffect(PlayerEffect_Beam);//攻撃エフェクトを出す
	}
	void PlayerAttackLongState::Update(float deltaTime)
	{
		//攻撃の時間計測
		m_timeOfAttack += deltaTime;

		//攻撃の時間を越えたら別のステートに移動する
		if (m_timeOfAttack >= m_timeMaxOfAttack)
		{
			auto stage = m_player->GetStage();
			m_timeOfAttack = 0.0f;//リセット

			m_player->ChangeState(L"PlayerWalk");
		}

	}
	void PlayerAttackLongState::Exit()
	{
		auto stage = m_player->GetStage();
		stage->RemoveGameObject<Cube>(m_AttackObj);//攻撃判定削除
		m_timeOfAttack = 0.0f;//リセット
	}


	//ダメージを受けた際のステート
	void PlayerHitState::Enter()
	{

		PlayerStateBase::Enter();

		m_SEManager->Start(L"DamageVoiceSE");
		auto hitInfo = m_player->GetHitInfo();
		auto HPNow = m_player->GetHP();
		//hitInfo.Damage
		m_player->HitBack();
		m_player->SetHP(HPNow - hitInfo.Damage);
	}
	void PlayerHitState::Update(float deltaTime)
	{
		m_timeOfHitBack += deltaTime;
		//ノックバックの時間を越えたら別のステートに移動する
		if (m_timeOfHitBack >= m_timeMaxOfHitBack)
		{
			auto stage = m_player->GetStage();
			m_timeOfHitBack = 0.0f;//リセット

			m_player->ChangeState(L"PlayerWalk");
		}

	}
	void PlayerHitState::Exit()
	{

	}



	//雑魚敵のステートマージした後EnemyStateファイルに移動する
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
			m_enemyZako->ChangeState(L"Alignment");//軸合わせから始まる
		}
		//近距離
		if (attackType == m_enemyZako->Zako_Melee && isLand)
		{
			m_enemyZako->ChangeState(L"PreparationforMelee");//接近して攻撃する
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
		auto meleeRange = 8.0f;//接近攻撃有効範囲

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


	//球を打つ直前の軸合わせのステート
	void EnemyZakoAlignmentState::Enter()
	{

	}
	void EnemyZakoAlignmentState::Update(float deltaTime)
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
	void EnemyZakoAlignmentState::Exit()
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
			dynamic_pointer_cast<Actor>(m_enemyZako),30.0f,ActorName_Enemy);

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

}
//end basecross
