/*!
@file PlayerState.cpp
@brief プレイヤーのステート
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
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
		//攻撃ステートに変更する 攻撃が実装されていないのでコメントアウト
		if (m_controller.wPressedButtons & XINPUT_GAMEPAD_X)
		{
			m_player->ChangeState(L"Attack1");
			m_player->AddEffect(PlayerEffect_Attack1);//攻撃エフェクトを出す
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
			Exit(0);
		}

	}
	void PlayerDodgeState::Exit(int a)//終了処理
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
		//攻撃ステートに変更する 攻撃が実装されていないのでコメントアウト
		if (m_controller.wPressedButtons & XINPUT_GAMEPAD_X)
		{
			m_player->ChangeState(L"Attack1");
			m_player->AddEffect(PlayerEffect_Attack1);//攻撃エフェクトを出す
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
		//攻撃判定の定義
		if (m_timeOfAttack <= 0) {
			auto tmp = m_player->GetAttackPtr()->GetHitInfo();
			tmp.HitOnce = true;
			tmp.Damage = 10;
			tmp.HitVel_Stand = Vec3(-20, 0, 0);
			tmp.HitTime_Stand = .8f;
			m_player->DefAttack(.5f, tmp);
			m_player->GetAttackPtr()->SetPos(Vec3(1, 1, 0));
		}

		//攻撃の時間計測
		m_timeOfAttack += deltaTime;

		//猶予時間以内に攻撃ボタンを押せたら次の攻撃ステートに遷移できる
		if (m_timeOfAttack < m_graceTimeOfNextAttack)
		{
			if (m_controller.wPressedButtons & XINPUT_GAMEPAD_X)
			{
				m_nestAttackFlag = true;
				m_player->AddEffect(PlayerEffect_Attack2);//攻撃エフェクトを出す
			}
		}
		//次の攻撃に移行できない状態なら攻撃判定の色を変える(デバック用)
		if (m_timeOfAttack >= m_graceTimeOfNextAttack)
		{
			m_AttackObj->GetComponent<PNTStaticDraw>()->SetDiffuse(Col4(1.0f, 0.0f, 0.0f, 1.0f));
			m_AttackObj->GetComponent<PNTStaticDraw>()->SetEmissive(Col4(1.0f, 0.0f, 0.0f, 1.0f));
		}
		//攻撃の時間を越えたら別のステートに移動する
		if (m_timeOfAttack >= m_timeMaxOfAttack)
		{
			Exit(0);
		}
		
	}
	void PlayerAttack1State::Exit(int a)
	{
		auto stage = m_player->GetStage();

		//ステート遷移
		if (m_nestAttackFlag)
		{
			m_player->ChangeState(L"Attack2");//まだAttack2ステート作成してない			
		}
		else
		{
			m_timeOfAttack = 0.0f;//リセット
			stage->RemoveGameObject<Cube>(m_AttackObj);//攻撃判定削除
			m_player->ChangeState(L"PlayerWalk");
		}
	}


	//攻撃ステート(２番目に出る攻撃)
	void PlayerAttack2State::Enter()
	{

	}
	void PlayerAttack2State::Update(float deltaTime)
	{
		//攻撃の時間計測
		m_timeOfAttack += deltaTime;

		//猶予時間以内に攻撃ボタンを押せたら次の攻撃ステートに遷移できる
		if (m_timeOfAttack < m_graceTimeOfNextAttack)
		{
			//m_nestAttackFlag = true;
		}
		//次の攻撃に移行できない状態なら攻撃判定の色を変える(デバック用)
		if (m_timeOfAttack >= m_graceTimeOfNextAttack)
		{
			m_AttackObj->GetComponent<PNTStaticDraw>()->SetDiffuse(Col4(1.0f, 0.0f, 0.0f, 1.0f));
			m_AttackObj->GetComponent<PNTStaticDraw>()->SetEmissive(Col4(1.0f, 0.0f, 0.0f, 1.0f));
		}
		//攻撃の時間を越えたら別のステートに移動する
		if (m_timeOfAttack >= m_timeMaxOfAttack)
		{
			Exit(0);
		}
	}
	void PlayerAttack2State::Exit(int a)
	{
		m_player->ChangeState(L"PlayerWalk");
	}

}
//end basecross
