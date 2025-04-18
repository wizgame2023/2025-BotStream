/*!
@file PlayerState.cpp
@brief プレイヤーのステート
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
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
		float meleeRange = 3.0f;
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

		}

	}
	void PlayerWalkState::Exit()
	{

	}

	//回避ステート
	void PlayerDodgeState::Enter()
	{

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

	}


	//ダッシュステート
	void PlayerDashState::Enter()
	{

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

		//歩きステートのアニメーション再生
		m_player->GetComponent<PNTBoneModelDraw>()->UpdateAnimation(deltaTime * 7);

		//歩くステートに変更する
		if (m_controller.wReleasedButtons & XINPUT_GAMEPAD_A)
		{
			m_player->ChangeState(L"PlayerWalk");
		}
		//攻撃ステートに変更する
		if (m_controller.wButtons & XINPUT_GAMEPAD_X)
		{
			m_timeOfPushAttackButton += deltaTime;//押している時間を測る
		}
		if (m_controller.wReleasedButtons & XINPUT_GAMEPAD_X)
		{
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

	}
	void PlayerDashState::Exit()
	{

	}


	//攻撃ステート(一番最初に出てくる攻撃)
	void PlayerAttack1State::Enter()
	{
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
			tmp.HitOnce = true;
			tmp.Damage = 10;
			tmp.HitVel_Stand = Vec3(-2, 10, 0);
			tmp.HitTime_Stand = .8f;
			m_player->DefAttack(.5f, tmp);
			m_player->GetAttackPtr()->SetPos(Vec3(1, 1, 0));
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
		//攻撃の当たり判定を出す
		auto stage = m_player->GetStage();
		//攻撃の当たり判定(仮)
		m_AttackObj = stage->AddGameObject<Cube>(Vec3(0.0f, 2.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f),Col4(0.0f,0.0f,1.0f,1.0f));
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
		//攻撃の当たり判定を出す
		auto stage = m_player->GetStage();
		//攻撃の当たり判定(仮)
		m_AttackObj = stage->AddGameObject<Cube>(Vec3(0.0f, 2.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Col4(1.0f, 0.0f, 1.0f, 1.0f));
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
	}


	//攻撃ステート(3番目に出る攻撃)
	void PlayerAttackExState::Enter()
	{
		//攻撃の当たり判定を出す
		auto stage = m_player->GetStage();
		//攻撃の当たり判定(仮)
		m_AttackObj = stage->AddGameObject<Cube>(Vec3(0.0f, 2.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Col4(0.0f, 0.0f, 0.0f, 1.0f));
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


	//void PlayerAttackLongState::Enter()
	//{
	//	//球を出す
	//}
	//void PlayerAttackLongState::Update(float deltaTime)
	//{
	//	//攻撃の時間計測
	//	m_timeOfAttack += deltaTime;

	//	//攻撃の時間を越えたら別のステートに移動する
	//	if (m_timeOfAttack >= m_timeMaxOfAttack)
	//	{
	//		auto stage = m_player->GetStage();
	//		m_timeOfAttack = 0.0f;//リセット

	//		m_player->ChangeState(L"PlayerWalk");
	//	}

	//}
	//void PlayerAttackLongState::Exit()
	//{
	//	auto stage = m_player->GetStage();
	//	stage->RemoveGameObject<Cube>(m_AttackObj);//攻撃判定削除
	//	m_timeOfAttack = 0.0f;//リセット
	//}

}
//end basecross
