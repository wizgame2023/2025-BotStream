#pragma once
/*!
@file StageThird.h
@brief Derived WaveStage
*/

#include "stdafx.h"

namespace basecross {

    class StageThird : public StageFirst
    {
    private:

    public:
        virtual void OnCreate()override;
        virtual void OnUpdate()override;
        //Wave�����Ɉڂ����Ƃ��ɌĂяo�����
        virtual void WaveInitialize();

    };

}