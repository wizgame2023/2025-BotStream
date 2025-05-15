/*!
@file WaveStage.cpp
@brief �Q�[���X�e�[�W����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

    //--------------------------------------------------------------------------------------
    //	�Q�[���X�e�[�W�N���X����
    //--------------------------------------------------------------------------------------
    void WaveStage::CreateViewLight() {
        //�S�̊m�F�J����
        const Vec3 eye(0.0f, 350.0f, -20.0f);
        //Wave1�m�F�J����
        //const Vec3 eye(0.0f, 1.0f, -25.5f);
        //Wave2�m�F�J����
        //const Vec3 eye(0.0f, 1.0f, -5.3f);
        //Boss�m�F�J����
        //const Vec3 eye(0.0f, 1.0f, 24.5f);

        const Vec3 at(0.0f, 0.0f, 0.0f);
        auto PtrView = CreateView<SingleView>();
        //�r���[�̃J�����̐ݒ�
        auto PtrCamera = ObjectFactory::Create<Camera>();
        PtrView->SetCamera(PtrCamera);
        PtrCamera->SetEye(eye);
        PtrCamera->SetAt(at);
        //�}���`���C�g�̍쐬
        auto PtrMultiLight = CreateLight<MultiLight>();
        //�f�t�H���g�̃��C�e�B���O���w��
        PtrMultiLight->SetDefaultLighting();
    }

    void WaveStage::OnCreate()
    {
        //�r���[�ƃ��C�g�̍쐬
        CreateViewLight();

        CreateFloor();
        CreateWall();
        CreateCeiling();

        m_waveNow = 1;//wave�P�J�n���Ă�

        shared_ptr<FadeoutSprite> fadeout;
        fadeout = AddGameObject<FadeoutSprite>(L"Fadeout");
        SetSharedGameObject(L"Fadeout", fadeout);
        fadeout->SetDrawLayer(3);

        CreateSharedObjectGroup(L"Actor");

        auto player = AddGameObject<Player>(Vec3(0.0f, 0.0f, -330.0f), Vec3(0.0f, 5.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
        SetSharedGameObject(L"Player", player);

        auto enemyMgr = AddGameObject<EnemyManager>();
        SetSharedGameObject(L"EnemyManager", enemyMgr);

        auto boss = AddGameObject<BossFirst>(Vec3(0.0f, 2.0f, 250.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
        SetSharedGameObject(L"Boss", boss);
        //enemyMgr->InstBoss(dynamic_pointer_cast<EnemyBase>(boss));

        auto playerUI = AddGameObject<PlayerGaugeUI>(100);
        SetSharedGameObject(L"PlayerUI", playerUI);

        //wave1�G
        enemyMgr->InstEnemy(Vec3(0.0f, 2.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));
        enemyMgr->InstEnemy(Vec3(10.0f, 2.0f, -255.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));
        enemyMgr->InstEnemy(Vec3(-10.0f,2.0f, -235.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));
        enemyMgr->InstEnemy(Vec3(20.0f, 2.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));
        enemyMgr->InstEnemy(Vec3(-20.0f,2.0f, -245.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));
        enemyMgr->InstEnemy(Vec3(30.0f, 2.0f, -225.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));
        enemyMgr->InstEnemy(Vec3(-30.0f,2.0f, -225.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));
        enemyMgr->InstEnemy(Vec3(40.0f, 2.0f, -255.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));
        enemyMgr->InstEnemy(Vec3(-40.0f,2.0f, -275.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));
        enemyMgr->InstEnemy(Vec3(50.0f, 2.0f, -285.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));
        enemyMgr->InstEnemy(Vec3(-50.0f,2.0f, -215.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));

        auto cameraManager = AddGameObject<CameraManager>();
        SetSharedGameObject(L"CameraManager", cameraManager);

        auto ptrSoundManager = AddGameObject<SoundManager>();
        SetSharedGameObject(L"SoundManager", ptrSoundManager);
        GetSharedGameObject<SoundManager>(L"SoundManager")->PlayBGM(3);
        GetSharedGameObject<SoundManager>(L"SoundManager")->PlaySE(13);

        auto colController = AddGameObject<StageCollisionController>();
        colController->SetCollisionSwhich(true);

        //�|�[�Y��������
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

            player->GetComponent<Transform>()->SetPosition(Vec3(0.0f, 0.0f, -75.0f));
            player->GetComponent<Transform>()->SetRotation(Vec3(0.0f, 5.0f, 0.0f));

            m_IsFadeInFlag = true;
            m_NextWaveFlag = false;

            //�E�F�[�u�Q�G
            enemyMgr->InstEnemy(Vec3(0.0f, 2.0f, 0.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));
            enemyMgr->InstEnemy(Vec3(10.0f, 2.0f, 30.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));
            enemyMgr->InstEnemy(Vec3(-10.0f,2.0f, -20.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));
            enemyMgr->InstEnemy(Vec3(20.0f, 2.0f, 10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));
            enemyMgr->InstEnemy(Vec3(-20.0f,2.0f, -10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));
            enemyMgr->InstEnemy(Vec3(30.0f, 2.0f, 30.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));
            enemyMgr->InstEnemy(Vec3(-30.0f,2.0f, -5.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));
            enemyMgr->InstEnemy(Vec3(40.0f, 2.0f, 0.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));
            enemyMgr->InstEnemy(Vec3(-40.0f,2.0f, 5.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));
            enemyMgr->InstEnemy(Vec3(50.0f, 2.0f, 10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));
            enemyMgr->InstEnemy(Vec3(-50.0f,2.0f, 20.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));

            enemyMgr->InstEnemy(Vec3(0.0f, 2.0f, 0.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));
            enemyMgr->InstEnemy(Vec3(10.0f, 2.0f, 30.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));
            enemyMgr->InstEnemy(Vec3(-10.0f,2.0f, -20.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));
            enemyMgr->InstEnemy(Vec3(20.0f, 2.0f, 10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));
            enemyMgr->InstEnemy(Vec3(-20.0f,2.0f, -10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));
            enemyMgr->InstEnemy(Vec3(30.0f, 2.0f, 30.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));
            enemyMgr->InstEnemy(Vec3(-30.0f,2.0f, -5.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));
            enemyMgr->InstEnemy(Vec3(40.0f, 2.0f, 0.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));
            enemyMgr->InstEnemy(Vec3(-40.0f,2.0f, 5.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));
            enemyMgr->InstEnemy(Vec3(50.0f, 2.0f, 10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));
            enemyMgr->InstEnemy(Vec3(-50.0f,2.0f, 20.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(4.0f, 4.0f, 4.0f));

        }


        if (m_NextWaveFlag == true && m_waveNow == 2)
        {
            //�{�X��J�n
            m_waveNow = 3;

            player->GetComponent<Transform>()->SetPosition(Vec3(0.0f, 0.0f, 165.0f));
            player->GetComponent<Transform>()->SetRotation(Vec3(0.0f, 5.0f, 0.0f));

            m_IsFadeInFlag = true;
            m_NextWaveFlag = false;

            EnemyNum = 1;

            enemyMgr->InstBoss(dynamic_pointer_cast<EnemyBase>(boss));


            GetSharedGameObject<SoundManager>(L"SoundManager")->StopBGM();
            GetSharedGameObject<SoundManager>(L"SoundManager")->PlayBGM(4);

        }

        if (m_waveNow == 3)
        {
            auto boss = enemyMgr->GetBoss();
            m_bossCurrentHP = boss->GetHPCurrent();//Boss��HP�擾
        }

        if (m_waveNow == 3 && m_bossCurrentHP <= 0)
        {
            GetSharedGameObject<SoundManager>(L"SoundManager")->StopBGM();
            scene->PostEvent(3.0f, GetThis<ObjectInterface>(),app->GetScene<Scene>(), L"ToGameClear");
            m_waveNow = 4;//�E�F�[�u�I��
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

    void WaveStage::SetNextWaveFlag(int setNextWaveFlag)
    {
        m_NextWaveFlag = setNextWaveFlag;
    }

    bool WaveStage::GetNextWaveFlag()
    {
        return m_NextWaveFlag;
    }


    //���쐬
    void WaveStage::CreateFloor()
    {
        vector < vector<Vec3> > vec =
        {
            //Boss
            {
                Vec3(212.0f, 3.0f, 212.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, -3.0f, 260.0f)
            },
            //Wave2
            {
                Vec3(172.0f, 3.0f, 172.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, -3.0f, 0.0f)
            },
            //Wave1
            {
                Vec3(172.0f, 3.0f, 172.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, -3.0f, -260.0f)
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
        AddTag(L"CameraObstacles");
        AddTag(L"Terrain");

        ptrDraw->SetDiffuse(Col4(0.7f, 0.8f, 0.9f, 0.0f));
        //ptrDraw->SetDiffuse(Col4(0.8f, 0.9f, 1.0f, 0.0f));
        //ptrDraw->SetTextureResource(L"Wall");

    }


    //�Ǎ쐬
    void WaveStage::CreateWall()
    {
        vector < vector<Vec3> > vec =
        {
            //Boss
            //�E
            {
                Vec3(41.0f, 1.0f, 210.5f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(105.5f, 19.0f, 260.0f)
            },
            //��
            {
                Vec3(41.0f, 1.0f, 210.5f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(-105.5f, 19.0f, 260.0f)
            },
            //�O
            {
                Vec3(41.0f, 1.0f, 210.5f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(0.0f, 19.0f, 365.5f)
            },
            //��
            {
                Vec3(41.0f, 1.0f, 210.5f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(0.0f, 19.0f, 154.5f)
            },
            //Wave2
            //�E
            {
                Vec3(41.0f, 1.0f, 170.5f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(85.5f, 19.0f, 0.0f)
            },
            //��
            {
                Vec3(41.0f, 1.0f, 170.5f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(-85.5f, 19.0f, 0.0f)
            },
            //�O
            {
                Vec3(41.0f, 1.0f, 170.5f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(0.0f, 19.0f, 85.5f)
            },
            //��
            {
                Vec3(41.0f, 1.0f, 170.5f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(0.0f, 19.0f, -85.5f)
            },
            //Wave1
            //�E
            {
                Vec3(41.0f, 1.0f, 170.5f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(85.5f, 19.0f, -260.0f)
            },
            //��
            {
                Vec3(41.0f, 1.0f, 170.5f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(-85.5f, 19.0f, -260.0f)
            },
            //�O
            {
                Vec3(41.0f, 1.0f, 170.5f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(0.0f, 19.0f, -174.5f)
            },
             //��
            {
                Vec3(41.0f, 1.0f, 170.5f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(0.0f, 19.0f, -345.5f)
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


    //�V��쐬
    void WaveStage::CreateCeiling()
    {
        vector < vector<Vec3> > vec =
        {
            //Boss
            {
                Vec3(210.0f, 1.0f, 210.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, 40.0f, 260.0f)
            },
            //Wave2
            {
                Vec3(170.0f, 1.0f, 170.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, 40.0f, 0.0f)
            },
            //Wave1
            {
                Vec3(170.0f, 1.0f, 170.0f),
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


    //�o�����쐬
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
