/*!
@file StageSanpei.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------
	void StageSanpei::CreateViewLight() {
		const Vec3 eye(0.0f, 15.0f, -15.0f);
		const Vec3 at(0.0f);
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(eye);
		PtrCamera->SetAt(at);
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//PtrMultiLight->
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}



	void StageSanpei::OnCreate() {
		try {
			//ビューとライトの作成
			CreateViewLight();
		}
		catch (...) {
			throw;
		}	

		CreateSharedObjectGroup(L"Actor");

		auto grond = AddGameObject<Ground>();
		grond->AddTag(L"CameraObstacles");//カメラの障害になりえる物に付けられるタグ
		grond->GetComponent<Transform>()->SetScale(Vec3(500.0f, 1.0f, 500.0f));

		
		//Player作成
		auto player = AddGameObject<Player>(Vec3(0.0f,0.0f,0.0f),Vec3(0.0f,0.0f,0.0f),Vec3(1.0f,1.0f,1.0f));
		SetSharedGameObject(L"Player", player);

		//テスト用のキューブ
		AddGameObject<Cube>(Vec3(10.0f, 0.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		AddGameObject<Cube>(Vec3(10.0f, 0.0f, -10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Col4(1.0f, 0.0f, 0.0f, 1.0f));
		AddGameObject<Cube>(Vec3(10.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Col4(1.0f, 1.0f, 0.0f, 1.0f));
		AddGameObject<Cube>(Vec3(-10.0f, 0.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Col4(0.0f, 1.0f, 0.0f, 1.0f));
		AddGameObject<Cube>(Vec3(-10.0f, 0.0f, -10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Col4(0.0f, 0.0f, 1.0f, 1.0f));
		AddGameObject<Cube>(Vec3(-10.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Col4(1.0f, 0.0f, 1.0f, 1.0f));
		AddGameObject<Cube>(Vec3(0.0f, 0.0f, -8.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Col4(1.0f, 0.0f, 1.0f, 1.0f));
		AddGameObject<Cube>(Vec3(0.0f, 0.0f, +10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Col4(1.0f, 0.5f, 0.5f, 1.0f));

		//カメラを邪魔するためのキューブ
		//auto zyama = AddGameObject<Cube>(Vec3(0.0f, 0.0f, +15.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(30.0f, 50.0f, 1.0f), Col4(1.0f, 0.5f, 0.5f, 1.0f));
		//zyama->AddTag(L"CameraObstacles");
		//zyama =AddGameObject<Cube>(Vec3(0.0f, 0.0f, -15.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(30.0f, 50.0f, 1.0f), Col4(1.0f, 0.5f, 0.5f, 1.0f));
		//zyama->AddTag(L"CameraObstacles");
		//zyama =AddGameObject<Cube>(Vec3(15.0f, 0.0f, 0.0f), Vec3(0.0f, XMConvertToRadians(90.0f), 0.0f), Vec3(30.0f, 50.0f, 1.0f), Col4(1.0f, 0.5f, 0.5f, 1.0f));
		//zyama->AddTag(L"CameraObstacles");
		//zyama =AddGameObject<Cube>(Vec3(-15.0f, 0.0f, 0.0f), Vec3(0.0f, XMConvertToRadians(90.0f), 0.0f), Vec3(30.0f, 50.0f, 1.0f), Col4(1.0f, 0.5f, 0.5f, 1.0f));
		//zyama->AddTag(L"CameraObstacles");
		//zyama =AddGameObject<Cube>(Vec3(0.0f, 0.0f, +20.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(50.0f, 50.0f, 1.0f), Col4(1.0f, 0.5f, 0.5f, 1.0f));
		//zyama->AddTag(L"CameraObstacles");
		//zyama =AddGameObject<Cube>(Vec3(0.0f, 0.0f, -20.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(50.0f, 50.0f, 1.0f), Col4(1.0f, 0.5f, 0.5f, 1.0f));
		//zyama->AddTag(L"CameraObstacles");
		//zyama =AddGameObject<Cube>(Vec3(20.0f, 0.0f, 0.0f), Vec3(0.0f, XMConvertToRadians(90.0f), 0.0f), Vec3(50.0f, 50.0f, 1.0f), Col4(1.0f, 0.5f, 0.5f, 1.0f));
		//zyama->AddTag(L"CameraObstacles");
		//zyama =AddGameObject<Cube>(Vec3(-20.0f, 0.0f, 0.0f), Vec3(0.0f, XMConvertToRadians(90.0f), 0.0f), Vec3(50.0f, 50.0f, 1.0f), Col4(1.0f, 0.5f, 0.5f, 1.0f));
		//zyama->AddTag(L"CameraObstacles");


		//AddGameObject<Cube>(Vec3(0.0f, -1.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(30.0f, 1.0f, 30.0f), Col4(1.0f, 1.0f, 1.0f, 1.0f));

		//AddGameObject<Enemy>(Vec3(10.0f, 0.0f, -10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		//エネミーマネージャー
		auto enemyMgr = AddGameObject<EnemyManager>();
		SetSharedGameObject(L"EnemyManager", enemyMgr);

		//enemyMgr->InstEnemy(Vec3(10.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		enemyMgr->InstEnemy(Vec3(10.0f, 0.0f, -10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		//enemyMgr->InstEnemy(Vec3(10.0f, 0.0f, +10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		//enemyMgr->InstEnemy(Vec3(-10.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		//enemyMgr->InstEnemy(Vec3(-10.0f, 0.0f, -10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		//enemyMgr->InstEnemy(Vec3(-10.0f, 0.0f, +10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		//enemyMgr->InstEnemy(Vec3(0.0f, 0.0f, +10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		//enemyMgr->InstEnemy(Vec3(0.0f, 0.0f, -10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));

		//カメラマネージャ作成
		auto cameraManager = AddGameObject<CameraManager>();
		SetSharedGameObject(L"CameraManager", cameraManager);
	}

	void StageSanpei::OnUpdate()
	{
		//エフェクト関係
		EffectManager::Instance().InterfaceUpdate();
	}

	void StageSanpei::OnDraw()
	{
		EffectManager::Instance().InterfaceDraw();
	}

}
//end basecross
