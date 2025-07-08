/*!
@file WaveStageBase.cpp
@brief ステージの基底クラス
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
    void WaveStageBase::CreateViewLight() {
        //カメラ
        const Vec3 eye(0.0f, 350.0f, -20.0f);

        const Vec3 at(0.0f, 0.0f, 0.0f);
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

        //ついでにActorグループ生成
        CreateSharedObjectGroup(L"Actor");
    }

    void WaveStageBase::CreatePlayer(Vec3 pos, Vec3 rot, Vec3 scale) {
        m_player = AddGameObject<Player>(pos, rot, scale);
        SetSharedGameObject(L"Player", m_player.lock());

    }

    void WaveStageBase::SetActorPause(bool isPause,bool andCamera) {
        m_isPaused = isPause;

        EffectManager::Instance().PauseAllEffects(isPause);

        auto objVec = GetGameObjectVec();

		//andCameraがオンならカメラも動かない処理にする,オフならActorのみ
		if (andCamera)
		{
			for (auto obj : objVec)
			{
				auto actor = dynamic_pointer_cast<Actor>(obj);
				auto cameraManager = dynamic_pointer_cast<CameraManager>(obj);
				auto parts = dynamic_pointer_cast<Parts>(obj);

				if (actor)
				{
					actor->SetPause(isPause);
				}
				if (cameraManager)
				{
					cameraManager->SetPause(isPause);
				}
				if (parts)
				{
					parts->SetPause(isPause);
				}
			}
		}
		else if (!andCamera)
		{
			for (auto obj : objVec)
			{
				auto actor = dynamic_pointer_cast<Actor>(obj);
				auto cameraManager = dynamic_pointer_cast<CameraManager>(obj);
				auto parts = dynamic_pointer_cast<Parts>(obj);

				if (actor)
				{
					actor->SetPause(isPause);
				}
				if (parts)
				{
					parts->SetPause(isPause);
				}
			}
		}

    }

    void WaveStageBase::CreateManagerObjects() {

        m_fadeout = AddGameObject<FadeoutSprite>(L"Fadeout");
        SetSharedGameObject(L"Fadeout", m_fadeout.lock());
		m_fadeout.lock()->SetDrawLayer(4);

        // ------- パーツ関係 ---------------------------------------------------------
        //パーツマネージャ生成
        auto partsManager = AddGameObject<PartsManager>();
        SetSharedGameObject(L"PartsManager", partsManager);

        auto equippedParts = AddGameObject<EquippedParts>();
        SetSharedGameObject(L"PartsPoach", equippedParts);
        //AddGameObject<PartsHiMoter>(Vec3(-20.0f,1.0f,-230.0f),Vec3(0.0f,0.0f,0.0f),Vec3(1.0f,1.0f,1.0f));

        //装備中のパーツを表示するＵＩ
        AddGameObject<PartsTextChange>();

        // ----------------------------------------------------------------------------
        
        //カメラマネージャー
        auto cameraManager = AddGameObject<CameraManager>();
        SetSharedGameObject(L"CameraManager", cameraManager);

        m_sndMgr = AddGameObject<SoundManager>();
        SetSharedGameObject(L"SoundManager", m_sndMgr.lock());
        GetSharedGameObject<SoundManager>(L"SoundManager")->PlayBGM(3);
        //GetSharedGameObject<SoundManager>(L"SoundManager")->PlaySE(13);

        auto colController = AddGameObject<StageCollisionController>();
        colController->SetCollisionSwhich(true);

        //ポーズ処理生成
        auto pauseUI = AddGameObject<PauseSprite>();
        SetSharedGameObject(L"PauseUI", pauseUI);

        // 戦闘用UI
        AddGameObject<PlayerWeaponUI>();
        auto playerUI = AddGameObject<PlayerGaugeUI>(m_player.lock());
        SetSharedGameObject(L"PlayerUI", playerUI);

		// 数字の位置,大きさ
		Vec2 digitPos(310.0f, -260.0f);
		constexpr float digitSize = 25.0f;
		// プレイヤーの弾のUI
		AddGameObject<PlayerBulletUI>(m_player.lock(), digitPos, digitSize);

		// プレイヤーがやばいときのやつ
		AddGameObject<PlayerEmergencyUI>(m_player.lock());

		// ボスゲージ
		m_bossGauge = AddGameObject<BossGaugeUI>(
			m_boss.lock(),
			m_boss.lock()->GetHPMax()
		);
		SetSharedGameObject(L"BossUI", m_bossGauge);

    }

	//EnemyManagerとボスの初期設定
	void WaveStageBase::CreateEnemyManager() {

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
	}

    void WaveStageBase::OnCreate()
    {
        auto& app = App::GetApp();
        m_scene = app->GetScene<Scene>();

        //ビューとライトの作成
        CreateViewLight();

        CreateFloor();
        CreateWall();
        CreateCeiling();
        CreatePlayer(Vec3(0.0f, 3.0f, -305.0f), Vec3(0.0f, 5.0f, 0.0f), Vec3(1.0f, 2.0f, 1.0f));

		CreateEnemyManager();

        //wave1敵
        m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(0.0f, 2.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(10.0f, 2.0f, -255.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(-10.0f, 2.0f, -235.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(20.0f, 2.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(-20.0f, 2.0f, -245.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(30.0f, 2.0f, -225.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(-30.0f, 2.0f, -225.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        m_enemyMgr.lock()->InstEnemy<EnemyZakoFlying>(Vec3(0.0f, 10.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        //m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(10.0f, 2.0f, -255.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        //m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(-10.0f, 2.0f, -235.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        //m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(20.0f, 2.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        //m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(-20.0f, 2.0f, -245.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        //m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(30.0f, 2.0f, -225.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        //m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(-30.0f, 2.0f, -225.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        //m_enemyMgr.lock()->InstEnemy<EnemyZakoFlying>(Vec3(0.0f, 10.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));

        CreateManagerObjects();

        m_gamePhase = GamePhase::GPhase_Start;
    }

	void WaveStageBase::SetPlayerTransform(Vec3 pos, Vec3 rot) {
		m_player.lock()->GetComponent<Transform>()->SetPosition(pos);
		m_player.lock()->GetComponent<Transform>()->SetRotation(rot);
		GetSharedGameObject<CameraManager>(L"CameraManager")->SetAngle(L"Y", rot.y);

	}

	bool WaveStageBase::ConsiderGoToNextWave() {
		auto EnemyVec = m_enemyMgr.lock()->GetEnemyVec(true);
		int EnemyNum = EnemyVec.size();

		bool ret = false;
		ret |= (m_waveCurrent == 1 && EnemyNum == 0);
		ret |= (m_waveCurrent == 2 && EnemyNum == 0);

		return ret;
	}

	bool WaveStageBase::ConsiderGameClear() {
		bool ret = false;
		ret |= m_boss.lock()->GetHPCurrent() <= 0;

		return ret;
	}

	bool WaveStageBase::ConsiderGameOver() {
		bool ret = false;
		ret |= m_player.lock()->GetHPCurrent() <= 0;

		return ret;
	}

	//GamePhaseの変更
	void WaveStageBase::UpdateGamePhase() {
		switch (m_gamePhase) {
		case GPhase_Start:
			//開始→ゲーム本編
			if (m_player.lock()->GetwalkFlag()) {
				m_gamePhase = GPhase_Playing;
			}
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

	//Waveが切り替わる際に行う処理
	void WaveStageBase::WaveInitialize() {
		switch (m_waveCurrent) {

			// ------- 1 -> 2 -------------------------------------------------------------
		case 1:
			//プレイヤーの位置を初期化
			SetPlayerTransform(Vec3(0.0f, 3.0f, -40.0f), Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f));
			m_enemyMgr.lock()->InstEnemy(Vec3(0.0f, 2.0f, 0.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			m_enemyMgr.lock()->InstEnemy(Vec3(10.0f, 2.0f, 30.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			m_enemyMgr.lock()->InstEnemy(Vec3(-10.0f, 2.0f, -20.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			m_enemyMgr.lock()->InstEnemy(Vec3(20.0f, 2.0f, 10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			m_enemyMgr.lock()->InstEnemy(Vec3(-20.0f, 2.0f, -10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			m_enemyMgr.lock()->InstEnemy(Vec3(30.0f, 2.0f, 30.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			m_enemyMgr.lock()->InstEnemy(Vec3(-30.0f, 2.0f, -5.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));

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

    void WaveStageBase::OnUpdate()
    {
        auto& app = App::GetApp();
        auto KeyState = app->GetInputDevice().GetKeyState();
        auto pad = app->GetInputDevice().GetControlerVec()[0];

        ResetDeltaScaleToDefault();
		UpdateGamePhase();

        EffectManager::Instance().InterfaceUpdate();

        if (ConsiderGoToNextWave())
        {
			m_fadeout.lock()->SetFadeOutFlag(true);
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

        if (m_waveCurrent == m_waveMax && ConsiderGameClear() && m_onceFlag == false)
        {
            m_sndMgr.lock()->StopBGM();
			m_onceFlag = true;
            m_scene.lock()->PostEvent(3.0f, GetThis<ObjectInterface>(), m_scene.lock(), L"ToGameClear");
        }

        if (ConsiderGameOver() && m_onceFlag == false)
        {
			AddGameObject<GameOverNoise>();
            m_sndMgr.lock()->StopBGM();
			m_onceFlag = true;
            m_scene.lock()->PostEvent(1.0f, GetThis<ObjectInterface>(), m_scene.lock(), L"ToGameOver");
        }

    }

    void WaveStageBase::OnDraw()
    {
        EffectManager::Instance().InterfaceDraw();
    }

    void WaveStageBase::OnDestroy()
    {
        //BGM��SE���~�߂�
        auto soundManager = GetSharedGameObject<SoundManager>(L"SoundManager");
        soundManager->StopBGM();
        soundManager->StopSE();
    }

    void WaveStageBase::SetNextWaveFlag(bool setNextWaveFlag)
    {
        m_nextWaveFlag = setNextWaveFlag;
    }

    bool WaveStageBase::GetNextWaveFlag()
    {
        return m_nextWaveFlag;
    }

	//--------------------------------------------------------------------------------
	// 地形生成

		//天井作成
	void WaveStageBase::CreateCeiling()
	{
		vector < vector<Vec3> > vec =
		{
			//Boss
			{
				Vec3(170.0f, 1.0f, 170.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 40.0f, 260.0f)
			},
			//Wave2
			{
				Vec3(120.0f, 1.0f, 120.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 40.0f, 0.0f)
			},
			//Wave1
			{
				Vec3(120.0f, 1.0f, 120.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 40.0f, -260.0f)
			}

		};
		for (auto v : vec)
		{
			AddGameObject<Ceiling>(v[0], v[1], v[2]);
		}
	}

	//壁作成
	void WaveStageBase::CreateWall()
	{
		// 壁の厚さ
		static constexpr float zakoWallDepth = 8.0f;
		// ボスの壁は厚くする
		static constexpr float bossWallDepth = 15.0f;

		// Sideの壁の大きさ
		Vec3 wallSideSclWv1(zakoWallDepth, 41.0f, 115.4f);
		Vec3 wallSideSclWv2(zakoWallDepth, 41.0f, 115.4f);
		Vec3 wallSideSclBoss(bossWallDepth, 41.0f, 157.5f);
		// Frontの壁の大きさ
		Vec3 wallFrontSclWv1(120.0f, 41.0f, zakoWallDepth);
		Vec3 wallFrontSclWv2(120.0f, 41.0f, zakoWallDepth);
		Vec3 wallFrontSclBoss(170.0f, 41.0f, bossWallDepth);

		vector < vector<Vec3> > vec =
		{
			//Boss
			//右
			{
				wallSideSclBoss,
				Vec3(0.0f),
				Vec3(86.9f, 19.0f, 260.0f)
			},
			//左
			{
				wallSideSclBoss,
				Vec3(0.0f),
				Vec3(-86.9f, 19.0f, 260.0f)
			},
			//Wave2
			//右
			{
				wallSideSclWv2,
				Vec3(0.0f),
				Vec3(62.0f, 19.0f, 0.0f)
			},
			//左
			{
				wallSideSclWv2,
				Vec3(0.0f),
				Vec3(-61.9f, 19.0f, 0.0f)
			},
			//Wave1
			//右
			{
				wallSideSclWv1,
				Vec3(0.0f),
				Vec3(62.0f, 19.0f, -260.0f)
			},
			//左
			{
				wallSideSclWv1,
				Vec3(0.0f),
				Vec3(-62.0f, 19.0f, -260.0f)
			},
		};
		for (auto v : vec)
		{
			AddGameObject<Wall>(v[0], v[1], v[2]);
		}

		vector < vector<Vec3> > vec2 =
		{
			////Boss
			//前
			{
				wallFrontSclBoss,
				Vec3(0.0f),
				Vec3(0.0f, 19.0f, 346.9f)
			},
			//後
			{
				wallFrontSclBoss,
				Vec3(0.0f),
				Vec3(0.0f, 19.0f, 173.1f)
			},
			////Wave2
			//前
			{
				wallFrontSclWv2,
				Vec3(0.0f),
				Vec3(0.0f, 19.0f, 62.0f)
			},
			//後
			{
				wallFrontSclWv2,
				Vec3(0.0f),
				Vec3(0.0f, 19.0f, -62.0f)
			},
			////Wave1
			//前
			{
				wallFrontSclWv1,
				Vec3(0.0f),
				Vec3(0.0f, 19.0f, -198.0f)
			},
			//後
		    {
			   wallFrontSclWv1,
			   Vec3(0.0f),
			   Vec3(0.0f, 19.0f, -322.0f)
		    },
		};
		for (auto v : vec2)
		{
			AddGameObject<Wall2>(v[0], v[1], v[2]);
		}
	}

	//床作成
	void WaveStageBase::CreateFloor()
	{
		vector < vector<Vec3> > vec =
		{
			// 最初はBossが一番初めに生成されていたけど、床に模様を貼る時の処理と相性が悪いので
			// 勝手にWave1とBossの生成順を変えさせてもらいました。なにかほかの所に影響があるならお伝えください

			//Wave1
			{
				Vec3(120.0f, 3.0f, 120.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, -3.0f, -260.0f)
			},
			//Wave2
			{
				Vec3(120.0f, 3.0f, 120.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, -3.0f, 0.0f)
			},
			//Boss
			{
				Vec3(170.0f, 3.0f, 170.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, -3.0f, 260.0f)
			},


		};
		for (auto v : vec)
		{
			AddGameObject<Floor>(v[0], v[1], v[2]);

			// ブロックの横と奥行きの静的定数
			constexpr float scaleXZ = Block::BLOCK_XZ_SCALE;
			// ブロック生成時のポジションのずれの修正
			constexpr float shiftPos = scaleXZ / 2;

			// 大きさと位置の代入(マジックナンバーになってしまうため追加)
			auto scl = v[0];
			auto pos = v[2];

			// 生成するブロックの数
			int blockNum = (int)scl.x / 10;

			for (int i = 0; i < blockNum; i++)
			{
				for (int j = 0; j < blockNum; j++)
				{
					//AddGameObject<Block>(
					//    Vec3(
					//        // ブロック1個の大きさ - ステージの位置 + (ステージの大きさ / 2) + (ブロックの大きさ / 2)
					//        //                              ↑こうしないとステージの中央から生成されてしまう
					//        j * scaleXZ - (pos.x + (scl.x / 2) - shiftPos),
					//        pos.y + 0.05f,
					//        // 基本は変わらないけど、Wave1とBossの床が逆になっていたのでこうすると直りました
					//        i * scaleXZ + (pos.z - (scl.z / 2) + shiftPos)
					//    )
					//);
				}
			}
		}
	}


}