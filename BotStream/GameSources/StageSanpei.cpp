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
		
		//Player作成
		auto player = AddGameObject<Player>(Vec3(0.0f,0.0f,0.0f),Vec3(0.0f,0.0f,0.0f),Vec3(1.0f,1.0f,1.0f));
		SetSharedGameObject(L"Player", player);

		AddGameObject<Cube>(Vec3(10.0f, 0.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		AddGameObject<Cube>(Vec3(10.0f, 0.0f, -10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f),Col4(1.0f,0.0f,0.0f,1.0f));
		AddGameObject<Cube>(Vec3(-10.0f, 0.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Col4(0.0f, 1.0f, 0.0f, 1.0f));
		AddGameObject<Cube>(Vec3(-10.0f, 0.0f, -10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Col4(0.0f, 0.0f, 1.0f, 1.0f));

		//カメラマネージャ作成
		AddGameObject<CameraManager>();
	}

}
//end basecross
