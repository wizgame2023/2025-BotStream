/*!
@file WaveStageBase.h
@brief �X�e�[�W�̊��N���X
*/

#pragma once
#include "stdafx.h"
#include "EnemyManager.h"

namespace basecross {
    class BossGaugeUI;
    class Player;
    class SoundManager;
    class FadeoutSprite;

    class WaveStageBase : public Stage {
    public:
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

        virtual void CreatePlayer(Vec3 pos, Vec3 rot, Vec3 scale);

        //�e�I�u�W�F�N�g�ւ̎Q�Ɨp
        // �V�[��
        weak_ptr<Scene> m_scene;
        //�T�E���h�}�l�[�W���[
        weak_ptr<SoundManager> m_sndMgr;
        //�t�F�[�h�p�I�u�W�F�N�g
        weak_ptr<FadeoutSprite> m_fadeout;
        // �v���C���[
        weak_ptr<Player> m_player;
        // �G�Ǘ��I�u�W�F�N�g
        weak_ptr<EnemyManager> m_enemyMgr;
        // �{�X
        weak_ptr<EnemyBase> m_boss;

        //�Q�[���̐i�s��
        GamePhase m_gamePhase = GPhase_None;
        //���݂�wave
        int m_waveCurrent = 1;
        //�ő�wave��
        int m_waveMax = 3;
        //BOSS�̌��݂�HP
        int m_bossCurrentHP = 99999;
        //���Ԃ̑���
        float m_deltaScale = 1.0f;
        //deltaScale���f�t�H���g�ɖ߂����߂̃J�E���g
        float m_deltaScaleCount = 0.0f;
        //deltaScale�̃f�t�H���g�l(����ۂǂ̂��Ƃ��Ȃ����1)
        const float m_deltaScaleDefault = 1.0f;

        float TimeOfGameClear = 0.0f;//�Q�[���N���A���邽�߂̎��Ԍv��

        //Wave�؂�ւ�
        bool m_nextWaveFlag = false;

        bool m_isPaused = false;

        shared_ptr<BossGaugeUI> m_bossGauge;

        //�}�l�[�W���AUI�̗ނ𐶐�����
        virtual void CreateManagerObjects();

        //Wave�����Ɉڂ����Ƃ��ɌĂяo�����
        virtual void WaveInitialize();

        //�v���C���[�̈ʒu�������I�ɕς���
        virtual void SetPlayerTransform(Vec3 pos, Vec3 rot);

        void ShowFPS() {
            ////�f�o�b�N�p
            //wstringstream wss(L"");
            //m_scene.lock() = App::GetApp()->GetScene<Scene>();
            //wss /* << L"�f�o�b�O�p������ "*/
            //    << L"\n FPS: " << App::GetApp()->App::GetApp()->GetStepTimer().GetFramesPerSecond() << endl;

            //m_scene.lock()->SetDebugString(wss.str());

        }

        //OnUpdate�ɒu��
        void ResetDeltaScaleToDefault() {
            if (m_deltaScaleCount > 0) {
                m_deltaScaleCount -= App::GetApp()->GetElapsedTime();
                
                if (m_deltaScaleCount <= 0) {
                    m_deltaScale = m_deltaScaleDefault;
                }
            }
        }

        virtual bool ConsiderGoToNextWave();
        virtual bool ConsiderGameClear();
        virtual bool ConsiderGameOver();

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
        void SetNextWaveFlag(bool setNextWaveFlag);

        GamePhase GetCurrentGamePhase() {
            return m_gamePhase;
        }

        int GetNowWaveNum() {
            return m_waveCurrent;
        }

        float GetDeltaScale() {
            return m_deltaScale;
        }

        void SetDeltaScale(float deltascale) {
            m_deltaScale = deltascale;
        }

        //�w�莞�Ԍ��deltaScale���f�t�H���g�̒l�ɖ߂�
        void SetDeltaScaleCount(float counttime) {
            m_deltaScaleCount = counttime;
        }

        void SetActorPause(bool isPause);

        bool GetActorPause() {
            return m_isPaused;
        }

    };

    class Floor : public GameObject {
        Vec3 m_Scale;
        Vec3 m_Rotation;
        Vec3 m_Position;
    public:
        Floor(const shared_ptr<Stage>& StagePtr,
            const Vec3& Scale,
            const Vec3& Rotation,
            const Vec3& Position
        );
        virtual ~Floor();
        virtual void OnCreate() override;
    };

    // ���̃e�N�X�`���p--------------------------------
    class Block : public MyGameObject
    {
    public:
        Block(const shared_ptr<Stage>& StagePtr,
            const Vec3& Scale,
            const Vec3& Position);
        virtual ~Block() {}

        virtual void OnCreate() override;

        static constexpr float BLOCK_XZ_SCALE = 10.0f;

    private:
        Vec3 m_pos;
        Vec3 m_scale;
    };
    // END --------------------------------------------

    class Wall : public GameObject {
        Vec3 m_Scale;
        Vec3 m_Rotation;
        Vec3 m_Position;
    public:
        Wall(const shared_ptr<Stage>& StagePtr,
            const Vec3& Scale,
            const Vec3& Rotation,
            const Vec3& Position
        );
        virtual ~Wall();
        virtual void OnCreate() override;
    };

    // �ǂ̃e�N�X�`�����Ⴄ��𐶐�--------------
    class Wall2 : public GameObject {
        Vec3 m_Scale;
        Vec3 m_Rotation;
        Vec3 m_Position;
    public:
        Wall2(const shared_ptr<Stage>& StagePtr,
            const Vec3& Scale,
            const Vec3& Rotation,
            const Vec3& Position
        );
        virtual ~Wall2();
        virtual void OnCreate() override;
    };
    // ---------------------------------------------

    class Ceiling : public GameObject {
        Vec3 m_Scale;
        Vec3 m_Rotation;
        Vec3 m_Position;
    public:
        Ceiling(const shared_ptr<Stage>& StagePtr,
            const Vec3& Scale,
            const Vec3& Rotation,
            const Vec3& Position
        );
        virtual ~Ceiling();
        virtual void OnCreate() override;
    };

}