/*!
@file TutorialStage.h
@brief Derived WaveStage
*/

#include "stdafx.h"

namespace basecross {

    class TutorialStage : public WaveStageBase {
    protected:
        enum TutorialPhase {
            Tutorial_MoveAndCamera,
            Tutorial_Evade,
            Tutorial_KeepRunning,
            Tutorial_HitTheCombos,
            Tutorial_ShootFromAfar,
            Tutorial_KillAsYouLike

        };

        TutorialPhase m_tutorialPhase;
    public:
        //\’z‚Æ”jŠü
        TutorialStage() : WaveStageBase() {}
        virtual ~TutorialStage() {}
        //‰Šú‰»
        virtual void OnCreate()override;
    };

    class TutorialDialog : public MyGameObject {

    public:
        //\’z‚Æ”jŠü
        TutorialDialog() : MyGameObject() {}
        virtual ~TutorialDialog() {}
        //‰Šú‰»
        virtual void OnCreate()override;
    };

}