/*!
@file PlayerState.cpp
@brief プレイヤーのステート
*/

#include "stdafx.h"
#include "Project.h"


namespace basecross {
	#define ControllerAttackButton m_controller.wReleasedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER	
	
	//ダッシュ関係
	#define MouseDashButton App::GetApp()->GetInputDevice().GetKeyState().m_bPushKeyTbl[VK_SPACE]
	#define ControllerDashButton (m_controller.wButtons & XINPUT_GAMEPAD_A)
	#define DashButton (MouseDashButton || ControllerDashButton)

	//回避関係
	#define ControllerDodgeButton m_controller.wPressedButtons & XINPUT_GAMEPAD_A
	#define MouseDodgeButton App::GetApp()->GetInputDevice().GetKeyState().m_bPressedKeyTbl[VK_SPACE]
	#define DodgeButton (ControllerDodgeButton || MouseDodgeButton)


	#define MouseAimButton GetAsyncKeyState(VK_RBUTTON) & 0x8000
	#define MouseAttackButton App::GetApp()->GetInputDevice().GetKeyState().m_bPressedKeyTbl[VK_LBUTTON]
	#define MouseGunButton App::GetApp()->GetInputDevice().GetKeyState().m_bPressedKeyTbl[VK_RBUTTON]
	#define MouseGunCancellationButton App::GetApp()->GetInputDevice().GetKeyState().m_bUpKeyTbl[VK_RBUTTON]
	#define MouseControllerCancellationButton m_controller.wReleasedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER

	#define AttackButton (ControllerAttackButton || MouseAttackButton)

	void PlayerStateBase::Enter()
	{
		m_SEManager = App::GetApp()->GetXAudio2Manager();

		// コントローラーを受け取る
		auto inputDevice = App::GetApp()->GetInputDevice();
		m_controller = inputDevice.GetControlerVec()[0];

		// Playerがどの行動をしていいかのフラグを受け取る
		m_attackFlag = m_player->GetAttackFlag();
		m_dodgeFlag = m_player->GetDodgeFlag();
		m_walkFlag = m_player->GetAttackFlag();

		m_keyState = App::GetApp()->GetInputDevice().GetKeyState();

		// SEのボリュームを受け取る
		m_SEVol = m_player->GetSEVol();
	}
	void PlayerStateBase::Update(float deltaTime)
	{
		m_deltaTime = deltaTime;

		// コントローラーを受け取る
		auto inputDevice = App::GetApp()->GetInputDevice();
		m_controller = inputDevice.GetControlerVec()[0];
		// キーボード,マウスを受け取る
		m_keyState = App::GetApp()->GetInputDevice().GetKeyState();

		// SEのボリュームを受け取る
		m_SEVol = m_player->GetSEVol();

		// カメラマネージャ取得
		auto cameraManager = m_player->GetStage()->GetSharedGameObject<CameraManager>(L"CameraManager");
		
		// 回避してよいかフラグを受け取る
		m_dodgeFlag = m_player->GetDodgeFlag();
		// 接近戦していいかのフラグ受け取る
		m_meleeFlag = cameraManager->GetMeleeFlag();

		// 接近戦していいかのフラグを管理する
		if (m_controller.wPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER || MouseGunButton)
		{
			m_meleeFlag = false;
			m_player->SetMeleeFlag(false);
		}
		else if(m_controller.wReleasedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER || MouseGunCancellationButton)
		{
			m_meleeFlag = true;
			m_player->SetMeleeFlag(true);
			//m_player->GetStage()->GetSharedGameObject<CameraManager>(L"CameraManager")->SetGunNow(true);

		}
	};
	// ターゲット対象との距離を取得する
	float PlayerStateBase::GetTargetDistance()
	{
		return m_targetDistance;
	}

	// 移動についての処理
	void PlayerStateBase::Walk(int state, bool onOff)
	{
		// フラグがオフなら移動しない
		if (!onOff) return;

		// 移動処理
		m_player->PlayerMove(state);
	}

	// 回避についての処理
	void PlayerStateBase::Dodge(bool onOff)
	{
		// フラグがオフなら移動しない
		if (!onOff) return;

		// 回避していいフラグ状態だったら回避ステートに変更
		if (DodgeButton)
		{
			m_player->ChangeState(L"Dodge");
		}
	}

