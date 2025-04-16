/*!
@file PlayerStaet.h
@brief プレイヤーステート
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class Player;
	//プレイヤーステートの元となるクラス
	class PlayerStateBase :public StateBase
	{
	protected:
		shared_ptr<Player> m_player;//プレイヤーの実体
		CONTROLER_STATE m_controller;//コントローラー
	public:
		PlayerStateBase(shared_ptr<GameObject>& obj) :
			StateBase(obj),
			m_player(dynamic_pointer_cast<Player>(obj))
		{

		}
		~PlayerStateBase()
		{
		}

		
		virtual void Enter() {};
		virtual void Update(float deltaTime) 
		{
			//コントローラーを受け取る
			auto inputDevice = App::GetApp()->GetInputDevice();
			m_controller = inputDevice.GetControlerVec()[0];
		};
		virtual void Exit() {};
	};

	//徒歩ステート
	class PlayerWalkState :public PlayerStateBase
	{
	private:
		//shared_ptr<Player> m_player;
	public:
		PlayerWalkState(shared_ptr<GameObject>& obj) :
			PlayerStateBase(obj)
			//m_player(dynamic_pointer_cast<Player>(obj))
		{
		}

		~PlayerWalkState()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();
	};

	//回避ステート
	class PlayerDodgeState : public PlayerStateBase
	{
	private:
	public:
		PlayerDodgeState(shared_ptr<GameObject>& obj) :
			PlayerStateBase(obj)
		{

		}
		~PlayerDodgeState()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit(int a);//なぜかExitで引数を入れないとエラー吐きます
	};

	//ダッシュステート
	class PlayerDashState : public PlayerStateBase
	{
	private:
		shared_ptr<Player> m_player;
	public:
		PlayerDashState(shared_ptr<GameObject>& obj) :
			PlayerStateBase(obj),
			m_player(dynamic_pointer_cast<Player>(obj))
		{

		}
		~PlayerDashState()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();
	};

	//攻撃ステート(一番最初に出てくる攻撃)
	class PlayerAttack1State :public PlayerStateBase
	{
	public:
		PlayerAttack1State(shared_ptr<GameObject>& obj) :
			PlayerStateBase(obj)
		{

		}
		~PlayerAttack1State()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();
	};

	//プレイヤーステートマシン
	class PlayerStateMachine : public StateMachineBase
	{
	public:
		PlayerStateMachine(shared_ptr<GameObject>& obj)
		{
			//ステートの登録
			AddState(L"PlayerWalk", shared_ptr<PlayerWalkState>(new PlayerWalkState(obj)));
			AddState(L"Dodge", shared_ptr<PlayerDodgeState>(new PlayerDodgeState(obj)));
			AddState(L"Dash", shared_ptr<PlayerDashState>(new PlayerDashState(obj)));

			//最初のステートはWalkここからいろんなステートに変更する イーブイみたいなもの
			ChangeState(L"PlayerWalk");
		}
	};

}
//end basecross
