/*!
@file StageFirst.h
@brief Derived WaveStage
*/

#include "stdafx.h"
#include "WaveStageBase.h"

namespace basecross {

    class StageFirst : public WaveStageBase {
    protected:

        void WaveInitialize() override;
        bool ConsiderGoToNextWave() override;
        bool ConsiderGameClear() override;
        bool ConsiderGameOver() override;

    public:
        //�\�z�Ɣj��
        StageFirst() : WaveStageBase() {}
        virtual ~StageFirst() {}
        //������
        virtual void OnCreate()override;
    };

}