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
		if (castCameraManager || castPlayer)
		{
			GetStage()->RemoveGameObject<RT_MovieController>(GetThis<RT_MovieController>());
		}
	}

	//�X�e�[�W�J�n���̃��[�r�[
	void RT_MovieController::StartMovie()
	{
		auto castCameraManager = m_cameraManager.lock();
		auto castPlayer = m_player.lock();

		//Player�Ƀ��[�r�[�p�X�e�[�g�ɂȂ�����A���̏������I�������J���������[�r�[�p�̓���������
		castPlayer->ChangeState(L"StartMovie");

		if (m_playerEnd)
		{
			castCameraManager->ChangeState(L"StartMovie");
		}
	}
}