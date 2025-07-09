/*!
@file CameraState.h
@brief カメラ挙動のステート
*/

#pragma once
#include "stdafx.h"
//#include "State.h"
//#include "CameraManager.h"

namespace basecross {
	class CameraManager;
	class EnemyBase;

	//カメラステートの元となるクラス
	class CameraStateBase :public StateBase
	{
	protected:
		shared_ptr<CameraManager> m_cameraManager;//プレイヤーの実体
		CONTROLER_STATE m_controller;//コントローラー
		Vec3 m_controllerVec;
		float m_timeOfPushAttackButton = 0.0f;//攻撃ボタンを押している時間
		shared_ptr<Actor> m_targetObj = nullptr;//ロックオン時の対象
		float m_targetDistance;//ターゲット対象との距離

		shared_ptr<Stage> m_stage;

		bool m_meleeFlag = true;
		bool m_meleeNow = false;
		bool m_gunNow = false;

	public:
		CameraStateBase(shared_ptr<GameObject>& obj) :
			StateBase(obj),
			m_cameraManager(dynamic_pointer_cast<CameraManager>(obj))
		{

		}
		~CameraStateBase()
		{
		}


		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();
	};



	//カメラの通常状態のステート(近接)
	class CameraNomalState :public CameraStateBase
	{
	private:

	public:
		CameraNomalState(shared_ptr<GameObject>& obj) :
			CameraStateBase(obj)
		{

		}
		~CameraNomalState()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();
	};
	//

	//カメラの銃モードのステート
	class CameraGunState :public CameraStateBase
	{
	private:

	public:
		CameraGunState(shared_ptr<GameObject>& obj) :
			CameraStateBase(obj)
		{

		}
		~CameraGunState()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();
	};
	//

	//カメラをプレイヤ方向に向かうステート
	class CameraResetState :public CameraStateBase
	{
	private:

	public:
		CameraResetState(shared_ptr<GameObject>& obj) :
			CameraStateBase(obj)
		{

		}
		~CameraResetState()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();
	};

	//カメラを通常モードから銃モードに切り替えるときの移動ステート
	class CameraNormalToGunState :public CameraStateBase
	{
	private:

	public:
		CameraNormalToGunState(shared_ptr<GameObject>& obj) :
			CameraStateBase(obj)
		{

		}
		~CameraNormalToGunState()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();

	};

	//カメラを銃モードから通常モードに切り替えるときの移動ステート
	class CameraGunToNormalState :public CameraStateBase
	{
	private:

	public:
		CameraGunToNormalState(shared_ptr<GameObject>& obj) :
			CameraStateBase(obj)
		{

		}
		~CameraGunToNormalState()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();

	};

	//ムービー用のカメラステート(開始時)
	class CameraStartMovieState_First :public CameraStateBase
	{
	private:

	public:
		CameraStartMovieState_First(shared_ptr<GameObject>& obj) :
			CameraStateBase(obj)
		{

		}
		~CameraStartMovieState_First()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();

	};

	//ムービー用のカメラステート(開始時)
	class CameraStartMovieState_Second :public CameraStateBase
	{
	private:

	public:
		CameraStartMovieState_Second(shared_ptr<GameObject>& obj) :
			CameraStateBase(obj)
		{

		}
		~CameraStartMovieState_Second()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();

	};

	//ボスムービー用のカメラステート(一番目)
	class CameraBossMovieState_First :public CameraStateBase
	{
	private:
		// ボス
		weak_ptr<EnemyBase> m_boss;

		// このステートの滞在時間
		float m_timeCount = 0.0f;
		// このステートの滞在可能時間
		float m_timeMax = 2.0f;

	public:
		CameraBossMovieState_First(shared_ptr<GameObject>& obj) :
			CameraStateBase(obj)
		{

		}
		~CameraBossMovieState_First()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();

	};

	//ボスムービー用のカメラステート(二番目)
	class CameraBossMovieState_Second :public CameraStateBase
	{
	private:
		// ボス
		weak_ptr<EnemyBase> m_boss;

		// このステートの滞在時間
		float m_timeCount = 0.0f;
		// このステートの滞在可能時間
		float m_timeMax = 2.2f;

