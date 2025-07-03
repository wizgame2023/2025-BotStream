/*!
@file CameraManager.h
@brief カメラの管理
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
#include "Player.h"

namespace basecross{
	//ターゲット候補のデータ
	class targetsDeta
	{
	public:
		float lockOnAngle;//プレイヤーから見てターゲットのいる方向
		int leftOrRight;//右か左どちらにいるのか　falseが左trueが右

		targetsDeta(float lockOnAngle, int leftOrRight) :
			lockOnAngle(lockOnAngle),
			leftOrRight(leftOrRight)
		{
		}
		~targetsDeta()
		{
		}
	};


	class EnemyBase;
	class CameraRayCast;
	class CameraManager : public MyGameObject
	{
	private:
		weak_ptr<Camera> m_stageCamera;		  //ステージ上のカメラ
		shared_ptr<Camera> m_lockStageCamera; //ロックできたステージ上のカメラ
		shared_ptr<PNTStaticDraw> m_ptrDraw;
		shared_ptr<Stage> m_stage;
		shared_ptr<Sprite> m_spriteAttack = nullptr;
		float m_delta;

		//ステートマシン用メンバ変数
		shared_ptr<StateMachineBase> m_stateMashine = nullptr;

		CONTROLER_STATE m_controler;//コントローラー
		Vec2 m_contrloerVec;		//コントローラーの右スティック入力

		//キーマウ
		POINT m_mouseCurrentPos; //マウスの現在位置
		POINT m_mouseBeforPos;	 //マウスの過去位置
		Vec2 m_mouseMoveVec;	 //マウスの移動ベクトル


		float m_cameraAngleY;		//Playerから見てカメラのいる角度Y軸
		float m_cameraAngleX;		//Playerから見てカメラのいる角度X軸
		float m_range;				//Playerからどのくらい離れるか
		float m_targetRange;		//ロックオンの範囲
		float m_speedXAxis;			//x軸の回転スピード
		float m_speedYAxis;			//y軸の回転スピード
		float m_addAngleXAxis;		//X軸の追加回転度
		float m_addAngleYAxis;		//Y軸の追加回転度
		float m_gunShiftLength;		//遠距離ステート用のプレイヤーからずれた長さ

		Vec3 m_pushPos = Vec3(5.0f, 10.0f, 5.0f);		//カメラがプレイヤーから離れる位置
		Vec3 m_pushAtPos = Vec3(-5.0f, -5.0f, -5.0f);	//カメラの注視点のずらす位置
		bool m_pushStart = true;



		//スプライト
		shared_ptr<Sprite> m_spriteAiming = nullptr;//射撃用のクロスヘアのスプライト


		shared_ptr<CameraRayCast> m_cameraRayCast;
		Vec3 m_playerPos;//プレイヤーポジション
		Vec3 m_cameraPos;//カメラポジション

		bool m_movePlayerAngleFlag; //プレイヤーの向いている方向に回転するかのフラグ
		float m_targetAngleY;	    //ターゲットを見るために向く角度(Y軸)
		float m_targetDis;			//LockOnTargetの距離デバック用

		//SE用
		shared_ptr<SoundItem> m_SE = nullptr;			  //再生しているSE
		shared_ptr<XAudio2Manager> m_SEManager = nullptr; //SEなどを再生するためのマネージャ

		//ロックオンの処理////////////////////////////////////////////////////////////
		vector<shared_ptr<EnemyBase>> m_targets;//ターゲット候補
		vector<Vec3> m_targetsPos;				//LockOnCanのPosを保存する配列
		shared_ptr<EnemyBase> m_targetObj;		//ターゲット対象
		bool m_lockOnFlag;						//ロックオンできるかできないかの変数
		bool m_lockOnUse;						//ロックオンするかしないかの変数
		bool m_lockOnChangeFlag;				//ロックオンを変えたかのフラグ
		int m_lockOnNum;						//LockOnTargetを決めるための変数

		bool m_stickFlag;						//スティックを傾ける入力を受け取るかのフラグ

		vector<targetsDeta> m_targesDeta;		//LockOnTargetのデータが入った配列
		vector<float> m_lockOnAngle;			//LockOnCanがPlayerにとってどの方向にいるのかの変数
		/////////////////////////////////////////////////////////////////////////////

		float m_meleeRange;	//近接戦闘の範囲
		bool m_meleeFlag;	//近接戦闘していいかのフラグ
		bool m_meleeNow;	//攻撃をしているフラグ
		bool m_gunNow;		//遠距離攻撃をしているフラグ

		bool m_PauseFlag;	//ポーズのフラグ

		//カーソルがワープした際のフラグ
		bool m_cursorFlagX = false; // x座標版
		bool m_cursorFlagY = false; // y座標版

		//右か左かそれとも真ん中か
		enum LeftOrRight
		{
			Middle,
			Left,
			Right
		};

	public:
		CameraManager(const shared_ptr<Stage>& stagePtr,float range = 15.0f,float targetRange = 25.0f,float melleRange = 5.0f,
			float speedXAxis = 1.0f,float speedYAxis = 3.0f);
		~CameraManager();

		void OnCreate()override;//作成
		void OnUpdate()override;//更新

		void LockOn(shared_ptr<GameObject> lockOnObj, shared_ptr<Player> originObj);//ロックオン機能
		void MovePlayerAngle(float playerAngle);									//Playerの背中を見える角度にする
		void MoveLockAt(Vec3 targetPos);											//注視点の移動処理//ここを作業する
		bool MoveAngle(float targetAngle,int XorY);									//回転度の移動処理
		void AdjustmentAngle();														//角度の調整
		
		void UpdateTargesDeta(Vec3 playerPos);					//LockOnCanのデータを更新する関数
		void ChangeLockOn(int leftOrRight,float targetAngle);	//LockOnTargetを変更する処理

		//LockOnCanを決める関数
		void LockOnCandidate(vector<shared_ptr<EnemyBase>> enemyVec, Vec3 playerPos);
		//ロックオンの解除
		void LockOff(vector<shared_ptr<EnemyBase>> enemyVec);
		//カメラのX軸回転の制限
		void CameraAngleXLimit(float maxRad= XMConvertToRadians(140.0f), float minRad = XMConvertToRadians(10.0f));
		bool CameraPosUpdate(float maxPushPosY = 10.0f, float maxGunLength = 0.0f,float CameraLenght = 15.0f,float cameraSpeed = 100.0f);//カメラのポジションの更新
		void InertialRotation(float MagnificationSpeed = 1.0f,float decelerationSpeed = 10.0f);//慣性付きの回転処理

		//カメラの操作をする処理
		void CameraControlNomalMode();
		void CameraControlShotMode();
		void CameraControlTransitionMode();
		void CameraControlStartMovieMode();

		//現在地から目的地までの移動処理
		float MoveToDestination(float nowOnePos,float destination, float speed = 20.0f);

		//ステート変更処理 引数に入れたステートに変更する
		void ChangeState(wstring stateName);
		
		//近距離攻撃をするかの処理のゲッターセッタ
		bool GetMeleeFlag();
		void SetMeleeFlag(bool onOff);

		//ターゲット対象との距離を渡す
		float GetTargetDis();

		//ポーズ処理のオンオフ
		void PoseSwitch(bool onOff);

		//近遠どちらの攻撃をするかの処理
		void MeleeFlagUpdate();

		//ロックオン処理
		void LockOn(shared_ptr<Player> player);
		//ロックオンする敵を決める処理
		void SelectTargetObj(vector<shared_ptr<EnemyBase>> enemyVec,float playerAngle);
		//ロックオンを解除する条件
		void ConditionsLockOff(vector<shared_ptr<EnemyBase>> enemyVec);

		//角度のゲッタセッタ
		//第一引数　X軸かY軸どちらの軸の角度を取るか
		float GetAngle(wstring XorY)
		{
			if (XorY == L"X")
			{
				return m_cameraAngleX;
			}
			if (XorY == L"Y")
			{
				return m_cameraAngleY;
			}

			return 0;
		}
		//第一引数　X軸かY軸どちらの軸の角度を取るか 第二引数 どの角度に向くか
		void SetAngle(wstring XorY,float angle)
		{
			if (XorY == L"X")
			{
				m_cameraAngleX = angle;
			}
			if (XorY == L"Y")
			{
				m_cameraAngleY = angle;
			}
		}

		//カメラのAtゲッタ
		Vec3 GetCameraAt()
		{
			return m_lockStageCamera->GetAt();
		}
		void SetCameraAt(Vec3 setAt)
		{
			m_lockStageCamera->SetAt(setAt);
		}

		//カメラのEyeセッタゲッタ
		Vec3 GetCameraEye()
		{
			return m_lockStageCamera->GetEye();
		}
		void SetCameraEye(Vec3 setEye)
		{
			m_lockStageCamera->SetEye(setEye);
		}

		//pushStartのゲッタセッタ
		bool GetPushStart()
		{
			return m_pushStart;
		}
		void SetPushStart(bool onOff)
		{
			m_pushStart = onOff;
		}

		//ポーズフラグのゲッタセッタ
		bool GetPose()
		{
			return m_PauseFlag;
		}
		void SetPause(bool onOff)
		{
			m_PauseFlag = onOff;
		}

		//pushPosのゲッタセッタ
		Vec3 GetPushPos()
		{
			return m_pushPos;
		}
		void SetPushPos(Vec3 pushPos)
		{
			m_pushPos = pushPos;
		}

		float GetAddAngleNAxis()
		{
			return Vec2(m_addAngleXAxis, m_addAngleYAxis).length();
		}

		//何の攻撃をしているかのセッタ
		void SetMeleeNow(bool onOff)
		{
			m_meleeNow = onOff;
		}
		void SetGunNow(bool onOff)
		{
			m_gunNow = onOff;
		}
		bool GetMeleeNow()
		{
			return m_meleeNow;
		}
		bool GetGunNow()
		{
			return m_gunNow;
		}

		//ターゲット対象を渡す関数
		shared_ptr<Actor> GetTargetObj();
	};

	//カメラのロックオン範囲
	class LockOnRange : public ObjectMove
	{
	private:
		Vec3 m_pos;//位置
		Vec3 m_rot;//回転
		Vec3 m_scale;//大きさ
		float m_range;//ロックオン射程

		shared_ptr<PNTStaticDraw> m_ptrDraw;
		shared_ptr<Transform> m_trans;
		weak_ptr<Player> m_player;//Player
	public:
		LockOnRange(const shared_ptr<Stage>& stagePtr,float range,shared_ptr<Player> player);
		~LockOnRange();

		void OnCreate()override;
		void OnUpdate()override;
	};

	class LockOnLook : public ObjectMove
	{
	private:
		//Vec3 m_pos;//位置
		Vec3 m_rot;//回転
		Vec3 m_scale;//大きさ
		Vec3 m_pushPos;//追加座標
		Col4 m_color;//色

		weak_ptr<Actor> m_parentObj;//追跡対象
		shared_ptr<Transform> m_trans;
	public:
		LockOnLook(const shared_ptr<Stage>& stagePtr, Vec3 rot, Vec3 scale,weak_ptr<Actor> parenatObj,Vec3 pushPos = Vec3(0.0f,0.0f,0.0f));
		~LockOnLook();
		void OnCreate()override;//作成
		void OnUpdate()override;//更新
	};


	//デバック用のただの四角いやつ
	class Cube : public ObjectNotMove
	{
	private:
		Vec3 m_pos;//位置
		Vec3 m_rot;//回転
		Vec3 m_scale;//大きさ
		Col4 m_color;//色
	public:
		Cube(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, Col4 color = Col4(1.0f, 1.0f, 1.0f, 1.0f));
		~Cube();
		void OnCreate()override;//作成
		void OnUpdate()override;
	};

	//デバック用のただの四角いやつ
	class EnemyCube : public Actor
	{
	private:
		Col4 m_color;//色
	public:
		EnemyCube(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, Col4 color = Col4(1.0f, 1.0f, 1.0f, 1.0f));
		~EnemyCube();
		void OnCreate()override;//作成
		void OnUpdate()override;//更新
	};


}
//end basecross

