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

        // ���[�r�[�̏������I������t���O
        bool m_playerEnd = false;
        bool m_cameraEnd = false;

        // ���[�r���Đ�����Ă��邩�̏���
        int m_startMovie = Movie_None;

    public:
        // �\�z�Ɣj��
        RT_MovieController(const shared_ptr<Stage>& stagePtr,shared_ptr<Player> player, shared_ptr<CameraManager> cameraManager) :
            MyGameObject(stagePtr),
            m_player(player),
            m_cameraManager(cameraManager)
        {
        }
        virtual ~RT_MovieController() 
        {
        }

        // ������
        virtual void OnCreate()override;
        virtual void OnUpdate()override;

        // �ŏ��̃��[�r�[����
        void StartMovie();

        // m_playerEnd�̃Q�b�^�Z�b�^
        void SetPlayerEnd(bool onOff)
        {
            m_playerEnd = onOff;
        }
        bool GetPlayerEnd()
        {
            return m_playerEnd;
        }

        // m_cameraEnd�̃Q�b�^�Z�b�^
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