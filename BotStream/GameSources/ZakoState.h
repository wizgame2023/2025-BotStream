#pragma once
/*!
@file ZakoState.h
@brief ŽG‹›‚ĚXe[g
*/

#include "stdafx.h"

namespace basecross {
	//雑魚敵のステート関係
	class EnemyZako;
	//雑魚敵のステートの元となるクラス
	class EnemyZakoStateBase :public StateBase
	{
	protected:
		shared_ptr<EnemyZako> m_enemyZako;
	public:
		EnemyZakoStateBase(shared_ptr<GameObject>& obj) :
			StateBase(obj),
			m_enemyZako(dynamic_pointer_cast<EnemyZako>(obj))
		{

		}

		virtual void Enter() {}
		virtual void Update(float deltatime) {}
		virtual void Exit() {}
	};

	//何もないときのステート
	class EnemyZakoStandState :public EnemyZakoStateBase
	{
	private:
		float m_timeOfShot = 0.0f;//打つ時間経過を測る変数
		float m_timeMaxOfShot = 4.0f;//打つ時間の保存用変数
	public:
		EnemyZakoStandState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	//接近戦をするときの準備ステート
	class EnemyZakoPreparationforMeleeState :public EnemyZakoStateBase
	{
	private:
		float m_timeOfShot = 0.0f;//打つ時間経過を測る変数
		float m_timeMaxOfShot = 4.0f;//打つ時間の保存用変数
		float m_speed = 1.0f;//足の速さ
	public:
		EnemyZakoPreparationforMeleeState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();

		void SppedChange();//脚のスピードを変える処理
	};

	//攻撃をするときのステート(近距離)
	class EnemyZakoMeleeState :public EnemyZakoStateBase
	{
	private:
		float m_timeOfAttack = 0.0f;//攻撃時間経過を測る変数
		float m_timeMaxOfAttack = 2.0f;//攻撃時間の保存用変数

		float m_timeOfAttackAdd = 1.2f;//攻撃判定の発生時間

		bool m_Attack = true;//攻撃判定を出したかのフラグ
	public:
		EnemyZakoMeleeState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();

	};

	//球を打つ直前の軸合わせのときのステート
	class EnemyZakoAlignmentState :public EnemyZakoStateBase
	{
	private:
		float m_timeOfShot = 0.0f;//打つ時間経過を測る変数
		float m_timeMaxOfShot = 3.0f;//打つ時間の保存用変数

		float m_speed = 1.0f;//足の速度
	public:
		EnemyZakoAlignmentState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	//攻撃をするときのステート(遠距離)
	class EnemyZakoShotState :public EnemyZakoStateBase
	{
	private:
		float m_timeOfAttack = 0.0f;//打つ時間経過を測る変数
		float m_timeMaxOfAttack = 1.0f;//打つ時間の保存用変数
	public:
		EnemyZakoShotState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();

	};

	//ダメージを受けた雑魚敵
	class EnemyZakoHitState :public EnemyZakoStateBase
	{
	private:

	public:
		EnemyZakoHitState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	class EnemyZakoStateMachine :public StateMachineBase
	{
	public:
		EnemyZakoStateMachine(shared_ptr<GameObject>& obj)
		{
			AddState(L"Stand", shared_ptr<EnemyZakoStandState>(new EnemyZakoStandState(obj)));
			AddState(L"Shot", shared_ptr<EnemyZakoShotState>(new EnemyZakoShotState(obj)));
			AddState(L"Alignment", shared_ptr<EnemyZakoAlignmentState>(new EnemyZakoAlignmentState(obj)));
			AddState(L"PreparationforMelee", shared_ptr<EnemyZakoPreparationforMeleeState>(new EnemyZakoPreparationforMeleeState(obj)));
			AddState(L"Melee", shared_ptr<EnemyZakoMeleeState>(new EnemyZakoMeleeState(obj)));
			AddState(L"Hit", shared_ptr<EnemyZakoHitState>(new EnemyZakoHitState(obj)));

			ChangeState(L"Stand");
		}
	};

	// 飛ぶザコのステート----------------------------------------------------------

	//何もないときのステート
	class EnemyZakoFlyingStandState :public EnemyZakoStateBase
	{
	private:
		float m_timeOfShot = 0.0f;//打つ時間経過を測る変数
		float m_timeMaxOfShot = 4.0f;//打つ時間の保存用変数
	public:
		EnemyZakoFlyingStandState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	//接近戦をするときの準備ステート
	class EnemyZakoFlyingPreparationforMeleeState :public EnemyZakoStateBase
	{
	private:
		float m_timeOfShot = 0.0f;//打つ時間経過を測る変数
		float m_timeMaxOfShot = 4.0f;//打つ時間の保存用変数
	public:
		EnemyZakoFlyingPreparationforMeleeState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	//攻撃をするときのステート(近距離)
	class EnemyZakoFlyingMeleeState :public EnemyZakoStateBase
	{
	private:
		float m_timeOfAttack = 0.0f;//攻撃時間経過を測る変数
		float m_timeMaxOfAttack = 2.0f;//攻撃時間の保存用変数

		float m_timeOfAttackAdd = 1.2f;//攻撃判定の発生時間

		bool m_Attack = true;//攻撃判定を出したかのフラグ
	public:
		EnemyZakoFlyingMeleeState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();

	};

	//球を打つ直前の軸合わせのときのステート
	class EnemyZakoFlyingAlignmentState :public EnemyZakoStateBase
	{
	private:
		float m_timeOfShot = 0.0f;//打つ時間経過を測る変数
		float m_timeMaxOfShot = 4.0f;//打つ時間の保存用変数
	public:
		EnemyZakoFlyingAlignmentState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	//攻撃をするときのステート(遠距離)
	class EnemyZakoFlyingShotState :public EnemyZakoStateBase
	{
	private:
		float m_timeOfAttack = 0.0f;//打つ時間経過を測る変数
		float m_timeMaxOfAttack = 1.0f;//打つ時間の保存用変数
	public:
		EnemyZakoFlyingShotState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();

	};

	//ダメージを受けたとき
	class EnemyZakoFlyingHitState :public EnemyZakoStateBase
	{
	private:

	public:
		EnemyZakoFlyingHitState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};


	// 飛ぶ敵のステートマシン
	class EnemyZakoFlyingStateMachine :public StateMachineBase
	{
	public:
		EnemyZakoFlyingStateMachine(shared_ptr<GameObject>& obj)
		{
			AddState(L"Stand", shared_ptr<EnemyZakoFlyingStandState>(new EnemyZakoFlyingStandState(obj)));
			AddState(L"Shot", shared_ptr<EnemyZakoFlyingShotState>(new EnemyZakoFlyingShotState(obj)));
			AddState(L"Alignment", shared_ptr<EnemyZakoFlyingAlignmentState>(new EnemyZakoFlyingAlignmentState(obj)));
			AddState(L"PreparationforMelee", shared_ptr<EnemyZakoFlyingPreparationforMeleeState>(new EnemyZakoFlyingPreparationforMeleeState(obj)));
			AddState(L"Melee", shared_ptr<EnemyZakoFlyingMeleeState>(new EnemyZakoFlyingMeleeState(obj)));
			AddState(L"Hit", shared_ptr<EnemyZakoFlyingHitState>(new EnemyZakoFlyingHitState(obj)));

			ChangeState(L"Stand");
		}
	};
}