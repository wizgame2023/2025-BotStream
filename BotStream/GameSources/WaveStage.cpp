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
        //全体確認カメラ
        const Vec3 eye(0.0f, 350.0f, -20.0f);
        //Wave1確認カメラ
        //const Vec3 eye(0.0f, 1.0f, -25.5f);
        //Wave2確認カメラ
        //const Vec3 eye(0.0f, 1.0f, -5.3f);
        //Boss確認カメラ
        //const Vec3 eye(0.0f, 1.0f, 24.5f);

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

            CreateSharedObjectGroup(L"Actor");

            auto player = AddGameObject<Player>(Vec3(0.0f, 5.0f, -90.0f), Vec3(0.0f, 5.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
            SetSharedGameObject(L"Player", player);

            auto enemyMgr = AddGameObject<EnemyManager>();
            SetSharedGameObject(L"EnemyManager", enemyMgr);

            auto boss = AddGameObject<BossFirst>(Vec3(0.0f, 10.0f, 100.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
            SetSharedGameObject(L"Boss", boss);
            //enemyMgr->InstBoss(dynamic_pointer_cast<EnemyBase>(boss));

            auto playerUI = AddGameObject<PlayerGaugeUI>(100);
            SetSharedGameObject(L"PlayerUI", playerUI);


            auto door = AddGameObject<Door>(Vec3(6.5f, 0.1f, 4.0f), Vec3(0.0f, 1.55f, 1.57f), Vec3(-2.0f, 1.75f, -125.0f));
            SetSharedGameObject(L"Door", door);
            auto door2 = AddGameObject<Door>(Vec3(6.5f, 0.1f, 4.0f), Vec3(0.0f, 1.58f, 1.57f), Vec3(2.0f, 1.75f, -125.0f));
            SetSharedGameObject(L"Door2", door2);
            auto door3 = AddGameObject<Door>(Vec3(6.5f, 0.1f, 4.0f), Vec3(0.0f, 1.58f, 1.57f), Vec3(2.0f, 1.75f, -55.0f));
            SetSharedGameObject(L"Door3", door3);
            auto door4 = AddGameObject<Door>(Vec3(6.5f, 0.1f, 4.0f), Vec3(0.0f, 1.58f, 1.57f), Vec3(-2.0f, 1.75f, -55.0f));
            SetSharedGameObject(L"Door4", door4);
            
            auto door5 = AddGameObject<Door>(Vec3(6.5f, 0.1f, 4.0f), Vec3(0.0f, 1.57f, 1.57f), Vec3(-2.0f, 1.75f, -35.0f));
            SetSharedGameObject(L"Door5", door5);
            auto door6 = AddGameObject<Door>(Vec3(6.5f, 0.1f, 4.0f), Vec3(0.0f, 1.58f, 1.57f), Vec3(2.0f, 1.75f, -35.0f));
            SetSharedGameObject(L"Door6", door6);
            auto door7 = AddGameObject<Door>(Vec3(6.5f, 0.1f, 4.0f), Vec3(0.0f, 1.6f, 1.57f), Vec3(2.0f, 1.75f, 35.0f));
            SetSharedGameObject(L"Door7", door7);
            auto door8 = AddGameObject<Door>(Vec3(6.5f, 0.1f, 4.0f), Vec3(0.0f, 1.57f, 1.57f), Vec3(-2.0f, 1.75f, 35.0f));
            SetSharedGameObject(L"Door8", door8);
           
            auto door9 = AddGameObject<Door>(Vec3(6.5f, 0.1f, 4.0f), Vec3(0.0f, 1.57f, 1.57f), Vec3(-2.0f, 1.75f, 55.0f));
            SetSharedGameObject(L"Door9", door9);
            auto door10 = AddGameObject<Door>(Vec3(6.5f, 0.1f, 4.0f), Vec3(0.0f, 1.57f, 1.57f), Vec3(2.0f, 1.75f, 55.0f));
            SetSharedGameObject(L"Door10", door10);

            //wave1敵
            enemyMgr->InstEnemy(Vec3(0.0f, 5.5f, -75.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(0.7f*10.0f, 0.7f, 0.7f*10.0f));
            enemyMgr->InstEnemy(Vec3(-10.0f, 5.5f, -85.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(0.7f * 10.0f, 0.7f, 0.7f * 10.0f));
            enemyMgr->InstEnemy(Vec3(10.0f, 5.5f, -85.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(0.7f * 10.0f, 0.7f, 0.7f * 10.0f));

            auto cameraManager = AddGameObject<CameraManager>();
            SetSharedGameObject(L"CameraManager", cameraManager);

            auto ptrSoundManager = AddGameObject<SoundManager>();
            SetSharedGameObject(L"SoundManager", ptrSoundManager);
            GetSharedGameObject<SoundManager>(L"SoundManager")->PlayBGM(3);
            GetSharedGameObject<SoundManager>(L"SoundManager")->PlaySE(13);

            auto colController = AddGameObject<StageCollisionController>();
            colController->SetCollisionSwhich(true);

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

        player->GetHP();
        auto plaHP = player->GetHP();

        auto ptrSoundManager = GetSharedGameObject<SoundManager>(L"SoundManager");

        auto door3 = GetSharedGameObject<Door>(L"Door3");
        auto door4 = GetSharedGameObject<Door>(L"Door4");
        auto door5 = GetSharedGameObject<Door>(L"Door5");
        auto door6 = GetSharedGameObject<Door>(L"Door6");
        auto door7 = GetSharedGameObject<Door>(L"Door7");
        auto door8 = GetSharedGameObject<Door>(L"Door8");
        auto door9 = GetSharedGameObject<Door>(L"Door9");
        auto door10 = GetSharedGameObject<Door>(L"Door10");

        auto EnemyVec = enemyMgr->GetEnemyVec(true);
        int EnemyNum = EnemyVec.size();

        EffectManager::Instance().InterfaceUpdate();


        //if (pad.wPressedButtons & XINPUT_GAMEPAD_X)
        //{
        //    GetSharedGameObject<SoundManager>(L"SoundManager")->PlaySE(5);
        //    
        //    //EnemyNum = 0;
        //}
        //if (pad.wPressedButtons & XINPUT_GAMEPAD_A)
        //{
        //    GetSharedGameObject<SoundManager>(L"SoundManager")->PlaySE(3);
        //}

        if (m_waveNow == 1 && EnemyNum == 0)
        {
            //wave２になった
            m_waveNow = 2;

            EnemyNum = 6;

            //ウェーブ２敵
            enemyMgr->InstEnemy(Vec3(-15.0f, 5.5f, 10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(0.7f * 10.0f, 0.7f, 0.7f * 10.0f));
            enemyMgr->InstEnemy(Vec3(-10.0f, 5.5f, 20.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(0.7f * 10.0f, 0.7f, 0.7f * 10.0f));
            enemyMgr->InstEnemy(Vec3(0.0f, 5.5f, 10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(0.7f * 10.0f, 0.7f, 0.7f * 10.0f));
            enemyMgr->InstEnemy(Vec3(10.0f, 5.5f, 20.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(0.7f * 10.0f, 0.7f, 0.7f * 10.0f));
            enemyMgr->InstEnemy(Vec3(15.0f, 5.5f, 10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(0.7f * 10.0f, 0.7f, 0.7f * 10.0f));


            door3->GetComponent<Transform>()->SetPosition(6.0f, 1.75f, -55.0f);
            door4->GetComponent<Transform>()->SetPosition(-6.0f, 1.75f, -55.0f);
            door5->GetComponent<Transform>()->SetPosition(6.0f, 1.75f, -35.0f);
            door6->GetComponent<Transform>()->SetPosition(-6.0f, 1.75f, -35.0f);

        }

        if (m_waveNow == 2 && EnemyNum == 0)
        {
            //ボス戦開始
            m_waveNow = 3;

            EnemyNum = 1;

            enemyMgr->InstBoss(dynamic_pointer_cast<EnemyBase>(boss));

            door7->GetComponent<Transform>()->SetPosition(6.0f, 1.75f, 35.0f);
            door8->GetComponent<Transform>()->SetPosition(-6.0f, 1.75f, 35.0f);
            door9->GetComponent<Transform>()->SetPosition(6.0f, 1.75f, 55.0f);
            door10->GetComponent<Transform>()->SetPosition(-6.0f, 1.75f, 55.0f);

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
            scene->PostEvent(2.0f, GetThis<ObjectInterface>(),app->GetScene<Scene>(), L"ToGameClear");
        }

        if (plaHP == 0)
        {
            GetSharedGameObject<SoundManager>(L"SoundManager")->StopBGM();
            scene->PostEvent(1.0f, GetThis<ObjectInterface>(), app->GetScene<Scene>(), L"ToGameOver");

        }
    }

    void WaveStage::OnDraw()
    {
        EffectManager::Instance().InterfaceDraw();
    }


    //床作成
    void WaveStage::CreateFloor()
    {
        vector < vector<Vec3> > vec =
        {
            //Boss
            {
                Vec3(70.0f, 1.0f, 70.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, -2.0f, 90.0f)
            },
            //通路
            {
                Vec3(8.0f, 1.0f, 20.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, -2.0f, 45.0f)
            },
            //Wave2
            {
                Vec3(70.0f, 1.0f, 70.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, -2.0f, 0.0f)
            },
            //通路
            {
                Vec3(8.0f, 1.0f, 20.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, -2.0f, -45.0f)
            },
            //Wave1
            {
                Vec3(70.0f, 1.0f, 70.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, -2.0f, -90.0f)
            }

        };
        for (auto v : vec)
        {
            AddGameObject<Floor>(v[0], v[1], v[2]);
        }
    }

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

        auto ptrColl = AddComponent<CollisionObb>();
        ptrColl->SetFixed(true);

        auto ptrDraw = AddComponent<BcPNTStaticDraw>();
        ptrDraw->SetMeshResource(L"DEFAULT_CUBE");

        AddTag(L"Floor");

        ptrDraw->SetDiffuse(Col4(0.7f, 0.8f, 0.9f, 0.0f));
        //ptrDraw->SetDiffuse(Col4(0.8f, 0.9f, 1.0f, 0.0f));
        //ptrDraw->SetTextureResource(L"Wall");

        AddTag(L"CameraObstacles");
    }


    //壁作成
    void WaveStage::CreateWall()
    {
        vector < vector<Vec3> > vec =
        {
            //Boss
            //右
            {
                Vec3(30.0f, 0.1f, 70.0f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(35.0f, 13.5f, 90.0f)
            },
            //左
            {
                Vec3(30.0f, 0.1f, 70.0f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(-35.0f, 13.5f, 90.0f)
            },
            //前
            {
                Vec3(30.0f, 0.1f, 70.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(0.0f, 13.5f, 125.0f)
            },
            //後左
            {
                Vec3(30.0f, 0.1f, 31.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(-19.5f, 13.5f, 55.2f)
            },
            //後真ん中
            {
                Vec3(25.0f, 0.1f, 8.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(0.0f, 17.5f, 55.2f)
            },
            //後右
            {
                Vec3(30.0f, 0.1f, 31.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(19.5f, 13.5f, 55.2f)
            },
            //Wave2
            //右
            {
                Vec3(30.0f, 0.1f, 70.0f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(35.0f, 13.5f, 0.0f)
            },
            //左
            {
                Vec3(30.0f, 0.1f, 70.0f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(-35.0f, 13.5f, 0.0f)
            },
            //前左
            {
                Vec3(30.0f, 0.1f, 31.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(-19.5f, 13.5f, 34.8f)
            },
            //前真ん中
            {
                Vec3(25.0f, 0.1f, 8.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(0.0f, 17.5f, 34.8f)
            },
            //前右
            {
                Vec3(30.0f, 0.1f, 31.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(19.5f, 13.5f, 34.8f)
            },
            //後左
            {
                Vec3(30.0f, 0.1f, 31.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(-19.5f, 13.5f, -34.8f)
            },
            //後真ん中
            {
                Vec3(25.0f, 0.1f, 8.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(0.0f, 17.5f, -34.8f)
            },
            //後右
            {
                Vec3(30.0f, 0.1f, 31.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(19.5f, 13.5f, -34.8f)
            },
            //Wave1
            //右
            {
                Vec3(30.0f, 0.1f, 70.0f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(35.0f, 13.5f, -90.0f)
            },
            //左
            {
                Vec3(30.0f, 0.1f, 70.0f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(-35.0f, 13.5f, -90.0f)
            },
            //前左
            {
                Vec3(30.0f, 0.1f, 31.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(-19.5f, 13.5f, -55.2f)
            },
            //前真ん中
            {
                Vec3(25.0f, 0.1f, 8.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(0.0f, 17.5f, -55.2f)
            },
            //前右
            {
                Vec3(30.0f, 0.1f, 31.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(19.5f, 13.5f, -55.2f)
            },
            //後左
            {
                Vec3(30.0f, 0.1f, 31.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(-19.5f, 13.5f, -125.0f)
            },
            //後真ん中
            {
                Vec3(25.0f, 0.1f, 8.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(0.0f, 17.5f, -125.0f)
            },
             //後右
            {
                Vec3(30.0f, 0.1f, 31.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(19.5f, 13.5f, -125.0f)
            },
            //Wave1,2通路
            //右
            {
                Vec3(6.5f, 0.1f, 20.0f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(4.0f, 1.8f, -45.0f)
            },
            //左
            {
                Vec3(6.5f, 0.1f, 20.0f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(-4.0f, 1.8f, -45.0f)
            },
            //Wave2,Boss通路
            //右
            {
                Vec3(6.5f, 0.1f, 20.0f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(4.0f, 1.8f, 45.0f)
            },
            //左
            {
                Vec3(6.5f, 0.1f, 20.0f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(-4.0f, 1.8f, 45.0f)
            },

        };
        for (auto v : vec)
        {
            AddGameObject<Wall>(v[0], v[1], v[2]);
        }
    }

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

        auto ptrDraw = AddComponent<BcPNTStaticDraw>();
        ptrDraw->SetMeshResource(L"DEFAULT_CUBE");


        //ptrDraw->SetDiffuse(Col4(0.8f, 0.9f, 1.0f, 0.0f));
        ptrDraw->SetDiffuse(Col4(0.4f, 0.5f, 0.6f, 0.0f));
        //ptrDraw->SetTextureResource(L"Wall");

        AddTag(L"CameraObstacles");

    }


    //天井作成
    void WaveStage::CreateCeiling()
    {
        vector < vector<Vec3> > vec =
        {
            //Boss
            {
                Vec3(70.0f, 0.1f, 70.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, 28.5f, 90.0f)
            },
            //通路
            {
                Vec3(8.0f, 0.1f, 20.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, 5.0f, 45.0f)
            },
            //Wave2
            {
                Vec3(70.0f, 0.1f, 70.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, 28.5f, 0.0f)
            },
            //通路
            {
                Vec3(8.0f, 0.1f, 20.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, 5.0f, -45.0f)
            },
            //Wave1
            {
                Vec3(70.0f, 0.1f, 70.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, 28.5f, -90.0f)
            }

        };
        for (auto v : vec)
        {
            AddGameObject<Ceiling>(v[0], v[1], v[2]);
        }
    }

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

        auto ptrDraw = AddComponent<BcPNTStaticDraw>();
        ptrDraw->SetMeshResource(L"DEFAULT_CUBE");

        ptrDraw->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 0.0f));

        AddTag(L"CameraObstacles");

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

        auto ptrDraw = AddComponent<BcPNTStaticDraw>();
        ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
        AddTag(L"CameraObstacles");

        ptrDraw->SetDiffuse(Col4(0.5f, 0.6f, 0.7f, 0.0f));
        
        //m_HPGauge = GetStage()->AddGameObject<BillBoard>(GetThis<GameObject>(), L"PLHP", 2, 8.0f);
        //m_ArmorGauge = GetStage()->AddGameObject<BillBoard>(GetThis<GameObject>(), L"PLSP", 2, 7.5f);

        //m_HPGauge->SetScale(Vec3(6.0f, 0.5f, 3.0f));
        //m_ArmorGauge->SetScale(Vec3(6.0f, 0.3f, 3.0f));
    }

}
//end basecross
