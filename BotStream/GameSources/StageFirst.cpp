/*!
@file StageFirst.cpp
@brief Derived WaveStage
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	void StageFirst::OnCreate()
	{
		auto& app = App::GetApp();
		m_scene = app->GetScene<Scene>();

		//ビューとライトの作成
		CreateViewLight();

		CreateFloor();
		CreateWall();
		CreateCeiling();
		CreatePlayer(Vec3(0.0f, 8.0f, -305.0f), Vec3(0.0f, 5.0f, 0.0f), Vec3(1.0f, 2.0f, 1.0f));

		//Enemyマネージャのテスト
		vector<EnemyVariation> enemyVariation;
		for (int i = 0; i <= 10; i++)
		{
			enemyVariation.push_back(EVar_Normal);
		}
		for (int i = 0; i <= 10; i++)
		{
			enemyVariation.push_back(EVar_Projectile);
		}
		for (int i = 0; i <= 10; i++)
		{
			enemyVariation.push_back(EVar_Aerial);
		}
		for (int i = 0; i <= 10; i++)
		{
			enemyVariation.push_back(EVar_Humanoid);
		}

		m_enemyMgr = AddGameObject<EnemyManager>(enemyVariation);
		SetSharedGameObject(L"EnemyManager", m_enemyMgr.lock());

		m_boss = AddGameObject<BossFirst>(Vec3(0.0f, 2.0f, 250.0f), Vec3(0.0f, XMConvertToRadians(90.0f), 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		SetSharedGameObject(L"Boss", m_boss.lock());

		//wave1敵
		m_enemyMgr.lock()->InstEnemy<EnemyZakoHumanoid>(Vec3(0.0f, 2.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
		m_enemyMgr.lock()->InstEnemy<EnemyZakoHumanoid>(Vec3(10.0f, 2.0f, -255.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
		m_enemyMgr.lock()->InstEnemy<EnemyZakoHumanoid>(Vec3(-10.0f, 2.0f, -235.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
		m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(20.0f, 2.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
		m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(-20.0f, 2.0f, -245.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
		m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(30.0f, 2.0f, -225.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
		m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(-30.0f, 2.0f, -225.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));

		//次のWaveに行くための暗闇生成
		auto darkObj = AddGameObject<DarkObj>(Vec3(-0.0f, 3.0f, -200.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(10.0f, 10.0f, 10.0f));
		m_darkObjs.push_back(darkObj);

		//Wave1のドア生成
		auto doorLeft = AddGameObject<Door>(Vec3(5.0f, 10.0f, 1.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(-2.5f, 3.0f, -200.5f));
		m_doors.push_back(doorLeft);
		auto doorRight = AddGameObject<Door>(Vec3(5.0f, 10.0f, 1.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(+2.5f, 3.0f, -200.5f));
		m_doors.push_back(doorRight);

		// マネージャー系生成
		CreateManagerObjects();
		
		//ムービコントローラー作成
		if (!m_player.lock()) return;
		m_movieController = AddGameObject<RT_MovieController>(m_player.lock(), GetSharedGameObject<CameraManager>(L"CameraManager"));
		SetSharedGameObject(L"MovieController", m_movieController);

		//ステージ開始時のムービ開始
		m_movieController->StartMovie();


		//m_gamePhase = GamePhase::GPhase_Start;
	}

	//次のWaveに行くか確認する処理
	bool StageFirst::ConsiderGoToNextWave() {
		auto EnemyVec = m_enemyMgr.lock()->GetEnemyVec(true);
		int EnemyNum = EnemyVec.size();

		bool ret = false;
		ret |= (m_waveCurrent == 1 && EnemyNum == 0);
		ret |= (m_waveCurrent == 2 && EnemyNum == 0);

		return ret;
	}

	//ゲームクリアしていいか確認する処理
	bool StageFirst::ConsiderGameClear() {
		return m_gameClearFlag;
	}

	//ボスのHPが0になっているか確認する処理
	bool StageFirst::ConsiderBossCheck()
	{
		bool ret = false;
		ret |= m_boss.lock()->GetHPCurrent() <= 0;

		return ret;
	}

	//ゲームオーバーしていいか確認する処理
	bool StageFirst::ConsiderGameOver() {
		bool ret = false;
		ret |= m_player.lock()->GetHPCurrent() <= 0;

		return ret;
	}
	
	//GamePhaseの変更 GamePhaseの状態によって時間が止まるか変わる
	void StageFirst::UpdateGamePhase() {
		auto pause = GetSharedGameObject<PauseSprite>(L"PauseUI");

		switch (m_gamePhase) {
		case GPhase_Start:
			////開始→ゲーム本編
			//if (m_player.lock()->GetwalkFlag()) {
			//	m_gamePhase = GPhase_Playing;
			//}
			break;
		case GPhase_Playing:
			pause->SetReallyPauseFlag(true);

			//仮
			if (false) {
				m_gamePhase = GPhase_Result;
			}
			if (false) {
				m_gamePhase = GPhase_GameOver;
			}	
			break;
		case GPhase_CutScene:
			pause->SetReallyPauseFlag(false);
			break;
		}

	}


	//ウェーブ移行処理
	void StageFirst::WaveInitialize() {
		switch (m_waveCurrent) {
			// ------- 1 -> 2 -------------------------------------------------------------
			case 1:
			{
				//プレイヤーの位置を初期化
				SetPlayerTransform(Vec3(0.0f, 3.0f, -40.0f), Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f));

				m_enemyMgr.lock()->InstEnemy(Vec3(0.0f, 2.0f, 0.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
				m_enemyMgr.lock()->InstEnemy(Vec3(10.0f, 2.0f, 30.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
				m_enemyMgr.lock()->InstEnemy(Vec3(-10.0f, 2.0f, -20.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
				m_enemyMgr.lock()->InstEnemy<EnemyZakoLong>(Vec3(20.0f, 2.0f, 10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
				m_enemyMgr.lock()->InstEnemy<EnemyZakoLong>(Vec3(-20.0f, 2.0f, -10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
				m_enemyMgr.lock()->InstEnemy(Vec3(30.0f, 2.0f, 30.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
				//AddGameObject<EnemyZakoFlying>(Vec3(0.0f, 10.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f), true);

				//m_enemyMgr.lock()->InstEnemy(Vec3(0.0f, 2.0f, 0.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
				//m_enemyMgr.lock()->InstEnemy(Vec3(10.0f, 2.0f, 30.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
				//m_enemyMgr.lock()->InstEnemy(Vec3(-10.0f,2.0f, -20.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
				//m_enemyMgr.lock()->InstEnemy(Vec3(20.0f, 2.0f, 10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
				m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(-20.0f, 3.0f, -15.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
				m_enemyMgr.lock()->InstEnemy<EnemyZakoLong>(Vec3(30.0f, 3.0f, 35.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
				m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(-30.0f, 3.0f, -15.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));

				//次のWaveに行くための暗闇生成
				auto darkObj = AddGameObject<DarkObj>(Vec3(-0.0f, 3.0f, 60.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(10.0f, 10.0f, 10.0f));
				m_darkObjs.push_back(darkObj);

				//Wave1のドア生成
				auto doorLeft = AddGameObject<Door>(Vec3(5.0f, 10.0f, 1.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(-2.5f, 3.0f, 59.5f));
				m_doors.push_back(doorLeft);
				auto doorRight = AddGameObject<Door>(Vec3(5.0f, 10.0f, 1.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(+2.5f, 3.0f, 59.5f));
				m_doors.push_back(doorRight);
				break;
			}
			// ----------------------------------------------------------------------------

			// ------- 2 -> 3 -------------------------------------------------------------
			case 2:
			{
				m_bossGauge->ClearBossGaugeUI(false);

				//プレイヤーの位置を初期化
				SetPlayerTransform(Vec3(0.0f, 1.2f, 195.0f), Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f));

				m_enemyMgr.lock()->InstBoss(dynamic_pointer_cast<EnemyBase>(m_boss.lock()));

				GetSharedGameObject<SoundManager>(L"SoundManager")->StopBGM();
				GetSharedGameObject<SoundManager>(L"SoundManager")->PlayBGM(4);

				//ボスムービー再生
				m_movieController->BossMovie();

				break;
			}
			// ----------------------------------------------------------------------------
		}

		m_waveCurrent++;
	}

	void StageFirst::OnUpdate()
	{
		//WaveStageBase::OnUpdate();

		auto& app = App::GetApp();
		auto KeyState = app->GetInputDevice().GetKeyState();
		auto pad = app->GetInputDevice().GetControlerVec()[0];

		ResetDeltaScaleToDefault();
		UpdateGamePhase();

		EffectManager::Instance().InterfaceUpdate();

		if (ConsiderGoToNextWave())
		{
			// ドアを動かす
			DoorsMove(m_waveCurrent);
			// 暗闇の有効化
			ActivateDark(m_waveCurrent);
		}
		if (m_fadeout.lock()->GetBlackFlag())
		{
			m_nextWaveFlag = true;
		}
		if (m_nextWaveFlag)
		{
			m_fadeout.lock()->SetFadeInFlag(true);
			m_nextWaveFlag = false;

			WaveInitialize();
		}

		if (m_waveCurrent == m_waveMax && ConsiderBossCheck() && m_onceFlag == false)
		{
			m_movieController->BossDieMovie();

			m_sndMgr.lock()->StopBGM();
			m_onceFlag = true;
		}

		if (ConsiderGameOver() && m_onceFlag == false)
		{
			AddGameObject<GameOverNoise>();
			m_sndMgr.lock()->StopBGM();
			m_onceFlag = true;
			m_fadeout.lock()->SetFadeOutFlag(true);// ブラックアウト
			m_scene.lock()->PostEvent(1.0f, GetThis<ObjectInterface>(), m_scene.lock(), L"ToGameOver");
		}

		//ゲームクリア処理
		if (m_gameClearFlag)
		{
			m_fadeout.lock()->SetFadeOutFlag(true);// ブラックアウト
			m_scene.lock()->PostEvent(1.2f, GetThis<ObjectInterface>(), m_scene.lock(), L"ToGameClear");
		}

	}

	//ドアを動かす処理
	void StageFirst::DoorsMove(int group)
	{
		int leftNum = 2 * group - 1;
		int rightNum = 2 * group;

		//ゴール基準値
		float goolLeght = 7.5f;

		float delta = App::GetApp()->GetElapsedTime();

		//指定しているグループまでドアがなかったリターンする
		if (m_doors.size() < rightNum)
		{
			return;
		}

		//ドアを動かす(←　こんな動き　→)
		auto leftDoor = m_doors[leftNum-1]->GetComponent<Transform>();
		auto rightDoor = m_doors[rightNum-1]->GetComponent<Transform>();

		auto leftPos = leftDoor->GetPosition();
		auto rightPos = rightDoor->GetPosition();

		//ゴール地点まできたら、過ぎないようにする
		if (leftPos.x <= -goolLeght &&rightPos.x >= goolLeght)
		{
			leftPos.x = -goolLeght;
			rightPos.x = goolLeght;

			leftDoor->SetPosition(leftPos);
			rightDoor->SetPosition(rightPos);
			return;
		}

		//移動処理
		float speed = 3.0f;
		leftPos.x -= speed * delta;
		leftDoor->SetPosition(leftPos);
		rightPos.x += speed * delta;
		rightDoor->SetPosition(rightPos);
	}

	//暗闇(次のWaveに移行するオブジェクト)の有効化
	void StageFirst::ActivateDark(int activeteNum)
	{
		// 配列外を選択しようとしたらreturnさせる
		if (m_darkObjs.size() < activeteNum)
		{
			return;
		}

		//コリジョンの有効化
		m_darkObjs[activeteNum - 1]->SetActivateCollsion(true);
	}

	//暗転を開始する処理
	void StageFirst::FadeOutStart()
	{
		m_fadeout.lock()->SetFadeOutFlag(true);
	}

	//ゲームクリアを渡す
	void StageFirst::SetGameClear(bool onOff)
	{
		m_gameClearFlag = onOff;
	}

}