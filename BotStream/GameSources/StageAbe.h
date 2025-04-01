/*!
@file StageAbe.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------

    class StageAbe : public Stage {
        //�r���[�̍쐬
        void CreateViewLight();
        void CreateFloor();
        void CreateWall();
        void CreateCeiling();
        void CreateDoor();
        void CreateTestEnemy();

    public:
        //�\�z�Ɣj��
        StageAbe() :Stage() {}
        virtual ~StageAbe() {}
        //������
        virtual void OnCreate()override;
        virtual void OnUpdate()override;


    };

    class Floor : public GameObject {
        Vec3 m_Scale;
        Vec3 m_Rotation;
        Vec3 m_Position;
    public:
        Floor(const shared_ptr<Stage>& StagePtr,
            const Vec3& Scale,
            const Vec3& Rotation,
            const Vec3& Position
        );
        virtual ~Floor();
        virtual void OnCreate() override;
    };

    class Wall : public GameObject {
        Vec3 m_Scale;
        Vec3 m_Rotation;
        Vec3 m_Position;
    public:
        Wall(const shared_ptr<Stage>& StagePtr,
            const Vec3& Scale,
            const Vec3& Rotation,
            const Vec3& Position
        );
        virtual ~Wall();
        virtual void OnCreate() override;
    };

    class Ceiling : public GameObject {
        Vec3 m_Scale;
        Vec3 m_Rotation;
        Vec3 m_Position;
    public:
        Ceiling(const shared_ptr<Stage>& StagePtr,
            const Vec3& Scale,
            const Vec3& Rotation,
            const Vec3& Position
        );
        virtual ~Ceiling();
        virtual void OnCreate() override;
    };

    class Door : public GameObject {
        Vec3 m_Scale;
        Vec3 m_Rotation;
        Vec3 m_Position;
    public:
        Door(const shared_ptr<Stage>& StagePtr,
            const Vec3& Scale,
            const Vec3& Rotation,
            const Vec3& Position
        );
        virtual ~Door();
        virtual void OnCreate() override;
    };

    class TestEnemy : public GameObject {
        Vec3 m_Scale;
        Vec3 m_Rotation;
        Vec3 m_Position;
    public:
        TestEnemy(const shared_ptr<Stage>& StagePtr,
            const Vec3& Scale,
            const Vec3& Rotation,
            const Vec3& Position
        );
        virtual ~TestEnemy();
        virtual void OnCreate() override;
        virtual void OnUpdate() override;

    };

}
//end basecross

