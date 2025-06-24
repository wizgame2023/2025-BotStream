#pragma once
/*!
@file StageSecond.h
@brief Derived WaveStage
*/

#include "stdafx.h"

namespace basecross {
	class StageSecond : public StageFirst
    {
    private:

    public:
        virtual void OnCreate()override;
        virtual void OnUpdate()override;
        //Wave‚ªŸ‚ÉˆÚ‚Á‚½‚Æ‚«‚ÉŒÄ‚Ño‚³‚ê‚é
        virtual void WaveInitialize();

    };

	class StageThird : public StageFirst
    {
    private:

    public:
        virtual void OnCreate()override;
        virtual void OnUpdate()override;
        //Wave‚ªŸ‚ÉˆÚ‚Á‚½‚Æ‚«‚ÉŒÄ‚Ño‚³‚ê‚é
        virtual void WaveInitialize();

    };
}