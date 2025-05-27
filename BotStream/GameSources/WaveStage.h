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
        int m_bossCurrentHP = 99999;//BOSSの現在のHP

        float TimeOfGameClear = 0.0f;//ゲームクリアするための時間計測

        bool m_NextWave = false;

        bool m_IsFadeOutFlag = false;
        bool m_IsFadeInFlag = false;
        bool m_BlackFlag = false;
        bool m_NextWaveFlag = false;

    public:
        //構築と破棄
        WaveStage() :Stage() {}
        virtual ~WaveStage() {}
        //初期化
        virtual void OnCreate()override;
        virtual void OnUpdate()override;
        virtual void OnDraw()override;
        virtual void OnDestroy()override;

        bool GetNextWaveFlag();
        void SetNextWaveFlag(int setNextWaveFlag);


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

    class FadeoutSprite : public GameObject {
        vector<VertexPositionColorTexture> m_Vertices;
        shared_ptr<PCTSpriteDraw> m_drawComp;
        wstring m_ResKey;
        bool m_IsFadeOutFlag = false;
        bool m_IsFadeInFlag = false;
        float m_Color;
        bool m_BlackFlag = false;

    public:
        FadeoutSprite(const shared_ptr<Stage>& stage, const wstring& ResKey);
        virtual ~FadeoutSprite();
        virtual void OnCreate() override;
        virtual void OnUpdate() override;

        bool GetBlackFlag();
        void SetBlackFlag(int setBlackFlag);
        bool GetFadeOutFlag();
        void SetFadeOutFlag(int setFadeOutFlag);
        bool GetFadeInFlag();
        void SetFadeInFlag(int setFadeInFlag);

    };

}
//end basecross
