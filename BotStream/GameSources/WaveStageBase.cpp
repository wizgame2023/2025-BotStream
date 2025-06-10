/*!
@file WaveStageBase.cpp
@brief �X�e�[�W�̊��N���X
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
    void WaveStageBase::CreateViewLight() {
        //�J����
        const Vec3 eye(0.0f, 350.0f, -20.0f);

        const Vec3 at(0.0f, 0.0f, 0.0f);
        auto PtrView = CreateView<SingleView>();
        //�r���[�̃J�����̐ݒ�
        auto PtrCamera = ObjectFactory::Create<Camera>();
        PtrView->SetCamera(PtrCamera);
        PtrCamera->SetEye(eye);
        PtrCamera->SetAt(at);
        //�}���`���C�g�̍쐬
        auto PtrMultiLight = CreateLight<MultiLight>();
        //�f�t�H���g�̃��C�e�B���O���w��
        PtrMultiLight->SetDefaultLighting();

        //���ł�Actor�O���[�v����
        CreateSharedObjectGroup(L"Actor");
    }

    void WaveStageBase::CreatePlayer(Vec3 pos, Vec3 rot, Vec3 scale) {
        m_player = AddGameObject<Player>(pos, rot, scale);
        SetSharedGameObject(L"Player", m_player.lock());

    }

    void WaveStageBase::SetActorPause(bool isPause) {
        m_isPaused = isPause;

        auto objVec = GetGameObjectVec();
        for (auto obj : objVec)
        {
            auto actor = dynamic_pointer_cast<Actor>(obj);
            auto cameraManager = dynamic_pointer_cast<CameraManager>(obj);
            auto parts = dynamic_pointer_cast<Parts>(obj);

            if (actor)
            {
                actor->SetPose(isPause);
            }
            if (cameraManager)
            {
                cameraManager->SetPose(isPause);
            }
            if (parts)
            {
                parts->SetPose(isPause);
            }
        }
    }
    void WaveStageBase::CreateManagerObjects() {

        shared_ptr<FadeoutSprite> fadeout;
        fadeout = AddGameObject<FadeoutSprite>(L"Fadeout");
        SetSharedGameObject(L"Fadeout", fadeout);
        fadeout->SetDrawLayer(4);

        // ------- �p�[�c�֌W ---------------------------------------------------------
        //�p�[�c�}�l�[�W������
        auto partsManager = AddGameObject<PartsManager>();
        SetSharedGameObject(L"PartsManager", partsManager);

        auto equippedParts = AddGameObject<EquippedParts>();
        SetSharedGameObject(L"PartsPoach", equippedParts);
        //AddGameObject<PartsHiMoter>(Vec3(-20.0f,1.0f,-230.0f),Vec3(0.0f,0.0f,0.0f),Vec3(1.0f,1.0f,1.0f));

        //�������̃p�[�c��\������t�h
        AddGameObject<PartsTextChange>();

        // ----------------------------------------------------------------------------
        
        //�J�����}�l�[�W���[
        auto cameraManager = AddGameObject<CameraManager>();
        SetSharedGameObject(L"CameraManager", cameraManager);

        auto ptrSoundManager = AddGameObject<SoundManager>();
        SetSharedGameObject(L"SoundManager", ptrSoundManager);
        GetSharedGameObject<SoundManager>(L"SoundManager")->PlayBGM(3);
        GetSharedGameObject<SoundManager>(L"SoundManager")->PlaySE(13);

        auto colController = AddGameObject<StageCollisionController>();
        colController->SetCollisionSwhich(true);

        //�|�[�Y��������
        AddGameObject<PauseSprite>();

        // �퓬�pUI
        AddGameObject<PlayerWeaponUI>();
        auto playerUI = AddGameObject<PlayerGaugeUI>(100);
        SetSharedGameObject(L"PlayerUI", playerUI);

    }

    void WaveStageBase::OnCreate()
    {
        auto& app = App::GetApp();
        m_scene = app->GetScene<Scene>();

        //�r���[�ƃ��C�g�̍쐬
        CreateViewLight();

        CreateFloor();
        CreateWall();
        CreateCeiling();
        CreatePlayer(Vec3(0.0f, 3.0f, -305.0f), Vec3(0.0f, 5.0f, 0.0f), Vec3(1.0f, 2.0f, 1.0f));

        //Enemy�}�l�[�W���̃e�X�g
        vector<EnemyVariation> enemyVariation;
        for (int i = 0; i <= 10; i++)
        {
            enemyVariation.push_back(EVar_Normal);
        }
        for (int i = 0; i <= 10; i++)
        {
            enemyVariation.push_back(EVar_Projectile);
        }

        enemyVariation.push_back(EVar_Aerial);

        m_enemyMgr = AddGameObject<EnemyManager>(enemyVariation);
        SetSharedGameObject(L"EnemyManager", m_enemyMgr.lock());

        m_boss = AddGameObject<BossFirst>(Vec3(0.0f, 2.0f, 250.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
        SetSharedGameObject(L"Boss", m_boss.lock());


        // �{�X�Q�[�W
        m_bossGauge = AddGameObject<BossGaugeUI>(
            m_boss.lock(),
            m_boss.lock()->GetHPMax()
        );
        SetSharedGameObject(L"BossUI", m_bossGauge);

        //wave1�G
        m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(0.0f, 2.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(10.0f, 2.0f, -255.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(-10.0f, 2.0f, -235.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(20.0f, 2.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(-20.0f, 2.0f, -245.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(30.0f, 2.0f, -225.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(-30.0f, 2.0f, -225.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        //auto flyingEnemy = AddGameObject<EnemyZakoFlying>(Vec3(0.0f, 10.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f), true);

        CreateManagerObjects();

        m_gamePhase = GamePhase::GPhase_Start;
    }

    void WaveStageBase::SetPlayerTransform(Vec3 pos, Vec3 rot) {
        m_player.lock()->GetComponent<Transform>()->SetPosition(pos);
        m_player.lock()->GetComponent<Transform>()->SetRotation(rot);
        GetSharedGameObject<CameraManager>(L"CameraManager")->SetAngle(L"Y", rot.y);

    }

    void WaveStageBase::WaveInitialize() {
        m_IsFadeInFlag = true;
        m_nextWaveFlag = false;

        switch (m_waveCurrent) {

            // ------- 1 -> 2 -------------------------------------------------------------
        case 1:
            //�v���C���[�̈ʒu��������
            SetPlayerTransform(Vec3(0.0f, 3.0f, -40.0f), Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f));

            m_nextWaveFlag = false;

            m_enemyMgr.lock()->InstEnemy(Vec3(0.0f, 2.0f, 0.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            m_enemyMgr.lock()->InstEnemy(Vec3(10.0f, 2.0f, 30.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            m_enemyMgr.lock()->InstEnemy(Vec3(-10.0f, 2.0f, -20.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            m_enemyMgr.lock()->InstEnemy(Vec3(20.0f, 2.0f, 10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            m_enemyMgr.lock()->InstEnemy(Vec3(-20.0f, 2.0f, -10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            m_enemyMgr.lock()->InstEnemy(Vec3(30.0f, 2.0f, 30.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            m_enemyMgr.lock()->InstEnemy(Vec3(-30.0f, 2.0f, -5.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));

            //m_enemyMgr.lock()->InstEnemy(Vec3(0.0f, 2.0f, 0.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            //m_enemyMgr.lock()->InstEnemy(Vec3(10.0f, 2.0f, 30.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            //m_enemyMgr.lock()->InstEnemy(Vec3(-10.0f,2.0f, -20.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            //m_enemyMgr.lock()->InstEnemy(Vec3(20.0f, 2.0f, 10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            m_enemyMgr.lock()->InstEnemy<EnemyZakoLong>(Vec3(-20.0f, 2.0f, -15.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            m_enemyMgr.lock()->InstEnemy<EnemyZakoLong>(Vec3(30.0f, 2.0f, 35.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            m_enemyMgr.lock()->InstEnemy<EnemyZakoLong>(Vec3(-30.0f, 2.0f, -15.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));

            break;
            // ----------------------------------------------------------------------------

            // ------- 2 -> 3 -------------------------------------------------------------
        case 2:
            m_bossGauge->ClearBossGaugeUI(false);

            //�v���C���[�̈ʒu��������
            SetPlayerTransform(Vec3(0.0f, 3.0f, 195.0f), Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f));

            m_IsFadeInFlag = true;
            m_nextWaveFlag = false;

            m_enemyMgr.lock()->InstBoss(dynamic_pointer_cast<EnemyBase>(m_boss.lock()));

            GetSharedGameObject<SoundManager>(L"SoundManager")->StopBGM();
            GetSharedGameObject<SoundManager>(L"SoundManager")->PlayBGM(4);

            break;
            // ----------------------------------------------------------------------------

        }

        m_waveCurrent++;
    }

    void WaveStageBase::OnUpdate()
    {
        ShowFPS();

        auto& app = App::GetApp();
        auto KeyState = app->GetInputDevice().GetKeyState();
        auto pad = app->GetInputDevice().GetControlerVec()[0];

        auto fadeout = GetSharedGameObject<FadeoutSprite>(L"Fadeout");

        fadeout->GetBlackFlag();
        m_BlackFlag = fadeout->GetBlackFlag();
        fadeout->GetFadeOutFlag();
        m_IsFadeOutFlag = fadeout->GetFadeOutFlag();
        fadeout->GetFadeInFlag();
        m_IsFadeInFlag = fadeout->GetFadeInFlag();

        m_player.lock()->GetHP();
        auto plaHP = m_player.lock()->GetHP();

        auto ptrSoundManager = GetSharedGameObject<SoundManager>(L"SoundManager");

        auto EnemyVec = m_enemyMgr.lock()->GetEnemyVec(true);
        int EnemyNum = EnemyVec.size();

        EffectManager::Instance().InterfaceUpdate();

        if (m_waveCurrent == 1 && EnemyNum == 0)
        {
            m_IsFadeOutFlag = true;
        }
        if (m_waveCurrent == 2 && EnemyNum == 0)
        {
            m_IsFadeOutFlag = true;
        }

        if (m_BlackFlag == true)
        {
            m_nextWaveFlag = true;
        }

        if (m_nextWaveFlag)
        {
            WaveInitialize();
        }

        if (m_waveCurrent == 3)
        {
            m_bossCurrentHP = m_boss.lock()->GetHPCurrent();//Boss��HP�擾
        }

        if (m_waveCurrent == 3 && m_bossCurrentHP <= 0)
        {
            GetSharedGameObject<SoundManager>(L"SoundManager")->StopBGM();
            m_scene.lock()->PostEvent(3.0f, GetThis<ObjectInterface>(), m_scene.lock(), L"ToGameClear");
            m_waveCurrent = 4;//�E�F�[�u�I��
        }

        if (plaHP <= 0)
        {
            GetSharedGameObject<SoundManager>(L"SoundManager")->StopBGM();
            m_scene.lock()->PostEvent(1.0f, GetThis<ObjectInterface>(), m_scene.lock(), L"ToGameOver");

        }

        //�t�F�[�h���o�I�u�W�F�N�g�փt���O��n��
        fadeout->SetFadeOutFlag(m_IsFadeOutFlag);
        fadeout->SetFadeInFlag(m_IsFadeInFlag);
        fadeout->SetBlackFlag(m_BlackFlag);
    }

    void WaveStageBase::OnDraw()
    {
        EffectManager::Instance().InterfaceDraw();
    }

    void WaveStageBase::OnDestroy()
    {
        //BGM��SE���~�߂�
        auto soundManager = GetSharedGameObject<SoundManager>(L"SoundManager");
        soundManager->StopBGM();
        soundManager->StopSE();
    }

    void WaveStageBase::SetNextWaveFlag(bool setNextWaveFlag)
    {
        m_nextWaveFlag = setNextWaveFlag;
    }

    bool WaveStageBase::GetNextWaveFlag()
    {
        return m_nextWaveFlag;
    }

//--------------------------------------------------------------------------------
// �n�`����

    //�V��쐬
    void WaveStageBase::CreateCeiling()
    {
        vector < vector<Vec3> > vec =
        {
            //Boss
            {
                Vec3(170.0f, 1.0f, 170.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, 40.0f, 260.0f)
            },
            //Wave2
            {
                Vec3(120.0f, 1.0f, 120.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, 40.0f, 0.0f)
            },
            //Wave1
            {
                Vec3(120.0f, 1.0f, 120.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, 40.0f, -260.0f)
            }

        };
        for (auto v : vec)
        {
            AddGameObject<Ceiling>(v[0], v[1], v[2]);
        }
    }

    //�Ǎ쐬
    void WaveStageBase::CreateWall()
    {
        // Wave1�̕ǂ̑傫��
        Vec3 wallSclWv1(120.0f, 4.0f, 41.0f);
        // Wave2�̕ǂ̑傫��
        Vec3 wallSclWv2(120.0f, 4.0f, 41.0f);
        // Boss�̕ǂ̑傫��
        Vec3 wallSclBoss(170.0f, 4.0f, 41.0f);

        vector < vector<Vec3> > vec =
        {
            //Boss
            //�E
            {
                wallSclBoss,
                Vec3(-XM_PIDIV2, 0.0f, 1.57f),
                Vec3(86.9f, 19.0f, 260.0f)
            },
            //��
            {
                wallSclBoss,
                Vec3(-XM_PIDIV2, 0.0f, 1.57f),
                Vec3(-86.9f, 19.0f, 260.0f)
            },
            //Wave2
            //�E
            {
                wallSclWv2,
                Vec3(-XM_PIDIV2, 0.0f, 1.57f),
                Vec3(62.0f, 19.0f, 0.0f)
            },
            //��
            {
                wallSclWv2,
                Vec3(-XM_PIDIV2, 0.0f, 1.57f),
                Vec3(-61.9f, 19.0f, 0.0f)
            },
            //Wave1
            //�E
            {
                wallSclWv1,
                Vec3(-XM_PIDIV2, 0.0f, 1.57f),
                Vec3(62.0f, 19.0f, -260.0f)
            },
            //��
            {
                wallSclWv1,
                Vec3(-XM_PIDIV2, 0.0f, 1.57f),
                Vec3(-62.0f, 19.0f, -260.0f)
            },
        };
        for (auto v : vec)
        {
            AddGameObject<Wall>(v[0], v[1], v[2]);
        }

        vector < vector<Vec3> > vec2 =
        {
            ////Boss
            //�O
            {
                wallSclBoss,
                Vec3(-XM_PIDIV2, 1.57f, 1.57f),
                Vec3(0.0f, 19.0f, 346.9f)
            },
            //��
            {
                wallSclBoss,
                Vec3(-XM_PIDIV2, 1.57f, 1.57f),
                Vec3(0.0f, 19.0f, 173.1f)
            },
            ////Wave2
            //�O
            {
                wallSclWv2,
                Vec3(-XM_PIDIV2, 1.57f, 1.57f),
                Vec3(0.0f, 19.0f, 62.0f)
            },
            //��
            {
                wallSclWv2,
                Vec3(-XM_PIDIV2, 1.57f, 1.57f),
                Vec3(0.0f, 19.0f, -62.0f)
            },
            ////Wave1
            //�O
            {
                wallSclWv1,
                Vec3(-XM_PIDIV2, 1.57f, 1.57f),
                Vec3(0.0f, 19.0f, -198.0f)
            },
            //��
           {
               wallSclWv1,
               Vec3(-XM_PIDIV2, 1.57f, 1.57f),
               Vec3(0.0f, 19.0f, -322.0f)
           },
        };
        for (auto v : vec2)
        {
            AddGameObject<Wall2>(v[0], v[1], v[2]);
        }
    }

    //���쐬
    void WaveStageBase::CreateFloor()
    {
        vector < vector<Vec3> > vec =
        {
            // �ŏ���Boss����ԏ��߂ɐ�������Ă������ǁA���ɖ͗l��\�鎞�̏����Ƒ����������̂�
            // �����Wave1��Boss�̐�������ς������Ă��炢�܂����B�Ȃɂ��ق��̏��ɉe��������Ȃ炨�`����������

            //Wave1
            {
                Vec3(120.0f, 3.0f, 120.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, -3.0f, -260.0f)
            },
            //Wave2
            {
                Vec3(120.0f, 3.0f, 120.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, -3.0f, 0.0f)
            },
            //Boss
            {
                Vec3(170.0f, 3.0f, 170.0f),
                Vec3(0.0f, 0.0f, 0.0f),
                Vec3(0.0f, -3.0f, 260.0f)
            },


        };
        for (auto v : vec)
        {
            AddGameObject<Floor>(v[0], v[1], v[2]);

            // �u���b�N�̉��Ɖ��s���̐ÓI�萔
            constexpr float scaleXZ = Block::BLOCK_XZ_SCALE;
            // �u���b�N�������̃|�W�V�����̂���̏C��
            constexpr float shiftPos = scaleXZ / 2;

            // �傫���ƈʒu�̑��(�}�W�b�N�i���o�[�ɂȂ��Ă��܂����ߒǉ�)
            auto scl = v[0];
            auto pos = v[2];

            // ��������u���b�N�̐�
            int blockNum = (int)scl.x / 10;

            for (int i = 0; i < blockNum; i++)
            {
                for (int j = 0; j < blockNum; j++)
                {
                    //AddGameObject<Block>(
                    //    Vec3(
                    //        // �u���b�N1�̑傫�� - �X�e�[�W�̈ʒu + (�X�e�[�W�̑傫�� / 2) + (�u���b�N�̑傫�� / 2)
                    //        //                              ���������Ȃ��ƃX�e�[�W�̒������琶������Ă��܂�
                    //        j * scaleXZ - (pos.x + (scl.x / 2) - shiftPos),
                    //        pos.y + 0.05f,
                    //        // ��{�͕ς��Ȃ����ǁAWave1��Boss�̏����t�ɂȂ��Ă����̂ł�������ƒ���܂���
                    //        i * scaleXZ + (pos.z - (scl.z / 2) + shiftPos)
                    //    )
                    //);
                }
            }
        }
    }


}