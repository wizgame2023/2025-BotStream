/*!
@file RT_MovieController.cpp
@brief リアルタイムのムービー制御処理
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

	//ステージ開始時のムービー
	void RT_MovieController::StartMovie()
	{
		auto castCameraManager = m_cameraManager.lock();
		auto castPlayer = m_player.lock();

		//Playerにムービー用ステートになった後、その処理が終わったらカメラがムービー用の動きをする
		castPlayer->ChangeState(L"StartMovie");

		if (m_playerEnd)
		{
			castCameraManager->ChangeState(L"StartMovie");
		}
	}
}