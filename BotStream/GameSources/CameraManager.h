/*!
@file CameraManager.h
@brief カメラの管理
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
#include "Player.h"

namespace basecross{
	class Enemy;
	class CameraManager : public MyGameObject
	{
	private:
		weak_ptr<Camera> m_stageCamera;//ステージ上のカメラ
		shared_ptr<Camera> m_lockStageCamera;//ロックできたステージ上のカメラ
		shared_ptr<PNTStaticDraw> m_ptrDraw;

		float m_cameraAngle;  //Playerから見てカメラのいる角度
		float m_range;//Playerからどのくらい離れるか
		float m_targetRange;//ロックオンの範囲

		shared_ptr<Enemy> m_targetObj;//ターゲット対象
		bool m_lockOn;//ロックオンするかしないかの変数
		int m_lockOnNum;//ロックオン対象を決めるための変数

	public:
		CameraManager(const shared_ptr<Stage>& stagePtr,float range = 20.0f,float targetRange = 15.0f);
		~CameraManager();

		void OnCreate()override;//作成
		void OnUpdate()override;//更新

		void LockOn(shared_ptr<GameObject> lockOnObj, shared_ptr<Player> originObj);//ロックオン機能
		void MovePlayerAngle(float playerAngle);//Playerの背中を見える角度にする
		void AdjustmentAngle();//角度の調整

	};

	//カメラのレイキャスト
	class CameraRayCast :public ObjectMove
	{
	private:
		Vec3 m_pos;//位置
		Vec3 m_rot;//回転
		Vec3 m_scale;//大きさ

	public:
		CameraRayCast(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale);
		~CameraRayCast();

		void OnCreate()override;
		void OnUpdate()override;
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
		//void OnUpdate()override;
	};

}
//end basecross

