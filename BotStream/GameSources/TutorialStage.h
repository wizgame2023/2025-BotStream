/*!
@file TutorialStage.h
@brief Derived WaveStage
*/

#include "stdafx.h"

namespace basecross {

    class TutorialStage : public WaveStageBase {
    protected:
        //�e�I�u�W�F�N�g�ւ̎Q�Ɨp
        // �V�[��
        weak_ptr<Scene> m_scene;
        // �v���C���[
        weak_ptr<Player> m_player;
        // �G�Ǘ��I�u�W�F�N�g
        weak_ptr<EnemyManager> m_enemyMgr;
        // �{�X
        weak_ptr<EnemyBase> m_boss;

        shared_ptr<BossGaugeUI> m_bossGauge;


    public:
        //�\�z�Ɣj��
        TutorialStage() : WaveStageBase() {}
        virtual ~TutorialStage() {}
        //������
        virtual void OnCreate()override;
    };

}