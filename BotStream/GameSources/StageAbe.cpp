/*!
@file StageAbe.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------
	void StageAbe::CreateViewLight() {
		const Vec3 eye(10.0f, 60.0f, -5.0f);
		//const Vec3 eye(0.0f, 1.0f, -5.0f);		
		const Vec3 at(10.0f);
        //const Vec3 at(0.0f);
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

    void StageAbe::OnCreate()
    {
        try {
            //ビューとライトの作成
            CreateViewLight();

            CreateFloor();
            CreateWall();
            CreateCeiling();
            CreateDoor();
            CreateTestEnemy();
        }
        catch (...) {
            throw;
        }
    }

    void StageAbe::OnUpdate()
    {
    }


    void StageAbe::CreateFloor()
    {
        vector < vector<Vec3> > vec =
        {
            {
                Vec3(10.0f, 0.1f, 10.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, 0.0f, 0.0f)
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

    void StageAbe::CreateWall()
    {
        vector < vector<Vec3> > vec =
        {
            {
                Vec3(5.0f, 0.1f, 10.0f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(5.0f, 2.4f, 0.0f)
            },
            {
                Vec3(5.0f, 0.1f, 10.0f),
                Vec3(0.0f, 0.0f, 1.57f),
                Vec3(-5.0f, 2.4f, 0.0f)
            },
            {
                Vec3(5.0f, 0.1f, 10.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(0.0f, 2.4f, 5.0f)
            },
            {
                Vec3(5.0f, 0.1f, 10.0f),
                Vec3(0.0f, 1.57f, 1.57f),
                Vec3(0.0f, 2.4f, -5.0f)
            }



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

        auto shadowPtr = AddComponent<Shadowmap>();
        shadowPtr->SetMeshResource(L"DEFAULT_CUBE");
        auto ptrDraw = AddComponent<BcPNTStaticDraw>();
        ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
        ptrDraw->SetFogEnabled(true);
        ptrDraw->SetOwnShadowActive(true);
    }


    void StageAbe::CreateCeiling()
    {
        vector < vector<Vec3> > vec =
        {
            {
                Vec3(10.0f, 0.1f, 10.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, 4.9f, 0.0f)
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


    void StageAbe::CreateDoor()
    {
        vector < vector<Vec3> > vec =
        {
            {
                Vec3(2.5f, 0.05f, 0.5f),
                Vec3(0.0f, -0.6f, 1.57f),
                Vec3(0.5f, 0.0f, 4.7f)
            },
            {
                Vec3(2.5f, 0.05f, 0.5f),
                Vec3(0.0f, 0.6f, 1.57f),
                Vec3(-0.5f, 0.0f, 4.7f)
            },
            {
                Vec3(2.5f, 0.05f, 0.5f),
                Vec3(0.0f, -0.6f, 1.57f),
                Vec3(-0.5f, 2.4f, -4.7f)
            },
            {
                Vec3(2.5f, 0.05f, 0.5f),
                Vec3(0.0f, 0.6f, 1.57f),
                Vec3(0.5f, 2.4f, -4.7f)
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

        auto ptrColl = AddComponent<CollisionObb>();
        ptrColl->SetFixed(true);

        auto shadowPtr = AddComponent<Shadowmap>();
        shadowPtr->SetMeshResource(L"DEFAULT_CUBE");
        auto ptrDraw = AddComponent<BcPNTStaticDraw>();
        ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
        ptrDraw->SetFogEnabled(true);
        ptrDraw->SetOwnShadowActive(true);
    }


    void StageAbe::CreateTestEnemy()
    {
        vector < vector<Vec3> > vec =
        {
            {
                Vec3(0.5f, 0.5f, 0.5f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, 0.3f, 0.0f)
            }
        };

        for (auto v : vec)
        {
            AddGameObject<TestEnemy>(v[0], v[1], v[2]);
        }
    }


    TestEnemy::TestEnemy(const shared_ptr<Stage>& StagePtr,
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
    TestEnemy::~TestEnemy() {}

    void TestEnemy::OnCreate()
    {
        auto ptrTransform = GetComponent<Transform>();
        ptrTransform->SetScale(m_Scale);
        ptrTransform->SetRotation(m_Rotation);
        ptrTransform->SetPosition(m_Position);

        auto ptrColl = AddComponent<CollisionSphere>();
        ptrColl->SetFixed(true);

        auto shadowPtr = AddComponent<Shadowmap>();
        shadowPtr->SetMeshResource(L"DEFAULT_SPHERE");
        auto ptrDraw = AddComponent<BcPNTStaticDraw>();
        ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
        ptrDraw->SetFogEnabled(true);
        ptrDraw->SetOwnShadowActive(true);

        AddTag(L"Enemy");
    }

    void TestEnemy::OnUpdate()
    {
        auto& app = App::GetApp();
        auto scene = app->GetScene<Scene>();
        auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
        int m_HP = 1;

        if (KeyState.m_bPressedKeyTbl[VK_SPACE])
        {
            m_HP -= 1;
        }

        if (m_HP == 0)
        {
            scene->PostEvent(1.0f, GetThis<ObjectInterface>(), app->GetScene<Scene>(), L"ToGameStage");
        }


    }

}
//end basecross