	// Playerの動く元となるステート//////////////////////////////////////////////////////////////////////////////////
	// 攻撃についての処理
	void PlayerMoveStateBase::AttackTransition(bool onOff)
	{
		// 攻撃していい判定でなければ通らない
		if (!onOff) return;

		//攻撃ステートに変更する　長押しだったら回転攻撃そうでなければ通常攻撃
		if (m_controller.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			//近接攻撃していい状態以外はチャージできない
			if (m_meleeFlag)
			{
				if (m_timeOfPushAttackButton == 0.0f)
				{
					m_chargeSE = m_SEManager->Start(L"ChargeSE", 0, 0.9 * m_SEVol);
				}
				if (m_timeOfPushAttackButton >= 0.5f && !m_chargeEffectFlag)
				{
					m_chargeEffect = m_player->AddEffect(PlayerEffect_Charge);
					m_chargeEffectFlag = true;
				}

				Vec3 playerPos = m_player->GetPosition();
				EffectManager::Instance().SetPosition(m_chargeEffect, playerPos);
				EffectManager::Instance().SetRotationFromAxisAngle(m_chargeEffect, Vec3(0, 1, 0), m_player->GetAngle() + XM_PIDIV2);

				// 押している時間を測る
				m_timeOfPushAttackButton += m_deltaTime;
			}
		}
		// 攻撃するときの処理(刀か銃にするか)
		if (AttackButton)
		{
			if (m_meleeFlag)
			{
				// 近距離
				if (m_timeOfPushAttackButton >= 1.36f)
				{
					// 長押しなら最終段の技が出る
					m_player->ChangeState(L"AttackEx");
					// 攻撃エフェクトを出す
					m_player->AddEffect(PlayerEffect_AttackEx);
				}
				else if (m_timeOfPushAttackButton < 1.36f)
				{
					m_player->ChangeState(L"Attack1");
				}
			}
			else if (!m_meleeFlag)
			{
				// 遠距離	
				auto BulletNumNow = m_player->GetBulletNum();
				// 弾が残っていれば打てる
				if (BulletNumNow > 0)
				{
					m_player->ChangeState(L"AttackLong");
					m_player->SetBulletNum(--BulletNumNow);
				}
				if (BulletNumNow <= 0)
				{
					m_SEManager->Start(L"CantShotSE", 0, 0.9f * m_SEVol);
				}
			}

		}

		//// もしSPゲージがMAXであれば必殺技が打てる　もしかしたら復活するかもしれないので残す
		//auto SPCurrent = m_player->GetSP();
		//auto SPMAX = m_player->GetMaxSP();
		//if (SPCurrent >= SPMAX)
		//{
		//	if (m_controller.wPressedButtons & XINPUT_GAMEPAD_X && m_controller.wPressedButtons & XINPUT_GAMEPAD_A)
		//	{
		//		m_player->ChangeState(L"AttackSpecial");
		//	}
		//}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	// Playerの歩くモーション
	void PlayerWalkState::Enter()
	{
		//初期化はちゃんと最初にしよう

		PlayerStateBase::Enter();

		// 何もなければ立ち止まるアニメーション
		//m_player->ChangeAnim(L"Idle");

		////現在攻撃していないことを渡す
		//m_player->SetMeleeNow(false);
		//m_player->SetGunNow(false);

		m_timeOfPushAttackButton = 0.0f;
		m_chargeEffectFlag = false;
	}
	void PlayerWalkState::Update(float deltaTime)
	{
		////プレイヤーが地面に接触していなければ操作は効かない(問題が発生したのでコメントアウト)
		//auto playerLand = m_player->GetLand();
		//if (!playerLand) return;

		PlayerStateBase::Update(deltaTime);
		Vec3 stick = Vec3(m_controller.fThumbLX, 0, m_controller.fThumbLY);

		//移動処理
		Vec3 move = m_player->GetMoveVector(PlayerState_Walk);
		Walk(PlayerState_Walk, m_walkFlag);

		//歩きアニメーション再生(移動しているかによって変わる)
		if (move.length() != 0 && m_meleeFlag)
		{
			m_player->ChangeAnim(L"Walk");
		}
		else if (move.length() != 0 && !m_meleeFlag)
		{
			m_player->ChangeAnim(L"Walk_Gun");
		}
		else if(m_meleeFlag)
		{
			m_player->ChangeAnim(L"Idle");
		}
		else if (!m_meleeFlag)
		{
			m_player->ChangeAnim(L"Shot_Gun");
		}

		// 歩きステートのアニメーション再生
		m_player->SetAddTimeAnimation(deltaTime * 1.5f);
	
		// 回避していいフラグ状態だったら回避ステートに変更
		Dodge(m_dodgeFlag);

		//攻撃の処理
		AttackTransition(m_attackFlag);
	}

	void PlayerWalkState::Exit()
	{
		// チャージ関係リセット
		if (m_chargeSE != nullptr)
		{
			m_SEManager->Stop(m_chargeSE);
			m_chargeSE.reset();
		}
		EffectManager::Instance().StopEffect(m_chargeEffect);
	}



	//回避ステート
	void PlayerDodgeState::Enter()
	{
		PlayerStateBase::Enter();

		//現在攻撃していないことを渡す
		m_player->SetMeleeNow(false);
		m_player->SetGunNow(false);

		//回避する瞬間にスティックを傾けていたらその方向に進む
		m_player->PlayerMove(PlayerState_Dodge);
		auto m_stickL = m_player->GetStickL();
		if (m_stickL != Vec3(0.0f, 0.0f, 0.0f))
		{
			m_player->MoveAngle(m_stickL);
		}

		//回避SE
		m_SEManager->Start(L"Dash", 0, 0.9f * m_SEVol);
		//回避アニメーション
		m_player->ChangeAnim(L"Dodge");
		//回避エフェクト
		m_effect = m_player->AddEffect(PlayerEffect_Dodge);


		//回避タグ追加
		m_player->AddTag(L"invincible");
		m_player->AddTag(L"Dodge");

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
		auto stickL = m_player->GetStickL();

		//アニメーション更新
		m_player->SetAddTimeAnimation(deltaTime * 1.7f);

		//回避した後の処理(どっちのステートに行くかの処理)
		bool endDodgeFlag = m_player->GetEndDodgeFlag();
		if (!endDodgeFlag)
		{
			//ダッシュステートにするか歩くステート,ブレーキステートにするか
 			if (DashButton && stickL != Vec3(0))
			{
				m_player->ChangeState(L"Dash");
			}
			else if (!DashButton && stickL != Vec3(0))
			{
				m_player->ChangeState(L"PlayerWalk");
			}
			else
			{
				m_player->ChangeState(L"DashEnd");
			}
			//回避終了判定リセット
			m_player->SetEndDodgeFlag(true);
			//回避したので回避のクールタイムを入れる
			m_player->SetDodgeFlag(false);
		}

		//エフェクトを追従させる
		EffectManager::Instance().SetPosition(m_effect, m_player->GetPosition());

	}
	void PlayerDodgeState::Exit()//終了処理
	{
		//回避タグ削除
		m_player->RemoveTag(L"invincible");
		m_player->RemoveTag(L"Dodge");
	}


	// ダッシュステート
	void PlayerDashState::Enter()
	{
		PlayerStateBase::Enter();
		// ダッシュ用SEを再生
		m_SE = m_SEManager->Start(L"Landing", 0, 0.9f * m_SEVol);
		// ダッシュ用エフェクトを再生
		m_effect = m_player->AddEffect(PlayerEffect_Dash);

		// 初期化処理
		m_countTimeOfDashSE = 0.0f;
		m_countTimeOfDashButton = 0.0f;
		m_timeOfPushAttackButton = 0.0f;
		m_chargeEffectFlag = false;
	}

	void PlayerDashState::Update(float deltaTime)
	{		
		m_countTimeOfDashSE += deltaTime;
		if (m_countTimeOfDashSE >= 0.6f)
		{
			// ダッシュ用SEを再生
			m_SE = m_SEManager->Start(L"Landing", 0, 0.9f * m_SEVol);
			m_countTimeOfDashSE = 0.0f;
		}

		// 入力デバイス取得
		PlayerStateBase::Update(deltaTime);

		//ダッシュ用SEの音量の更新
		auto SEVoice = m_SE->m_SourceVoice;
		if (SEVoice)
		{
			SEVoice->SetVolume(0.9f * m_SEVol);
		}

		// エフェクト追従処理
		Vec3 playerPos = m_player->GetPosition();
		Quat playerQt = m_player->GetComponent<Transform>()->GetQuaternion();

		EffectManager::Instance().SetPosition(m_effect, playerPos+Vec3(0.0f,1.9f,0.0f));
		EffectManager::Instance().SetRotationFromAxisAngle(m_effect, Vec3(0, 1, 0), m_player->GetAngle() + XM_PIDIV2);

		// 移動処理
		Vec3 move = m_player->GetMoveVector(PlayerState_Dash);
		Walk(PlayerState_Dash, m_walkFlag);

		// ダッシュステートのアニメーション再生
		m_player->SetAddTimeAnimation(deltaTime * 1.5f);

		// 歩きアニメーション変更処理
		if (move.length() != 0 && m_meleeFlag)
		{
			m_player->ChangeAnim(L"Dash");
		}
		else if (move.length() != 0 && !m_meleeFlag)
		{
			m_player->ChangeAnim(L"Walk_Gun");
			m_player->SetAddTimeAnimation(deltaTime * 2.5f);
		}
		else
		{
			m_player->ChangeAnim(L"Idle");
		}


		// Aボタン離したらorスティックを離したら歩くステートに変更する	
		Vec3 stickVec = m_player->GetStickL();

		// ダッシュボタンを一定時間離したらダッシュをやめる
		if (!DashButton)
		{
			m_releasedDashButton = true;
		}
		if (m_releasedDashButton)
		{
			m_countTimeOfDashButton += deltaTime;

			if (DashButton)
			{
				m_releasedDashButton = false;
			}

		}
		
		// ダッシュをやめるときに移動をやめているかによって移行するステートを変える
		if (m_countTimeOfDashButton >= m_timeOfDashButton)
		{
			if (stickVec != Vec3(0.0f))
			{
				m_player->ChangeState(L"PlayerWalk");
			}
			if (stickVec == Vec3(0.0f))
			{
				m_player->ChangeState(L"DashEnd");
			}

			//リセット
			m_countTimeOfDashButton = 0.0f;
			m_releasedDashButton = false;
		}

		if (move == Vec3(0.0f))
		{
			m_player->ChangeState(L"DashEnd");
		}

		// 攻撃の処理
		AttackTransition(m_attackFlag);
	}

	void PlayerDashState::Exit()
	{
		// ダッシュSEを止める
		m_SEManager->Stop(m_SE);
		// ダッシュ用のエフェクトを止める
		EffectManager::Instance().StopEffect(m_effect);

		// チャージ関係リセット
		if (m_chargeSE != nullptr)
		{
			m_SEManager->Stop(m_chargeSE);
			m_chargeSE.reset();

		}		
		
		EffectManager::Instance().StopEffect(m_chargeEffect);
	}


	// ダッシュ終了ステート
	void PlayerDashEndState::Enter()
	{
		PlayerStateBase::Enter();

		// アニメーション変更
		m_player->ChangeAnim(L"DashEnd");
	}
	void PlayerDashEndState::Update(float deltaTime)
	{
		// 入力デバイス取得
		PlayerStateBase::Update(deltaTime);

		//アニメーション更新
		m_player->SetAddTimeAnimation(deltaTime * 1.7f);

		m_timeOfAnimation += deltaTime;

		if (m_timeOfAnimation >= 0.735f)
		{
			m_player->ChangeState(L"PlayerWalk");
		}

		Walk(PlayerState_DashEnd,m_walkFlag);

		// 攻撃移行可能
		if (AttackButton)
		{
			m_player->ChangeState(L"Attack1");
		}
		// 回避移行可能
		Dodge(m_dodgeFlag);


		//ブレーキした後の処理(歩きステートに移行)
		bool endBrakeFlag = m_player->GetEndBrakeFlag();
		if (endBrakeFlag)
		{
			m_player->ChangeState(L"PlayerWalk");
		}
		
	}
	void PlayerDashEndState::Exit()
	{
		// リセット
		m_timeOfAnimation = 0.0f;
		m_player->SetEndBrakeFlag(false);
		m_player->ResetBrakeTime();
	}
	//

	//攻撃ステートの元となるクラス	
	void PlayerAttackBaseState::Enter()
	{
		PlayerStateBase::Enter();
		//プラスする攻撃力を入れる
		m_plusAttack = m_player->GetEquippedParts().addAttack;

		//現在接近戦していることを渡す
		m_player->SetMeleeNow(true);
	}

	void PlayerAttackBaseState::Exit()
	{
		//現在接近戦終えたことを渡す
		m_player->SetMeleeNow(false);
	}

	//次の攻撃発生フラグ処理
	void PlayerAttackBaseState::NextAttackFlag(bool onOff)
	{
		//フラグがオフだと通らない
		if (!onOff) return;

		//攻撃ステートに変更する
		if (AttackButton)
		{
			m_nestAttackFlag = true;
		}
	};
	//////////////////////////////

	//攻撃ステート(一番最初に出てくる攻撃)===============================================================>
	void PlayerAttack1State::Enter()
	{
		PlayerAttackBaseState::Enter();

		//SE再生
		m_SE = m_SEManager->Start(L"Attack1", 0, 0.9f * m_SEVol);
		// 攻撃エフェクトを出す
		m_player->AddEffect(PlayerEffect_Attack1);


		//Attack1アニメーションに変更
		m_player->ChangeAnim(L"Attack1");

		//攻撃の瞬間にスティックを傾けていたらその方向に進む
		auto m_stickL = m_player->GetStickL();
		if (m_stickL != Vec3(0.0f, 0.0f, 0.0f))
		{
			m_player->MoveAngle(m_stickL);
		}

	}

	void PlayerAttack1State::Update(float deltaTime)
	{
		// 入力デバイス取得
		PlayerStateBase::Update(deltaTime);

		//アニメーションの更新
		auto mag = 1.42f;//倍率
		m_player->SetAddTimeAnimation((deltaTime * 1.5f) * mag);
		//アニメーションの経過時間計測
		m_timeOfAnimation += (deltaTime * 1.5f) * mag;

		//アニメーションが終わったら移動はしない(違和感が出てくるため)
		if (m_timeOfAnimation <= 0.63f)
		{
			//移動処理
			m_player->PlayerMove(PlayerState_Attack1);
		}
    
		//攻撃判定の定義
		AttackCollisionOccurs();

		//攻撃の時間計測
		m_timeOfAttack += deltaTime;
		//一定時間後にボタンを押すと次の攻撃が撃てるようになる
		NextAttackFlag(m_attackFlag);

		//次のステートに移行する処理
		NextState();		
	}

	void PlayerAttack1State::Exit()
	{	
		PlayerAttackBaseState::Exit();

		auto stage = m_player->GetStage();
		m_timeOfAttack = 0.0f;//リセット
		m_nestAttackFlag = false;
		AttackCollisionFlag = true;//リセット
		m_timeOfAnimation = 0.0f;
	}

	//攻撃コリジョンの発生処理
	void PlayerAttack1State::AttackCollisionOccurs()
	{
		//攻撃判定の定義
		if (m_timeOfAttack >= m_timeOfStartAttack && AttackCollisionFlag)
		{
			auto tmp = m_player->GetAttackPtr()->GetHitInfo();
			//一回しかヒットしないか
			tmp.HitOnce = true;
			//ダメージ
			tmp.Damage = 10 + m_plusAttack;
			//ヒットバック距離 本来のヒットバックはVec3(-2,5,0)
			tmp.HitVel_Stand = Vec3(-2, 8, 0);
			tmp.HitTime_Stand = .5f;
			m_player->DefAttack(.5f, tmp);
			m_player->GetAttackPtr()->SetPos(Vec3(3, 1, 0));
			auto AttackPtr = m_player->GetAttackPtr();
			AttackPtr->GetComponent<Transform>()->SetScale(6.0f, 5.0f, 2.0f);
			AttackPtr->SetCollScale(1.0f);
			AttackPtr->ActivateCollision(m_timeMaxOfAttackFirst);

			//攻撃判定はもう出ない
			AttackCollisionFlag = false;
		}
	}

	//次のステートに移行する処理
	void PlayerAttack1State::NextState()
	{
		//一定時間たったら回避行動ができる(一段目と二段目の攻撃のみ)
		if (m_timeOfAttack > m_timeOfStartDodge)
		{
			// 回避していいフラグ状態だったら回避ステートに変更
			Dodge(m_dodgeFlag);
		}

		//次の攻撃に遷移する	//一定時間後からフラグがオンになってたら次の攻撃が撃てるようになる
		if (m_nestAttackFlag && m_timeOfAttack > m_graceTimeOfNextAttack)
		{
			m_player->ChangeState(L"Attack2");////次の攻撃ステートに移動
			m_player->AddEffect(PlayerEffect_Attack2);//攻撃エフェクトを出す
		}
		else if (m_timeOfAttack >= m_timeMaxOfAttack)//攻撃時間を過ぎたら元のステートに戻る
		{
			m_player->ChangeState(L"PlayerWalk");
		}
	}
	//=========================================================================================================>

	//攻撃ステート(２番目に出る攻撃)===========================================================================>
	void PlayerAttack2State::Enter()
	{
		PlayerAttackBaseState::Enter();
		m_SE = m_SEManager->Start(L"Attack1", 0, 0.9f * m_SEVol);//SE再生

		//Attack2アニメーションに変更
		m_player->ChangeAnim(L"Attack2");	

		//攻撃の瞬間にスティックを傾けていたらその方向に進む
		auto m_stickL = m_player->GetStickL();
		if (m_stickL != Vec3(0.0f, 0.0f, 0.0f))
		{
			m_player->MoveAngle(m_stickL);
		}
	}

	void PlayerAttack2State::Update(float deltaTime)
	{
		// 入力デバイス取得
		PlayerStateBase::Update(deltaTime);

		//アニメーションの更新
		auto mag = 1.66f;//倍率
		m_player->SetAddTimeAnimation((deltaTime * 1.9f) * mag);
		//アニメーションの経過時間計測
		m_timeOfAnimation += (deltaTime * 1.5f) * mag;

		//アニメーションが終わったら移動はしない(違和感が出てくるため)
		if (m_timeOfAnimation <= 0.99f)
		{
			//移動処理
			m_player->PlayerMove(PlayerState_Attack2);
		}


		//攻撃の時間計測
		m_timeOfAttack += deltaTime;

		//攻撃判定の発生
		AttackCollisionOccurs();
		//一定時間後にボタンを押すと次の攻撃が撃てるようになる
		NextAttackFlag(m_attackFlag);
		// 次のステートに移行する処理
		NextState();
	}

	void PlayerAttack2State::Exit()
	{
		PlayerAttackBaseState::Exit();

		auto stage = m_player->GetStage();
		m_timeOfAttack = 0.0f;//リセット
		m_nestAttackFlag = false;
		m_attackCollisionFlag = 0;//リセット
		m_timeOfAnimation = 0.0f;
	}

	//攻撃コリジョン発生
	void PlayerAttack2State::AttackCollisionOccurs()
	{
		//攻撃判定の定義(一撃目)
		if (m_timeOfAttack >= m_timeOfStartAttackFirst && m_attackCollisionFlag == 0)
		{
			auto tmp = m_player->GetAttackPtr()->GetHitInfo();
			tmp.HitOnce = true;
			tmp.Damage = 13 + (m_plusAttack / 2);
			tmp.HitVel_Stand = Vec3(-2, 8, 0);//ヒットバック距離 本来のヒットバックはVec3(-2,5,0)
			tmp.HitTime_Stand = .5f;//のけぞり時間なし
			m_player->DefAttack(.5f, tmp);
			m_player->GetAttackPtr()->SetPos(Vec3(3, 3, 0));
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
			tmp.Damage = 14 + (m_plusAttack / 2);
			tmp.HitVel_Stand = Vec3(-2, 8, 0);//ヒットバック距離 本来のヒットバックはVec3(-2,5,0)
			tmp.HitTime_Stand = .5f;//のけぞり時間なし
			m_player->DefAttack(.5f, tmp);
			m_player->GetAttackPtr()->SetPos(Vec3(3, 3, 0));
			auto AttackPtr = m_player->GetAttackPtr();
			AttackPtr->GetComponent<Transform>()->SetScale(6.0f, 5.0f, 2.0f);//縦長の当たり判定
			AttackPtr->SetCollScale(1.0f);
			AttackPtr->ActivateCollision(m_timeMaxOfAttackSecond);

			//２段目の攻撃はもうない
			m_attackCollisionFlag = 2;
		}
	}

	//次のステートに移行する処理
	void PlayerAttack2State::NextState()
	{
		//一定時間たったら回避行動ができる(一段目と二段目の攻撃のみ)
		if (m_timeOfAttack > m_timeOfStartDodge)
		{
			Dodge(m_dodgeFlag);
		}

		//次の攻撃に遷移する	//一定時間後からフラグがオンになってたら次の攻撃が撃てるようになる
		if (m_nestAttackFlag && m_timeOfAttack > m_graceTimeOfNextAttack)
		{
			m_player->ChangeState(L"Attack3");////次の攻撃ステートに移動
			m_player->AddEffect(PlayerEffect_Attack3);//攻撃エフェクトを出す
		}
		else if (m_timeOfAttack >= m_timeMaxOfAttackTotal)//攻撃時間を過ぎたら元のステートに戻る
		{
			m_player->ChangeState(L"PlayerWalk");
		}
	}
	//=========================================================================================================>


	//攻撃ステート(3番目に出る攻撃)============================================================================>
	void PlayerAttack3State::Enter()
	{
		PlayerAttackBaseState::Enter();
		m_SE = m_SEManager->Start(L"Attack2", 0, 0.9f * m_SEVol);//SE再生

		//Attack3アニメーションに変更
		m_player->ChangeAnim(L"Attack3");

		//攻撃の瞬間にスティックを傾けていたらその方向に進む
		auto m_stickL = m_player->GetStickL();
		if (m_stickL != Vec3(0.0f, 0.0f, 0.0f))
		{
			m_player->MoveAngle(m_stickL);
		}
	}

	void PlayerAttack3State::Update(float deltaTime)
	{
		// 入力デバイス取得
		PlayerStateBase::Update(deltaTime);

		//アニメーションの更新
		auto mag = 1.42f;//倍率
		m_player->SetAddTimeAnimation((deltaTime * 1.8f)*mag);

		//アニメーションの経過時間計測
		m_timeOfAnimation += (deltaTime * 1.5f) * mag;

		//アニメーションが終わったら移動はしない(違和感が出てくるため)
		if (m_timeOfAnimation <= 1.24f)
		{
			//移動処理
			m_player->PlayerMove(PlayerState_Attack3);
		}


		//攻撃の時間計測
		m_timeOfAttack += deltaTime;

		//攻撃発生の処理
		AttackCollisionOccurs();
		//攻撃ステートに変更する
		NextAttackFlag(m_attackFlag);
		//次のステートに行く処理
		NextState();
	}

	void PlayerAttack3State::Exit()
	{
		PlayerAttackBaseState::Exit();

		auto stage = m_player->GetStage();
		m_timeOfAttack = 0.0f;//リセット
		m_nestAttackFlag = false;
		m_attackCollisionFlag = 0;//リセット
		m_timeOfAnimation = 0.0f;
	}

	//攻撃コリジョン発生
	void PlayerAttack3State::AttackCollisionOccurs()
	{
		//攻撃判定の定義(１撃目)
		if (m_timeOfAttack >= m_timeOfStartAttackFirst && m_attackCollisionFlag == 0)
		{
			auto tmp = m_player->GetAttackPtr()->GetHitInfo();
			tmp.HitOnce = true;
			tmp.Damage = 18 + (m_plusAttack / 2);
			tmp.HitVel_Stand = Vec3(-10, 8, 3);//ヒットバック距離
			tmp.HitTime_Stand = .5f;
			m_player->DefAttack(.5f, tmp);
			m_player->GetAttackPtr()->SetPos(Vec3(3, 3, 0));
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
			tmp.Damage = 20 + (m_plusAttack / 2);
			tmp.HitVel_Stand = Vec3(-10, 5, 0);//ヒットバック距離
			tmp.HitTime_Stand = .5f;
			m_player->DefAttack(.5f, tmp);
			m_player->GetAttackPtr()->SetPos(Vec3(3, 8, 0));
			auto AttackPtr = m_player->GetAttackPtr();
			AttackPtr->GetComponent<Transform>()->SetScale(5.0f, 5.0f, 7.0f);
			AttackPtr->SetCollScale(1.0f);
			AttackPtr->ActivateCollision(m_timeMaxOfAttackSecond);

			//攻撃判定はもう出ない(１撃目)
			m_attackCollisionFlag = 2;
		}
	}

	//次のステートに行く処理
	void PlayerAttack3State::NextState()
	{
		//一定時間たったら回避行動ができる(一段目と二段目の攻撃のみ)
		if (m_timeOfAttack > m_graceTimeOfNextAttack)
		{
			Dodge(m_dodgeFlag);
		}

		//次の攻撃に遷移する	//一定時間後からフラグがオンになってたら次の攻撃が撃てるようになる
		if (m_nestAttackFlag && m_timeOfAttack > m_graceTimeOfNextAttack)
		{
			m_player->ChangeState(L"AttackEx");////次の攻撃ステートに移動
			m_player->AddEffect(PlayerEffect_AttackEx);//攻撃エフェクトを出す
		}
		else if (m_timeOfAttack >= m_timeMaxOfAttackTotal)//攻撃時間を過ぎたら元のステートに戻る
		{
			m_player->ChangeState(L"PlayerWalk");
		}
	}
	//=========================================================================================================>


	//攻撃ステート(4番目に出る攻撃)============================================================================>
	void PlayerAttackExState::Enter()
	{
		PlayerAttackBaseState::Enter();
		m_SE = m_SEManager->Start(L"Attack3", 0, 0.9f * m_SEVol);//SE再生

		//AttackExアニメーションに変更
		m_player->ChangeAnim(L"AttackEx");

		//攻撃の瞬間にスティックを傾けていたらその方向に進む
		auto m_stickL = m_player->GetStickL();
		if (m_stickL != Vec3(0.0f, 0.0f, 0.0f))
		{
			m_player->MoveAngle(m_stickL);
		}
	}

	void PlayerAttackExState::Update(float deltaTime)
	{
		//攻撃の時間計測
		m_timeOfAttack += deltaTime;

		//回避処理
		float timeSpeed = 80.0f;
		m_curveTime += XMConvertToRadians(deltaTime * timeSpeed);

		//二次関数的な動きで回避行動をする
		//今は向いている方向に前方回避をする
		//float dodge = 8.0f * 2.5f;
		//totalVec.x = cos(m_angle) * dodge * abs(cos(m_dodgeTime));
		//totalVec.z = sin(m_angle) * dodge * abs(cos(m_dodgeTime));
		 
		 
		auto mag = 1.66f;
		//アニメーションの経過時間計測
		m_timeOfAnimation += ((deltaTime * 1.5f) * mag) * abs(cos(m_curveTime));

		////アニメーションが終わったら移動はしない(違和感が出てくるため)
		////if (m_timeOfAnimation <= 1.68f)
		////{
		////	//移動処理
		////	m_player->PlayerMove(PlayerState_Attack3);
		////}

		//アニメーションの更新
		//auto mag = 1.66f;//倍率
		m_player->SetAddTimeAnimation(((deltaTime * 2.2f) * mag) * cos(m_curveTime));

		AttackCollisionOccurs();
		//次のステートに行く処理
		NextState();
	}

	void PlayerAttackExState::Exit()
	{
		PlayerAttackBaseState::Exit();

		auto stage = m_player->GetStage();
		m_timeOfAttack = 0.0f;//リセット
		AttackCollisionFlag = true;//リセット
		m_timeOfAnimation = 0.0f;
		m_curveTime = 0.0f;

	}

	//攻撃の発生
	void PlayerAttackExState::AttackCollisionOccurs()
	{
		//攻撃判定の定義
		if (m_timeOfAttack >= m_timeOfStartAttack && AttackCollisionFlag)
		{
			auto tmp = m_player->GetAttackPtr()->GetHitInfo();
			tmp.HitOnce = true;
			tmp.Damage = 35 + m_plusAttack;
			tmp.HitVel_Stand = Vec3(-20, 10, 0);//ヒットバック距離
			tmp.HitTime_Stand = .8f;
			//tmp.ForceRecover = false;//ノックバックする
			m_player->DefAttack(.5f, tmp);
			m_player->GetAttackPtr()->SetPos(Vec3(0, 1, 0));
			auto AttackPtr = m_player->GetAttackPtr();
			AttackPtr->GetComponent<Transform>()->SetScale(10.0f, 5.0f, 10.0f);
			AttackPtr->SetCollScale(1.0f);
			AttackPtr->ActivateCollision(m_timeOfStartAttackFirst);

			//攻撃判定はもう出ない
			AttackCollisionFlag = false;
		}
	}

	//次のステートに行く処理
	void PlayerAttackExState::NextState()
	{
		//攻撃の時間を越えたら別のステートに移動する
		if (m_timeOfAttack >= m_timeMaxOfAttack)
		{
			auto stage = m_player->GetStage();
			m_timeOfAttack = 0.0f;//リセット

			m_player->ChangeState(L"AttackExEnd");
		}
	}
	//=========================================================================================================>

	//回転攻撃が終わった時のモーション用のステート=========================================================================>
	void PlayerAttackExEndState::Enter()
	{
		PlayerStateBase::Enter();

		//回転攻撃が終わった後モーションを再生
		m_player->ChangeAnim(L"AttackExEnd");
	}

	void PlayerAttackExEndState::Update(float deltaTime)
	{
		PlayerStateBase::Update(deltaTime);

		auto mag = 1.25f;

		m_countTimeOfState += deltaTime * mag;
		//アニメーション更新
		m_player->SetAddTimeAnimation(1.0f * deltaTime * mag);

		//このステートは回避できる
		Dodge(m_dodgeFlag);

		//モーションが終わったら歩きステートに戻る
		if (m_countTimeOfState >= m_maxTimeOfState)
		{
			m_player->ChangeState(L"PlayerWalk");
		}
	}
	//=========================================================================================================>

	void PlayerAttackExEndState::Exit()
	{
		m_countTimeOfState = 0.0f;
	}

	//必殺技===================================================================================================>
	void PlayerAttackSpecialState::Enter()
	{
		PlayerStateBase::Enter();

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
	//=========================================================================================================>


	//遠距離攻撃===============================================================================================>
	void PlayerAttackLongState::Enter()
	{
		PlayerStateBase::Enter();

		//遠距離攻撃をしていることを渡す
		m_player->SetGunNow(true);

		//球を出す
		auto stage = m_player->GetStage();
		auto playerPos = m_player->GetPosition();
		m_SEManager->Start(L"HandGun", 0, 0.9f * m_SEVol);//銃SE再生
		auto bullet = stage->AddGameObject<Bullet>(playerPos+Vec3(0.0f,2.0f,0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), 60.0f, m_player, 100.0f);

		auto cameraManager = m_player->GetStage()->GetSharedGameObject<CameraManager>(L"CameraManager");
		float m_angleYAxis = -(cameraManager->GetAngle(L"Y")) - XMConvertToRadians(180.0f);
		auto PlayerQt = m_player->GetQuaternion();
		auto playerRot = m_player->GetComponent<Transform>()->GetRotation();
		playerRot.y = m_angleYAxis;
		m_player->GetComponent<Transform>()->SetRotation(Vec3(0.0f,m_angleYAxis,0.0f));


		m_player->ChangeAnim(L"Shot_Gun");//撃つアニメーション変更
	}

	void PlayerAttackLongState::Update(float deltaTime)
	{
		PlayerStateBase::Update(deltaTime);

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
	//=========================================================================================================>


	//ダメージを受けた際のステート=============================================================================>
	void PlayerHitState::Enter()
	{
		PlayerStateBase::Enter();

		m_SEManager->Start(L"DamageSE",false,0.3f * m_SEVol);
		auto hitInfo = m_player->GetHitInfo();
		auto HPNow = m_player->GetHP();
		m_player->HitBack();
		m_player->SetHP(HPNow - hitInfo.Damage);
		m_timeMaxOfHitBack = m_player->GetHitInfo().HitTime_Stand;

		//ダメージを受けたときのアニメーション
		m_player->ChangeAnim(L"Hit");

		if (m_player->GetHitInfo().InvincibleOnHit) {
			m_player->AddTag(L"invincible");//タメージを受けているときは無敵にする
		}
	}

	void PlayerHitState::Update(float deltaTime)
	{
		PlayerStateBase::Update(deltaTime);

		//アニメーション更新
		m_player->SetAddTimeAnimation(deltaTime/2.0f);

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
		if (m_player->FindTag(L"invincible")) 
		{
			m_player->RemoveTag(L"invincible");//タメージを受けているときは無敵にする

		}
	}
	//=========================================================================================================>

	//ステージ開始ムービ用ステート=============================================================================>
	void PlayerStartMovieState::Enter()
	{
		//ステージ開始用ムービーのアニメーションになるかも
	}

	void PlayerStartMovieState::Update(float deltaTime)
	{
		//ステージの状態がPlayingになったらこのステートは終了
		auto gamePhase = m_player->GetWaveStage(true)->GetCurrentGamePhase();
		if (gamePhase == WaveStageBase::GamePhase::GPhase_Playing)
		{
			m_player->ChangeState(L"PlayerWalk");
		}

		
		//地面に着地したらこのステートの処理は終了
		if (m_player->GetLand())
		{
			//ムービー用コントローラー取得
			auto RT_movieController = m_player->GetStage()->GetSharedGameObject<RT_MovieController>(L"MovieController");

			RT_movieController->SetPlayerEnd(true);
		}
	}

	void PlayerStartMovieState::Exit()
	{

	}
	//=========================================================================================================>


}
//end basecross
