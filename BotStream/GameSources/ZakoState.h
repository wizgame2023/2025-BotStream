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

		shared_ptr<SoundItem> m_SE = nullptr;//再生しているSE
		shared_ptr<XAudio2Manager> m_SEManager = nullptr;//SEなどを再生するためのマネージャ
		float m_deltaScale = 0.0f;

	public:
		EnemyZakoStateBase(shared_ptr<GameObject>& obj) :
			StateBase(obj),
			m_enemyZako(dynamic_pointer_cast<EnemyZako>(obj))
		{

		}

		virtual void Enter() 
		{
			//SEマネージャー取得
			m_SEManager = App::GetApp()->GetXAudio2Manager();
		}
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

	//プレイヤーに距離を取る時のステート
	class EnemyZakoEscapeState :public EnemyZakoStateBase
	{
	private:

	public:
		EnemyZakoEscapeState(shared_ptr<GameObject>& obj) :
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

	//突進の準備ステート
	class EnemyZakoPreparationforChargeState :public EnemyZakoStateBase
	{
	private:
		float m_backDistance = 0.0f;//後ろに進む距離
		float m_speed = 0.0f;//速度
	public:
		EnemyZakoPreparationforChargeState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	//攻撃をするときのステート(突進)ステート
	class EnemyZakoChargeState :public EnemyZakoStateBase
	{
	private:
		float m_speed = 0.0f;//速度

		float m_timeOfAttack = 0.0f;//攻撃時間経過を測る変数
		float m_timeMaxOfAttack = 2.0f;//攻撃時間の保存用変数

		float m_timeOfAttackAdd = 1.2f;//攻撃判定の発生時間

		bool m_Attack = true;//攻撃判定を出したかのフラグ

		float m_playerdistance = 0.0f;//Playerとの現在距離
		float m_playerStartdistance = 0.0f;//Playerとの最初の距離
		float m_moveAngle = 0.0f;

	public:
		EnemyZakoChargeState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();

	};


	//球を打つ準備のステート
	class EnemyZakoPreparationforLongState :public EnemyZakoStateBase
	{
	private:
		float m_timeOfShot = 0.0f;//打つ時間経過を測る変数
		float m_timeMaxOfShot = 3.0f;//打つ時間の保存用変数

		float m_speed = 1.0f;//足の速度
	public:
		EnemyZakoPreparationforLongState(shared_ptr<GameObject>& obj) :
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
		float m_timeOfStartAttack = 0.5f;//弾が発射される時間
		float m_timeMaxOfAttack = 1.0f;//打つ時間の保存用変数

		bool m_attackFlag = true;//攻撃していいフラグ
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
		//ステート活動時間
		float timeOfStateCount;
		//このステートでいられる上限の時間
		float timeOfStateCountMax;
	public:
		EnemyZakoHitState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();
	};

	//やられたときの雑魚敵
	class EnemyZakoDieState :public EnemyZakoStateBase
	{
	private:
		float m_timeOfState = 0.0f;//ステートが始まってからの時間経過を測る変数
		float m_timeMaxOfState = 1.8f;//ステートでいられる上限時間の保存用変数

	public:
		EnemyZakoDieState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}

		virtual void Enter();
		virtual void Update(float deltatime);
		virtual void Exit();

	};

	//スタン時のステート(雑魚敵)
	class EnemyZakoStanState : public EnemyZakoStateBase
	{
	private:
		//スタン時間計測
		float m_stunTimeMax = 3.0f;
		float m_stunTimeCount;
	public:
		EnemyZakoStanState(shared_ptr<GameObject>& obj) :
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
			AddState(L"Escape", shared_ptr<EnemyZakoEscapeState>(new EnemyZakoEscapeState(obj)));//逃げる
			AddState(L"Shot", shared_ptr<EnemyZakoShotState>(new EnemyZakoShotState(obj)));
			AddState(L"PreparationforLong", shared_ptr<EnemyZakoPreparationforLongState>(new EnemyZakoPreparationforLongState(obj)));
			AddState(L"PreparationforMelee", shared_ptr<EnemyZakoPreparationforMeleeState>(new EnemyZakoPreparationforMeleeState(obj)));
			AddState(L"PreparationforCharge", shared_ptr<EnemyZakoPreparationforChargeState>(new EnemyZakoPreparationforChargeState(obj)));//突進準備
			AddState(L"Charge", shared_ptr<EnemyZakoChargeState>(new EnemyZakoChargeState(obj)));//突進
			AddState(L"Melee", shared_ptr<EnemyZakoMeleeState>(new EnemyZakoMeleeState(obj)));
			AddState(L"Hit", shared_ptr<EnemyZakoHitState>(new EnemyZakoHitState(obj)));
			AddState(L"Stun", shared_ptr<EnemyZakoStanState>(new EnemyZakoStanState(obj)));
			AddState(L"Die", shared_ptr<EnemyZakoDieState>(new EnemyZakoDieState(obj)));

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

		float m_speed = 1.0f;//足の速さ
	public:
		EnemyZakoFlyingPreparationforMeleeState(shared_ptr<GameObject>& obj) :
			EnemyZakoStateBase(obj)
		{

		}
		void SpeedChange();

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

		float m_speed = 1.0f;//足の速さ
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

	class EnemyZakoFlyingChargeState : public EnemyZakoStateBase
	{
		Vec3   m_chargeDir;       // 突進方向
		float  m_chargeSpeed = 20.0f;  // 突進速度
		float  m_chargeTime = 0.0f;   // 経過時間カウント
		float  m_maxChargeTime = 1.5f; // 突進継続時間（秒）

		bool m_Attack = true;//攻撃判定を出したかのフラグ
	public:
		EnemyZakoFlyingChargeState(shared_ptr<GameObject>& obj)
			: EnemyZakoStateBase(obj) {
		}

		virtual void Enter() override;
		virtual void Update(float deltaTime) override;
		virtual void Exit() override;
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
			AddState(L"Charge", shared_ptr<EnemyZakoFlyingChargeState>(new EnemyZakoFlyingChargeState(obj)));

			ChangeState(L"Stand");
		}
	};
}