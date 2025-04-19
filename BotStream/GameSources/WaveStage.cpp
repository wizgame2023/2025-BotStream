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
            //CreateDoor();

            CreateSharedObjectGroup(L"Actor");

            auto player = AddGameObject<Player>(Vec3(0.0f, 0.0f, -90.0f), Vec3(0.0f, 5.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
            SetSharedGameObject(L"Player", player);

            auto enemyMgr = AddGameObject<EnemyManager>();
            SetSharedGameObject(L"EnemyManager", enemyMgr);

            auto door = AddGameObject<Door>(Vec3(6.5f, 0.1f, 4.0f), Vec3(0.0f, 1.57f, 1.57f), Vec3(-2.0f, 1.75f, -55.0f));
            SetSharedGameObject(L"Door", door);
            auto door2 = AddGameObject<Door>(Vec3(6.5f, 0.1f, 4.0f), Vec3(0.0f, 1.57f, 1.57f), Vec3(2.0f, 1.75f, -55.0f));
            SetSharedGameObject(L"Door2", door2);

            enemyMgr->InstEnemy(Vec3(5.0f, -1.5f, -80.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(0.3f, 0.3f, 0.3f));
            enemyMgr->InstEnemy(Vec3(-5.0f, -1.5f, -80.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(0.3f, 0.3f, 0.3f));

            //enemyMgr->InstEnemy(Vec3(10.0f, -1.5f, 10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(0.3f, 0.3f, 0.3f));
            //enemyMgr->InstEnemy(Vec3(0.0f, -1.5f, 10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(0.3f, 0.3f, 0.3f));
            //enemyMgr->InstEnemy(Vec3(-10.0f, -1.5f, 10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(0.3f, 0.3f, 0.3f));

            //enemyMgr->InstEnemy(Vec3(0.0f, -1.5f, 100.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(1.5f, 1.5f, 1.5f));


            AddGameObject<CameraManager>();

            auto ptrSoundManager = AddGameObject<SoundManager>();
            SetSharedGameObject(L"SoundManager", ptrSoundManager);
            GetSharedGameObject<SoundManager>(L"SoundManager")->PlayBGM(3);
            GetSharedGameObject<SoundManager>(L"SoundManager")->PlaySE(13);


    }


    void WaveStage::OnUpdate()
    {
        auto& app = App::GetApp();
        auto scene = app->GetScene<Scene>();
        auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
        auto pad = App::GetApp()->GetInputDevice().GetControlerVec()[0];
        auto player = GetSharedGameObject<Player>(L"Player");
        auto enemyMgr = GetSharedGameObject<EnemyManager>(L"EnemyManager");
        auto door = GetSharedGameObject<Door>(L"Door");
        auto door2 = GetSharedGameObject<Door>(L"Door2");

        auto EnemyVec = enemyMgr->GetEnemyVec(true);
        int EnemyNum = EnemyVec.size();

        EffectManager::Instance().InterfaceUpdate();


        if (pad.wPressedButtons & XINPUT_GAMEPAD_X)
        {
            GetSharedGameObject<SoundManager>(L"SoundManager")->PlaySE(5);
            //RemoveGameObject<EnemyBase>(GetSharedGameObject<EnemyBase>(L"Enemy"));
            door->GetComponent<Transform>()->SetPosition(-6.0f, 1.75f, -55.0f);
            door2->GetComponent<Transform>()->SetPosition(6.0f, 1.75f, -55.0f);

        }
        if (pad.wPressedButtons & XINPUT_GAMEPAD_A)
        {
            GetSharedGameObject<SoundManager>(L"SoundManager")->PlaySE(3);
        }
        if (pad.wPressedButtons & XINPUT_GAMEPAD_B)
        {
            player->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, -30.0f);
        }

        if (EnemyNum == 0)
        {
            enemyMgr->InstEnemy(Vec3(10.0f, -1.5f, 10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(0.3f, 0.3f, 0.3f));
            enemyMgr->InstEnemy(Vec3(0.0f, -1.5f, 10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(0.3f, 0.3f, 0.3f));
            enemyMgr->InstEnemy(Vec3(-10.0f, -1.5f, 10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(0.3f, 0.3f, 0.3f));

            player->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, -30.0f);
        }
    }

    void WaveStage::OnDraw()
    {
        EffectManager::Instance().InterfaceDraw();
    }


    //���쐬
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
            //�ʘH
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
            //�ʘH
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

    }


    //�Ǎ쐬
    void WaveStage::CreateWall()
    {
        vector < vector<Vec3> > vec =
        {
            //Boss
            //�E
            {
                Vec3(30.0f, 0.1f, 70.0f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(35.0f, 13.5f, 90.0f)
            },
            //��
            {
                Vec3(30.0f, 0.1f, 70.0f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(-35.0f, 13.5f, 90.0f)
            },
            //�O
            {
                Vec3(30.0f, 0.1f, 70.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(0.0f, 13.5f, 125.0f)
            },
            //�㍶
            {
                Vec3(30.0f, 0.1f, 31.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(-19.5f, 13.5f, 55.0f)
            },
            //��^��
            {
                Vec3(25.0f, 0.1f, 8.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(0.0f, 17.5f, 55.0f)
            },
            //��E
            {
                Vec3(30.0f, 0.1f, 31.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(19.5f, 13.5f, 55.0f)
            },
            //Wave2
            //�E
            {
                Vec3(30.0f, 0.1f, 70.0f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(35.0f, 13.5f, 0.0f)
            },
            //��
            {
                Vec3(30.0f, 0.1f, 70.0f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(-35.0f, 13.5f, 0.0f)
            },
            //�O��
            {
                Vec3(30.0f, 0.1f, 31.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(-19.5f, 13.5f, 35.0f)
            },
            //�O�^��
            {
                Vec3(25.0f, 0.1f, 8.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(0.0f, 17.5f, 35.0f)
            },
            //�O�E
            {
                Vec3(30.0f, 0.1f, 31.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(19.5f, 13.5f, 35.0f)
            },
            //�㍶
            {
                Vec3(30.0f, 0.1f, 31.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(-19.5f, 13.5f, -35.0f)
            },
            //��^��
            {
                Vec3(25.0f, 0.1f, 8.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(0.0f, 17.5f, -35.0f)
            },
            //��E
            {
                Vec3(30.0f, 0.1f, 31.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(19.5f, 13.5f, -35.0f)
            },
            //Wave1
            //�E
            {
                Vec3(30.0f, 0.1f, 70.0f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(35.0f, 13.5f, -90.0f)
            },
            //��
            {
                Vec3(30.0f, 0.1f, 70.0f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(-35.0f, 13.5f, -90.0f)
            },
            //�O��
            {
                Vec3(30.0f, 0.1f, 31.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(-19.5f, 13.5f, -55.0f)
            },
            //�O�^��
            {
                Vec3(25.0f, 0.1f, 8.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(0.0f, 17.5f, -55.0f)
            },
            //�O�E
            {
                Vec3(30.0f, 0.1f, 31.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(19.5f, 13.5f, -54.9f)
            },
            //�㍶
            {
                Vec3(30.0f, 0.1f, 31.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(-19.5f, 13.5f, -125.0f)
            },
            //��^��
            {
                Vec3(25.0f, 0.1f, 8.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(0.0f, 17.5f, -125.0f)
            },
             //��E
            {
                Vec3(30.0f, 0.1f, 31.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(19.5f, 13.5f, -125.0f)
            },
            //Wave1,2�ʘH
            //�E
            {
                Vec3(6.5f, 0.1f, 20.0f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(4.0f, 1.8f, -45.0f)
            },
            //��
            {
                Vec3(6.5f, 0.1f, 20.0f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(-4.0f, 1.8f, -45.0f)
            },
            //Wave2,Boss�ʘH
            //�E
            {
                Vec3(6.5f, 0.1f, 20.0f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(4.0f, 1.8f, 45.0f)
            },
            //��
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

    }


    //�V��쐬
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
            //�ʘH
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
            //�ʘH
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

    }


    //�o�����쐬
    void WaveStage::CreateDoor()
    {
        vector < vector<Vec3> > vec =
        {
            //Boss
            //�E
            {
                Vec3(6.5f, 0.1f, 4.0f),
                Vec3(0.0f, -1.58f, 1.57f),
                Vec3(-2.0f, 1.75f, 55.0f)
            },
            //��
            {
                Vec3(6.5f, 0.1f, 4.0f),
                Vec3(0.0f, 1.55f, 1.57f),
                Vec3(2.0f, 1.75f, 55.0f)
            },
            //Wave2
            //�O�E
            {
                Vec3(6.5f, 0.1f, 4.0f),
                Vec3(0.0f, -1.55f, 1.57f),
                Vec3(2.0f, 1.75f, 35.0f)
            },
            //�O��
            {
                Vec3(6.5f, 0.1f, 4.0f),
                Vec3(0.0f, 1.58f, 1.57f),
                Vec3(-2.0f, 1.75f, 35.0f)
            },
            //��E
            {
                Vec3(6.5f, 0.1f, 4.0f),
                Vec3(0.0f, -1.58f, 1.57f),
                Vec3(-2.0f, 1.75f, -35.0f)
            },
            //�㍶
            {
                Vec3(6.5f, 0.1f, 4.0f),
                Vec3(0.0f, 1.55f, 1.57f),
                Vec3(2.0f, 1.75f, -35.0f)
            },
            //Wave1
            //�O�E
            {
                Vec3(6.5f, 0.1f, 4.0f),
                Vec3(0.0f, 1.55f, 1.57f),
                Vec3(-2.0f, 1.75f, -125.0f)
            },
             //�O��
            {
                Vec3(6.5f, 0.1f, 4.0f),
                Vec3(0.0f, 1.58f, 1.57f),
                Vec3(2.0f, 1.75f, -125.0f)
            },
            //��E
            {
                Vec3(6.5f, 0.1f, 4.0f),
                Vec3(0.0f, 1.58f, 1.57f),
                Vec3(2.0f, 1.75f, -55.0f)
            },
            //�㍶
            {
                Vec3(6.5f, 0.1f, 4.0f),
                Vec3(0.0f, 1.55f, 1.57f),
                Vec3(-2.0f, 1.75f, -55.0f)
            }
        };
        for (auto v : vec)
        {
            AddGameObject<Door>(v[0], v[1], v[2]);
        }
    }

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

        //auto ptrColl = AddComponent<CollisionObb>();
        //ptrColl->SetFixed(true);

        auto ptrDraw = AddComponent<BcPNTStaticDraw>();
        ptrDraw->SetMeshResource(L"DEFAULT_CUBE");

        ptrDraw->SetDiffuse(Col4(0.5f, 0.6f, 0.7f, 0.0f));

        m_HPGauge = GetStage()->AddGameObject<BillBoard>(GetThis<GameObject>(), L"PLHP", 2, 8.0f);
        m_ArmorGauge = GetStage()->AddGameObject<BillBoard>(GetThis<GameObject>(), L"PLSP", 2, 7.5f);

        m_HPGauge->SetScale(Vec3(6.0f, 0.5f, 3.0f));
        m_ArmorGauge->SetScale(Vec3(6.0f, 0.3f, 3.0f));
    }

}
//end basecross
