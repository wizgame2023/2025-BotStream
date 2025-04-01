/*!
@file CameraManager.h
@brief カメラの管理
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross{
	class CameraManager : public MyGameObject
	{
	private:
		weak_ptr<Camera> m_stageCamera;//ステージ上のカメラ
		shared_ptr<Camera> m_lockStageCamera;//ロックできたステージ上のカメラ
		shared_ptr<PNTStaticDraw> m_ptrDraw;

		float m_cameraAngle;  //Playerから見てカメラのいる角度
		float m_range;//Playerからどのくらい離れるか

	public:
		CameraManager(const shared_ptr<Stage>& stagePtr);
		~CameraManager();

		void OnCreate()override;//作成
		void OnUpdate()override;//更新

		void LockOn(shared_ptr<GameObject> lockOnObj);//ロックオン機能
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

