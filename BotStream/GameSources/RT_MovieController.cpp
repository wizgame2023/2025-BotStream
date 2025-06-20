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

		//StartMoive
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
		//
	}

	//�X�e�[�W�J�n���̃��[�r�[
	void RT_MovieController::StartMovie()
	{
		auto castCameraManager = m_cameraManager.lock();
		auto castPlayer = m_player.lock();

		//Player�Ƀ��[�r�[�p�X�e�[�g�ɂȂ�����A���̏������I�������J���������[�r�[�p�̓���������
		castPlayer->ChangeState(L"StartMovie");
		castCameraManager->ChangeState(L"StartMovie_First");

		//�X�e�[�W�J�n���̃��[�r�[���n�܂���
		m_startMovie = StartMovie_First;

	}
}