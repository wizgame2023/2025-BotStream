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
		CreatePlayer(Vec3(0.0f, 1.0f, -305.0f), Vec3(0.0f, 5.0f, 0.0f), Vec3(1.0f, 2.0f, 1.0f));

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

		enemyVariation.push_back(EVar_Aerial);

		m_enemyMgr = AddGameObject<EnemyManager>(enemyVariation);
		SetSharedGameObject(L"EnemyManager", m_enemyMgr.lock());

		m_boss = AddGameObject<BossFirst>(Vec3(0.0f, 2.0f, 250.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		SetSharedGameObject(L"Boss", m_boss.lock());

		//wave1敵
		m_enemyMgr.lock()->InstEnemy<EnemyZakoLong>(Vec3(0.0f, 1.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
		m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(10.0f, 1.0f, -255.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
		m_enemyMgr.lock()->InstEnemy<EnemyZakoLong>(Vec3(-10.0f, 1.0f, -235.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
		m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(20.0f, 1.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
		m_enemyMgr.lock()->InstEnemy<EnemyZakoLong>(Vec3(-20.0f, 1.0f, -245.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
		m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(30.0f, 1.0f, -225.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
		m_enemyMgr.lock()->InstEnemy<EnemyZakoLong>(Vec3(-30.0f, 1.0f, -225.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));

		CreateManagerObjects();

		m_gamePhase = GamePhase::GPhase_Start;
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
		switch (m_gamePhase) {
		case GPhase_Start:
			//開始→ゲーム本編
			//if (m_player.lock()->GetwalkFlag()) {
			//	m_gamePhase = GPhase_Playing;
			//}
			break;
		case GPhase_Playing:
			//仮
			if (false) {
				m_gamePhase = GPhase_Result;
			}
			if (false) {
				m_gamePhase = GPhase_GameOver;
			}
		}
	}


	//ウェーブ移行処理
	void StageFirst::WaveInitialize() {
		switch (m_waveCurrent) {

			// ------- 1 -> 2 -------------------------------------------------------------
		case 1:
			//プレイヤーの位置を初期化
			SetPlayerTransform(Vec3(0.0f, 1.0f, -40.0f), Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f));

			m_enemyMgr.lock()->InstEnemy(Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			m_enemyMgr.lock()->InstEnemy(Vec3(10.0f, 1.0f, 30.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			m_enemyMgr.lock()->InstEnemy(Vec3(-10.0f, 1.0f, -20.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			m_enemyMgr.lock()->InstEnemy(Vec3(20.0f, 1.0f, 10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			m_enemyMgr.lock()->InstEnemy(Vec3(-20.0f, 1.0f, -10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			m_enemyMgr.lock()->InstEnemy(Vec3(30.0f, 1.0f, 30.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			AddGameObject<EnemyZakoFlying>(Vec3(0.0f, 10.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f), true);

			//m_enemyMgr.lock()->InstEnemy(Vec3(0.0f, 2.0f, 0.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			//m_enemyMgr.lock()->InstEnemy(Vec3(10.0f, 2.0f, 30.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			//m_enemyMgr.lock()->InstEnemy(Vec3(-10.0f,2.0f, -20.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			//m_enemyMgr.lock()->InstEnemy(Vec3(20.0f, 2.0f, 10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			m_enemyMgr.lock()->InstEnemy<EnemyZakoLong>(Vec3(-20.0f, 2.0f, -15.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			m_enemyMgr.lock()->InstEnemy<EnemyZakoLong>(Vec3(30.0f, 2.0f, 35.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			m_enemyMgr.lock()->InstEnemy<EnemyZakoLong>(Vec3(-30.0f, 2.0f, -15.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));

			break;
			// ----------------------------------------------------------------------------

			// ------- 2 -> 3 -------------------------------------------------------------
		case 2:
			m_bossGauge->ClearBossGaugeUI(false);

			//プレイヤーの位置を初期化
			SetPlayerTransform(Vec3(0.0f, 3.0f, 195.0f), Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f));

			m_enemyMgr.lock()->InstBoss(dynamic_pointer_cast<EnemyBase>(m_boss.lock()));

			GetSharedGameObject<SoundManager>(L"SoundManager")->StopBGM();
			GetSharedGameObject<SoundManager>(L"SoundManager")->PlayBGM(4);

			break;
			// ----------------------------------------------------------------------------
		}

		m_waveCurrent++;
	}

	void StageFirst::OnUpdate()
	{
		WaveStageBase::OnUpdate();

		//フェーズの変更処理
		UpdateGamePhase();

		//フェーズの状態によって処理が変わる
		if (m_gamePhase == GPhase_Start)
		{
			//アクターがUpdeta出来なくする
			//SetActorPause(true);
			m_deltaScaleCount = 1.0f; // deltaScaleを元に戻らないようにする
			m_deltaScale = 0.0f;
		}
	}

}