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
            //CreateCeiling();
            CreateDoor();

            auto player = AddGameObject<Player>(Vec3(0.0f, 0.0f, -90.0f), Vec3(0.0f, 5.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
            SetSharedGameObject(L"Player", player);

            auto enemyMgr = AddGameObject<EnemyManager>();
            SetSharedGameObject(L"EnemyManager", enemyMgr);

            enemyMgr->InstEnemy(Vec3(5.0f, 0.0f, -80.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
            enemyMgr->InstEnemy(Vec3(-5.0f, 0.0f, -80.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));

            enemyMgr->InstEnemy(Vec3(5.0f, 0.0f, 10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
            enemyMgr->InstEnemy(Vec3(0.0f, 0.0f, 10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
            enemyMgr->InstEnemy(Vec3(-5.0f, 0.0f, 10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));

            enemyMgr->InstEnemy(Vec3(0.0f, 0.0f, 100.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(3.0f, 3.0f, 3.0f));


            AddGameObject<CameraManager>();

            auto ptrSoundManager = AddGameObject<SoundManager>();
            SetSharedGameObject(L"SoundManager", ptrSoundManager);
            GetSharedGameObject<SoundManager>(L"SoundManager")->PlayBGM(3);

            
    }


    void WaveStage::OnUpdate()
    {
        auto& app = App::GetApp();
        auto scene = app->GetScene<Scene>();
        auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
        auto pad = App::GetApp()->GetInputDevice().GetControlerVec()[0];
        auto player = GetSharedGameObject<Player>(L"Player");
        auto enemyMgr = AddGameObject<EnemyManager>();

        if (pad.wPressedButtons & XINPUT_GAMEPAD_X)
        {
            GetSharedGameObject<SoundManager>(L"SoundManager")->PlaySE(5);
        }
        if (pad.wPressedButtons & XINPUT_GAMEPAD_A)
        {
            GetSharedGameObject<SoundManager>(L"SoundManager")->PlaySE(3);
        }

    }


    //床作成
    void WaveStage::CreateFloor()
    {
        vector < vector<Vec3> > vec =
        {
            //Boss
            {
                Vec3(70.0f, 0.1f, 70.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, 0.0f, 90.0f)
            },
            //Wave2
            {
                Vec3(70.0f, 0.1f, 70.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, 0.0f, 0.0f)
            },
            //Wave1
            {
                Vec3(70.0f, 0.1f, 70.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, 0.0f, -90.0f)
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

        auto shadowPtr = AddComponent<Shadowmap>();
        shadowPtr->SetMeshResource(L"DEFAULT_CUBE");
        auto ptrDraw = AddComponent<BcPNTStaticDraw>();
        ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
        ptrDraw->SetFogEnabled(true);
        ptrDraw->SetOwnShadowActive(true);
    }

    void WaveStage::CreateWall()
    {
        vector < vector<Vec3> > vec =
        {
            //Boss
            //右
            {
                Vec3(15.0f, 0.1f, 70.0f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(35.0f, 7.4f, 90.0f)
            },
            //左
            {
                Vec3(15.0f, 0.1f, 70.0f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(-35.0f, 7.4f, 90.0f)
            },
            //前
            {
                Vec3(15.0f, 0.1f, 70.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(0.0f, 7.4f, 125.0f)
            },
            //後
            {
                Vec3(15.0f, 0.1f, 70.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(0.0f, 7.4f, 55.0f)
            },
            //Wave2
            //右
            {
                Vec3(15.0f, 0.1f, 70.0f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(35.0f, 7.4f, 0.0f)
            },
            //左
            {
                Vec3(15.0f, 0.1f, 70.0f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(-35.0f, 7.4f, 0.0f)
            },
            //前
            {
                Vec3(15.0f, 0.1f, 70.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(0.0f, 7.4f, 35.0f)
            },
            //後
            {
                Vec3(15.0f, 0.1f, 70.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(0.0f, 7.4f, -35.0f)
            },
            //Wave1
            //右
            {
                Vec3(15.0f, 0.1f, 70.0f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(35.0f, 7.4f, -90.0f)
            },
            //左
            {
                Vec3(15.0f, 0.1f, 70.0f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(-35.0f, 7.4f, -90.0f)
            },
            //前
            {
                Vec3(15.0f, 0.1f, 70.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(0.0f, 7.4f, -55.0f)
            },
            //後
            {
                Vec3(15.0f, 0.1f, 70.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(0.0f, 7.4f, -125.0f)
            }
        };
        for (auto v : vec)
        {
            AddGameObject<Wall>(v[0], v[1], v[2]);
        }
    }

    //壁作成
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

        auto shadowPtr = AddComponent<Shadowmap>();
        shadowPtr->SetMeshResource(L"DEFAULT_CUBE");
        auto ptrDraw = AddComponent<BcPNTStaticDraw>();
        ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
        ptrDraw->SetFogEnabled(true);
        ptrDraw->SetOwnShadowActive(true);

        ptrColl->SetDrawActive(true);
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
                Vec3(0.0f, 15.0f, 90.0f)
            },
            //Wave2
            {
                Vec3(70.0f, 0.1f, 70.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, 15.0f, 0.0f)
            },
            //Wave1
            {
                Vec3(70.0f, 0.1f, 70.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, 15.0f, -90.0f)
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

        auto shadowPtr = AddComponent<Shadowmap>();
        shadowPtr->SetMeshResource(L"DEFAULT_CUBE");
        auto ptrDraw = AddComponent<BcPNTStaticDraw>();
        ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
        ptrDraw->SetFogEnabled(true);
        ptrDraw->SetOwnShadowActive(true);
    }


    void WaveStage::CreateDoor()
    {
        vector < vector<Vec3> > vec =
        {
            //Boss
            //入口右
            {
                Vec3(6.0f, 0.1f, 3.0f),
                Vec3(0.0f, -0.6f, 1.57f),
                Vec3(-3.5f, 3.0f, 56.4f)
            },
            //入口左
            {
                Vec3(6.0f, 0.1f, 3.0f),
                Vec3(0.0f, 0.6f, 1.57f),
                Vec3(3.5f, 3.0f, 56.4f)
            },
            //Wave2
            //出口右
            {
                Vec3(6.0f, 0.1f, 3.0f),
                Vec3(0.0f, -0.6f, 1.57f),
                Vec3(3.5f, 3.0f, 33.7f)
            },
            //出口左
            {
                Vec3(6.0f, 0.1f, 3.0f),
                Vec3(0.0f, 0.6f, 1.57f),
                Vec3(-3.5f, 3.0f, 33.7f)
            },
            //入口右
            {
                Vec3(6.0f, 0.1f, 3.0f),
                Vec3(0.0f, -0.6f, 1.57f),
                Vec3(-3.5f, 3.0f, -33.7f)
            },
            //入口左
            {
                Vec3(6.0f, 0.1f, 3.0f),
                Vec3(0.0f, 0.6f, 1.57f),
                Vec3(3.5f, 3.0f, -33.7f)
            },
            //Wave1
            //入口右
            {
                Vec3(6.0f, 0.1f, 3.0f),
                Vec3(0.0f, -0.6f, 1.57f),
                Vec3(-3.5f, 3.0f, -123.7f)
            },
             //入口左
            {
                Vec3(6.0f, 0.1f, 3.0f),
                Vec3(0.0f, 0.6f, 1.57f),
                Vec3(3.5f, 3.0f, -123.7f)
            },
            //出口右
            {
                Vec3(6.0f, 0.1f, 3.0f),
                Vec3(0.0f, -0.6f, 1.57f),
                Vec3(3.5f, 3.0f, -56.3f)
            },
            //出口左
            {
                Vec3(6.0f, 0.1f, 3.0f),
                Vec3(0.0f, 0.6f, 1.57f),
                Vec3(-3.5f, 3.0f, -56.3f)
            }
        };
        for (auto v : vec)
        {
            AddGameObject<Door>(v[0], v[1], v[2]);
        }
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

        auto shadowPtr = AddComponent<Shadowmap>();
        shadowPtr->SetMeshResource(L"DEFAULT_CUBE");
        auto ptrDraw = AddComponent<BcPNTStaticDraw>();
        ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
        ptrDraw->SetFogEnabled(true);
        ptrDraw->SetOwnShadowActive(true);
    }

}
//end basecross
