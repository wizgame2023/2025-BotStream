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
		if (!castCameraManager || !castPlayer)
		{
			GetStage()->RemoveGameObject<RT_MovieController>(GetThis<RT_MovieController>());
		}

		//StartMoive
		//Player側の動きが終わったらカメラが動く
		if (m_playerEnd && m_startMovie == StartMovie_First)
		{
			castCameraManager->ChangeState(L"StartMovie_Second");
			m_startMovie = StartMovie_Second;
		}
		//やりたい動きが終わったらStartMovieの処理は終了
		if (m_cameraEnd && m_startMovie == StartMovie_Second)
		{
			//リセット
			m_playerEnd = false;
			m_cameraEnd = false;
			m_startMovie = Movie_None;

			// デルタスケールを元に戻す
			auto WaveStage = m_player.lock()->GetWaveStage(true);
			WaveStage->SetDeltaScale(1.0f);
			// ステージのフェーズをPlaying(プレイ状態)にする
			WaveStage->SetGamePhase(WaveStageBase::GamePhase::GPhase_Playing);
		}
		//
	}

	//ステージ開始時のムービー
	void RT_MovieController::StartMovie()
	{
		auto castCameraManager = m_cameraManager.lock();
		auto castPlayer = m_player.lock();

		//Playerにムービー用ステートになった後、その処理が終わったらカメラがムービー用の動きをする
		castPlayer->ChangeState(L"StartMovie");
		castCameraManager->ChangeState(L"StartMovie_First");

		//ステージ開始時のムービーが始まった
		m_startMovie = StartMovie_First;

	}
}