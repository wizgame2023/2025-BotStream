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
        //�\�z�Ɣj��
        TutorialStage() : WaveStageBase() {}
        virtual ~TutorialStage() {}
        //������
        virtual void OnCreate()override;
    };

    class TutorialDialog : public MyGameObject {

    public:
        //�\�z�Ɣj��
        TutorialDialog() : MyGameObject() {}
        virtual ~TutorialDialog() {}
        //������
        virtual void OnCreate()override;
    };

}