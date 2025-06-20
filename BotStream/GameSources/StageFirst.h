/*!
@file StageFirst.h
@brief Derived WaveStage
*/

#include "stdafx.h"
#include "WaveStageBase.h"

namespace basecross {
    class Door;

    class StageFirst : public WaveStageBase {
    protected:

        void WaveInitialize() override;
        bool ConsiderGoToNextWave() override;
        bool ConsiderGameClear() override;
        bool ConsiderGameOver() override;

        // ドアの実体を入れる配列2つで１組
        vector<shared_ptr<Door>> m_doors;

        // 次のフェーズに行く用の黒いやつの配列
        vector<shared_ptr<DarkObj>> m_darkObjs;

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
    };

}