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

		auto delta = App::GetApp()->GetElapsedTime();

		//StartMoive=======================================================================================
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
		//===================================================================================


		// BossMovie=========================================================================
		// ボスムービー用のアニメーション更新
		if (m_startMovie == Movie_Boss)
		{
			auto boss = GetStage()->GetSharedGameObject<EnemyBase>(L"Boss");

			// ボスのアニメーション再生
			boss->GetComponent<PNTBoneModelDraw>()->UpdateAnimation(delta);
		}
		
		//カメラを動かし終わったらプレイ状態にする(カメラしか動かさないため)
		if (m_cameraEnd && m_startMovie == Movie_Boss)
		{
			auto waveStage = GetWaveStage(true);

			// プレイフェーズに移行
			waveStage->SetGamePhase(waveStage->GPhase_Playing);

			// ムービーをしなくなったため
			m_startMovie = Movie_None;
			m_cameraEnd = false;

			//ポーズ状態解除
			waveStage->SetActorPause(false, false);
		}
		//==========================================================================


		//Movie_BossDie=============================================================
		
		//ボスを倒した際の時間の流れを遅くする
		if (m_startMovie == Movie_BossDie)
		{
			auto waveStage = GetWaveStage(true);
			waveStage->SetDeltaScaleCount(0.5f);
		}

		//ボスを倒したときのボイス
		if (m_startMovie == Movie_BossDie && m_cameraEnd)
		{
			//オーディオマネージャーの取得
			auto SEManager = App::GetApp()->GetXAudio2Manager();
			//ミッション完了SE
			SEManager->Start(L"ClearVoiceSE", false, 0.8f);

			//フェーズの変更
			m_startMovie = Movie_BossDie_Second;
		}

		//二番目のボイス
		if (m_startMovie == Movie_BossDie_Second)
		{
			m_timeCount += delta;

			if (m_timeCount >= m_maxTimeOfBossDieSecond)
			{
				//オーディオマネージャーの取得
				auto SEManager = App::GetApp()->GetXAudio2Manager();
				//ミッション完了SE(二番目)
				SEManager->Start(L"ClearVoice2SE", false, 0.8f);

				//リセット
				m_timeCount = 0.0f;
				m_startMovie = Movie_BossDie_Three;
				m_cameraEnd = false;
				auto waveStage = GetWaveStage(true);
				waveStage->SetDeltaScaleCount(1.0f);
				auto firstStage = dynamic_pointer_cast<StageFirst>(waveStage);
			}
		}

		//ボイスが流れ終わった後にゲームクリアシーンに行くように調整
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
				//ゲームクリアしたことをステージに伝える
				firstStage->SetGameClear(true);
			}
		}
		//==========================================================================================================
	}

	// ステージ開始時のムービー
	void RT_MovieController::StartMovie()
	{
		auto waveStage = GetWaveStage(true);
		//カットシーンフェーズに移行
		waveStage->SetGamePhase(waveStage->GPhase_CutScene);

		auto castCameraManager = m_cameraManager.lock();
		auto castPlayer = m_player.lock();

		//Playerにムービー用ステートになった後、その処理が終わったらカメラがムービー用の動きをする
		castPlayer->ChangeState(L"StartMovie");
		castCameraManager->ChangeState(L"StartMovie_First");

		//ステージ開始時のムービーが始まった
		m_startMovie = StartMovie_First;

	}

	// ボスムービー
	void RT_MovieController::BossMovie()
	{
		auto waveStage = GetWaveStage(true);

		//ムービー中はPlayerや敵が動かないようにステージをポーズ状態にする
		waveStage->SetActorPause(true, false);

		//カットシーンフェーズに移行
		waveStage->SetGamePhase(waveStage->GPhase_CutScene);

		// ボスムービー用にボスのアニメーションを変更させる
		auto boss = GetStage()->GetSharedGameObject<EnemyBase>(L"Boss");
		boss->ChangeAnim(L"Walk");

		// カメラをボスムービー用にチェンジ
		auto castCameraManager = m_cameraManager.lock();
		castCameraManager->ChangeState(L"BossMovie_First");

		m_startMovie = Movie_Boss;
	}

	//ボスを倒したときのムービー
	void RT_MovieController::BossDieMovie()
	{
		auto waveStage = GetWaveStage(true);

		waveStage->SetDeltaScale(0.5f);

		////カットシーンフェーズに移行
		//waveStage->SetGamePhase(waveStage->GPhase_CutScene);

		// カメラのステートを変更
		auto castCameraManager = m_cameraManager.lock();
		castCameraManager->ChangeState(L"BossDieMovie_First");

		
		m_startMovie = Movie_BossDie;


	}
}