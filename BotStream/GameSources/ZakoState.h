#pragma once
/*!
@file ZakoState.h
@brief ŽG‹›‚ĚXe[g
*/

#include "stdafx.h"

namespace basecross {
	/*

	//ŽG‹›“G‚ĚXe[g‚ĚŚł‚Ć‚Č‚éN‰X
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

	//‰˝‚ŕ‚Č‚˘‚Ć‚«‚ĚXe[g
	class EnemyZakoStandState :public EnemyZakoStateBase
	{
	private:
		float m_timeOfShot = 0.0f;//‘Ĺ‚ÂŽžŠÔŚo‰ß‚đ‘Ş‚é•Ď”
		float m_timeMaxOfShot = 4.0f;//‘Ĺ‚ÂŽžŠÔ‚Ě•Ű‘¶—p•Ď”
	public:
		EnemyZakoStandState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	//Ú‹ßí‚đ‚·‚é‚Ć‚«‚ĚŹ€”őXe[g
	class EnemyZakoPreparationforMeleeState :public EnemyZakoStateBase
	{
	private:
		float m_timeOfShot = 0.0f;//‘Ĺ‚ÂŽžŠÔŚo‰ß‚đ‘Ş‚é•Ď”
		float m_timeMaxOfShot = 4.0f;//‘Ĺ‚ÂŽžŠÔ‚Ě•Ű‘¶—p•Ď”
		float m_speed = 1.0f;//‘«‚Ě‘¬‚ł
	public:
		EnemyZakoPreparationforMeleeState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();

		void SppedChange();//‹r‚ĚXs[h‚đ•Ď‚¦‚éŹ—ť
	};

	//ŤUŚ‚‚đ‚·‚é‚Ć‚«‚ĚXe[g(‹ß‹——Ł)
	class EnemyZakoMeleeState :public EnemyZakoStateBase
	{
	private:
		float m_timeOfAttack = 0.0f;//ŤUŚ‚ŽžŠÔŚo‰ß‚đ‘Ş‚é•Ď”
		float m_timeMaxOfAttack = 2.0f;//ŤUŚ‚ŽžŠÔ‚Ě•Ű‘¶—p•Ď”

		float m_timeOfAttackAdd = 1.2f;//ŤUŚ‚”»’č‚Ě”­¶ŽžŠÔ

		bool m_Attack = true;//ŤUŚ‚”»’č‚đŹo‚µ‚˝‚©‚Ět‰O
	public:
		EnemyZakoMeleeState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();

	};

	//‹…‚đ‘Ĺ‚Â’Ľ‘O‚ĚŽ˛Ť‡‚í‚ą‚Ě‚Ć‚«‚ĚXe[g
	class EnemyZakoAlignmentState :public EnemyZakoStateBase
	{
	private:
		float m_timeOfShot = 0.0f;//‘Ĺ‚ÂŽžŠÔŚo‰ß‚đ‘Ş‚é•Ď”
		float m_timeMaxOfShot = 3.0f;//‘Ĺ‚ÂŽžŠÔ‚Ě•Ű‘¶—p•Ď”

		float m_speed = 1.0f;//‘«‚Ě‘¬“x
	public:
		EnemyZakoAlignmentState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	//ŤUŚ‚‚đ‚·‚é‚Ć‚«‚ĚXe[g(‰“‹——Ł)
	class EnemyZakoShotState :public EnemyZakoStateBase
	{
	private:
		float m_timeOfAttack = 0.0f;//‘Ĺ‚ÂŽžŠÔŚo‰ß‚đ‘Ş‚é•Ď”
		float m_timeMaxOfAttack = 1.0f;//‘Ĺ‚ÂŽžŠÔ‚Ě•Ű‘¶—p•Ď”
	public:
		EnemyZakoShotState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();

	};

	//_[W‚đŽó‚Ż‚˝ŽG‹›“G
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

	// ”ň‚ÔUR‚ĚXe[g----------------------------------------------------------

	//‰˝‚ŕ‚Č‚˘‚Ć‚«‚ĚXe[g
	class EnemyZakoFlyingStandState :public EnemyZakoStateBase
	{
	private:
		float m_timeOfShot = 0.0f;//‘Ĺ‚ÂŽžŠÔŚo‰ß‚đ‘Ş‚é•Ď”
		float m_timeMaxOfShot = 4.0f;//‘Ĺ‚ÂŽžŠÔ‚Ě•Ű‘¶—p•Ď”
	public:
		EnemyZakoFlyingStandState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	//Ú‹ßí‚đ‚·‚é‚Ć‚«‚ĚŹ€”őXe[g
	class EnemyZakoFlyingPreparationforMeleeState :public EnemyZakoStateBase
	{
	private:
		float m_timeOfShot = 0.0f;//‘Ĺ‚ÂŽžŠÔŚo‰ß‚đ‘Ş‚é•Ď”
		float m_timeMaxOfShot = 4.0f;//‘Ĺ‚ÂŽžŠÔ‚Ě•Ű‘¶—p•Ď”
	public:
		EnemyZakoFlyingPreparationforMeleeState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	//ŤUŚ‚‚đ‚·‚é‚Ć‚«‚ĚXe[g(‹ß‹——Ł)
	class EnemyZakoFlyingMeleeState :public EnemyZakoStateBase
	{
	private:
		float m_timeOfAttack = 0.0f;//ŤUŚ‚ŽžŠÔŚo‰ß‚đ‘Ş‚é•Ď”
		float m_timeMaxOfAttack = 2.0f;//ŤUŚ‚ŽžŠÔ‚Ě•Ű‘¶—p•Ď”

		float m_timeOfAttackAdd = 1.2f;//ŤUŚ‚”»’č‚Ě”­¶ŽžŠÔ

		bool m_Attack = true;//ŤUŚ‚”»’č‚đŹo‚µ‚˝‚©‚Ět‰O
	public:
		EnemyZakoFlyingMeleeState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();

	};

	//‹…‚đ‘Ĺ‚Â’Ľ‘O‚ĚŽ˛Ť‡‚í‚ą‚Ě‚Ć‚«‚ĚXe[g
	class EnemyZakoFlyingAlignmentState :public EnemyZakoStateBase
	{
	private:
		float m_timeOfShot = 0.0f;//‘Ĺ‚ÂŽžŠÔŚo‰ß‚đ‘Ş‚é•Ď”
		float m_timeMaxOfShot = 4.0f;//‘Ĺ‚ÂŽžŠÔ‚Ě•Ű‘¶—p•Ď”
	public:
		EnemyZakoFlyingAlignmentState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	//ŤUŚ‚‚đ‚·‚é‚Ć‚«‚ĚXe[g(‰“‹——Ł)
	class EnemyZakoFlyingShotState :public EnemyZakoStateBase
	{
	private:
		float m_timeOfAttack = 0.0f;//‘Ĺ‚ÂŽžŠÔŚo‰ß‚đ‘Ş‚é•Ď”
		float m_timeMaxOfAttack = 1.0f;//‘Ĺ‚ÂŽžŠÔ‚Ě•Ű‘¶—p•Ď”
	public:
		EnemyZakoFlyingShotState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();

	};

	//_[W‚đŽó‚Ż‚˝‚Ć‚«
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


	// ”ň‚Ô“G‚ĚXe[g}V“
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
	*/
}