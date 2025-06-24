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

        // ���[�r�[�p�R���g���[���[
        shared_ptr<RT_MovieController> m_movieController;

        // �h�A�̎��̂�����z��2�łP�g
        vector<shared_ptr<Door>> m_doors;
        // ���̃t�F�[�Y�ɍs���p�̍�����̔z��
        vector<shared_ptr<DarkObj>> m_darkObjs;

        //�Q�[���N���A�t���O
        bool m_gameClearFlag = false;

    public:
        // �\�z�Ɣj��
        StageFirst() : WaveStageBase() {}
        virtual ~StageFirst() {}
        // ������
        virtual void OnCreate()override;
        virtual void OnUpdate()override;

        // �h�A�𓮂�������
        void DoorsMove(int group);

        // �Èł̎�Wave��L���������鏈��
        void ActivateDark(int ActivateNum);

        // �Ó]�������J�n�����鏈��
        void FadeOutStart();

        void UpdateGamePhase()override;

        //�Q�[���N���A�t���O��n������
        void SetGameClear(bool onOff);
    };

}