/*!
@file RT_MovieController.cpp
@brief ���A���^�C���̃��[�r�[���䏈��
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	
	void RT_MovieController::OnCreate()
	{
		
	}

	void RT_MovieController::OnUpdate()
	{
		auto castCameraManager = m_cameraManager.lock();
		auto castPlayer = m_player.lock();
		if (!castCameraManager || !castPlayer)
		{
			GetStage()->RemoveGameObject<RT_MovieController>(GetThis<RT_MovieController>());
		}

		auto delta = App::GetApp()->GetElapsedTime();

		//StartMoive=======================================================================================
		//Player���̓������I�������J����������
		if (m_playerEnd && m_startMovie == StartMovie_First)
		{
			castCameraManager->ChangeState(L"StartMovie_Second");
			m_startMovie = StartMovie_Second;
		}
		//��肽���������I�������StartMovie�̏����͏I��
		if (m_cameraEnd && m_startMovie == StartMovie_Second)
		{
			//���Z�b�g
			m_playerEnd = false;
			m_cameraEnd = false;
			m_startMovie = Movie_None;

			// �f���^�X�P�[�������ɖ߂�
			auto WaveStage = m_player.lock()->GetWaveStage(true);
			WaveStage->SetDeltaScale(1.0f);
			// �X�e�[�W�̃t�F�[�Y��Playing(�v���C���)�ɂ���
			WaveStage->SetGamePhase(WaveStageBase::GamePhase::GPhase_Playing);
		}
		//===================================================================================


		// BossMovie=========================================================================
		// �{�X���[�r�[�p�̃A�j���[�V�����X�V
		if (m_startMovie == Movie_Boss)
		{
			auto boss = GetStage()->GetSharedGameObject<EnemyBase>(L"Boss");

			// �{�X�̃A�j���[�V�����Đ�
			boss->GetComponent<PNTBoneModelDraw>()->UpdateAnimation(delta);
		}
		
		//�J�����𓮂����I�������v���C��Ԃɂ���(�J���������������Ȃ�����)
		if (m_cameraEnd && m_startMovie == Movie_Boss)
		{
			auto waveStage = GetWaveStage(true);

			// �v���C�t�F�[�Y�Ɉڍs
			waveStage->SetGamePhase(waveStage->GPhase_Playing);

			// ���[�r�[�����Ȃ��Ȃ�������
			m_startMovie = Movie_None;
			m_cameraEnd = false;

			//�|�[�Y��ԉ���
			waveStage->SetActorPause(false, false);
		}
		//==========================================================================


		//Movie_BossDie=============================================================
		
		//�{�X��|�����ۂ̎��Ԃ̗����x������
		if (m_startMovie == Movie_BossDie)
		{
			auto waveStage = GetWaveStage(true);
			waveStage->SetDeltaScaleCount(0.5f);
		}

		//�{�X��|�����Ƃ��̃{�C�X
		if (m_startMovie == Movie_BossDie && m_cameraEnd)
		{
			//�I�[�f�B�I�}�l�[�W���[�̎擾
			auto SEManager = App::GetApp()->GetXAudio2Manager();
			//�~�b�V��������SE
			SEManager->Start(L"ClearVoiceSE", false, 0.8f);

			//�t�F�[�Y�̕ύX
			m_startMovie = Movie_BossDie_Second;
		}

		//��Ԗڂ̃{�C�X
		if (m_startMovie == Movie_BossDie_Second)
		{
			m_timeCount += delta;

			if (m_timeCount >= m_maxTimeOfBossDieSecond)
			{
				//�I�[�f�B�I�}�l�[�W���[�̎擾
				auto SEManager = App::GetApp()->GetXAudio2Manager();
				//�~�b�V��������SE(��Ԗ�)
				SEManager->Start(L"ClearVoice2SE", false, 0.8f);

				//���Z�b�g
				m_timeCount = 0.0f;
				m_startMovie = Movie_BossDie_Three;
				m_cameraEnd = false;
				auto waveStage = GetWaveStage(true);
				waveStage->SetDeltaScaleCount(1.0f);
				auto firstStage = dynamic_pointer_cast<StageFirst>(waveStage);
			}
		}

		//�{�C�X������I�������ɃQ�[���N���A�V�[���ɍs���悤�ɒ���
		if (m_startMovie == Movie_BossDie_Three)
		{
			m_timeCount += delta;

			float maxTimeOfBossDieThree = 1.5f;
			if (m_timeCount >= maxTimeOfBossDieThree)
			{
				m_startMovie = Movie_None;
				m_timeCount = 0.0f;

				auto waveStage = GetWaveStage(true);
				auto firstStage = dynamic_pointer_cast<StageFirst>(waveStage);
				//�Q�[���N���A�������Ƃ��X�e�[�W�ɓ`����
				firstStage->SetGameClear(true);
			}
		}
		//==========================================================================================================
	}

	// �X�e�[�W�J�n���̃��[�r�[
	void RT_MovieController::StartMovie()
	{
		auto waveStage = GetWaveStage(true);
		//�J�b�g�V�[���t�F�[�Y�Ɉڍs
		waveStage->SetGamePhase(waveStage->GPhase_CutScene);

		auto castCameraManager = m_cameraManager.lock();
		auto castPlayer = m_player.lock();

		//Player�Ƀ��[�r�[�p�X�e�[�g�ɂȂ�����A���̏������I�������J���������[�r�[�p�̓���������
		castPlayer->ChangeState(L"StartMovie");
		castCameraManager->ChangeState(L"StartMovie_First");

		//�X�e�[�W�J�n���̃��[�r�[���n�܂���
		m_startMovie = StartMovie_First;

	}

	// �{�X���[�r�[
	void RT_MovieController::BossMovie()
	{
		auto waveStage = GetWaveStage(true);

		//���[�r�[����Player��G�������Ȃ��悤�ɃX�e�[�W���|�[�Y��Ԃɂ���
		waveStage->SetActorPause(true, false);

		//�J�b�g�V�[���t�F�[�Y�Ɉڍs
		waveStage->SetGamePhase(waveStage->GPhase_CutScene);

		// �{�X���[�r�[�p�Ƀ{�X�̃A�j���[�V������ύX������
		auto boss = GetStage()->GetSharedGameObject<EnemyBase>(L"Boss");
		boss->ChangeAnim(L"Walk");

		// �J�������{�X���[�r�[�p�Ƀ`�F���W
		auto castCameraManager = m_cameraManager.lock();
		castCameraManager->ChangeState(L"BossMovie_First");

		m_startMovie = Movie_Boss;
	}

	//�{�X��|�����Ƃ��̃��[�r�[
	void RT_MovieController::BossDieMovie()
	{
		auto waveStage = GetWaveStage(true);

		waveStage->SetDeltaScale(0.5f);

		////�J�b�g�V�[���t�F�[�Y�Ɉڍs
		//waveStage->SetGamePhase(waveStage->GPhase_CutScene);

		// �J�����̃X�e�[�g��ύX
		auto castCameraManager = m_cameraManager.lock();
		castCameraManager->ChangeState(L"BossDieMovie_First");

		
		m_startMovie = Movie_BossDie;


	}
}