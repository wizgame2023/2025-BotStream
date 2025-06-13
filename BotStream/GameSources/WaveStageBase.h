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

        //Waveクリア、フェードアウト開始
        bool m_IsFadeOutFlag = false;
        //Wave切り替え終了
        bool m_IsFadeInFlag = false;
        //暗転中
        bool m_BlackFlag = false;
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

}