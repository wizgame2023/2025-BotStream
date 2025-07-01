/*!
@file StageThird.cpp
@brief Derived WaveStage
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	// =============================================================
	// StageThird�擪
	// =============================================================
	void StageThird::OnCreate()
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
		for (int i = 0; i <= 10; i++)
		{
			enemyVariation.push_back(EVar_Aerial);
		}

		m_enemyMgr = AddGameObject<EnemyManager>(enemyVariation);
		SetSharedGameObject(L"EnemyManager", m_enemyMgr.lock());

		m_boss = AddGameObject<BossFirst>(Vec3(0.0f, 2.0f, 250.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		SetSharedGameObject(L"Boss", m_boss.lock());
		dynamic_pointer_cast<BossFirst>(m_boss.lock())->ChangeHPMax(1200);
		dynamic_pointer_cast<BossFirst>(m_boss.lock())->ChangeArmorMax(400.0f, 10.0f);

		//wave1�G
		// ���̃|�W�V���� Vec3(0.0f, -3.0f, -260.0f), �傫�� Vec3(120.0f, 3.0f, 120.0f)
		m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(0.0f, 2.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
		m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(10.0f, 2.0f, -255.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
		m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(-10.0f, 2.0f, -235.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
		m_enemyMgr.lock()->InstEnemy<EnemyZako>(Vec3(-5.0f, 2.0f, -235.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
		m_enemyMgr.lock()->InstEnemy<EnemyZakoLong>(Vec3(20.0f, 2.0f, -265.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
		m_enemyMgr.lock()->InstEnemy<EnemyZakoLong>(Vec3(-20.0f, 2.0f, -245.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
		m_enemyMgr.lock()->InstEnemy<EnemyZakoLong>(Vec3(30.0f, 2.0f, -225.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
		m_enemyMgr.lock()->InstEnemy<EnemyZakoFlying>(Vec3(-20.0f, 3.0f, -225.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
		m_enemyMgr.lock()->InstEnemy<EnemyZakoFlying>(Vec3(20.0f, 3.0f, -225.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));


		//����Wave�ɍs�����߂̈ÈŐ���
		auto darkObj = AddGameObject<DarkObj>(Vec3(-0.0f, 3.0f, -200.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(10.0f, 10.0f, 10.0f));
		m_darkObjs.push_back(darkObj);

		//Wave1�̃h�A����
		auto doorLeft = AddGameObject<Door>(Vec3(5.0f, 10.0f, 1.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(-2.5f, 3.0f, -200.5f));
		m_doors.push_back(doorLeft);
		auto doorRight = AddGameObject<Door>(Vec3(5.0f, 10.0f, 1.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(+2.5f, 3.0f, -200.5f));
		m_doors.push_back(doorRight);

		CreateManagerObjects();

		//���[�r�R���g���[���[�쐬
		if (!m_player.lock()) return;
		m_movieController = AddGameObject<RT_MovieController>(m_player.lock(), GetSharedGameObject<CameraManager>(L"CameraManager"));
		SetSharedGameObject(L"MovieController", m_movieController);

		//�X�e�[�W�J�n���̃��[�r�J�n
		m_movieController->StartMovie();

		m_gamePhase = GamePhase::GPhase_Start;

	}

	void StageThird::OnUpdate()
	{
		//WaveStageBase::OnUpdate();

		auto& app = App::GetApp();
		auto KeyState = app->GetInputDevice().GetKeyState();
		auto pad = app->GetInputDevice().GetControlerVec()[0];

		ResetDeltaScaleToDefault();
		UpdateGamePhase();

		EffectManager::Instance().InterfaceUpdate();

		if (ConsiderGoToNextWave())
		{
			// �h�A�𓮂���
			DoorsMove(m_waveCurrent);
			// �Èł̗L����
			ActivateDark(m_waveCurrent);
		}
		if (m_fadeout.lock()->GetBlackFlag())
		{
			m_nextWaveFlag = true;
		}
		if (m_nextWaveFlag)
		{
			m_fadeout.lock()->SetFadeInFlag(true);
			m_nextWaveFlag = false;

			WaveInitialize();
		}

		if (m_waveCurrent == m_waveMax && ConsiderBossCheck() && m_onceFlag == false)
		{
			m_movieController->BossDieMovie();

			m_sndMgr.lock()->StopBGM();
			m_onceFlag = true;
		}

		if (ConsiderGameOver() && m_onceFlag == false)
		{
			m_sndMgr.lock()->StopBGM();
			m_onceFlag = true;
			m_fadeout.lock()->SetFadeOutFlag(true);// �u���b�N�A�E�g
			m_scene.lock()->PostEvent(1.2f, GetThis<ObjectInterface>(), m_scene.lock(), L"ToGameOver");
		}

		//�Q�[���N���A����
		if (m_gameClearFlag)
		{
			m_fadeout.lock()->SetFadeOutFlag(true);// �u���b�N�A�E�g
			m_scene.lock()->PostEvent(1.2f, GetThis<ObjectInterface>(), m_scene.lock(), L"ToGameClear");
		}


		//�t�F�[�Y�̏�Ԃɂ���ď������ς��
		if (m_gamePhase == GPhase_Start)
		{
			//�A�N�^�[��Updeta�o���Ȃ�����
			//SetActorPause(true);
			m_deltaScaleCount = 1.0f; // deltaScale�����ɖ߂�Ȃ��悤�ɂ���
			m_deltaScale = 0.0f;
		}
		if (m_gamePhase == GPhase_Playing)
		{
			m_deltaScale = 1.0f;
			m_deltaScaleCount = 0.0f; // deltaScale�����ɖ߂�Ȃ��悤�ɂ���

			//SetActorPause(false);
		}

	}


	//Wave���؂�ւ��ۂɍs������
	void StageThird::WaveInitialize() {
		switch (m_waveCurrent) {
			// ------- 1 -> 2 -------------------------------------------------------------
		case 1:
		{
			//�v���C���[�̈ʒu��������
			SetPlayerTransform(Vec3(0.0f, 3.0f, -40.0f), Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f));

			m_enemyMgr.lock()->InstEnemy(Vec3(0.0f, 2.0f, 0.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			m_enemyMgr.lock()->InstEnemy(Vec3(10.0f, 2.0f, -10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			m_enemyMgr.lock()->InstEnemy(Vec3(-10.0f, 2.0f, -20.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			m_enemyMgr.lock()->InstEnemy(Vec3(20.0f, 2.0f, -15.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			m_enemyMgr.lock()->InstEnemy(Vec3(-20.0f, 2.0f, 0.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			m_enemyMgr.lock()->InstEnemy<EnemyZakoLong>(Vec3(20.0f, 2.0f, 10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			m_enemyMgr.lock()->InstEnemy<EnemyZakoLong>(Vec3(-20.0f, 2.0f, 20.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			m_enemyMgr.lock()->InstEnemy<EnemyZakoLong>(Vec3(-30.0f, 2.0f, 5.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			//m_enemyMgr.lock()->InstEnemy<EnemyZakoLong>(Vec3(0.0f, 2.0f, 15.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));

			////m_enemyMgr.lock()->InstEnemy(Vec3(0.0f, 2.0f, 0.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			////m_enemyMgr.lock()->InstEnemy(Vec3(10.0f, 2.0f, 30.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			////m_enemyMgr.lock()->InstEnemy(Vec3(-10.0f,2.0f, -20.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			////m_enemyMgr.lock()->InstEnemy(Vec3(20.0f, 2.0f, 10.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			//m_enemyMgr.lock()->InstEnemy<EnemyZakoFlying>(Vec3(0.0f, 2.0f, 35.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			m_enemyMgr.lock()->InstEnemy<EnemyZakoFlying>(Vec3(30.0f, 2.0f, 35.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			m_enemyMgr.lock()->InstEnemy<EnemyZakoFlying>(Vec3(-30.0f, 2.0f, 35.0f), Vec3(0.0f, -5.0f, 0.0f), Vec3(5.0f, 5.0f, 5.0f));
			//m_enemyMgr.lock()->InstEnemy<EnemyZakoFlying>(Vec3(-30.0f,))

			//����Wave�ɍs�����߂̈ÈŐ���
			auto darkObj = AddGameObject<DarkObj>(Vec3(-0.0f, 3.0f, 60.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(10.0f, 10.0f, 10.0f));
			m_darkObjs.push_back(darkObj);

			//Wave1�̃h�A����
			auto doorLeft = AddGameObject<Door>(Vec3(5.0f, 10.0f, 1.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(-2.5f, 3.0f, 59.5f));
			m_doors.push_back(doorLeft);
			auto doorRight = AddGameObject<Door>(Vec3(5.0f, 10.0f, 1.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(+2.5f, 3.0f, 59.5f));
			m_doors.push_back(doorRight);


			break;
		}
		// ----------------------------------------------------------------------------

		// ------- 2 -> 3 -------------------------------------------------------------
		case 2:
		{
			m_bossGauge->ClearBossGaugeUI(false);

			////�v���C���[�̈ʒu��������
			SetPlayerTransform(Vec3(0.0f, 3.0f, 195.0f), Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f));

			m_enemyMgr.lock()->InstBoss(dynamic_pointer_cast<EnemyBase>(m_boss.lock()));

			GetSharedGameObject<SoundManager>(L"SoundManager")->StopBGM();
			GetSharedGameObject<SoundManager>(L"SoundManager")->PlayBGM(4);

			//�{�X���[�r�[�Đ�
			m_movieController->BossMovie();

			break;
		}
		// ----------------------------------------------------------------------------
		}

		m_waveCurrent++;
	}
	// END =========================================================

}