
/*!
@file Scene.cpp
@brief シーン実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	void Scene::OnCreate(){
		try {
			//クリアする色を設定
			Col4 Col;
			Col.set(31.0f / 255.0f, 30.0f / 255.0f, 71.0f / 255.0f, 255.0f / 255.0f);
			SetClearColor(Col);
			//自分自身にイベントを送る
			//これにより各ステージやオブジェクトがCreate時にシーンにアクセスできる
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToGameStage");
			GameResourses();
		}
		catch (...) {
			throw;
		}
	}

	Scene::~Scene() {
	}

	void Scene::OnEvent(const shared_ptr<Event>& event) {
		if (event->m_MsgStr == L"ToGameStage") {
			//最初のアクティブステージの設定
			ResetActiveStage<StageTuboi>();
		}
	}

	void Scene::GameResourses()
	{
		auto& app = App::GetApp();

		auto path = app->GetDataDirWString();
		auto texPath = path + L"Textures/";
		auto modPath = path + L"Models/";
		auto SoundPath = path + L"Sounds/";

		//テクスチャ
		wstring strTexture = texPath + L"StoneBrick.png";
		app->RegisterTexture(L"StoneBrick", strTexture);

		//モデル

		//ボーンマルチメッシュ
		auto boneMultiModelMesh = MultiMeshResource::CreateBoneModelMultiMesh(modPath, L"Spearmen_Animation.bmf");//仮のプレイヤーメッシュ(槍兵)
		app->RegisterResource(L"Spearmen", boneMultiModelMesh);
		//ボーンマルチメッシュ用テクスチャ
		auto boneMultiModelTexture = modPath + L"Spearmen_T.png";
		app->RegisterTexture(L"SpearmenTexture", boneMultiModelTexture);

	}

}
//end basecross
