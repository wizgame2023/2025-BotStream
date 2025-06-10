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

    void WaveStageBase::SetActorPause(bool isPause) {
        m_isPaused = isPause;

        auto objVec = GetGameObjectVec();
        for (auto obj : objVec)
        {
            auto actor = dynamic_pointer_cast<Actor>(obj);
            auto cameraManager = dynamic_pointer_cast<CameraManager>(obj);
            auto parts = dynamic_pointer_cast<Parts>(obj);

            if (actor)
            {
                actor->SetPose(isPause);
            }
            if (cameraManager)
            {
                cameraManager->SetPose(isPause);
            }
            if (parts)
            {
                parts->SetPose(isPause);
            }
        }
    }
    void WaveStageBase::CreateManagerObjects() {

        shared_ptr<FadeoutSprite> fadeout;
        fadeout = AddGameObject<FadeoutSprite>(L"Fadeout");
        SetSharedGameObject(L"Fadeout", fadeout);
        fadeout->SetDrawLayer(4);

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

        auto ptrSoundManager = AddGameObject<SoundManager>();
        SetSharedGameObject(L"SoundManager", ptrSoundManager);
        GetSharedGameObject<SoundManager>(L"SoundManager")->PlayBGM(3);
        GetSharedGameObject<SoundManager>(L"SoundManager")->PlaySE(13);

        auto colController = AddGameObject<StageCollisionController>();
        colController->SetCollisionSwhich(true);

        //ポーズ処理生成
        AddGameObject<PauseSprite>();

        // 戦闘用UI
        AddGameObject<PlayerWeaponUI>();
        auto playerUI = AddGameObject<PlayerGaugeUI>(100);
        SetSharedGameObject(L"PlayerUI", playerUI);

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


        // ボスゲージ
        m_bossGauge = AddGameObject<BossGaugeUI>(
            m_boss.lock(),
            m_boss.lock()->GetHPMax()
        );
        SetSharedGameObject(L"BossUI", m_bossGauge);

        //wave1敵
        m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(0.0f, 2.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(10.0f, 2.0f, -255.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(-10.0f, 2.0f, -235.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(20.0f, 2.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(-20.0f, 2.0f, -245.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(30.0f, 2.0f, -225.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(-30.0f, 2.0f, -225.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        //auto flyingEnemy = AddGameObject<EnemyZakoFlying>(Vec3(0.0f, 10.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f), true);

        CreateManagerObjects();

        m_gamePhase = GamePhase::GPhase_Start;
    }

    void WaveStageBase::SetPlayerTransform(Vec3 pos, Vec3 rot) {
        m_player.lock()->GetComponent<Transform>()->SetPosition(pos);
        m_player.lock()->GetComponent<Transform>()->SetRotation(rot);
        GetSharedGameObject<CameraManager>(L"CameraManager")->SetAngle(L"Y", rot.y);

    }

    void WaveStageBase::WaveInitialize() {
        m_IsFadeInFlag = true;
        m_nextWaveFlag = false;

        switch (m_waveCurrent) {

            // ------- 1 -> 2 -------------------------------------------------------------
        case 1:
            //プレイヤーの位置を初期化
            SetPlayerTransform(Vec3(0.0f, 3.0f, -40.0f), Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f));

            m_nextWaveFlag = false;

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

            m_IsFadeInFlag = true;
            m_nextWaveFlag = false;

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
        ShowFPS();

        auto& app = App::GetApp();
        auto KeyState = app->GetInputDevice().GetKeyState();
        auto pad = app->GetInputDevice().GetControlerVec()[0];

        auto fadeout = GetSharedGameObject<FadeoutSprite>(L"Fadeout");

        fadeout->GetBlackFlag();
        m_BlackFlag = fadeout->GetBlackFlag();
        fadeout->GetFadeOutFlag();
        m_IsFadeOutFlag = fadeout->GetFadeOutFlag();
        fadeout->GetFadeInFlag();
        m_IsFadeInFlag = fadeout->GetFadeInFlag();

        m_player.lock()->GetHP();
        auto plaHP = m_player.lock()->GetHP();

        auto ptrSoundManager = GetSharedGameObject<SoundManager>(L"SoundManager");

        auto EnemyVec = m_enemyMgr.lock()->GetEnemyVec(true);
        int EnemyNum = EnemyVec.size();

        EffectManager::Instance().InterfaceUpdate();

        if (m_waveCurrent == 1 && EnemyNum == 0)
        {
            m_IsFadeOutFlag = true;
        }
        if (m_waveCurrent == 2 && EnemyNum == 0)
        {
            m_IsFadeOutFlag = true;
        }

        if (m_BlackFlag == true)
        {
            m_nextWaveFlag = true;
        }

        if (m_nextWaveFlag)
        {
            WaveInitialize();
        }

        if (m_waveCurrent == 3)
        {
            m_bossCurrentHP = m_boss.lock()->GetHPCurrent();//BossのHP取得
        }

        if (m_waveCurrent == 3 && m_bossCurrentHP <= 0)
        {
            GetSharedGameObject<SoundManager>(L"SoundManager")->StopBGM();
            m_scene.lock()->PostEvent(3.0f, GetThis<ObjectInterface>(), m_scene.lock(), L"ToGameClear");
            m_waveCurrent = 4;//ウェーブ終了
        }

        if (plaHP <= 0)
        {
            GetSharedGameObject<SoundManager>(L"SoundManager")->StopBGM();
            m_scene.lock()->PostEvent(1.0f, GetThis<ObjectInterface>(), m_scene.lock(), L"ToGameOver");

        }

        //フェード演出オブジェクトへフラグを渡す
        fadeout->SetFadeOutFlag(m_IsFadeOutFlag);
        fadeout->SetFadeInFlag(m_IsFadeInFlag);
        fadeout->SetBlackFlag(m_BlackFlag);
    }

    void WaveStageBase::OnDraw()
    {
        EffectManager::Instance().InterfaceDraw();
    }

    void WaveStageBase::OnDestroy()
    {
        //BGMとSEを止める
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
        // Wave1の壁の大きさ
        Vec3 wallSclWv1(120.0f, 4.0f, 41.0f);
        // Wave2の壁の大きさ
        Vec3 wallSclWv2(120.0f, 4.0f, 41.0f);
        // Bossの壁の大きさ
        Vec3 wallSclBoss(170.0f, 4.0f, 41.0f);

        vector < vector<Vec3> > vec =
        {
            //Boss
            //右
            {
                wallSclBoss,
                Vec3(-XM_PIDIV2, 0.0f, 1.57f),
                Vec3(86.9f, 19.0f, 260.0f)
            },
            //左
            {
                wallSclBoss,
                Vec3(-XM_PIDIV2, 0.0f, 1.57f),
                Vec3(-86.9f, 19.0f, 260.0f)
            },
            //Wave2
            //右
            {
                wallSclWv2,
                Vec3(-XM_PIDIV2, 0.0f, 1.57f),
                Vec3(62.0f, 19.0f, 0.0f)
            },
            //左
            {
                wallSclWv2,
                Vec3(-XM_PIDIV2, 0.0f, 1.57f),
                Vec3(-61.9f, 19.0f, 0.0f)
            },
            //Wave1
            //右
            {
                wallSclWv1,
                Vec3(-XM_PIDIV2, 0.0f, 1.57f),
                Vec3(62.0f, 19.0f, -260.0f)
            },
            //左
            {
                wallSclWv1,
                Vec3(-XM_PIDIV2, 0.0f, 1.57f),
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
                wallSclBoss,
                Vec3(-XM_PIDIV2, 1.57f, 1.57f),
                Vec3(0.0f, 19.0f, 346.9f)
            },
            //後
            {
                wallSclBoss,
                Vec3(-XM_PIDIV2, 1.57f, 1.57f),
                Vec3(0.0f, 19.0f, 173.1f)
            },
            ////Wave2
            //前
            {
                wallSclWv2,
                Vec3(-XM_PIDIV2, 1.57f, 1.57f),
                Vec3(0.0f, 19.0f, 62.0f)
            },
            //後
            {
                wallSclWv2,
                Vec3(-XM_PIDIV2, 1.57f, 1.57f),
                Vec3(0.0f, 19.0f, -62.0f)
            },
            ////Wave1
            //前
            {
                wallSclWv1,
                Vec3(-XM_PIDIV2, 1.57f, 1.57f),
                Vec3(0.0f, 19.0f, -198.0f)
            },
            //後
           {
               wallSclWv1,
               Vec3(-XM_PIDIV2, 1.57f, 1.57f),
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