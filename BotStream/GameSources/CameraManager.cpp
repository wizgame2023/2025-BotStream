/*!
@file CameraManager.cpp
@brief カメラの制御処理
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	CameraManager::CameraManager(const shared_ptr<Stage>& stagePtr) :
		MyGameObject(stagePtr),
		m_cameraAngle(XMConvertToRadians(270.0f)),
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
		m_lockStageCamera = m_stageCamera.lock();	

		//カメラとPlayerの距離ベクトルを測って大きさを決める
		auto player = stage->GetSharedGameObject<Player>(L"Player");
		Vec3 playerPos = player->GetComponent<Transform>()->GetPosition();
		
		//カメラをPlayerに追従
		m_lockStageCamera->SetEye(Vec3(playerPos.x + (cos(m_cameraAngle) * m_range), playerPos.y + 10.0f, playerPos.z + (sin(m_cameraAngle) * m_range)));
		//注視点はPlayerの位置よりも少し先にしたい
		m_lockStageCamera->SetAt(playerPos);

		//Vec3 CameraPos = m_lockStageCamera->GetEye();
		
		////クォータニオンの値を取得してカメラとPlayerの差を見えるようにするオブジェクトを出そう！
		//Vec3 CameraVec = CameraPos - playerPos;
		//Vec3 CameraScale = CameraVec = Vec3(abs(CameraVec.x), abs(CameraVec.y), abs(CameraVec.z));
		//Vec3 up = Vec3(0, 1, 0);
		//Mat4x4 CameraQt = (Mat4x4)XMMatrixLookAtLH(CameraPos, playerPos, -up);
		
		////レイキャスト生成
		//auto raycast = stage->AddGameObject<CameraRayCast>(Vec3(CameraVec.x/2, CameraVec.y / 2, -CameraVec.z / 2), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f,1.0f,CameraScale.z));
		//raycast->GetComponent<Transform>()->SetQuaternion((Quat)CameraQt);




		//もしステージ用のカメラを取得できなかったらreturnして自分を削除します
		if (!m_lockStageCamera)
		{
			GetStage()->RemoveGameObject<CameraManager>(GetThis<CameraManager>());
			return;
		}
	}

	//更新
	void CameraManager::OnUpdate()
	{
		auto delta = App::GetApp()->GetElapsedTime();
		m_lockStageCamera = m_stageCamera.lock();
		auto stage = GetStage();

		//もしステージ用のカメラを取得できなかったらreturnして自分を削除します
		if (!m_lockStageCamera)
		{
			GetStage()->RemoveGameObject<CameraManager>(GetThis<CameraManager>());
			return;
		}


		auto player = stage->GetSharedGameObject<Player>(L"Player");
		Vec3 playerPos = player->GetComponent<Transform>()->GetPosition();

		Vec3 hitPos;//当たった場所を保存する変数
		//当たり判定テスト
		TRIANGLE testTriangle;
		size_t testsize;
		//m_ptrDraw->HitTestStaticMeshSegmentTriangles(playerPos, m_lockStageCamera->GetEye(), hitPos,testTriangle,testsize);


		// インプットデバイスオブジェクト
		InputDevice inputDevice = App::GetApp()->GetInputDevice(); // 様々な入力デバイスを管理しているオブジェクトを取得
		//コントローラーの取得
		CONTROLER_STATE m_controler = inputDevice.GetControlerVec()[0];
		Vec2 contrloerVec = Vec2(m_controler.fThumbRX, m_controler.fThumbRY);

		//左スティックを傾けてカメラがPlayerの周りを回転する処理
		float addAngle = 3.0f * contrloerVec.x * delta;//追加する角度を決めて
		m_cameraAngle += addAngle;//追加


		float playerAngle = player->GetAngle();
		//RBボタンを押すと範囲内に対象がいるならロックオンそうでなければPlayerが向いている方向に回転する
		if (m_controler.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			//Playerの向いている方向の鏡合わせになるように角度を変更する
			MovePlayerAngle(playerAngle);
		}
		//角度リセット(デバック用)
		if (m_controler.wPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
			m_cameraAngle = XMConvertToRadians(270.0f);
		}


		//カメラをPlayerに追従
		m_lockStageCamera->SetEye(Vec3(playerPos.x + (cos(m_cameraAngle) * m_range), playerPos.y + 10.0f, playerPos.z + (sin(m_cameraAngle) * m_range)));
		//注視点はPlayerの位置よりも少し先にしたい
		m_lockStageCamera->SetAt(playerPos);

		//角度の調整0~360度までしか出ないようにする
		AdjustmentAngle();


		////デバック用
		wstringstream wss(L"");
		auto scene = App::GetApp()->GetScene<Scene>();

		wss /* << L"デバッグ用文字列 "*/
			<< L"\nPlayerから見てカメラの角度: " << XMConvertToDegrees(m_cameraAngle)
			<< L"\nPlayerの向いている角度: " << XMConvertToDegrees(-playerAngle)
			<< L"\n当たった場所x: " << hitPos.x
			<< L"\n当たった場所y: " << hitPos.y
			<< L"\n当たった場所z: " << hitPos.z
			//<<L"\nコントローラーの入力 x:"<<contrloerVec.x<<L" y:"<<contrloerVec.y
			//<<L"\nFPS:"<< 1.0f/delta
			<< endl;

		scene->SetDebugString(wss.str());

	}

	//ロックオン機能 対象のオブジェクトを中心点とする
	void CameraManager::LockOn(shared_ptr<GameObject> lockOnObj)
	{
		Vec3 lockOnPos = lockOnObj->GetComponent<Transform>()->GetPosition();

		m_lockStageCamera->SetAt(lockOnPos);
	}

	//Playerの向いている方向の鏡合わせになるように角度を変更する
	void CameraManager::MovePlayerAngle(float playerAngle)
	{
		m_cameraAngle = -playerAngle + XMConvertToRadians(180.0f);
	}

	//角度の調整0~360度までしか出ないようにする
	void CameraManager::AdjustmentAngle()
	{
		if (m_cameraAngle >= XMConvertToRadians(360.0f))
		{
			m_cameraAngle -= XMConvertToRadians(360.0f);
		}
		else if (m_cameraAngle < XMConvertToRadians(0.0f))
		{
			m_cameraAngle += XMConvertToRadians(360.0f);
		}
	}


	//カメラのレイキャスト用
	CameraRayCast::CameraRayCast(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale) :
		ObjectMove(stagePtr),
		m_pos(pos),
		m_rot(rot),
		m_scale(scale)
	{

	}
	CameraRayCast::~CameraRayCast()
	{

	}

	void CameraRayCast::OnCreate()
	{
		//Transform設定
		auto m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

		//ドローメッシュの設定
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetTextureResource(L"StoneBrick");
		//ptrDraw->SetDiffuse(m_color);
		//ptrDraw->SetOwnShadowActive(false);//影は消す
		ptrDraw->SetDrawActive(true);

	}
	void CameraRayCast::OnUpdate()
	{

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
		ptrDraw->SetDrawActive(true);
		//ptrDraw->HitTestSkinedMeshSegmentTriangles();

		//ptrDraw->HitT
	}



}
//end basecross
