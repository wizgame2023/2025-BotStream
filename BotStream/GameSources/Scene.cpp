
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

			ResetActiveStage<StageSanpei>();

		}

		// ここは後にWaveStageに変更する
		// (現在:2025/04/19/23:19時点ではシーン遷移が出来ないため応急処置としてStageSanpeiに遷移するものとします)
		if (event->m_MsgStr == L"ToWaveStage")
		{
			ResetActiveStage<StageSanpei>();
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
		// Gauge
		strTexture = texPath + L"PlayerGauge.png";
		app->RegisterTexture(L"PLGauge", strTexture);

		strTexture = texPath + L"PlayerHPMater.png";
		app->RegisterTexture(L"PLHP", strTexture);

		strTexture = texPath + L"PlayerSPMater.png";
		app->RegisterTexture(L"PLSP", strTexture);

		// Buttons
		strTexture = texPath + L"Buttons.png";
		app->RegisterTexture(L"Buttons", strTexture);

		strTexture = texPath + L"Select.png";
		app->RegisterTexture(L"Select", strTexture);
		
		// TextSprite
		strTexture = texPath + L"Texts.png";
		app->RegisterTexture(L"Texts", strTexture);
		strTexture = texPath + L"Numbers.png";
		app->RegisterTexture(L"Numbers", strTexture);
		strTexture = texPath + L"Questions.png";
		app->RegisterTexture(L"Questions", strTexture);
		strTexture = texPath + L"Answer.png";
		app->RegisterTexture(L"Answer", strTexture);
		strTexture = texPath + L"ResultText.png";
		app->RegisterTexture(L"ResultText", strTexture);
		strTexture = texPath + L"ClearOverText.png";
		app->RegisterTexture(L"ClearOverText", strTexture);

		//使っている武器を表示するUIテクスチャ
		strTexture = texPath + L"Katana.png";
		app->RegisterTexture(L"KatanaTex", strTexture);
		strTexture = texPath + L"Gun.png";
		app->RegisterTexture(L"GunTex", strTexture);

		//モデル

		//ボーンマルチメッシュ
		auto boneMultiModelMesh = MultiMeshResource::CreateBoneModelMultiMesh(modPath, L"Spearmen_Animation.bmf");//仮のプレイヤーメッシュ(槍兵)
		app->RegisterResource(L"Spearmen", boneMultiModelMesh);
		//ボーンマルチメッシュ用テクスチャ
		auto boneMultiModelTexture = modPath + L"Spearmen_T.png";
		app->RegisterTexture(L"SpearmenTexture", boneMultiModelTexture);

		//ボーンモデルの通常リソース
		auto modelMesh = MeshResource::CreateBoneModelMesh(modPath, L"Boss_1.bmf");
		app->RegisterResource(L"Boss1", modelMesh);

		//ボーンマルチメッシュ用テクスチャ
		auto texture = modPath + L"Boss_1_T.png";
		app->RegisterTexture(L"Tx_Boss1", texture);

		// エフェクトの登録
		// 仮置き--------------------------
		EffectManager::Instance().RegisterEffect(L"Laser", efkPath + L"Laser01.efk");
		EffectManager::Instance().RegisterEffect(L"Sword", efkPath + L"Sword.efk");
		EffectManager::Instance().RegisterEffect(L"Landing", efkPath + L"Landing.efk");
		// ---------------------------------

		// 本編-----------------------------
		EffectManager::Instance().RegisterEffect(L"Beam", efkPath + L"beam.efkefc");
		EffectManager::Instance().RegisterEffect(L"Dodge", efkPath + L"avoidance.efkefc");
		EffectManager::Instance().RegisterEffect(L"Dash", efkPath + L"dash.efkefc");
		EffectManager::Instance().RegisterEffect(L"PathBullet", efkPath + L"guntrajectory.efkefc");
		EffectManager::Instance().RegisterEffect(L"Slap", efkPath + L"slap.efkefc");
		EffectManager::Instance().RegisterEffect(L"SpinAttack", efkPath + L"spinningsword.efkefc");
		EffectManager::Instance().RegisterEffect(L"Charge", efkPath + L"charge.efkefc");
		// ---------------------------------

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
		app->RegisterWav(L"Landing", SoundPath + L"Landing2.wav");
		app->RegisterWav(L"Dash", SoundPath + L"Dash.wav");
		app->RegisterWav(L"ArmorBreak", SoundPath + L"ArmorBreak.wav");
		app->RegisterWav(L"Attack1", SoundPath + L"Attack1.wav");
		app->RegisterWav(L"Attack2", SoundPath + L"Attack2.wav");
		app->RegisterWav(L"Attack3", SoundPath + L"Attack3.wav");
		app->RegisterWav(L"HandGun", SoundPath + L"HandGun.wav");
		app->RegisterWav(L"Reload", SoundPath + L"Reload.wav");
		app->RegisterWav(L"CantShotSE", SoundPath + L"CantShot.wav");
		app->RegisterWav(L"AssaultRifle", SoundPath + L"AssaultRifle.wav");
		app->RegisterWav(L"LockOnSE", SoundPath + L"Lock-on.wav");
		app->RegisterWav(L"DamageVoiceSE", SoundPath + L"DamageVoice.wav");
		app->RegisterWav(L"StartVoiceSE", SoundPath + L"StartVoice2.wav");

	}

}
//end basecross
