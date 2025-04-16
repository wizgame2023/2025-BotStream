/*!
@file PlayerStaet.h
@brief プレイヤーステート
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class Player;
	class PlayerWalkState :public StateBase
	{
	private:
		shared_ptr<Player> m_player;
	public:
		PlayerWalkState(shared_ptr<GameObject>& obj) :
			StateBase(obj),
			m_player(dynamic_pointer_cast<Player>(obj))
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
	class PlayerDodgeState : public StateBase
	{
	private:
		shared_ptr<Player> m_player;
		CONTROLER_STATE m_controller;
	public:
		PlayerDodgeState(shared_ptr<GameObject>& obj) :
			StateBase(obj),
			m_player(dynamic_pointer_cast<Player>(obj))
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
	class PlayerDashState : public StateBase
	{
	private:
		shared_ptr<Player> m_player;
	public:
		PlayerDashState(shared_ptr<GameObject>& obj) :
			StateBase(obj),
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
