/*!
@file WaveStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

    //--------------------------------------------------------------------------------------
    //	ゲームステージクラス
    //--------------------------------------------------------------------------------------

    class WaveStage : public Stage {
        //ビューの作成
        void CreateViewLight();
        void CreateFloor();
        void CreateWall();
        void CreateCeiling();
        void CreateDoor();

        int m_waveNow = 0;//現在のwave


    public:
        //構築と破棄
        WaveStage() :Stage() {}
        virtual ~WaveStage() {}
        //初期化
        virtual void OnCreate()override;
        virtual void OnUpdate()override;
        virtual void OnDraw()override;

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

        //shared_ptr<BillBoard> m_HPGauge;
        //shared_ptr<BillBoard> m_ArmorGauge;

    public:
        Door(const shared_ptr<Stage>& StagePtr,
            const Vec3& Scale,
            const Vec3& Rotation,
            const Vec3& Position
        );
        virtual ~Door();
        virtual void OnCreate() override;
    };

}
//end basecross
