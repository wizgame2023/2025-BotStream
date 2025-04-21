/*!
@file PlayerState.cpp
@brief プレイヤーのステート
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void PlayerStateBase::Enter()
	{
		m_SEManager = App::GetApp()->GetXAudio2Manager();
	}
	void PlayerStateBase::Update(float deltaTime)
	{

		//コントローラーを受け取る
		auto inputDevice = App::GetApp()->GetInputDevice();
		m_controller = inputDevice.GetControlerVec()[0];

		//カメラマネージャからロックオン対象の位置を取得する
		shared_ptr<CameraManager> camera = nullptr;
		camera = m_player->GetStage()->GetSharedGameObject<CameraManager>(L"CameraManager");
		shared_ptr<Actor> targetObj = nullptr;
		targetObj = camera->GetTargetObj();
		//ターゲット対象がいる時対象との距離を測る
		if (targetObj)
		{
			//Vec3 targetPos = Vec3(0.0f);
			//Vec3 playerPos = Vec3(0.0f);
			//targetPos = targetObj->GetComponent<Transform>()->GetPosition();
			//playerPos = m_player->GetComponent<Transform>()->GetPosition();
			////ロックオン対象との距離を計算する
			m_targetDistance = camera->GetTargetDis();
		}
		else if (!targetObj)
		{
			m_targetDistance = 0.0f;
		}


	};
	//ターゲット対象との距離を取得する
	float PlayerStateBase::GetTargetDistance()
	{
		return m_targetDistance;
	}

	//Playerの歩くモーション
	void PlayerWalkState::Enter()
	{

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
		m_player->GetComponent<PNTBoneModelDraw>()->UpdateAnimation(deltaTime * 5);
	
		//回避ステートに変更する
		if (m_controller.wPressedButtons & XINPUT_GAMEPAD_A)
		{
			m_player->ChangeState(L"Dodge");
		}
		//攻撃ステートに変更する　長押しだったら回転攻撃そうでなければ通常攻撃
		//ロックオン対象の距離によって攻撃方法を変えるロックオンしてなければ近距離のみ
		float meleeRange = 200.0f;
		if (m_controller.wButtons & XINPUT_GAMEPAD_X)
		{
			m_timeOfPushAttackButton += deltaTime;//押している時間を測る
		}
		if (m_controller.wReleasedButtons & XINPUT_GAMEPAD_X)
		{
			//攻撃する際距離によって攻撃方法を変える
			if (m_targetDistance < meleeRange)
			{//近距離
				if (m_timeOfPushAttackButton >= 1.5f)
				{
					m_player->ChangeState(L"AttackEx");
					m_player->AddEffect(PlayerEffect_AttackEx);//攻撃エフェクトを出す
				}
				if (m_timeOfPushAttackButton < 1.5f)
				{
					m_player->ChangeState(L"Attack1");
					m_player->AddEffect(PlayerEffect_Attack1);//攻撃エフェクトを出す
				}
			}
			else if (m_targetDistance >= meleeRange)
			{//遠距離
				m_player->ChangeState(L"AttackLong");
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

		m_SEManager->Start(L"Dash", 0, 0.9f);//回避SE

		//回避タグ追加
		m_player->AddTag(L"DodgeNow");
	}
	void PlayerDodgeState::Update(float deltaTime)
	{
		// 入力デバイス取得
		PlayerStateBase::Update(deltaTime);

		//移動処理
		Vec3 move = m_player->GetMoveVector(PlayerState_Dodge);
		m_player->PlayerMove(PlayerState_Dodge);

		bool dodgeFlag = m_player->GetDodgeFlag();
		if (!dodgeFlag)
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
			m_player->ChangeAnim(L"Walk");
		}
		else
		{
			m_player->ChangeAnim(L"Idle");
		}

		//ダッシュステートのアニメーション再生
		m_player->GetComponent<PNTBoneModelDraw>()->UpdateAnimation(deltaTime * 7);

		//Aボタン離したら歩くステートに変更する
		if (m_controller.wReleasedButtons & XINPUT_GAMEPAD_A)
		{
			m_player->ChangeState(L"PlayerWalk");
		}

		//攻撃ステートに変更する　長押しだったら回転攻撃そうでなければ通常攻撃
		//ロックオン対象の距離によって攻撃方法を変えるロックオンしてなければ近距離のみ
		float meleeRange = 200.0f;
		if (m_controller.wButtons & XINPUT_GAMEPAD_X)
		{
			m_timeOfPushAttackButton += deltaTime;//押している時間を測る
		}
		if (m_controller.wReleasedButtons & XINPUT_GAMEPAD_X)
		{
			//攻撃する際距離によって攻撃方法を変える
			if (m_targetDistance < meleeRange)
			{//近距離
				if (m_timeOfPushAttackButton >= 1.5f)
				{
					m_player->ChangeState(L"AttackEx");
					m_player->AddEffect(PlayerEffect_AttackEx);//攻撃エフェクトを出す
				}
				if (m_timeOfPushAttackButton < 1.5f)
				{
					m_player->ChangeState(L"Attack1");
					m_player->AddEffect(PlayerEffect_Attack1);//攻撃エフェクトを出す
				}
			}
			else if (m_targetDistance >= meleeRange)
			{//遠距離
				m_player->ChangeState(L"AttackLong");
				m_player->AddEffect(PlayerEffect_Beam);//攻撃エフェクトを出す
			}
			m_timeOfPushAttackButton = 0.0f;//押した時間リセット
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

		//攻撃の当たり判定を出す
		auto stage = m_player->GetStage();
		//攻撃の当たり判定(仮)
		m_AttackObj = stage->AddGameObject<Cube>(Vec3(0.0f, 2.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
	}
	void PlayerAttack1State::Update(float deltaTime)
	{
		// 入力デバイス取得
		PlayerStateBase::Update(deltaTime);
    
		//攻撃判定の定義
		if (m_timeOfAttack <= 0) {
			auto tmp = m_player->GetAttackPtr()->GetHitInfo();
			//tmp.HitSound = L"Attack1";

			tmp.HitOnce = true;//一回しかヒットしないか
			tmp.Damage = 10;//ダメージ
			tmp.HitVel_Stand = Vec3(-2, 5, 0);//ヒットバック距離
			tmp.HitTime_Stand = .8f;//のけぞり時間
			//tmp.ForceRecover = true;
			m_player->DefAttack(.5f, tmp);
			m_player->GetAttackPtr()->SetPos(Vec3(1, 1, 0));
			auto AttackPtr = m_player->GetAttackPtr();
			AttackPtr->SetCollScale(8.0f);
		}

		//攻撃の時間計測
		m_timeOfAttack += deltaTime;
		//猶予時間以内に攻撃ボタンを押せたら次の攻撃ステートに遷移できる
		if (m_timeOfAttack < m_graceTimeOfNextAttack)
		{
			//攻撃ステートに変更する
			if (m_controller.wPressedButtons & XINPUT_GAMEPAD_X)
			{			
				m_nestAttackFlag = true;	
			}
		}
		//次の攻撃に移行できない状態なら攻撃判定の色を変える(デバック用)
		if (m_timeOfAttack >= m_graceTimeOfNextAttack)
		{
			m_AttackObj->GetComponent<PNTStaticDraw>()->SetDiffuse(Col4(1.0f, 0.0f, 0.0f, 0.0f));
			m_AttackObj->GetComponent<PNTStaticDraw>()->SetEmissive(Col4(1.0f, 0.0f, 0.0f, 0.0f));
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
				stage->RemoveGameObject<Cube>(m_AttackObj);//攻撃判定削除
				m_player->ChangeState(L"PlayerWalk");
			}
		}
		
	}
	void PlayerAttack1State::Exit()
	{
		auto stage = m_player->GetStage();
		stage->RemoveGameObject<Cube>(m_AttackObj);//攻撃判定削除
		m_timeOfAttack = 0.0f;//リセット
		m_nestAttackFlag = false;
	}


	//攻撃ステート(２番目に出る攻撃)
	void PlayerAttack2State::Enter()
	{
		PlayerStateBase::Enter();
		m_SE = m_SEManager->Start(L"Attack1", 0, 0.9f);//SE再生

		//攻撃の当たり判定を出す
		auto stage = m_player->GetStage();
		//攻撃の当たり判定(仮)
		m_AttackObj = stage->AddGameObject<Cube>(Vec3(0.0f, 2.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f),Col4(0.0f,0.0f,1.0f,1.0f));
	
		//攻撃判定の定義
		if (m_timeOfAttack <= 0) {
			auto tmp = m_player->GetAttackPtr()->GetHitInfo();
			tmp.HitOnce = true;
			tmp.Damage = 12;
			tmp.HitVel_Stand = Vec3(-5, 5, 0);//ヒットバック距離
			tmp.HitTime_Stand = .3f;
			m_player->DefAttack(.5f, tmp);
			m_player->GetAttackPtr()->SetPos(Vec3(1, 1, 0));
		}

	}
	void PlayerAttack2State::Update(float deltaTime)
	{
		// 入力デバイス取得
		PlayerStateBase::Update(deltaTime);

		//攻撃の時間計測
		m_timeOfAttack += deltaTime;

		//猶予時間以内に攻撃ボタンを押せたら次の攻撃ステートに遷移できる
		if (m_timeOfAttack < m_graceTimeOfNextAttack)
		{
			//攻撃ステートに変更する
			if (m_controller.wPressedButtons & XINPUT_GAMEPAD_X)
			{
				m_nestAttackFlag = true;
			}
		}
		//次の攻撃に移行できない状態なら攻撃判定の色を変える(デバック用)
		if (m_timeOfAttack >= m_graceTimeOfNextAttack)
		{
			m_AttackObj->GetComponent<PNTStaticDraw>()->SetDiffuse(Col4(1.0f, 0.0f, 0.0f, 0.0f));
			m_AttackObj->GetComponent<PNTStaticDraw>()->SetEmissive(Col4(1.0f, 0.0f, 0.0f, 0.0f));
		}
		//攻撃の時間を越えたら別のステートに移動する
		if (m_timeOfAttack >= m_timeMaxOfAttack)
		{
			auto stage = m_player->GetStage();
			m_timeOfAttack = 0.0f;//リセット

			//ステート遷移
			if (m_nestAttackFlag)
			{
				stage->RemoveGameObject<Cube>(m_AttackObj);//攻撃判定削除
				m_player->ChangeState(L"Attack3");//次の攻撃ステートに移動
				m_player->AddEffect(PlayerEffect_Attack3);//攻撃エフェクトを出す
			}
			else
			{
				stage->RemoveGameObject<Cube>(m_AttackObj);//攻撃判定削除
				m_player->ChangeState(L"PlayerWalk");
			}
		}
	}
	void PlayerAttack2State::Exit()
	{
		auto stage = m_player->GetStage();
		stage->RemoveGameObject<Cube>(m_AttackObj);//攻撃判定削除
		m_timeOfAttack = 0.0f;//リセット
		m_nestAttackFlag = false;
	}


	//攻撃ステート(3番目に出る攻撃)
	void PlayerAttack3State::Enter()
	{
		PlayerStateBase::Enter();
		m_SE = m_SEManager->Start(L"Attack2", 0, 0.9f);//SE再生

		//攻撃の当たり判定を出す
		auto stage = m_player->GetStage();
		//攻撃の当たり判定(仮)
		m_AttackObj = stage->AddGameObject<Cube>(Vec3(0.0f, 2.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Col4(1.0f, 0.0f, 1.0f, 1.0f));
	
		//攻撃判定の定義
		if (m_timeOfAttack <= 0) {
			auto tmp = m_player->GetAttackPtr()->GetHitInfo();
			tmp.HitOnce = true;
			tmp.Damage = 15;
			tmp.HitVel_Stand = Vec3(-10, 5, 0);//ヒットバック距離
			tmp.HitTime_Stand = .5f;
			m_player->DefAttack(.5f, tmp);
			m_player->GetAttackPtr()->SetPos(Vec3(1, 1, 0));
		}
	}
	void PlayerAttack3State::Update(float deltaTime)
	{
		// 入力デバイス取得
		PlayerStateBase::Update(deltaTime);

		//攻撃の時間計測
		m_timeOfAttack += deltaTime;

		//猶予時間以内に攻撃ボタンを押せたら次の攻撃ステートに遷移できる
		if (m_timeOfAttack < m_graceTimeOfNextAttack)
		{
			//攻撃ステートに変更する
			if (m_controller.wPressedButtons & XINPUT_GAMEPAD_X)
			{
				m_nestAttackFlag = true;
			}
		}
		//次の攻撃に移行できない状態なら攻撃判定の色を変える(デバック用)
		if (m_timeOfAttack >= m_graceTimeOfNextAttack)
		{
			m_AttackObj->GetComponent<PNTStaticDraw>()->SetDiffuse(Col4(1.0f, 0.0f, 0.0f, 0.0f));
			m_AttackObj->GetComponent<PNTStaticDraw>()->SetEmissive(Col4(1.0f, 0.0f, 0.0f, 0.0f));
		}
		//攻撃の時間を越えたら別のステートに移動する
		if (m_timeOfAttack >= m_timeMaxOfAttack)
		{
			auto stage = m_player->GetStage();
			m_timeOfAttack = 0.0f;//リセット

			//ステート遷移
			if (m_nestAttackFlag)
			{
				stage->RemoveGameObject<Cube>(m_AttackObj);//攻撃判定削除
				m_player->ChangeState(L"AttackEx");//次の攻撃ステートに移動
				m_player->AddEffect(PlayerEffect_AttackEx);//攻撃エフェクトを出す
			}
			else
			{
				stage->RemoveGameObject<Cube>(m_AttackObj);//攻撃判定削除
				m_player->ChangeState(L"PlayerWalk");
			}
		}
	}
	void PlayerAttack3State::Exit()
	{
		auto stage = m_player->GetStage();
		stage->RemoveGameObject<Cube>(m_AttackObj);//攻撃判定削除
		m_timeOfAttack = 0.0f;//リセット
		m_nestAttackFlag = false;

		//攻撃判定の定義
		if (m_timeOfAttack <= 0) {
			auto tmp = m_player->GetAttackPtr()->GetHitInfo();
			tmp.HitOnce = true;
			tmp.Damage = 25;
			tmp.HitVel_Stand = Vec3(-15, 5, 0);//ヒットバック距離
			tmp.HitTime_Stand = .8f;
			m_player->DefAttack(.5f, tmp);
			m_player->GetAttackPtr()->SetPos(Vec3(1, 1, 0));
		}
	}


	//攻撃ステート(4番目に出る攻撃)
	void PlayerAttackExState::Enter()
	{
		PlayerStateBase::Enter();
		m_SE = m_SEManager->Start(L"Attack3", 0, 0.9f);//SE再生

		//攻撃の当たり判定を出す
		auto stage = m_player->GetStage();
		//攻撃の当たり判定(仮)
		m_AttackObj = stage->AddGameObject<Cube>(Vec3(0.0f, 2.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Col4(0.0f, 0.0f, 0.0f, 1.0f));
		

		//攻撃判定の定義
		if (m_timeOfAttack <= 0) {
			auto tmp = m_player->GetAttackPtr()->GetHitInfo();
			tmp.HitOnce = true;
			tmp.Damage = 20;
			tmp.HitVel_Stand = Vec3(-20, 5, 0);//ヒットバック距離
			tmp.HitTime_Stand = .8f;
			//tmp.ForceRecover = false;//ノックバックする
			m_player->DefAttack(.5f, tmp);
			m_player->GetAttackPtr()->SetPos(Vec3(0, 1, 0));
			auto AttackPtr = m_player->GetAttackPtr();
			AttackPtr->SetCollScale(12.0f);
		}

	}
	void PlayerAttackExState::Update(float deltaTime)
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
	void PlayerAttackExState::Exit()
	{
		auto stage = m_player->GetStage();
		stage->RemoveGameObject<Cube>(m_AttackObj);//攻撃判定削除
		m_timeOfAttack = 0.0f;//リセット
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
			tmp.Damage = 10;
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

		////攻撃判定の定義
		//if (m_timeOfAttack <= 0) {
		//	auto tmp = m_player->GetAttackPtr()->GetHitInfo();
		//	tmp.HitOnce = true;
		//	tmp.Damage = 10;
		//	tmp.HitVel_Stand = Vec3(-1, 5, 0);
		//	tmp.HitTime_Stand = .8f;
		//	//tmp.ForceRecover = false;//ノックバックする
		//	m_player->DefAttack(.5f, tmp);
		//	m_player->GetAttackPtr()->SetPos(Vec3(1, 1, 0));
		//	auto AttackPtr = m_player->GetAttackPtr();
		//	auto AttackPos = AttackPtr->GetPos();
		//	

		//	AttackPtr->SetCollScale(12.0f);
		//}

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
		stage->AddGameObject<Bullet>(playerPos, Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), 30.0f, m_player,30.0f);
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


}
//end basecross
