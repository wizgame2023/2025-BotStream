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

        m_player = AddGameObject<Player>(Vec3(0.0f, 3.0f, -305.0f), Vec3(0.0f, 5.0f, 0.0f), Vec3(1.0f, 2.0f, 1.0f));
        SetSharedGameObject(L"Player", m_player.lock());

        //Enemyマネージャのテスト
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

        //wave1敵
        m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(0.0f, 2.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(10.0f, 2.0f, -255.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(-10.0f, 2.0f, -235.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(20.0f, 2.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(-20.0f, 2.0f, -245.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(30.0f, 2.0f, -225.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
        m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(-30.0f, 2.0f, -225.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));

        CreateManagerObjects();

        m_gamePhase = GamePhase::GPhase_Start;
    }

}