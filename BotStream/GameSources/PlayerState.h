/*!
@file PlayerStaet.h
@brief プレイヤーステート
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class Player;	
	class Actor;
	class Cube;
	//プレイヤーステートの元となるクラス
	class PlayerStateBase :public StateBase
	{
	protected:
		shared_ptr<Player> m_player;//プレイヤーの実体
		CONTROLER_STATE m_controller;//コントローラー
		float m_timeOfPushAttackButton = 0.0f;//攻撃ボタンを押している時間
		shared_ptr<Actor> m_targetObj = nullptr;//ロックオン時の対象
		float m_targetDistance;//ターゲット対象との距離
		shared_ptr<SoundItem> m_SE = nullptr;//再生しているSE
		shared_ptr<XAudio2Manager> m_SEManager = nullptr;//SEなどを再生するためのマネージャ

	public:
		PlayerStateBase(shared_ptr<GameObject>& obj) :
			StateBase(obj),
			m_player(dynamic_pointer_cast<Player>(obj))
		{

		}
		~PlayerStateBase()
		{
		}

		
		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit() {};

		float GetTargetDistance();
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
		virtual void Exit();

	};

	//ダッシュステート
	class PlayerDashState : public PlayerStateBase
	{
	private:
		shared_ptr<Player> m_player;
		float m_timeOfPushAttackButton = 0.0f;//攻撃ボタンを押している時間
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
	private:
		//攻撃時間
		float m_timeMaxOfAttack = 0.5f;
		//攻撃時間計測
		float m_timeOfAttack;
		//次の攻撃の猶予時間
		float m_graceTimeOfNextAttack = 0.3f;
		//次の攻撃をするかのフラグ
		float m_nestAttackFlag = false;

		shared_ptr<Cube> m_AttackObj = nullptr;

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

	//攻撃ステート(２番目に出る攻撃)
	class PlayerAttack2State :public PlayerStateBase
	{
	private:
		//攻撃時間
		float m_timeMaxOfAttack = 0.7f;
		//攻撃時間計測
		float m_timeOfAttack;
		//次の攻撃の猶予時間
		float m_graceTimeOfNextAttack = 0.5f;
		//次の攻撃をするかのフラグ
		float m_nestAttackFlag = false;

		shared_ptr<Cube> m_AttackObj = nullptr;

	public:
		PlayerAttack2State(shared_ptr<GameObject>& obj) :
			PlayerStateBase(obj)
		{

		}
		~PlayerAttack2State()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();
	};

	//攻撃ステート(3番目に出る攻撃)
	class PlayerAttack3State :public PlayerStateBase
	{
	private:
		//攻撃時間
		float m_timeMaxOfAttack = 1.0f;
		//攻撃時間計測
		float m_timeOfAttack;
		//次の攻撃の猶予時間
		float m_graceTimeOfNextAttack = 0.9f;
		//次の攻撃をするかのフラグ
		float m_nestAttackFlag = false;

		shared_ptr<Cube> m_AttackObj = nullptr;

	public:
		PlayerAttack3State(shared_ptr<GameObject>& obj) :
			PlayerStateBase(obj)
		{

		}
		~PlayerAttack3State()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();
	};

	//攻撃ステート(最後に出る攻撃)
	class PlayerAttackExState :public PlayerStateBase
	{
	private:
		//攻撃時間
		float m_timeMaxOfAttack = 1.2f;
		//攻撃時間計測
		float m_timeOfAttack;
		////次の攻撃の猶予時間
		//float m_graceTimeOfNextAttack = 0.9f;
		////次の攻撃をするかのフラグ
		//float m_nestAttackFlag = false;

		shared_ptr<Cube> m_AttackObj = nullptr;

	public:
		PlayerAttackExState(shared_ptr<GameObject>& obj) :
			PlayerStateBase(obj)
		{

		}
		~PlayerAttackExState()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();
	};

	//攻撃ステート(必殺技)
	class PlayerAttackSpecialState :public PlayerStateBase
	{
	private:
		//攻撃時間
		float m_timeMaxOfAttack = 2.0f;
		//攻撃時間計測
		float m_timeOfAttack;
		////次の攻撃の猶予時間
		//float m_graceTimeOfNextAttack = 0.9f;
		////次の攻撃をするかのフラグ
		//float m_nestAttackFlag = false;

		shared_ptr<Cube> m_AttackObj = nullptr;

	public:
		PlayerAttackSpecialState(shared_ptr<GameObject>& obj) :
			PlayerStateBase(obj)
		{

		}
		~PlayerAttackSpecialState()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();
	};


	//攻撃ステート(最後に出る攻撃)
	class PlayerAttackLongState :public PlayerStateBase
	{
	private:
		//攻撃時間
		float m_timeMaxOfAttack = 1.2f;
		//攻撃時間計測
		float m_timeOfAttack;
		////次の攻撃の猶予時間
		//float m_graceTimeOfNextAttack = 0.9f;
		////次の攻撃をするかのフラグ
		//float m_nestAttackFlag = false;

		shared_ptr<Cube> m_AttackObj = nullptr;

	public:
		PlayerAttackLongState(shared_ptr<GameObject>& obj) :
			PlayerStateBase(obj)
		{

		}
		~PlayerAttackLongState()
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
			AddState(L"Attack1", shared_ptr<PlayerAttack1State>(new PlayerAttack1State(obj)));
			AddState(L"Attack2", shared_ptr<PlayerAttack2State>(new PlayerAttack2State(obj)));
			AddState(L"Attack3", shared_ptr<PlayerAttack3State>(new PlayerAttack3State(obj)));
			AddState(L"AttackEx", shared_ptr<PlayerAttackExState>(new PlayerAttackExState(obj)));
			AddState(L"AttackLong", shared_ptr<PlayerAttackLongState>(new PlayerAttackLongState(obj)));
			AddState(L"AttackSpecial", shared_ptr<PlayerAttackSpecialState>(new PlayerAttackSpecialState(obj)));

			//最初のステートはWalkここからいろんなステートに変更する イーブイみたいなもの
			ChangeState(L"PlayerWalk");
		}
		
	};

}
//end basecross
