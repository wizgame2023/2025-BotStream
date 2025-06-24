
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
			
			ResetActiveStage<TitleStage>();

		}

		if (event->m_MsgStr == L"EnemyTest") {
			//最初のアクティブステージの設定

			ResetActiveStage<StageTuboi>();

		}

		if (event->m_MsgStr == L"ToStageSelect")
		{
			ResetActiveStage<StageSelect>();

		}
		
		if (event->m_MsgStr == L"ToPersonalityStage")
		{
			ResetActiveStage<StageSato>();

		}

		// ここは後にWaveStageに変更する
		// (現在:2025/04/19/23:19時点ではシーン遷移が出来ないため応急処置としてStageSanpeiに遷移するものとします)
		if (event->m_MsgStr == L"ToWaveStage")
		{
			ResetActiveStage<StageFirst>();
		}

		if (event->m_MsgStr == L"ToGameOver") {
			//最初のアクティブステージの設定

			ResetActiveStage<GameOver>();

		}

		if (event->m_MsgStr == L"ToGameClear") {
			//最初のアクティブステージの設定

			ResetActiveStage<GameClear>();

		}

		if (event->m_MsgStr == L"ToTitleStage") {
			//最初のアクティブステージの設定

			ResetActiveStage<TitleStage>();

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
		strTexture = texPath + L"Fadeout.png";
		app->RegisterTexture(L"Fadeout", strTexture);
		strTexture = texPath + L"SlowSpeite.png";
		app->RegisterTexture(L"SlowTex", strTexture);
		strTexture = texPath + L"Door.png";
		app->RegisterTexture(L"DoorTex", strTexture);

		// title
		strTexture = texPath + L"TitleBack.png";
		app->RegisterTexture(L"TitleBack", strTexture);
		strTexture = texPath + L"TitleLogo2.png";
		app->RegisterTexture(L"TitleLogo", strTexture);
		strTexture = texPath + L"TitleText.png";
		app->RegisterTexture(L"TitleText", strTexture);

		// PlayerType
		strTexture = texPath + L"PlayerTypes.png";
		app->RegisterTexture(L"PlayerType", strTexture);

		// StageSelect
		strTexture = texPath + L"StageSelectString.png";
		app->RegisterTexture(L"StageSelectString", strTexture);
		strTexture = texPath + L"SelectBack2.png";
		app->RegisterTexture(L"SelectBack", strTexture);
		strTexture = texPath + L"StagePhoto1_1.png";

		// StageSelectのステージ写真
		app->RegisterTexture(L"Stage1-1Tex", strTexture);
		strTexture = texPath + L"StagePhoto1_2.png";
		app->RegisterTexture(L"Stage1-2Tex", strTexture);

		// GameClear / GameOver
		strTexture = texPath + L"GameOverBack.png";
		app->RegisterTexture(L"GameOverBack", strTexture);
		strTexture = texPath + L"GameClearBack.png";
		app->RegisterTexture(L"GameClearBack", strTexture);

		// UISprite
		// Gauge
		strTexture = texPath + L"PlayerGauge.png";
		app->RegisterTexture(L"PLGauge", strTexture);

		strTexture = texPath + L"PlayerHPMater.png";
		app->RegisterTexture(L"PLHP", strTexture);

		strTexture = texPath + L"PlayerSPMater.png";
		app->RegisterTexture(L"PLSP", strTexture);

		//敵のゲージフレーム
		strTexture = texPath + L"ZakoGaugeFrame.png";
		app->RegisterTexture(L"ZakoGaugeFrame", strTexture);
		//敵のHPバー
		strTexture = texPath + L"BossHPMater2.png";
		app->RegisterTexture(L"BossHPMater", strTexture);
		//雑魚敵のHPバー
		strTexture = texPath + L"ZakoHPMater3.png";
		app->RegisterTexture(L"ZakoHPMater", strTexture);
		// ボスのアーマーゲージ
		strTexture = texPath + L"BossArmor.png";
		app->RegisterTexture(L"BossArmor", strTexture);
		// ボスのゲージフレーム
		strTexture = texPath + L"BossGaugeFrame.png";
		app->RegisterTexture(L"BossGaugeFrame", strTexture);


		// Floor
		strTexture = texPath + L"FloorTex2.png";
		app->RegisterTexture(L"FloorTex", strTexture);

		// Buttons
		strTexture = texPath + L"Buttons.png";
		app->RegisterTexture(L"Buttons", strTexture);

		strTexture = texPath + L"Select.png";
		app->RegisterTexture(L"Select", strTexture);
		
		// PauseSprite
		strTexture = texPath + L"PauseBack.png";
		app->RegisterTexture(L"PauseBack", strTexture);
		strTexture = texPath + L"PauseText.png";
		app->RegisterTexture(L"PauseText", strTexture);
		strTexture = texPath + L"AudioMater.png";
		app->RegisterTexture(L"AudioMater", strTexture);
		strTexture = texPath + L"AudioOther.png";
		app->RegisterTexture(L"AudioOther", strTexture);

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

		// 戦闘UI
		strTexture = texPath + L"Attack_UI.png";
		app->RegisterTexture(L"KatanaTex", strTexture);
		strTexture = texPath + L"Gun_UI.png";
		app->RegisterTexture(L"GunTex", strTexture);
		strTexture = texPath + L"Avoidance_UI.png";
		app->RegisterTexture(L"AvoidTex", strTexture);

		// パーツのステータス
		strTexture = texPath + L"PartsUI1_2.png";
		app->RegisterTexture(L"PartsTestTEX", strTexture);
		strTexture = texPath + L"PartsUI2_2.png";
		app->RegisterTexture(L"PartsHiMoterTEX", strTexture);
		strTexture = texPath + L"PartsUI3_2.png";
		app->RegisterTexture(L"PartsBattlePatchTEX", strTexture);
		app->RegisterTexture(L"Parts3TEX", strTexture);
		strTexture = texPath + L"PartsTexts.png";
		app->RegisterTexture(L"PartsText", strTexture);

		//パーツ用のテクスチャ
		strTexture = texPath + L"gear.png";
		app->RegisterTexture(L"PartsTEX", strTexture);

		
		//ターゲット範囲用テクスチャ
		strTexture = texPath + L"Bar.png";
		app->RegisterTexture(L"BarTex", strTexture);

		//射撃用テクスチャ
		strTexture = texPath + L"Aiming.png";
		app->RegisterTexture(L"AimingTex", strTexture);

		// その他ゲーム中テクスチャ
		strTexture = texPath + L"Shock_wave_ring001.png";
		app->RegisterTexture(L"Tex_Shockwave", strTexture);

		//壁用のテクスチャ
		strTexture = texPath + L"WallTexture.png";
		app->RegisterTexture(L"WallTex", strTexture);

		// ステージの壁とか
		strTexture = texPath + L"WallSide.png";
		app->RegisterTexture(L"WallSideTex", strTexture);
		strTexture = texPath + L"WallFront.png";
		app->RegisterTexture(L"WallFrontTex", strTexture);
		strTexture = texPath + L"Ceiling.png";
		app->RegisterTexture(L"CeilingTex", strTexture);

		//スタティックメッシュ
		auto staticModelMesh = MeshResource::CreateStaticModelMesh(modPath, L"Parts_A.bmf");//パーツメッシュ
		app->RegisterResource(L"PartsMesh", staticModelMesh);
		staticModelMesh = MeshResource::CreateStaticModelMesh(modPath, L"Parts_C.bmf");//パーツメッシュ(モーター)
		app->RegisterResource(L"MoterPartsMesh", staticModelMesh);

		auto staticMultiModelMesh = MultiMeshResource::CreateStaticModelMultiMesh(modPath, L"Parts_B.bmf");//パーツメッシュ(パッチ)
		app->RegisterResource(L"PatchPartsMesh", staticMultiModelMesh);

		//ボーンマルチメッシュ
		auto boneMultiModelMesh = MultiMeshResource::CreateBoneModelMultiMesh(modPath, L"Spearmen_Animation.bmf");//仮のプレイヤーメッシュ(槍兵)
		app->RegisterResource(L"Spearmen", boneMultiModelMesh);

		boneMultiModelMesh = MultiMeshResource::CreateBoneModelMultiMesh(modPath, L"Player_test.bmf");//仮のプレイヤーメッシュ
		app->RegisterResource(L"PlayerModelTest", boneMultiModelMesh);
		boneMultiModelMesh = MultiMeshResource::CreateBoneModelMultiMesh(modPath, L"Player.bmf");//仮のプレイヤーメッシュ
		app->RegisterResource(L"PlayerModelTestVer2.0", boneMultiModelMesh);
		boneMultiModelMesh = MultiMeshResource::CreateBoneModelMultiMesh(modPath, L"Enemy_B.bmf");//雑魚敵のメッシュ(空中)
		app->RegisterResource(L"Enemy_B", boneMultiModelMesh);

		auto boneModelMesh = MeshResource::CreateBoneModelMesh(modPath, L"Enemy_A.bmf");//雑魚敵のメッシュ(遠距離)
		app->RegisterResource(L"Enemy_A", boneModelMesh);
		boneModelMesh = MeshResource::CreateBoneModelMesh(modPath, L"Enemy_C.bmf");//雑魚敵のメッシュ(近距離)
		app->RegisterResource(L"Enemy_C", boneModelMesh);

		//スタティックメッシュ用テクスチャ
		auto boneMultiModelTexture = modPath + L"Spearmen_T.png";
		app->RegisterTexture(L"SpearmenTexture", boneMultiModelTexture);

		//ボーンマルチメッシュ用テクスチャ
		auto staticModelTexture = modPath + L"Parts_A_T.png";
		app->RegisterTexture(L"PartsTex", staticModelTexture);
		staticModelTexture = modPath + L"Parts_B_T.png";
		app->RegisterTexture(L"PatchPartsTex", staticModelTexture);
		staticModelTexture = modPath + L"Parts_C_T.png";
		app->RegisterTexture(L"MoterPartsTex", staticModelTexture);

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
		EffectManager::Instance().RegisterEffect(L"Stun", efkPath + L"stun.efkefc");
		EffectManager::Instance().RegisterEffect(L"ArmorBreak", efkPath + L"shieldbreak.efkefc");
		EffectManager::Instance().RegisterEffect(L"EnergySphere", efkPath + L"EnergySphere.efk");
		EffectManager::Instance().RegisterEffect(L"EnergySphereEnd", efkPath + L"EnergySphereEnd.efk");
		EffectManager::Instance().RegisterEffect(L"EnemyWave", efkPath + L"wave.efkefc");
		EffectManager::Instance().RegisterEffect(L"Damage", efkPath + L"damage.efkefc");
		EffectManager::Instance().RegisterEffect(L"SlashHitEfk", efkPath + L"slashHit.efkefc");
		EffectManager::Instance().RegisterEffect(L"GunHitEfk", efkPath + L"GunHit.efkefc");
		EffectManager::Instance().RegisterEffect(L"EnemyHitEfk", efkPath + L"EnemyHit.efkefc");
		EffectManager::Instance().RegisterEffect(L"EnemyLongHitEfk", efkPath + L"PlayerGunDamage.efkefc");
		EffectManager::Instance().RegisterEffect(L"RushEfk", efkPath + L"Rush.efkefc");
		EffectManager::Instance().RegisterEffect(L"EnemyAttack", efkPath + L"PrayerDamage.efkefc");
		EffectManager::Instance().RegisterEffect(L"GunLine", efkPath + L"GunLine.efkefc");

		// 連続攻撃
		EffectManager::Instance().RegisterEffect(L"Slash01Efk", efkPath + L"slash01.efkefc");
		EffectManager::Instance().RegisterEffect(L"Slash02Efk", efkPath + L"slash02.efkefc");
		EffectManager::Instance().RegisterEffect(L"Slash03Efk", efkPath + L"slash03.efkefc");
		EffectManager::Instance().RegisterEffect(L"Slash04Efk", efkPath + L"slash04.efkefc");
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

		app->RegisterWav(L"ArmorBreak", SoundPath + L"ArmorBreak.wav");
		app->RegisterWav(L"Enemy_Slash", SoundPath + L"Enemy_Slash.wav");
		app->RegisterWav(L"Enemy_Spin", SoundPath + L"Enemy_Spin.wav");
		app->RegisterWav(L"Enemy_SpinFinish", SoundPath + L"Enemy_SpinFinish.wav");
		app->RegisterWav(L"Enemy_Slam", SoundPath + L"Enemy_Slam.wav");
		app->RegisterWav(L"Beam", SoundPath + L"Beam.wav");
		app->RegisterWav(L"Enemy_Defeat", SoundPath + L"Enemy_Defeat.wav");
		app->RegisterWav(L"EnemyZako_Shot", SoundPath + L"EnemyZako_Shot.wav");//雑魚敵の遠距離攻撃
		app->RegisterWav(L"EnemyZako_Charge", SoundPath + L"EnemyZako_Charge.wav");//雑魚敵の遠距離攻撃

		app->RegisterWav(L"Damage", SoundPath + L"Damage.wav");
		app->RegisterWav(L"Landing", SoundPath + L"Landing2.wav");
		app->RegisterWav(L"Dash", SoundPath + L"Dash.wav");
		app->RegisterWav(L"Attack1", SoundPath + L"Attack1.wav");
		app->RegisterWav(L"Attack2", SoundPath + L"Attack2.wav");
		app->RegisterWav(L"Attack3", SoundPath + L"Attack3.wav");
		app->RegisterWav(L"HandGun", SoundPath + L"HandGun.wav");
		app->RegisterWav(L"Reload", SoundPath + L"Reload.wav");
		app->RegisterWav(L"CantShotSE", SoundPath + L"CantShot.wav");
		app->RegisterWav(L"DamageVoiceSE", SoundPath + L"DamageVoice.wav");
		app->RegisterWav(L"StartVoiceSE", SoundPath + L"StartVoice2.wav");
		app->RegisterWav(L"ClearVoiceSE", SoundPath + L"ClearVoice.wav");
		app->RegisterWav(L"ClearVoice2SE", SoundPath + L"ClearVoice2.wav");

		// パーツ入手
		app->RegisterWav(L"GetPartsSE", SoundPath + L"GetParts.wav");
		// ジャスト回避
		app->RegisterWav(L"JastDodgeSE", SoundPath + L"JastDodge.wav");
	}

}
//end basecross
