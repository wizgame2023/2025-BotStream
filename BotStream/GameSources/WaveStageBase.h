/*!
@file WaveStageBase.h
@brief �X�e�[�W�̊��N���X
*/

#pragma once
#include "stdafx.h"

namespace basecross {
    class BossGaugeUI;
    class Player;
    class EnemyManager;

    class WaveStageBase : public Stage {
        //�Q�[���̐i�s��
        enum GamePhase {
            GPhase_None,
            GPhase_Start,
            GPhase_Playing,
            GPhase_CutScene,
            GPhase_Result,
            GPhase_GameOver
        };

    protected:
        //�r���[�̍쐬
        virtual void CreateViewLight();

        virtual void CreateFloor();
        virtual void CreateWall();
        virtual void CreateCeiling();

        //�e�I�u�W�F�N�g�ւ̎Q�Ɨp
        // �V�[��
        weak_ptr<Scene> m_scene;
        // �v���C���[
        weak_ptr<Player> m_player;
        // �G�Ǘ��I�u�W�F�N�g
        weak_ptr<EnemyManager> m_enemyMgr;
        // �{�X
        weak_ptr<EnemyBase> m_boss;

        //�Q�[���̐i�s��
        GamePhase m_gamePhase = GPhase_None;
        //���݂�wave
        int m_waveCurrent = 0;
        //BOSS�̌��݂�HP
        int m_bossCurrentHP = 99999;

        float TimeOfGameClear = 0.0f;//�Q�[���N���A���邽�߂̎��Ԍv��

        bool m_IsFadeOutFlag = false;
        bool m_IsFadeInFlag = false;
        bool m_BlackFlag = false;
        bool m_nextWaveFlag = false;

        shared_ptr<BossGaugeUI> m_bossGauge;

        //�}�l�[�W���AUI�̗ނ𐶐�����
        virtual void CreateManagerObjects();

        //Wave�����Ɉڂ����Ƃ��ɌĂяo�����
        virtual void WaveInitialize();

        virtual void SetPlayerTransform(Vec3 pos, Vec3 rot);

    public:
        //�\�z�Ɣj��
        WaveStageBase() : Stage() {}
        virtual ~WaveStageBase() {}
        //������
        virtual void OnCreate()override;
        virtual void OnUpdate()override;
        virtual void OnDraw()override;
        virtual void OnDestroy()override;

        bool GetNextWaveFlag();
        void SetNextWaveFlag(int setNextWaveFlag);

        GamePhase GetCurrentGamePhase() {
            return m_gamePhase;
        }

        int GetNowWaveNum() {
            return m_waveCurrent;
        }
    };

}