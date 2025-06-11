/*!
@file WaveStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

    //--------------------------------------------------------------------------------------
    //	ゲームステージクラス実体
    //--------------------------------------------------------------------------------------
    void WaveStage::CreateViewLight() {
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
    }

    void WaveStage::OnCreate()
    {
        //ビューとライトの作成
        CreateViewLight();

        CreateFloor();
        CreateWall();
        CreateCeiling();

        m_waveNow = 1;//wave１開始してる

        shared_ptr<FadeoutSprite> fadeout;
        fadeout = AddGameObject<FadeoutSprite>(L"Fadeout");
        SetSharedGameObject(L"Fadeout", fadeout);
        fadeout->SetDrawLayer(4);

        CreateSharedObjectGroup(L"Actor");

        auto player = AddGameObject<Player>(Vec3(0.0f, 3.0f, -305.0f), Vec3(0.0f, 5.0f, 0.0f), Vec3(1.0f, 2.0f, 1.0f));
        SetSharedGameObject(L"Player", player);

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


        auto enemyMgr = AddGameObject<EnemyManager>(enemyVariation);
        SetSharedGameObject(L"EnemyManager", enemyMgr);

        auto boss = AddGameObject<BossFirst>(Vec3(0.0f, 2.0f, 250.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
        SetSharedGameObject(L"Boss", boss);
        //enemyMgr->InstBoss(dynamic_pointer_cast<EnemyBase>(boss));

        auto playerUI = AddGameObject<PlayerGaugeUI>(100);
        SetSharedGameObject(L"PlayerUI", playerUI);


        // ボスゲージ
        m_bossGauge = AddGameObject<BossGaugeUI>(
            boss,
            boss->GetHPMax()
            );
        SetSharedGameObject(L"BossUI", m_bossGauge);
        
        //wave1敵
        enemyMgr->InstEnemy<EnemyZako>(Vec3(0.0f, 2.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        //enemyMgr->InstEnemy<EnemyZako>(Vec3(10.0f, 2.0f, -255.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        //enemyMgr->InstEnemy<EnemyZako>(Vec3(-10.0f,2.0f, -235.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        //enemyMgr->InstEnemy<EnemyZako>(Vec3(20.0f, 2.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        //enemyMgr->InstEnemy<EnemyZako>(Vec3(-20.0f,2.0f, -245.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        //enemyMgr->InstEnemy<EnemyZako>(Vec3(30.0f, 2.0f, -225.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        enemyMgr->InstEnemy<EnemyZako>(Vec3(-30.0f,2.0f, -225.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        //auto flyingEnemy = AddGameObject<EnemyZakoFlying>(Vec3(0.0f, 10.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f), true);

        //パーツ関係//
        //パーツマネージャ生成
        auto partsManager = AddGameObject<PartsManager>();
        SetSharedGameObject(L"PartsManager", partsManager);

        auto equippedParts = AddGameObject<EquippedParts>();
        SetSharedGameObject(L"PartsPoach", equippedParts);
        //AddGameObject<PartsHiMoter>(Vec3(-20.0f,1.0f,-230.0f),Vec3(0.0f,0.0f,0.0f),Vec3(1.0f,1.0f,1.0f));

        //装備したパーツが何かわかるようにする
        AddGameObject<PartsTextChange>();
        //////////////
        
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
    }


    void WaveStage::OnUpdate()
    {
        auto& app = App::GetApp();
        auto scene = app->GetScene<Scene>();
        auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
        auto pad = App::GetApp()->GetInputDevice().GetControlerVec()[0];

        auto player = GetSharedGameObject<Player>(L"Player");
        auto enemyMgr = GetSharedGameObject<EnemyManager>(L"EnemyManager");
        auto boss = GetSharedGameObject<BossFirst>(L"Boss");

        auto fadeout = GetSharedGameObject<FadeoutSprite>(L"Fadeout");


        fadeout->GetBlackFlag();
        m_BlackFlag = fadeout->GetBlackFlag();
        fadeout->GetFadeOutFlag();
        m_IsFadeOutFlag = fadeout->GetFadeOutFlag();
        fadeout->GetFadeInFlag();
        m_IsFadeInFlag = fadeout->GetFadeInFlag();


        player->GetHP();
        auto plaHP = player->GetHP();

        auto ptrSoundManager = GetSharedGameObject<SoundManager>(L"SoundManager");

        auto EnemyVec = enemyMgr->GetEnemyVec(true);
        int EnemyNum = EnemyVec.size();

        EffectManager::Instance().InterfaceUpdate();

        ////デバック用
        wstringstream wss(L"");
        wss /* << L"デバッグ用文字列 "*/
            << L"\n FPS: " << App::GetApp()->App::GetApp()->GetStepTimer().GetFramesPerSecond() << endl;

        scene->SetDebugString(wss.str());


        if (m_waveNow == 1 && EnemyNum == 0)
        {
            m_IsFadeOutFlag = true;
        }
        if (m_waveNow == 2 && EnemyNum == 0)
        {
            m_IsFadeOutFlag = true;
        }

        if (m_BlackFlag == true)
        {
            m_NextWaveFlag = true;
        }


        if (m_NextWaveFlag == true && m_waveNow == 1)
        {

            m_waveNow = 2;

            player->GetComponent<Transform>()->SetPosition(Vec3(0.0f, 3.0f, -40.0f));
            player->GetComponent<Transform>()->SetRotation(Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f));
            GetSharedGameObject<CameraManager>(L"CameraManager")->SetAngle(L"Y", XMConvertToRadians(270.0f));

            m_IsFadeInFlag = true;
            m_NextWaveFlag = false;

            //ウェーブ２敵
            enemyMgr->InstEnemy(Vec3(0.0f, 2.0f, 0.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            enemyMgr->InstEnemy(Vec3(10.0f, 2.0f, 30.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            enemyMgr->InstEnemy(Vec3(-10.0f,2.0f, -20.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            enemyMgr->InstEnemy(Vec3(20.0f, 2.0f, 10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            enemyMgr->InstEnemy(Vec3(-20.0f,2.0f, -10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            enemyMgr->InstEnemy(Vec3(30.0f, 2.0f, 30.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            enemyMgr->InstEnemy(Vec3(-30.0f,2.0f, -5.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));

            //enemyMgr->InstEnemy(Vec3(0.0f, 2.0f, 0.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            //enemyMgr->InstEnemy(Vec3(10.0f, 2.0f, 30.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            //enemyMgr->InstEnemy(Vec3(-10.0f,2.0f, -20.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            //enemyMgr->InstEnemy(Vec3(20.0f, 2.0f, 10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            enemyMgr->InstEnemy<EnemyZakoLong>(Vec3(-20.0f,2.0f, -15.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            enemyMgr->InstEnemy<EnemyZakoLong>(Vec3(30.0f, 2.0f, 35.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            enemyMgr->InstEnemy<EnemyZakoLong>(Vec3(-30.0f,2.0f, -15.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));

        }


        if (m_NextWaveFlag == true && m_waveNow == 2)
        {
            //ボス戦開始
            m_waveNow = 3;

            m_bossGauge->ClearBossGaugeUI(false);

            player->GetComponent<Transform>()->SetPosition(Vec3(0.0f, 3.0f, 195.0f));

            player->GetComponent<Transform>()->SetRotation(Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f));
            GetSharedGameObject<CameraManager>(L"CameraManager")->SetAngle(L"Y", XMConvertToRadians(270.0f));

            m_IsFadeInFlag = true;
            m_NextWaveFlag = false;


            enemyMgr->InstBoss(dynamic_pointer_cast<EnemyBase>(boss));


            GetSharedGameObject<SoundManager>(L"SoundManager")->StopBGM();
            GetSharedGameObject<SoundManager>(L"SoundManager")->PlayBGM(4);

        }

        if (m_waveNow == 3)
        {
            auto boss = enemyMgr->GetBoss();
            m_bossCurrentHP = boss->GetHPCurrent();//BossのHP取得
        }

        if (m_waveNow == 3 && m_bossCurrentHP <= 0)
        {
            GetSharedGameObject<SoundManager>(L"SoundManager")->StopBGM();
            scene->PostEvent(3.0f, GetThis<ObjectInterface>(),app->GetScene<Scene>(), L"ToGameClear");
            m_waveNow = 4;//ウェーブ終了
        }

        if (plaHP <= 0)
        {
            GetSharedGameObject<SoundManager>(L"SoundManager")->StopBGM();
            scene->PostEvent(1.0f, GetThis<ObjectInterface>(), app->GetScene<Scene>(), L"ToGameOver");

        }

        fadeout->SetFadeOutFlag(m_IsFadeOutFlag);
        fadeout->SetFadeInFlag(m_IsFadeInFlag);
        fadeout->SetBlackFlag(m_BlackFlag);
    }

    void WaveStage::OnDraw()
    {
        EffectManager::Instance().InterfaceDraw();
    }

    void WaveStage::OnDestroy()
    {
        //BGMとSEを止める
        auto soundManager = GetSharedGameObject<SoundManager>(L"SoundManager");
        soundManager->StopBGM();
        soundManager->StopSE();
    }

    void WaveStage::SetNextWaveFlag(int setNextWaveFlag)
    {
        m_NextWaveFlag = setNextWaveFlag;
    }

    bool WaveStage::GetNextWaveFlag()
    {
        return m_NextWaveFlag;
    }


    //床作成
    void WaveStage::CreateFloor()
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

            // ブロック生成
			//AddGameObject<Block>(v[0],v[2]);

            /*
            // 認識を間違えていたので没です。でも一応残す
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
                    AddGameObject<Block>(
                        Vec3(
                            // ブロック1個の大きさ - ステージの位置 + (ステージの大きさ / 2) + (ブロックの大きさ / 2)
                            //                              ↑こうしないとステージの中央から生成されてしまう
                            j * scaleXZ - (pos.x + (scl.x / 2) - shiftPos),
                            pos.y + 0.05f,
                            // 基本は変わらないけど、Wave1とBossの床が逆になっていたのでこうすると直りました
                            i * scaleXZ + (pos.z - (scl.z / 2) + shiftPos)
                        )
                    );
                }
            }
            */
        }


    }

    //天井,床
    Floor::Floor(const shared_ptr<Stage>& StagePtr,
        const Vec3& Scale,
        const Vec3& Rotation,
        const Vec3& Position
    ) :
        GameObject(StagePtr),
        m_Scale(Scale),
        m_Rotation(Rotation),
        m_Position(Position)
    {
    }
    Floor::~Floor() {}

    void Floor::OnCreate()
    {
        auto ptrTransform = GetComponent<Transform>();
        ptrTransform->SetScale(m_Scale);
        ptrTransform->SetRotation(m_Rotation);
        ptrTransform->SetPosition(m_Position);

        //Transformに対しての等差数列
        Mat4x4 spanMat;
        spanMat.affineTransformation(
            Vec3(1.0f, 1.0f, 1.0f),
            Vec3(0.0f, 0.0f, 0.0f),
            Vec3(0.0f, 0.0f, 0.0f),
            Vec3(0.0f, 0.0f, 0.0f)
        );

        auto ptrColl = AddComponent<CollisionObb>();
        ptrColl->SetFixed(true);
        

        auto ptrDraw = AddComponent<PNTStaticInstanceDraw>();
        ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
        ptrDraw->SetTextureResource(L"FloorTex");

        ptrDraw->SetOwnShadowActive(true);

        ptrDraw->SetMeshToTransformMatrix(spanMat);

		for (int i = 0; i < (int)m_Scale.x / 10; i++)
		{
			for (int j = 0; j < (int)m_Scale.z / 10; j++)
			{

				// ブロックの位置を取得
				float x = ((j * 10.0f) + 5.0) - (m_Scale.x / 2);
				float z = (m_Scale.z / 2) - (5.0f + (i * 10.0f));
				//インスタンス用の行列を作成する
				Mat4x4 matrix;
                matrix.affineTransformation(
                    Vec3(10.0f, 0.1f, 10.0f),
                    Vec3(),
                    Vec3(),
                    Vec3(x + m_Position.x, -1.5f, z + m_Position.z)
                );
                ptrDraw->AddMatrix(matrix);//ブロックを表示
			}
		}

        GetStage()->SetCollisionPerformanceActive(true);
        GetStage()->SetUpdatePerformanceActive(true);
        GetStage()->SetDrawPerformanceActive(true);

        AddTag(L"Floor");
        AddTag(L"CameraObstacles");
        AddTag(L"Terrain");

        Shadowmap::SetLightHeight(80.0f);
        //Shadowmap::SetLightNear(10.0f);
        Shadowmap::SetLightFar(400.0f);
        Shadowmap::SetViewSize(150.0f);

        ptrDraw->SetDiffuse(Col4(0.7f, 0.8f, 0.9f, 0.0f));
        //ptrDraw->SetDiffuse(Col4(0.8f, 0.9f, 1.0f, 0.0f));
        //ptrDraw->SetTextureResource(L"Wall");

    }

    // 床の見た目変更用のブロックのクラス ---------------------------------------------------------
    // Floorクラスを参考に作りました
    Block::Block(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
        const Vec3& Position
    ) :
        MyGameObject(StagePtr),
        m_pos(Position)
    {
    }

    void Block::OnCreate()
    {
        auto ptrTransform = GetComponent<Transform>();
        // 回転は固定  必要だったらメンバ変数増やします
        ptrTransform->SetScale(m_scale);
        ptrTransform->SetRotation(Vec3(0.0f));
        ptrTransform->SetPosition(Vec3(m_pos.x, m_pos.y + 0.05f, m_pos.z));


        //Transformに対しての等差数列
        Mat4x4 spanMat;
        spanMat.affineTransformation(
            Vec3(1.0f, 1.0f, 1.0f),
            Vec3(0.0f, 0.0f, 0.0f),
            Vec3(0.0f, 0.0f, 0.0f),
            Vec3(0.0f, 0.0f, 0.0f)
        );

        auto ptrDraw = AddComponent<PNTStaticDraw>();
        ptrDraw->SetMeshResource(L"DEFAULT_CUBE");

        ptrDraw->SetDiffuse(Col4(0.7f, 0.8f, 0.9f, 0.0f));

		ptrDraw->SetMeshToTransformMatrix(spanMat);

        // テクスチャ
        ptrDraw->SetTextureResource(L"FloorTex");

        //影をつける（シャドウマップを描画する）
        //auto shadowPtr = AddComponent<Shadowmap>();
        //ptrDraw->SetOwnShadowActive(true);

        float texOffset = 5.0f;

   //     for (int i = 0; i < (int)m_scale.x / 10; i++)
   //     {
			//for (int j = 0; j < (int)m_scale.z / 10; j++)
			//{
   //             // ブロックの位置を取得
   //             float x = (j * 10.0f) - texOffset;
			//	float z = texOffset - (i * 10.0f);

   //             //インスタンス用の行列を作成する
   //             Mat4x4 matrix;

   //             matrix.affineTransformation(
   //                 Vec3(10.0f, 0.1f, 10.0f),
   //                 Vec3(),
   //                 Vec3(),
   //                 Vec3(x, 0.0f, z)
   //             );
   //             ptrDraw->AddMatrix(matrix);//ブロックを表示
   //         }
   //     }
    }
    // END -----------------------------------------------------------------------------------------

    //壁作成
    void WaveStage::CreateWall()
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

    //壁
    Wall::Wall(const shared_ptr<Stage>& StagePtr,
        const Vec3& Scale,
        const Vec3& Rotation,
        const Vec3& Position
    ) :
        GameObject(StagePtr),
        m_Scale(Scale),
        m_Rotation(Rotation),
        m_Position(Position)
    {
    }
    Wall::~Wall() {}

    void Wall::OnCreate()
    {
        auto ptrTransform = GetComponent<Transform>();
        ptrTransform->SetScale(m_Scale);
        ptrTransform->SetRotation(m_Rotation);
        ptrTransform->SetPosition(m_Position);

        auto ptrColl = AddComponent<CollisionObb>();
        ptrColl->SetFixed(true);
        ptrColl->SetDrawActive(true);

        auto ptrDraw = AddComponent<PNTStaticDraw>();
        ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
        ptrDraw->SetTextureResource(L"WallSideTex");
        

        //Transformに対しての等差数列
        Mat4x4 spanMat;
        spanMat.affineTransformation(
            Vec3(0.5f, 1.0f, 1.2f),
            Vec3(0.0f, 0.0f, 0.0f),
            Vec3(0.0f, 0.0f, 0.0f),
            Vec3(0.0f, 0.0f, 0.0f)
        );
        ptrDraw->SetMeshToTransformMatrix(spanMat);

        //ptrDraw->SetDiffuse(Col4(0.8f, 0.9f, 1.0f, 0.0f));
        ptrDraw->SetDiffuse(Col4(0.4f, 0.5f, 0.6f, 0.0f));
        //ptrDraw->SetTextureResource(L"Wall");

        AddTag(L"CameraObstacles");
        AddTag(L"Terrain");

    }

    //壁2------------------------------------------------
    Wall2::Wall2(const shared_ptr<Stage>& StagePtr,
        const Vec3& Scale,
        const Vec3& Rotation,
        const Vec3& Position
    ) :
        GameObject(StagePtr),
        m_Scale(Scale),
        m_Rotation(Rotation),
        m_Position(Position)
    {
    }
    Wall2::~Wall2() {}

    void Wall2::OnCreate()
    {
        auto ptrTransform = GetComponent<Transform>();
        ptrTransform->SetScale(m_Scale);
        ptrTransform->SetRotation(m_Rotation);
        ptrTransform->SetPosition(m_Position);

        auto ptrColl = AddComponent<CollisionObb>();
        ptrColl->SetFixed(true);
        ptrColl->SetDrawActive(true);

        auto ptrDraw = AddComponent<PNTStaticDraw>();
        ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
        ptrDraw->SetTextureResource(L"WallFrontTex");

        //Transformに対しての等差数列
        Mat4x4 spanMat;
        spanMat.affineTransformation(
            Vec3(1.0f, 1.0f, 0.5f),
            Vec3(0.0f, 0.0f, 0.0f),
            Vec3(0.0f, 0.0f, 0.0f),
            Vec3(0.0f, 0.0f, 0.0f)
        );
        ptrDraw->SetMeshToTransformMatrix(spanMat);


        //ptrDraw->SetDiffuse(Col4(0.8f, 0.9f, 1.0f, 0.0f));
        ptrDraw->SetDiffuse(Col4(0.4f, 0.5f, 0.6f, 0.0f));
        //ptrDraw->SetTextureResource(L"Wall");

        AddTag(L"CameraObstacles");
        AddTag(L"Terrain");

    }
    //-------------------------------------------------------

    //天井作成
    void WaveStage::CreateCeiling()
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

    //天井
    Ceiling::Ceiling(const shared_ptr<Stage>& StagePtr,
        const Vec3& Scale,
        const Vec3& Rotation,
        const Vec3& Position
    ) :
        GameObject(StagePtr),
        m_Scale(Scale),
        m_Rotation(Rotation),
        m_Position(Position)
    {
    }
    Ceiling::~Ceiling() {}

    void Ceiling::OnCreate()
    {
        auto ptrTransform = GetComponent<Transform>();
        ptrTransform->SetScale(m_Scale);
        ptrTransform->SetRotation(m_Rotation);
        ptrTransform->SetPosition(m_Position);
        
        auto ptrColl = AddComponent<CollisionObb>();
        ptrColl->SetFixed(true);

        auto ptrDraw = AddComponent<PNTStaticDraw>();
        ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
        ptrDraw->SetTextureResource(L"CeilingTex");

        ptrDraw->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 0.0f));

        AddTag(L"CameraObstacles");
        AddTag(L"Terrain");

    }


    //出入口作成
    Door::Door(const shared_ptr<Stage>& StagePtr,
        const Vec3& Scale,
        const Vec3& Rotation,
        const Vec3& Position
    ) :
        GameObject(StagePtr),
        m_Scale(Scale),
        m_Rotation(Rotation),
        m_Position(Position)
    {
    }
    Door::~Door() {}

    void Door::OnCreate()
    {
        auto ptrTransform = GetComponent<Transform>();
        ptrTransform->SetScale(m_Scale);
        ptrTransform->SetRotation(m_Rotation);
        ptrTransform->SetPosition(m_Position);

        auto ptrColl = AddComponent<CollisionObb>();
        ptrColl->SetFixed(true);

        auto ptrDraw = AddComponent<PNTStaticDraw>();
        ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
        AddTag(L"CameraObstacles");
        AddTag(L"Terrain");

        ptrDraw->SetDiffuse(Col4(0.5f, 0.6f, 0.7f, 0.0f));
        
    }


    FadeoutSprite::FadeoutSprite(const shared_ptr<Stage>& stage, const wstring& ResKey) :
        GameObject(stage),
        m_ResKey(ResKey),
        m_IsFadeOutFlag(false),
        m_IsFadeInFlag(false),
        m_Color(0.0f),
        m_BlackFlag(false)
    {
    }

    FadeoutSprite::~FadeoutSprite() {

    }

    void FadeoutSprite::OnCreate() {
        Col4 color(0, 0, 0, 0);
        const float windowWidth = 1280.0f;
        const float windowHeight = 800.0f;

        if (m_ResKey == L"Fadeout") {
            m_Vertices = {
                { Vec3(-windowWidth, 400, 0.1f), color, Vec2(0, 0) },
                { Vec3(+windowWidth, 400, 0.1f), color, Vec2(1, 0) },
                { Vec3(-windowWidth,-400, 0.1f), color, Vec2(0, 1) },
                { Vec3(+windowWidth,-400, 0.1f), color, Vec2(1, 1) },
            };
            vector<uint16_t> indices = {
                0, 1, 2,
                2, 1, 3,
            };
            m_drawComp = AddComponent<PCTSpriteDraw>(m_Vertices, indices);
            m_drawComp->SetTextureResource(m_ResKey);
        }
        m_drawComp->SetSamplerState(SamplerState::LinearWrap);
        m_drawComp->SetDiffuse(Col4(0, 0, 0, 0));
        SetAlphaActive(true);
    }

    void FadeoutSprite::OnUpdate() {



        if (m_IsFadeOutFlag == true)
        {
            m_Color += 0.01;
        }

        if (m_Color >= 1)
        {
            m_Color = 1;
            m_BlackFlag = true;
            m_IsFadeOutFlag = false;
        }


        if (m_IsFadeInFlag == true)
        {
            m_BlackFlag = false;
            m_Color -= 0.005;
        }

        if (m_Color <= 0)
        {
            m_Color = 0;
            m_IsFadeInFlag = false;
        }

        m_drawComp->SetDiffuse(Col4(m_Color, m_Color, m_Color, m_Color));

    }

    void FadeoutSprite::SetBlackFlag(int setBlackFlag)
    {
        m_BlackFlag = setBlackFlag;
    }

    bool FadeoutSprite::GetBlackFlag()
    {
        return m_BlackFlag;
    }

    void FadeoutSprite::SetFadeOutFlag(int setFadeOutFlag)
    {
        m_IsFadeOutFlag = setFadeOutFlag;
    }

    bool FadeoutSprite::GetFadeOutFlag()
    {
        return m_IsFadeOutFlag;
    }

    void FadeoutSprite::SetFadeInFlag(int setFadeInFlag)
    {
        m_IsFadeInFlag = setFadeInFlag;
    }

    bool FadeoutSprite::GetFadeInFlag()
    {
        return m_IsFadeInFlag;
    }

}
//end basecross
