
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
			EffectManager::Instance().CreateEfkInterface();
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
		auto efkPath = path + L"Effect/";

		//テクスチャ
		wstring strTexture = texPath + L"StoneBrick.png";
		app->RegisterTexture(L"StoneBrick", strTexture);
		
		// UISprite
		// Player's
		strTexture = texPath + L"PlayerGauge.png";
		app->RegisterTexture(L"PLGauge", strTexture);

		// Buttons
		strTexture = texPath + L"Buttons.png";
		app->RegisterTexture(L"Buttons", strTexture);
		
		// TextSprite
		strTexture = texPath + L"Texts.png";
		app->RegisterTexture(L"Texts", strTexture);
		strTexture = texPath + L"Numbers.png";
		app->RegisterTexture(L"Numbers", strTexture);

		//モデル

		//ボーンマルチメッシュ
		auto boneMultiModelMesh = MultiMeshResource::CreateBoneModelMultiMesh(modPath, L"Spearmen_Animation.bmf");//仮のプレイヤーメッシュ(槍兵)
		app->RegisterResource(L"Spearmen", boneMultiModelMesh);
		//ボーンマルチメッシュ用テクスチャ
		auto boneMultiModelTexture = modPath + L"Spearmen_T.png";
		app->RegisterTexture(L"SpearmenTexture", boneMultiModelTexture);

		// エフェクトの登録
		EffectManager::Instance().RegisterEffect(L"Laser", efkPath + L"Laser01.efk");
		EffectManager::Instance().RegisterEffect(L"Sword", efkPath + L"Sword.efk");
		EffectManager::Instance().RegisterEffect(L"Landing", efkPath + L"Landing.efk");

		//BGM
		app->RegisterWav(L"Title", SoundPath + L"Title.wav");
		app->RegisterWav(L"Diagnosis", SoundPath + L"Diagnosis.wav");
		app->RegisterWav(L"SelectStage", SoundPath + L"SelectStage.wav");
		app->RegisterWav(L"EnemyWave", SoundPath + L"EnemyWave.wav");
		app->RegisterWav(L"BossWave", SoundPath + L"BossWave.wav");
		app->RegisterWav(L"Result", SoundPath + L"Result.wav");


		//SE
		app->RegisterWav(L"Decision", SoundPath + L"Decision.wav");
		app->RegisterWav(L"Decision2", SoundPath + L"Decision2.wav");
		app->RegisterWav(L"Landing", SoundPath + L"Landing.wav");
		app->RegisterWav(L"Dash", SoundPath + L"Dash.wav");
		app->RegisterWav(L"ArmorBreak", SoundPath + L"ArmorBreak.wav");
		app->RegisterWav(L"Attack1", SoundPath + L"Attack1.wav");
		app->RegisterWav(L"Attack2", SoundPath + L"Attack2.wav");
		app->RegisterWav(L"Attack3", SoundPath + L"Attack3.wav");
		app->RegisterWav(L"HandGun", SoundPath + L"HandGun.wav");
		app->RegisterWav(L"Reload", SoundPath + L"Reload.wav");
		app->RegisterWav(L"AssaultRifle", SoundPath + L"AssaultRifle.wav");

	}

}
//end basecross
