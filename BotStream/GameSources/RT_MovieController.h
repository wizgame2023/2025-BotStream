#pragma once
#include "stdafx.h"
#include "MyGameObject.h"

namespace basecross {
    enum RT_MoviePhase
    {
        Movie_None,
        StartMovie_First,
        StartMovie_Second,
        StartMovie_Three
    };

    class RT_MovieController : public MyGameObject {
    protected:
        weak_ptr<Player> m_player;
        weak_ptr<CameraManager> m_cameraManager;

        // ムービーの処理が終わったフラグ
        bool m_playerEnd = false;
        bool m_cameraEnd = false;

        // ムービが再生されているかの処理
        int m_startMovie = Movie_None;

    public:
        // 構築と破棄
        RT_MovieController(const shared_ptr<Stage>& stagePtr,shared_ptr<Player> player, shared_ptr<CameraManager> cameraManager) :
            MyGameObject(stagePtr),
            m_player(player),
            m_cameraManager(cameraManager)
        {
        }
        virtual ~RT_MovieController() 
        {
        }

        // 初期化
        virtual void OnCreate()override;
        virtual void OnUpdate()override;

        // 最初のムービー処理
        void StartMovie();

        // m_playerEndのゲッタセッタ
        void SetPlayerEnd(bool onOff)
        {
            m_playerEnd = onOff;
        }
        bool GetPlayerEnd()
        {
            return m_playerEnd;
        }

        // m_cameraEndのゲッタセッタ
        bool GetCameraEnd()
        {
            return m_cameraEnd;
        }
        void SetCameraEnd(bool onOff)
        {
            m_cameraEnd = onOff;
        }
    };

}