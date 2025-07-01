/*!
@file TutorialStage.cpp
@brief Derived WaveStage
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

    void TutorialStage::OnCreate()
    {
        auto& app = App::GetApp();
        m_scene = app->GetScene<Scene>();

        //ビューとライトの作成
        CreateViewLight();

        CreateFloor();
        CreateWall();
        CreateCeiling();
        CreatePlayer(Vec3(0.0f, 3.0f, -305.0f), Vec3(0.0f, 5.0f, 0.0f), Vec3(1.0f, 2.0f, 1.0f));

        CreateEnemyManager();
        CreateManagerObjects();

        m_gamePhase = GamePhase::GPhase_Start;
        m_waveMax = 1;
        m_tutorialPhase = Tutorial_Start;

        m_dialog = AddGameObject<TutorialDialog>(L"Tuto_txt1", Vec2(0, 50), Col4(1, 1, 1, 0));

        m_bar[0] = AddGameObject<TutorialProgressFrame>(L"Tuto_frame", Vec3(200, 250, 0));
        m_bar[1] = AddGameObject<TutorialProgressFrame>(L"Tuto_frame", Vec3(200, 200, 0));

    }

    void TutorialStage::UpdateTutorialPhase() {
        auto delta = App::GetApp()->GetElapsedTime();
        switch (m_tutorialPhase) 
        {
        case Tutorial_Start:
            DisplayDialog(L"Tuto_txt1", m_dialog.lock());
            ChangeTutorialPhase(Tutorial_MoveAndCamera);
            break;
        case Tutorial_MoveAndCamera:
        {
            m_bar[0].lock()->SetDrawActive(m_dialog.lock()->IsInvisible());
            m_bar[1].lock()->SetDrawActive(m_dialog.lock()->IsInvisible());

            Vec3 move = m_player.lock()->GetVelocity();
            move.y = 0;
            m_progress[0] += move.length() * m_tutorialMoveRequired * delta;
            auto camera = dynamic_pointer_cast<CameraManager>(GetSharedGameObject<CameraManager>(L"CameraManager"));
            m_progress[1] += camera->GetAddAngleNAxis() * m_tutorialCameraRequired * delta;

            m_bar[0].lock()->SetPercent(m_progress[0]);
            m_bar[1].lock()->SetPercent(m_progress[1]);

            if (m_progress[0] >= 1.0f && m_progress[1] >= 1.0f) {
                ChangeTutorialPhase(Tutorial_Evade);
            }
        }
            break;
        case Tutorial_Evade:
        {
            m_bar[0].lock()->SetDrawActive(m_dialog.lock()->IsInvisible());
            m_bar[0].lock()->SetPercent(m_progress[0]);

            float add = m_player.lock()->GetDodge() ? m_tutorialEvadeRequired * delta : 0;
            m_progress[0] += add;

            if (m_enemyMgr.lock()->GetEnemyVec(true).size() < m_enemyNum) {
                m_enemyMgr.lock()->InstEnemy<EnemyZakoLong>(Vec3(0.0f, 10.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            }

            if (m_progress[0] >= 1.0f) {
                ChangeTutorialPhase(Tutorial_KeepRunning);
            }
        }
            break;
        case Tutorial_KeepRunning:
        {
            m_bar[0].lock()->SetDrawActive(m_dialog.lock()->IsInvisible());
            m_bar[0].lock()->SetPercent(m_progress[0]);

            float add = (m_player.lock()->GetStateName() == L"Dash") ? m_tutorialDashRequired * delta : 0;
            m_progress[0] += add;

            if (m_progress[0] >= 1.0f) {
                ChangeTutorialPhase(Tutorial_HitTheCombos);
            }
        }
            break;
        case Tutorial_HitTheCombos:
        {
            m_bar[0].lock()->SetDrawActive(m_dialog.lock()->IsInvisible());
            m_bar[0].lock()->SetPercent(m_progress[0]);

            wstring stateName = m_player.lock()->GetStateName();
            if (stateName == L"Attack1" || stateName == L"Attack2" || stateName == L"Attack3") {
                m_progress[0] += m_tutorialAttackRequired * delta;
            }
            else if (stateName == L"AttackEx") {
                m_progress[0] += m_tutorialAttackRequiredEx * delta;
            }

            if (m_enemyMgr.lock()->GetEnemyVec(true).size() < m_enemyNum) {
                m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(0.0f, 10.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            }

            if (m_progress[0] >= 1.0f) {
                ChangeTutorialPhase(Tutorial_ShootFromAfar);
            }
        }
            break;
        case Tutorial_ShootFromAfar:
        {
            m_bar[0].lock()->SetDrawActive(m_dialog.lock()->IsInvisible());
            m_bar[0].lock()->SetPercent(m_progress[0]);

            vector<shared_ptr<EnemyBase>> enemyVec = m_enemyMgr.lock()->GetEnemyVec(true);
            for (auto& e : enemyVec) {
                wstring stateName = e->GetStateName();
                if (stateName == L"Stun") {
                    m_progress[0] += m_tutorialAttackRequired * delta;
                }
            }

            if (m_enemyMgr.lock()->GetEnemyVec(true).size() < m_enemyNum) {
                m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(0.0f, 10.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            }

            if (m_progress[0] >= 1.0f) {
                ChangeTutorialPhase(Tutorial_KillAsYouLike);
            }
        }
            break;
        case Tutorial_KillAsYouLike:
        {
            bool drawActive = m_dialog.lock()->IsInvisible();
            m_bar[0].lock()->SetDrawActive(drawActive);
            m_bar[0].lock()->SetPercent(m_progress[0]);

            float enemyNum = static_cast<float>(m_enemyMgr.lock()->GetEnemyVec(true).size());
            m_progress[0] = 1.0f - (enemyNum / m_enemyNumFinalPhase);
            if (m_progress[0] >= 1.0f) {
                ChangeTutorialPhase(Tutorial_Cleared);
            }
        }
            break;
        case Tutorial_Cleared:
        {
            bool drawActive = m_dialog.lock()->IsInvisible();
            m_bar[0].lock()->SetDrawActive(drawActive);
            m_bar[0].lock()->SetPercent(m_progress[0]);

        }
            break;
        default:
            break;
        }
    }

    void TutorialStage::ChangeTutorialPhase(TutorialPhase phase) {
        switch (phase) {
        case Tutorial_MoveAndCamera:
            m_bar[0].lock()->ChangeDescription(1);
            m_bar[1].lock()->ChangeDescription(2);
            break;
        case Tutorial_Evade:
            m_enemyMgr.lock()->InstEnemy<EnemyZakoLong>(Vec3(-20.0f, 10.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            m_enemyMgr.lock()->InstEnemy<EnemyZakoLong>(Vec3(-10.0f, 10.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            m_enemyMgr.lock()->InstEnemy<EnemyZakoLong>(Vec3(0.0f, 10.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            m_enemyMgr.lock()->InstEnemy<EnemyZakoLong>(Vec3(10.0f, 10.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            m_enemyMgr.lock()->InstEnemy<EnemyZakoLong>(Vec3(20.0f, 10.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));

            m_progress[0] = 0;
            m_bar[0].lock()->SetDrawActive(true);
            m_bar[1].lock()->SetDrawActive(false);

            m_bar[0].lock()->ChangeDescription(3);

            DisplayDialog(L"Tuto_txt2", m_dialog.lock());
            break;
        case Tutorial_KeepRunning:
            m_progress[0] = 0;
            m_bar[0].lock()->SetDrawActive(true);
            m_bar[0].lock()->ChangeDescription(4);

            DisplayDialog(L"Tuto_txt3", m_dialog.lock());
            break;
        case Tutorial_HitTheCombos:
            m_progress[0] = 0;
            m_bar[0].lock()->SetDrawActive(true);
            m_bar[0].lock()->ChangeDescription(5);

            DisplayDialog(L"Tuto_txt4", m_dialog.lock());
            break;
        case Tutorial_ShootFromAfar:
            m_progress[0] = 0;
            m_bar[0].lock()->SetDrawActive(true);
            m_bar[0].lock()->ChangeDescription(6);

            DisplayDialog(L"Tuto_txt5", m_dialog.lock());
            break;
        case Tutorial_KillAsYouLike:
        {
            m_progress[0] = 0;
            m_bar[0].lock()->SetDrawActive(true);
            m_bar[0].lock()->ChangeDescription(7);

            const int currentEnemyNum = m_enemyMgr.lock()->GetEnemyVec(true).size();
            for (int i = currentEnemyNum; i < m_enemyNumFinalPhase; i++) {
                m_enemyMgr.lock()->InstEnemy<EnemyZako>(m_enemySpawnPos[i], Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
            }

            DisplayDialog(L"Tuto_txt6", m_dialog.lock());
        }
            break;
        case Tutorial_Cleared:
            m_progress[0] = 1;
            break;
        }

        m_tutorialPhase = phase;
    }

    //GamePhaseの変更
    void TutorialStage::UpdateGamePhase() {
        switch (m_gamePhase) {
        case GPhase_Start:
            //開始→ゲーム本編
            if (m_player.lock()->GetwalkFlag()) {
                m_gamePhase = GPhase_Playing;
            }
            break;
        case GPhase_Playing:
            //仮
            if (false) {
                m_gamePhase = GPhase_Result;
            }
            if (false) {
                m_gamePhase = GPhase_GameOver;
            }
        }
    }

    void TutorialStage::DisplayDialog(wstring key, shared_ptr<TutorialDialog> ui) {
        SetActorPause(true);
        ui->SetResource(key);
        ui->SetMovePos(Vec2(0, 0), .5f, Lerp::rate::EaseOut);
        ui->SetColorChange(Col4(1, 1, 1, 1), .5f, Lerp::rate::Linear);
    }

    void TutorialStage::OnUpdate()
    {
        auto& app = App::GetApp();
        auto KeyState = app->GetInputDevice().GetKeyState();
        auto pad = app->GetInputDevice().GetControlerVec()[0];

        UpdateGamePhase();
        EffectManager::Instance().InterfaceUpdate();

        ResetDeltaScaleToDefault();
        
        switch (m_gamePhase) {
        case GPhase_Playing:
            UpdateTutorialPhase();
            break;
        }

        //if (ConsiderGoToNextWave())
        //{
        //    m_fadeout.lock()->SetFadeOutFlag(true);
        //}
        //if (m_fadeout.lock()->GetBlackFlag())
        //{
        //    m_nextWaveFlag = true;
        //}
        //if (m_nextWaveFlag)
        //{
        //    m_fadeout.lock()->SetFadeInFlag(true);
        //    m_nextWaveFlag = false;

        //    WaveInitialize();
        //}

        if (m_waveCurrent == m_waveMax && ConsiderGameClear() && m_onceFlag == false)
        {
            m_sndMgr.lock()->StopBGM();
            m_onceFlag = true;
            m_scene.lock()->PostEvent(3.0f, GetThis<ObjectInterface>(), m_scene.lock(), L"ToGameClear");
        }

        if (ConsiderGameOver() && m_onceFlag == false)
        {
            m_sndMgr.lock()->StopBGM();
            m_onceFlag = true;
            m_scene.lock()->PostEvent(1.0f, GetThis<ObjectInterface>(), m_scene.lock(), L"ToGameOver");
        }

    }

    bool TutorialStage::ConsiderGameClear() {
        bool ret = false;
        ret |= (m_tutorialPhase == Tutorial_Cleared);

        return ret;
    }
    //=============================================================================
    // ゲージフレーム
    //=============================================================================

    void TutorialProgressFrame::OnCreate() {
        Col4 color(1, 1, 1, 1);

        m_vertices = {
            {Vec3(0, m_height, 0.0f), color, Vec2(0, 0)},
            {Vec3(m_width, m_height, 0.0f), color, Vec2(1, 0)},
            {Vec3(0, 0, 0.0f), color, Vec2(0, 1)},
            {Vec3(m_width, 0, 0.0f), color, Vec2(1, 1)},
        };
        vector<uint16_t> indices = {
            0, 1, 2,
            2, 1, 3,
        };

        m_drawComp = AddComponent<PCTSpriteDraw>(m_vertices, indices);
        m_drawComp->SetDiffuse(color);
        m_drawComp->SetTextureResource(m_resKey);
        m_drawComp->SetDrawActive(true);
        m_drawComp->SetSamplerState(SamplerState::LinearWrap);
        SetAlphaActive(true);
        SetDrawLayer(1);

        SetPosition(m_addPos);

        m_drawComp = AddComponent<PCTSpriteDraw>(m_vertices, indices);
        m_drawComp->SetTextureResource(m_resKey);


        auto stage = GetStage();
        m_bar = stage->AddGameObject<TutorialProgress>(L"Tuto_bar", GetThis<TutorialProgressFrame>());
        m_desc = stage->AddGameObject<TutorialDescription>(L"Tuto_barTxt1", Vec2(87.2f, 21.4f), GetThis<TutorialProgressFrame>());
    }

    //=============================================================================
    // ゲージ
    //=============================================================================

    void TutorialProgress::OnCreate() {
        Col4 color(1, 1, 1, 1);

        m_vertices = {
            {Vec3(0, m_height, 0.0f), color, Vec2(0, 0)},
            {Vec3((m_width * m_percentageDisplay), m_height, 0.0f), color, Vec2(1, 0)},
            {Vec3(m_bottomSlip, 0, 0.0f), color, Vec2(0, 1)},
            {Vec3((m_width * m_percentageDisplay) + m_bottomSlip, 0, 0.0f), color, Vec2(1, 1)},
        };
        vector<uint16_t> indices = {
            0, 1, 2,
            2, 1, 3,
        };

        GetComponent<Transform>()->SetParent(m_parent.lock());

        m_addPos = Vec3(96, 6, 0);

        m_drawComp = AddComponent<PCTSpriteDraw>(m_vertices, indices);
        m_drawComp->SetDiffuse(color);
        m_drawComp->SetTextureResource(m_resKey);
        m_drawComp->SetDrawActive(true);
        m_drawComp->SetSamplerState(SamplerState::LinearWrap);
        SetDrawLayer(1);
        SetAlphaActive(true);

        SetPosition(m_addPos);

        m_done = GetStage()->AddGameObject<TutorialDone>(L"Tuto_ok", GetThis<TutorialSpriteBase>());
    }

    void TutorialProgress::OnUpdate() {
        float delta = App::GetApp()->GetElapsedTime();

        if (m_percentageDisplay != m_percentage) {
            float speed = m_progressSpeed * delta;

            //ぴったり合わせる
            if (speed >= abs(m_percentageDisplay - m_percentage)) {
                m_percentageDisplay = m_percentage;
            }
            else {
                //減ることはないだろうけど一応
                float plus = m_percentageDisplay < m_percentage ? speed : -speed;
                m_percentageDisplay += plus;
            }
            
            if (m_percentageDisplay > 1.0f) {
                m_percentageDisplay = 1.0f;
            }

            m_vertices[1].position = Vec3((m_width * m_percentageDisplay), m_height, 0.0f);
            m_vertices[3].position = Vec3((m_width * m_percentageDisplay) + m_bottomSlip, 0, 0.0f);
            m_drawComp->UpdateVertices(m_vertices);
        }
    }

    //=============================================================================
    // OKの表示
    //=============================================================================

    void TutorialDone::OnCreate() {
        Col4 color(1, 1, 1, 1);

        m_vertices = {
            {Vec3(0, m_height, 0.0f), color, Vec2(0, 0)},
            {Vec3(m_width, m_height, 0.0f), color, Vec2(1, 0)},
            {Vec3(0, 0, 0.0f), color, Vec2(0, 1)},
            {Vec3(m_width, 0, 0.0f), color, Vec2(1, 1)},
        };
        vector<uint16_t> indices = {
            0, 1, 2,
            2, 1, 3,
        };

        GetComponent<Transform>()->SetParent(m_parent.lock());

        m_addPos = Vec3(26.3f, -2.8, 0);

        m_drawComp = AddComponent<PCTSpriteDraw>(m_vertices, indices);
        m_drawComp->SetDiffuse(color);
        m_drawComp->SetTextureResource(m_resKey);
        m_drawComp->SetDrawActive(false);
        m_drawComp->SetSamplerState(SamplerState::LinearWrap);
        SetDrawLayer(2);
        SetAlphaActive(true);

        SetPosition(m_addPos);

    }

    //=============================================================================
    // チュートリアルの内容
    //=============================================================================

    void TutorialDescription::OnCreate() {
        
        SetUpVertices();

        GetComponent<Transform>()->SetParent(m_parent.lock());

        vector<uint16_t> indices = {
            0, 1, 2,
            2, 1, 3,
        };

        m_drawComp = AddComponent<PCTSpriteDraw>(m_vertices, indices);
        m_drawComp->SetDiffuse(Col4(1,1,1,1));
        m_drawComp->SetDrawActive(true);

        m_drawComp = AddComponent<PCTSpriteDraw>(m_vertices, indices);
        m_drawComp->SetTextureResource(m_resKey);

        m_drawComp->SetSamplerState(SamplerState::LinearWrap);
        SetAlphaActive(true);
        SetDrawLayer(1);
    }


    //=============================================================================
    // ダイアログ
    //=============================================================================

    void TutorialDialog::MoveTowardTargetPos() {
        if (m_isMoveDone) {
            return;
        }
        float delta = App::GetApp()->GetElapsedTime();

        m_elapsedTime += delta;
        if (m_elapsedTime > m_arriveTime) {
            m_elapsedTime = m_arriveTime;
            m_isMoveDone = true;
        }

        float x = Lerp::CalculateLerp(m_originPos.x, m_targetPos.x, 0, m_arriveTime, m_elapsedTime, m_lerpRate);
        float y = Lerp::CalculateLerp(m_originPos.y, m_targetPos.y, 0, m_arriveTime, m_elapsedTime, m_lerpRate);

        m_currentPos = Vec2(x, y);

        m_movedVertices = m_vertices;
        for (int i = 0; i < m_movedVertices.size(); i++) {
            m_movedVertices[i].position += Vec3(x, y, 0);
        }
    }

    void TutorialDialog::UpdateColor() {
        if (m_changeColorDone) {
            return;
        }
        float delta = App::GetApp()->GetElapsedTime();

        m_color_elapsedTime += delta;
        if (m_color_elapsedTime > m_color_arriveTime) {
            m_color_elapsedTime = m_color_arriveTime;
            m_changeColorDone = true;
        }

        float r = Lerp::CalculateLerp(m_originColor.x, m_targetColor.x, 0, m_color_arriveTime, m_color_elapsedTime, m_color_lerpRate);
        float g = Lerp::CalculateLerp(m_originColor.y, m_targetColor.y, 0, m_color_arriveTime, m_color_elapsedTime, m_color_lerpRate);
        float b = Lerp::CalculateLerp(m_originColor.z, m_targetColor.z, 0, m_color_arriveTime, m_color_elapsedTime, m_color_lerpRate);
        float a = Lerp::CalculateLerp(m_originColor.w, m_targetColor.w, 0, m_color_arriveTime, m_color_elapsedTime, m_color_lerpRate);

        m_isInvisible = a == 0 ? true : false;

        m_drawComp->SetDiffuse(Col4(r, g, b, a));
    }

    void TutorialDialog::OnCreate() {
        const Col4 color(1, 1, 1, 1);
        const float windowWidth = 1280.0f;
        const float windowHeight = 720.0f;

        m_vertices = {
            {Vec3(-windowWidth * 0.5f,  windowHeight * 0.5f, 0.1f), color, Vec2(0, 0)},
            {Vec3(windowWidth * 0.5f,  windowHeight * 0.5f, 0.1f), color, Vec2(1, 0)},
            {Vec3(-windowWidth * 0.5f, -windowHeight * 0.5f, 0.1f), color, Vec2(0, 1)},
            {Vec3(windowWidth * 0.5f, -windowHeight * 0.5f, 0.1f), color, Vec2(1, 1)},
        };
        m_movedVertices = m_vertices;

        vector<uint16_t> indices = {
            0, 1, 2,
            2, 1, 3,
        };

        m_drawComp = AddComponent<PCTSpriteDraw>(m_vertices, indices);
        m_drawComp->SetTextureResource(m_resKey);

        m_drawComp->SetSamplerState(SamplerState::LinearWrap);
        SetAlphaActive(true);
        SetDrawLayer(1);
        m_drawComp->SetDiffuse(Col4(1, 1, 1, 0));

        auto wave = GetWaveStage(false);
        if (wave) {
            wave->SetActorPause(true);
        }
    }

    void TutorialDialog::OnUpdate() {
        m_pad = App::GetApp()->GetInputDevice().GetControlerVec()[0];
        m_key = App::GetApp()->GetInputDevice().GetKeyState();

        MoveTowardTargetPos();
        UpdateColor();

        m_drawComp->UpdateVertices(m_movedVertices);

        //押されたらポーズ解除
        bool canProceed = GetMoveDone() && GetChangeColorDone();
        bool pushed = (m_pad.wPressedButtons & XINPUT_GAMEPAD_A || m_key.m_bPressedKeyTbl[VK_RETURN]);
        if (canProceed && pushed && !m_isInvisible) {
            auto wave = GetWaveStage(false);
            if (wave) {
                wave->SetActorPause(false);
                SetMovePos(Vec2(0, -50), 1, Lerp::rate::EaseOut);
                SetColorChange(Col4(1, 1, 1, 0), .5, Lerp::rate::Linear);
            }
        }
    }

}