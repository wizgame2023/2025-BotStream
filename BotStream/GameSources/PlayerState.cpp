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
		////攻撃ステートに変更する 攻撃が実装されていないのでコメントアウト
		//if (m_controller.wPressedButtons & XINPUT_GAMEPAD_X)
		//{
		//	m_player->ChangeState(L"Attack1");
		//}

	
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
		////攻撃ステートに変更する 攻撃が実装されていないのでコメントアウト
		//if (m_controller.wPressedButtons & XINPUT_GAMEPAD_X)
		//{
		//	m_player->ChangeState(L"Attack1");
		//}

	}
	void PlayerDashState::Exit()
	{

	}


	//攻撃ステート(一番最初に出てくる攻撃)
	void PlayerAttack1State::Enter()
	{

	}
	void PlayerAttack1State::Update(float deltaTime)
	{
		
	}
	void PlayerAttack1State::Exit()
	{

	}


}
//end basecross