	public:
		CameraBossMovieState_Second(shared_ptr<GameObject>& obj) :
			CameraStateBase(obj)
		{

		}
		~CameraBossMovieState_Second()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();

	};


	//ボスムービー用のカメラステート(三番目)
	class CameraBossMovieState_Three :public CameraStateBase
	{
	private:
		// ボス
		weak_ptr<EnemyBase> m_boss;

		// このステートの滞在時間
		float m_timeCount = 0.0f;
		// このステートの滞在可能時間
		float m_timeMax = 0.5f;

	public:
		CameraBossMovieState_Three(shared_ptr<GameObject>& obj) :
			CameraStateBase(obj)
		{

		}
		~CameraBossMovieState_Three()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();

	};


	//ボスムービー用のカメラステート(四番目)
	class CameraBossMovieState_Fourth :public CameraStateBase
	{
	private:
		// ボス
		weak_ptr<EnemyBase> m_boss;


		// このステートの滞在時間
		float m_timeCount = 0.0f;
		// このステートの滞在可能時間
		float m_timeMax = 2.0f;

	public:
		CameraBossMovieState_Fourth(shared_ptr<GameObject>& obj) :
			CameraStateBase(obj)
		{

		}
		~CameraBossMovieState_Fourth()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();

	};


	//ボスムービー用のカメラステート
	class CameraBossDieMovieState_First :public CameraStateBase
	{
	private:
		// ボス
		weak_ptr<EnemyBase> m_boss;

		//ボスの向いている方向
		float m_bossAngle = 0.0f;

		// このステートの滞在時間
		float m_timeCount = 0.0f;
		// このステートの滞在可能時間
		float m_timeMax = 2.0f;

	public:
		CameraBossDieMovieState_First(shared_ptr<GameObject>& obj) :
			CameraStateBase(obj)
		{

		}
		~CameraBossDieMovieState_First()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();

	};


	//カメラステートマシン
	class CameraStateMachine :public StateMachineBase
	{
	public:
		CameraStateMachine(shared_ptr<GameObject>& obj)
		{
			//ステートの登録
			//通常状態
			AddState(L"Normal", shared_ptr<CameraNomalState>(new CameraNomalState(obj)));
			//銃状態
			AddState(L"Gun", shared_ptr<CameraGunState>(new CameraGunState(obj)));
			//通常状態→銃状態
			AddState(L"NormalToGun", shared_ptr<CameraNormalToGunState>(new CameraNormalToGunState(obj)));
			//銃状態→通常状態
			AddState(L"GunToNormal", shared_ptr<CameraGunToNormalState>(new CameraGunToNormalState(obj)));
			//カメラをプレイヤーの方向に向かう状態
			AddState(L"Reset", shared_ptr<CameraResetState>(new CameraResetState(obj)));
			//ムービー用のカメラステート(開始時一段階目)
			AddState(L"StartMovie_First", shared_ptr<CameraStartMovieState_First>(new CameraStartMovieState_First(obj)));
			//ムービー用のカメラステート(開始時二段階目)
			AddState(L"StartMovie_Second", shared_ptr<CameraStartMovieState_Second>(new CameraStartMovieState_Second(obj)));
			//ボスムービー用のカメラステート
			AddState(L"BossMovie_First", shared_ptr<CameraBossMovieState_First>(new CameraBossMovieState_First(obj)));
			//ボスムービー用のカメラステート
			AddState(L"BossMovie_Second", shared_ptr<CameraBossMovieState_Second>(new CameraBossMovieState_Second(obj)));
			//ボスムービー用のカメラステート
			AddState(L"BossMovie_Three", shared_ptr<CameraBossMovieState_Three>(new CameraBossMovieState_Three(obj)));
			//ボスムービー用のカメラステート
			AddState(L"BossMovie_Fourth", shared_ptr<CameraBossMovieState_Fourth>(new CameraBossMovieState_Fourth(obj)));
			//ボスを倒したときのムービー用のカメラステート
			AddState(L"BossDieMovie_First", shared_ptr<CameraBossDieMovieState_First>(new CameraBossDieMovieState_First(obj)));

			ChangeState(L"Normal");
		}
	};

}