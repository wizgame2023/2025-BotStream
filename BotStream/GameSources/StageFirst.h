/*!
@file StageFirst.h
@brief Derived WaveStage
*/

#include "stdafx.h"
#include "WaveStageBase.h"

namespace basecross {
    class Door;
    class RT_MovieController;

    class StageFirst : public WaveStageBase {
    protected:

        void WaveInitialize() override;
        bool ConsiderGoToNextWave() override;
        bool ConsiderGameClear() override;
        bool ConsiderBossCheck();
        bool ConsiderGameOver() override;

        // ムービー用コントローラー
        shared_ptr<RT_MovieController> m_movieController;

        // ドアの実体を入れる配列2つで１組
        vector<shared_ptr<Door>> m_doors;
        // 次のフェーズに行く用の黒いやつの配列
        vector<shared_ptr<DarkObj>> m_darkObjs;

        //ゲームクリアフラグ
        bool m_gameClearFlag = false;

    public:
        // 構築と破棄
        StageFirst() : WaveStageBase() {}
        virtual ~StageFirst() {}
        // 初期化
        virtual void OnCreate()override;
        virtual void OnUpdate()override;

        // ドアを動かす処理
        void DoorsMove(int group);

        // 暗闇の次Waveを有効化させる処理
        void ActivateDark(int ActivateNum);

        // 暗転処理を開始させる処理
        void FadeOutStart();

        void UpdateGamePhase()override;

        //ゲームクリアフラグを渡す処理
        void SetGameClear(bool onOff);
    };

}