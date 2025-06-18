/*!
@file WaveStageBase.h
@brief ステージの基底クラス
*/

#pragma once
#include "stdafx.h"
#include "EnemyManager.h"

namespace basecross {
    class BossGaugeUI;
    class Player;
    class SoundManager;
    class FadeoutSprite;

    class WaveStageBase : public Stage {
    public:
        //ゲームの進行状況
        enum GamePhase {
            GPhase_None,
            GPhase_Start,
            GPhase_Playing,
            GPhase_CutScene,
            GPhase_Result,
            GPhase_GameOver
        };

    protected:
        //ビューの作成
        virtual void CreateViewLight();

        virtual void CreateFloor();
        virtual void CreateWall();
        virtual void CreateCeiling();

        virtual void CreatePlayer(Vec3 pos, Vec3 rot, Vec3 scale);

        //各オブジェクトへの参照用
        // シーン
        weak_ptr<Scene> m_scene;
        //サウンドマネージャー
        weak_ptr<SoundManager> m_sndMgr;
        //フェード用オブジェクト
        weak_ptr<FadeoutSprite> m_fadeout;
        // プレイヤー
        weak_ptr<Player> m_player;
        // 敵管理オブジェクト
        weak_ptr<EnemyManager> m_enemyMgr;
        // ボス
        weak_ptr<EnemyBase> m_boss;

        //ゲームの進行状況
        GamePhase m_gamePhase = GPhase_None;
        //現在のwave
        int m_waveCurrent = 1;
        //最大wave数
        int m_waveMax = 3;
        //BOSSの現在のHP
        int m_bossCurrentHP = 99999;
        //時間の早さ
        float m_deltaScale = 1.0f;
        //deltaScaleをデフォルトに戻すためのカウント
        float m_deltaScaleCount = 0.0f;
        //deltaScaleのデフォルト値(よっぽどのことがなけりゃ1)
        const float m_deltaScaleDefault = 1.0f;

        float TimeOfGameClear = 0.0f;//ゲームクリアするための時間計測

        //Wave切り替え
        bool m_nextWaveFlag = false;

        bool m_isPaused = false;

        shared_ptr<BossGaugeUI> m_bossGauge;

        //マネージャ、UIの類を生成する
        virtual void CreateManagerObjects();

        //Waveが次に移ったときに呼び出される
        virtual void WaveInitialize();

        //プレイヤーの位置を強制的に変える
        virtual void SetPlayerTransform(Vec3 pos, Vec3 rot);

        void ShowFPS() {
            ////デバック用
            //wstringstream wss(L"");
            //m_scene.lock() = App::GetApp()->GetScene<Scene>();
            //wss /* << L"デバッグ用文字列 "*/
            //    << L"\n FPS: " << App::GetApp()->App::GetApp()->GetStepTimer().GetFramesPerSecond() << endl;

            //m_scene.lock()->SetDebugString(wss.str());

        }

        //OnUpdateに置く
        void ResetDeltaScaleToDefault() {
            if (m_deltaScaleCount > 0) {
                m_deltaScaleCount -= App::GetApp()->GetElapsedTime();
                
                if (m_deltaScaleCount <= 0) {
                    m_deltaScale = m_deltaScaleDefault;
                }
            }
        }

        virtual bool ConsiderGoToNextWave();
        virtual bool ConsiderGameClear();
        virtual bool ConsiderGameOver();

    public:
        //構築と破棄
        WaveStageBase() : Stage() {}
        virtual ~WaveStageBase() {}
        //初期化
        virtual void OnCreate()override;
        virtual void OnUpdate()override;
        virtual void OnDraw()override;
        virtual void OnDestroy()override;

        bool GetNextWaveFlag();
        void SetNextWaveFlag(bool setNextWaveFlag);

        GamePhase GetCurrentGamePhase() {
            return m_gamePhase;
        }

        int GetNowWaveNum() {
            return m_waveCurrent;
        }

        float GetDeltaScale() {
            return m_deltaScale;
        }

        void SetDeltaScale(float deltascale) {
            m_deltaScale = deltascale;
        }

        //指定時間後にdeltaScaleをデフォルトの値に戻す
        void SetDeltaScaleCount(float counttime) {
            m_deltaScaleCount = counttime;
        }

        void SetActorPause(bool isPause);

        bool GetActorPause() {
            return m_isPaused;
        }

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

    // 床のテクスチャ用--------------------------------
    class Block : public MyGameObject
    {
    public:
        Block(const shared_ptr<Stage>& StagePtr,
            const Vec3& Scale,
            const Vec3& Position);
        virtual ~Block() {}

        virtual void OnCreate() override;

        static constexpr float BLOCK_XZ_SCALE = 10.0f;

    private:
        Vec3 m_pos;
        Vec3 m_scale;
    };
    // END --------------------------------------------

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

    // 壁のテクスチャが違うやつを生成--------------
    class Wall2 : public GameObject {
        Vec3 m_Scale;
        Vec3 m_Rotation;
        Vec3 m_Position;
    public:
        Wall2(const shared_ptr<Stage>& StagePtr,
            const Vec3& Scale,
            const Vec3& Rotation,
            const Vec3& Position
        );
        virtual ~Wall2();
        virtual void OnCreate() override;
    };
    // ---------------------------------------------

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

}