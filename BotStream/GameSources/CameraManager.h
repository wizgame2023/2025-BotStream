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
		weak_ptr<Camera> m_stageCamera;//ステージ上のカメラ
		shared_ptr<Camera> m_lockStageCamera;//ロックできたステージ上のカメラ
		shared_ptr<PNTStaticDraw> m_ptrDraw;
		shared_ptr<Stage> m_stage;
		float m_delta;

		CONTROLER_STATE m_controler;//コントローラー
		Vec2 m_contrloerVec;//コントローラーの右スティック入力

		float m_cameraAngleY;  //Playerから見てカメラのいる角度Y軸
		float m_cameraAngleX;  //Playerから見てカメラのいる角度X軸
		float m_range;//Playerからどのくらい離れるか
		float m_targetRange;//ロックオンの範囲
		float m_speedXAxis;//x軸の回転スピード
		float m_speedYAxis;//y軸の回転スピード
		float m_addAngleXAxis;//X軸の追加回転度
		float m_addAngleYAxis;//Y軸の追加回転度


		shared_ptr<CameraRayCast> m_cameraRayCast;
		Vec3 m_playerPos;//プレイヤーポジション
		Vec3 m_cameraPos;//カメラポジション

		bool m_movePlayerAngleFlag;//プレイヤーの向いている方向に回転するかのフラグ
		float m_targetAngleY;//ターゲットを見るために向く角度(Y軸)

		//ロックオンの処理////////////////////////////////////////////////////////////
		vector<shared_ptr<EnemyBase>> m_targets;//ターゲット候補
		vector<Vec3> m_targetsPos;//ロックオン候補のPosを保存する配列
		shared_ptr<EnemyBase> m_targetObj;//ターゲット対象
		bool m_lockOnFlag;//ロックオンできるかできないかの変数
		bool m_lockOnUse;//ロックオンするかしないかの変数
		bool m_lockOnChangeFlag;//ロックオンを変えたかのフラグ
		int m_lockOnNum;//ロックオン対象を決めるための変数

		bool m_stickFlag;//スティックを傾ける入力を受け取るかのフラグ

		vector<targetsDeta> m_targesDeta;//ロックオン対象のデータが入った配列
		vector<float> m_lockOnAngle;//ロックオン候補がPlayerにとってどの方向にいるのかの変数
		/////////////////////////////////////////////////////////////////////////////

		float m_meleeRange;//近接戦闘の範囲
		bool m_meleeFlag;//近接戦闘していいかのフラグ	
		
		//右か左かそれとも真ん中か
		enum LeftOrRight
		{
			Middle,
			Left,
			Right
		};

	public:
		CameraManager(const shared_ptr<Stage>& stagePtr,float range = 20.0f,float targetRange = 15.0f,float melleRange = 5.0f,
			float speedXAxis = 1.0f,float speedYAxis = 3.0f);
		~CameraManager();

		void OnCreate()override;//作成
		void OnUpdate()override;//更新

		void LockOn(shared_ptr<GameObject> lockOnObj, shared_ptr<Player> originObj);//ロックオン機能
		void MovePlayerAngle(float playerAngle);//Playerの背中を見える角度にする
		void MoveLockAt(Vec3 targetPos);//注視点の移動処理//ここを作業する
		bool MoveAngle(float targetAngle,int XorY);//回転度の移動処理
		void AdjustmentAngle();//角度の調整
		
		void UpdateTargesDeta(Vec3 playerPos);//ロックオン候補のデータを更新する関数
		void ChangeLockOn(int leftOrRight,float targetAngle);//ロックオン対象を変更する処理

		//ロックオン候補を決める関数
		void LockOnCandidate(vector<shared_ptr<EnemyBase>> enemyVec, Vec3 playerPos);
		//ロックオンの解除
		void LockOff(vector<shared_ptr<EnemyBase>> enemyVec);

		void CameraAngleXLimit(float maxRad= XMConvertToRadians(140.0f), float minRad = XMConvertToRadians(10.0f));//カメラのX軸回転の制限
		void CameraPosUpdate();//カメラのポジションの更新
		void InertialRotation();//慣性付きの回転処理
		
		void GetMeleeRange();
		void SetMeleeRange();
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



}
//end basecross

