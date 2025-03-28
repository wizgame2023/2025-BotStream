/*!
@file CameraManager.cpp
@brief カメラの制御処理
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	CameraManager::CameraManager(const shared_ptr<Stage>& stagePtr) :
		MyGameObject(stagePtr),
		m_cameraAngle(XMConvertToRadians(-90.0f)),
		m_range(20.0f)
	{

	}
	CameraManager::~CameraManager()
	{

	}

	//作成
	void CameraManager::OnCreate()
	{
		//ステージ上で使われるカメラを取得する
		auto stage = GetStage();
		m_stageCamera = OnGetDrawCamera();//ステージのカメラ取得
		auto lockStageCamera = m_stageCamera.lock();

		//もしステージ用のカメラを取得できなかったらreturnして自分を削除します
		if (!lockStageCamera)
		{
			GetStage()->RemoveGameObject<CameraManager>(GetThis<CameraManager>());
			return;
		}
	}

	//更新
	void CameraManager::OnUpdate()
	{
		auto delta = App::GetApp()->GetElapsedTime();
		auto lockStageCamera = m_stageCamera.lock();
		auto stage = GetStage();

		//もしステージ用のカメラを取得できなかったらreturnして自分を削除します
		if (!lockStageCamera)
		{
			GetStage()->RemoveGameObject<CameraManager>(GetThis<CameraManager>());
			return;
		}

		auto player = stage->GetSharedGameObject<Player>(L"Player");
		Vec3 playerPos = player->GetComponent<Transform>()->GetPosition();
		
		// インプットデバイスオブジェクト
		auto inputDevice = App::GetApp()->GetInputDevice(); // 様々な入力デバイスを管理しているオブジェクトを取得
		//コントローラーの取得
		auto m_controler = inputDevice.GetControlerVec()[0];
		Vec2 contrloerVec = Vec2(m_controler.fThumbRX, m_controler.fThumbRY);

		//左スティックを傾けてカメラがPlayerの周りを回転する処理
		float addAngle = 3.0f * contrloerVec.x * delta;//追加する角度を決めて
		m_cameraAngle += addAngle;//追加

		//RBボタンを押すと範囲内に対象がいるならロックオンそうでなければPlayerが向いている方向に回転する
		if (m_controler.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			auto test = 0;
		}


		//カメラをPlayerに追従
		lockStageCamera->SetEye(Vec3(playerPos.x + (cos(m_cameraAngle) * m_range), playerPos.y + 10.0f, playerPos.z + (sin(m_cameraAngle) * m_range)));

		//注視点はPlayerの位置よりも少し先にしたい
		lockStageCamera->SetAt(playerPos);
	}


	//デバック用のただの四角い奴
	Cube::Cube(const shared_ptr<Stage>& stagePtr,Vec3 pos,Vec3 rot, Vec3 scale,Col4 color) :
		ObjectNotMove(stagePtr),
		m_pos(pos),
		m_rot(rot),
		m_scale(scale),
		m_color(color)
	{

	}
	Cube::~Cube()
	{

	}

	void Cube::OnCreate()
	{
		//Transform設定
		auto m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

		//ドローメッシュの設定
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetDiffuse(m_color);
		ptrDraw->SetOwnShadowActive(false);//影は消す
	}



}
//end basecross
