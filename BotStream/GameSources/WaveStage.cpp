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

        //auto headParts = AddGameObject<HeadParts>();
        //SetSharedGameObject(L"HeadParts", headParts);
        //headParts->GetComponent<Transform>()->SetPosition(0.0f, -1.0f, -280.0f);

        //auto bodyParts = AddGameObject<BodyParts>();
        //SetSharedGameObject(L"BodyParts", bodyParts);
        //bodyParts->GetComponent<Transform>()->SetPosition(15.0f, -1.0f, -280.0f);

        //auto legParts = AddGameObject<LegParts>();
        //SetSharedGameObject(L"LegParts", legParts);
        //legParts->GetComponent<Transform>()->SetPosition(-15.0f, -1.0f, -280.0f);

        //auto partsMgr = AddGameObject<PartsManager>();
        //SetSharedGameObject(L"PartsManager", partsMgr);

        // ボスゲージ
        m_bossGauge = AddGameObject<BossGaugeUI>(
            boss,
            boss->GetHPMax()
            );
        SetSharedGameObject(L"BossUI", m_bossGauge);
        
        //wave1敵
        enemyMgr->InstEnemy<EnemyZako>(Vec3(0.0f, 2.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        enemyMgr->InstEnemy<EnemyZako>(Vec3(10.0f, 2.0f, -255.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        enemyMgr->InstEnemy<EnemyZako>(Vec3(-10.0f,2.0f, -235.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        enemyMgr->InstEnemy<EnemyZako>(Vec3(20.0f, 2.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        enemyMgr->InstEnemy<EnemyZako>(Vec3(-20.0f,2.0f, -245.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        enemyMgr->InstEnemy<EnemyZako>(Vec3(30.0f, 2.0f, -225.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        enemyMgr->InstEnemy<EnemyZako>(Vec3(-30.0f,2.0f, -225.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        //auto flyingEnemy = AddGameObject<EnemyZakoFlying>(Vec3(0.0f, 10.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f), true);
        
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

        //auto headParts = GetSharedGameObject<HeadParts>(L"HeadParts");
        //auto bodyParts = GetSharedGameObject<BodyParts>(L"BodyParts");
        //auto legParts = GetSharedGameObject<LegParts>(L"LegParts");
        //auto partsMgr = GetSharedGameObject<PartsManager>(L"PartsManager");

        fadeout->GetBlackFlag();
        m_BlackFlag = fadeout->GetBlackFlag();
        fadeout->GetFadeOutFlag();
        m_IsFadeOutFlag = fadeout->GetFadeOutFlag();
        fadeout->GetFadeInFlag();
        m_IsFadeInFlag = fadeout->GetFadeInFlag();

        //headParts->GetHeadParts();
        //partsMgr->GetAttachHeadParts();
        //m_GetHeadParts = headParts->GetHeadParts();
        //m_AttachHeadParts = partsMgr->GetAttachHeadParts();

        //bodyParts->GetBodyParts();
        //partsMgr->GetAttachBodyParts();
        //m_GetBodyParts = bodyParts->GetBodyParts();
        //m_AttachBodyParts = partsMgr->GetAttachBodyParts();

        //legParts->GetLegParts();
        //partsMgr->GetAttachLegParts();
        //m_GetLegParts = legParts->GetLegParts();
        //m_AttachLegParts = partsMgr->GetAttachLegParts();

        player->GetHP();
        auto plaHP = player->GetHP();

        auto ptrSoundManager = GetSharedGameObject<SoundManager>(L"SoundManager");

        auto EnemyVec = enemyMgr->GetEnemyVec(true);
        int EnemyNum = EnemyVec.size();

        EffectManager::Instance().InterfaceUpdate();


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

            player->GetComponent<Transform>()->SetPosition(Vec3(0.0f, 2.0f, -40.0f));
            player->GetComponent<Transform>()->SetRotation(Vec3(0.0f, 5.0f, 0.0f));

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

            enemyMgr->InstEnemy(Vec3(0.0f, 2.0f, 0.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            enemyMgr->InstEnemy(Vec3(10.0f, 2.0f, 30.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            enemyMgr->InstEnemy(Vec3(-10.0f,2.0f, -20.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            enemyMgr->InstEnemy(Vec3(20.0f, 2.0f, 10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            enemyMgr->InstEnemy<EnemyZakoLong>(Vec3(-20.0f,2.0f, -10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            enemyMgr->InstEnemy<EnemyZakoLong>(Vec3(30.0f, 2.0f, 30.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            enemyMgr->InstEnemy<EnemyZakoLong>(Vec3(-30.0f,2.0f, -5.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));

        }


        if (m_NextWaveFlag == true && m_waveNow == 2)
        {
            //ボス戦開始
            m_waveNow = 3;

            m_bossGauge->ClearBossGaugeUI(false);

            player->GetComponent<Transform>()->SetPosition(Vec3(0.0f, 2.0f, 195.0f));

            player->GetComponent<Transform>()->SetRotation(Vec3(0.0f, 5.0f, 0.0f));

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
            //Boss
            {
                Vec3(170.0f, 3.0f, 170.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, -3.0f, 260.0f)
            },
            //Wave2
            {
                Vec3(120.0f, 3.0f, 120.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, -3.0f, 0.0f)
            },
            //Wave1
            {
                Vec3(120.0f, 3.0f, 120.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, -3.0f, -260.0f)
            },


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
        AddTag(L"CameraObstacles");
        AddTag(L"Terrain");

        ptrDraw->SetDiffuse(Col4(0.7f, 0.8f, 0.9f, 0.0f));
        //ptrDraw->SetDiffuse(Col4(0.8f, 0.9f, 1.0f, 0.0f));
        //ptrDraw->SetTextureResource(L"Wall");

    }


    //壁作成
    void WaveStage::CreateWall()
    {
        vector < vector<Vec3> > vec =
        {
            //Boss
            //右
            {
                Vec3(41.0f, 4.0f, 160.5f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(77.5f, 19.0f, 260.0f)
            },
            //左
            {
                Vec3(41.0f, 4.0f, 160.5f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(-77.5f, 19.0f, 260.0f)
            },
            //前
            {
                Vec3(41.0f, 4.0f, 160.5f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(0.0f, 19.0f, 336.0f)
            },
            //後
            {
                Vec3(41.0f, 4.0f, 160.5f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(0.0f, 19.0f, 183.0f)
            },
            //Wave2
            //右
            {
                Vec3(41.0f, 4.0f, 110.5f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(57.5f, 19.0f, 0.0f)
            },
            //左
            {
                Vec3(41.0f, 4.0f, 110.5f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(-57.5f, 19.0f, 0.0f)
            },
            //前
            {
                Vec3(41.0f, 4.0f, 115.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(0.0f, 19.0f, 56.0f)
            },
            //後
            {
                Vec3(41.0f, 4.0f, 115.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(0.0f, 19.0f, -56.0f)
            },
            //Wave1
            //右
            {
                Vec3(41.0f, 4.0f, 110.5f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(57.0f, 19.0f, -260.0f)
            },
            //左
            {
                Vec3(41.0f, 4.0f, 110.5f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(-57.0f, 19.0f, -260.0f)
            },
            //前
            {
                Vec3(41.0f, 4.0f, 110.5f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(0.0f, 19.0f, -203.0f)
            },
             //後
            {
                Vec3(41.0f, 4.0f, 110.5f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(0.0f, 19.0f, -317.0f)
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
        AddTag(L"Terrain");

    }


    //天井作成
    void WaveStage::CreateCeiling()
    {
        vector < vector<Vec3> > vec =
        {
            //Boss
            {
                Vec3(160.0f, 1.0f, 160.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, 40.0f, 260.0f)
            },
            //Wave2
            {
                Vec3(110.0f, 1.0f, 110.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, 40.0f, 0.0f)
            },
            //Wave1
            {
                Vec3(110.0f, 1.0f, 110.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, 40.0f, -260.0f)
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

        auto ptrDraw = AddComponent<BcPNTStaticDraw>();
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
