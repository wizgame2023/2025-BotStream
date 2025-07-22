/*!
@file CameraState.cpp
@brief カメラ挙動のステート
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	/// <summary>
	/// カメラのステートの元となるクラス
	/// </summary>
	void CameraStateBase::Enter()
	{
		//コントローラーを受け取る
		auto inputDevice = App::GetApp()->GetInputDevice();
		m_controller = inputDevice.GetControlerVec()[0];
		m_controllerVec = Vec2(m_controller.fThumbRX, m_controller.fThumbRY);
	}
	
	void CameraStateBase::Update(float deltaTime)
	{
		//近接戦闘していいかのフラグを受け取る
		m_meleeFlag = m_cameraManager->GetMeleeFlag();
		//現在どの攻撃をしているか受け取る
		m_gunNow = m_cameraManager->GetGunNow();
		m_meleeNow = m_cameraManager->GetMeleeNow();

		//コントローラーを受け取る
		auto inputDevice = App::GetApp()->GetInputDevice();
		m_controller = inputDevice.GetControlerVec()[0];
		m_controllerVec = Vec2(m_controller.fThumbRX, m_controller.fThumbRY);

		//ステージ取得
		m_stage = m_cameraManager->GetStage();

		////慣性付きの回転処理
		//m_cameraManager->InertialRotation();
		//X軸回転の制限処理
		m_cameraManager->CameraAngleXLimit(XMConvertToRadians(120.0f), XMConvertToRadians(60.0f));

		//角度の調整0~360度までしか出ないようにする
		m_cameraManager->AdjustmentAngle();
		////カメラの位置更新
		//m_cameraManager->CameraPosUpdate();
	}

	void CameraStateBase::Exit()
	{

	}
	//////////////////////////////////////////////////


	///カメラの通常状態のステート(近接モード)
	void CameraNomalState::Enter()
	{
		CameraStateBase::Enter();
	}

	void CameraNomalState::Update(float deltaTime)
	{
		CameraStateBase::Update(deltaTime);

		//カメラの位置更新
		m_cameraManager->CameraPosUpdate(8, 0, 17.0f, 100.0f,CameraManager::CameraMoveMode::DirectMove);

		//通常モード時のカメラ操作処理
		m_cameraManager->CameraControlNomalMode();
		//慣性付きの回転処理
		m_cameraManager->InertialRotation();

		//現在ステート状態を渡す
		m_cameraManager->SetStateMode(CameraManager::DebagState::Normal);

		//もし,LBボタンを押していたら銃ステートに移行する
		if (!m_meleeNow)
		{
			if (!m_meleeFlag)
			{
				m_cameraManager->ChangeState(L"NormalToGun");
			}
		}

		//右スティックを押したら、移動ステートに移行する
		if (m_controller.wPressedButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
		{
			m_cameraManager->ChangeState(L"Reset");
		}
	}

	void CameraNomalState::Exit()
	{

	}
	//


	//カメラの銃モードのステート
	void CameraGunState::Enter()
	{
		CameraStateBase::Enter();
	}

	void CameraGunState::Update(float deltaTime)
	{	
		CameraStateBase::Update(deltaTime);

		//もし,LBボタンを押していなかったら通常モードに戻る
		if (m_meleeFlag)
		{
			m_cameraManager->ChangeState(L"GunToNormal");
			return;
		}

		//現在ステート状態を渡す
		m_cameraManager->SetStateMode(CameraManager::DebagState::Gun);

		//カメラの位置更新
		m_cameraManager->CameraPosUpdate(3.0f, 3.0f, 8.0f,100.0f, CameraManager::CameraMoveMode::DirectMove);
		//慣性付きの回転処理
		m_cameraManager->InertialRotation(0.7f, 60.0f);

		//銃モード時のカメラ操作処理
		m_cameraManager->CameraControlShotMode();
	}

	void CameraGunState::Exit()
	{

	}
	//


	//カメラのリセットモードのステート
	void CameraResetState::Enter()
	{
		CameraStateBase::Enter();
	}

	void CameraResetState::Update(float deltaTime)
	{
		CameraStateBase::Update(deltaTime);

		//カメラの位置更新
		m_cameraManager->CameraPosUpdate();

		//移動モード時のカメラ操作処理
		m_cameraManager->CameraControlTransitionMode();
	}

	void CameraResetState::Exit()
	{

	}
	//


	//カメラが通常から銃モードに切り替えるための移動ステート
	void CameraNormalToGunState::Enter()
	{
		CameraStateBase::Enter();
	}

	void CameraNormalToGunState::Update(float deltaTime)
	{
		CameraStateBase::Update(deltaTime);

		//もし、LBボタンを押していなかったら通常モードに移行しようとする
		if (m_meleeFlag)
		{
			m_cameraManager->ChangeState(L"GunToNormal");
			return;
		}

		//現在ステート状態を渡す
		m_cameraManager->SetStateMode(CameraManager::DebagState::NormalToGun);

		//カメラの位置更新
		auto moveEnd = m_cameraManager->CameraPosUpdate(3.0f, 3.0f, 8.0f, 40.0f);
		//慣性付きの回転処理
		m_cameraManager->InertialRotation(0.7f, 60.0f);
		//銃モード時のカメラ操作処理
		m_cameraManager->CameraControlShotMode();

		//移動処理が終わったらステートを銃モードに変更する
		if (moveEnd)
		{
			m_cameraManager->ChangeState(L"Gun");
		}
	}

	void CameraNormalToGunState::Exit()
	{

	}
	//


	//銃モードから通常モードに戻る処理
	void CameraGunToNormalState::Enter()
	{

	}

	void CameraGunToNormalState::Update(float deltaTime)
	{
		CameraStateBase::Update(deltaTime);

		//もし、LBボタンを押していたら銃モードに移行しようとする
		if (!m_meleeFlag)
		{
			m_cameraManager->ChangeState(L"NormalToGun");
			return;
		}

		//現在ステート状態を渡す
		m_cameraManager->SetStateMode(CameraManager::DebagState::GunToNormal);

		//カメラの位置更新
		auto moveEnd = m_cameraManager->CameraPosUpdate(8.0f, 0.0f, 17.0f, 40.0f);

		//銃モード時のカメラ操作処理
		m_cameraManager->CameraControlNomalMode();

		//移動処理が終わったらステートを銃モードに変更する
		if (moveEnd)
		{
			m_cameraManager->ChangeState(L"Normal");
		}
	}

	void CameraGunToNormalState::Exit()
	{

	}

	//ムービー用のステート(ステージ開始時一段階目)//
	void CameraStartMovieState_First::Enter()
	{
		CameraStateBase::Enter();

		auto player = m_cameraManager->GetStage()->GetSharedGameObject<Player>(L"Player");
		auto playerPos = player->GetPosition();

		//カメラのポジション決定
		//Playerよりちょっと前にいる
		m_cameraManager->SetCameraEye(Vec3(playerPos.x, 3.0f, playerPos.z + 10.0f));

	}

	void CameraStartMovieState_First::Update(float deltaTime)
	{
		//CameraStateBase::Update(deltaTime);

		//移動モード時のカメラ操作処理
		m_cameraManager->CameraControlStartMovieMode();

		//角度の調整0~360度までしか出ないようにする
		m_cameraManager->AdjustmentAngle();
	}

	void CameraStartMovieState_First::Exit()
	{

	}
	//

	//ムービー用のステート(ステージ開始時二段階目)//
	void CameraStartMovieState_Second::Enter()
	{
		CameraStateBase::Enter();
		auto player = m_cameraManager->GetStage()->GetSharedGameObject<Player>(L"Player");
		auto playerPos = player->GetPosition();

		float pushY = 3.0f - playerPos.y;
		m_cameraManager->SetPushPos(Vec3(0.0f, pushY, 10.0f));
		m_cameraManager->SetPushStart(false);
	}

	void CameraStartMovieState_Second::Update(float deltaTime)
	{
		//CameraStateBase::Update(deltaTime);

		//移動モード時のカメラ操作処理
		m_cameraManager->CameraControlStartMovieMode();
		//角度の調整0~360度までしか出ないようにする
		m_cameraManager->AdjustmentAngle();

		//カメラの位置更新
		auto moveEnd = m_cameraManager->CameraPosUpdate(8, 0, 17.0f,60.0f);

		//ムービー用コントローラー取得
		auto RT_movieController = m_cameraManager->GetStage()->GetSharedGameObject<RT_MovieController>(L"MovieController");

		//移動処理が終わったらそのことをムービーコントローラーに伝えて通常状態に戻る
		if (moveEnd)
		{
			RT_movieController->SetCameraEnd(true);
			m_cameraManager->ChangeState(L"Normal");
		}
	}

	void CameraStartMovieState_Second::Exit()
	{

	}
	//


	//ボスムービー用のカメラステート(一番目)//
	void CameraBossMovieState_First::Enter()
	{
		//ボスを取得
		auto boss = m_cameraManager->GetStage()->GetSharedGameObject<EnemyBase>(L"Boss");
		m_boss = boss;

	}
	
	void CameraBossMovieState_First::Update(float deltaTime)
	{
		auto lockBoss = m_boss.lock();
		// ロック出来なかったらreturn
		if (!lockBoss) return;

		auto bossPos = lockBoss->GetPosition();

		//ボスの左足を見る
		m_cameraManager->SetCameraEye(bossPos + Vec3(12.0f, 5.0f, -5.0f));
		m_cameraManager->SetCameraAt(bossPos+Vec3(2.0f,-3.0f,0.0f));

		m_timeCount += deltaTime;
		
		if (m_timeCount > m_timeMax)
		{
			//二番目のステートに移行する
			m_cameraManager->ChangeState(L"BossMovie_Second");
		}

	}

	void CameraBossMovieState_First::Exit()
	{
		//リセット
		m_timeCount = 0.0f;
	}
	//

	//ボスムービー用のカメラステート(二番目)//
	void CameraBossMovieState_Second::Enter()
	{
		//ボスを取得
		auto boss = m_cameraManager->GetStage()->GetSharedGameObject<EnemyBase>(L"Boss");
		m_boss = boss;

	}

	void CameraBossMovieState_Second::Update(float deltaTime)
	{
		auto lockBoss = m_boss.lock();
		// ロック出来なかったらreturn
		if (!lockBoss) return;

		auto bossPos = lockBoss->GetPosition();

		//ボスの左足を見る
		m_cameraManager->SetCameraEye(bossPos + Vec3(-12.0f, 5.0f, -5.0f));
		m_cameraManager->SetCameraAt(bossPos + Vec3(2.0f, -3.0f, 0.0f));

		m_timeCount += deltaTime;

		if (m_timeCount > m_timeMax)
		{
			//三番目のステートに移行する
			m_cameraManager->ChangeState(L"BossMovie_Three");
		}

	}

	void CameraBossMovieState_Second::Exit()
	{
		//リセット
		m_timeCount = 0.0f;
	}
	//


	//ボスムービー用のカメラステート(三番目)//
	void CameraBossMovieState_Three::Enter()
	{
		//ボスを取得
		auto boss = m_cameraManager->GetStage()->GetSharedGameObject<EnemyBase>(L"Boss");
		m_boss = boss;
		// ボスは待機状態になる
		boss->ChangeAnim(L"Idle");

	}

	void CameraBossMovieState_Three::Update(float deltaTime)
	{
		auto lockBoss = m_boss.lock();
		// ロック出来なかったらreturn
		if (!lockBoss) return;

		auto bossPos = lockBoss->GetPosition();

		//ボスの左足を見る
		m_cameraManager->SetCameraEye(bossPos + Vec3(0.0f, 2.0f, -20.0f));
		m_cameraManager->SetCameraAt(bossPos + Vec3(0.0f, 0.0f, 0.0f));

		m_timeCount += deltaTime;

		if (m_timeCount > m_timeMax)
		{
			//四番目のステートに移行する
			m_cameraManager->ChangeState(L"BossMovie_Fourth");
		}

	}

	void CameraBossMovieState_Three::Exit()
	{
		//リセット
		m_timeCount = 0.0f;
	}
	//


	//ボスムービー用のカメラステート(四番目)//
	void CameraBossMovieState_Fourth::Enter()
	{
		//ボスを取得
		auto boss = m_cameraManager->GetStage()->GetSharedGameObject<EnemyBase>(L"Boss");
		m_boss = boss;

		auto playerPos = m_cameraManager->GetStage()->GetSharedGameObject<Player>(L"Player")->GetPosition();
		m_cameraManager->SetPushPos(m_cameraManager->GetCameraEye() - playerPos);

		//アニメーション変更
		boss->ChangeAnim(L"Bonus");
	}

	void CameraBossMovieState_Fourth::Update(float deltaTime)
	{
		auto lockBoss = m_boss.lock();
		// ロック出来なかったらreturn
		if (!lockBoss) return;

		auto bossPos = lockBoss->GetPosition();

		//ボスの正面を見る
		//m_cameraManager->SetCameraEye(bossPos + Vec3(0.0f, 2.0f, -20.0f));
		m_cameraManager->SetCameraAt(bossPos + Vec3(0.0f, 0.0f, 0.0f));

		m_timeCount += deltaTime;

		//カメラの位置更新
		auto moveEnd = m_cameraManager->CameraPosUpdate(8, 0, 17.0f,60.0f);
		m_cameraManager->SetPushStart(false);

		if (moveEnd)
		{
			m_cameraManager->ChangeState(L"Normal");
		}
	}

	void CameraBossMovieState_Fourth::Exit()
	{
		//リセット
		m_timeCount = 0.0f;

		m_cameraManager->GetStage()->GetSharedGameObject<RT_MovieController>(L"MovieController")->SetCameraEnd(true);
	}
	//


	//ボスを倒したときのムービー用のカメラステート(一番目)//
	void CameraBossDieMovieState_First::Enter()
	{
		//ボスを取得
		auto boss = m_cameraManager->GetStage()->GetSharedGameObject<EnemyBase>(L"Boss");
		m_boss = boss;
		Vec3 fwd = boss->GetForward();
		m_bossAngle = atan2(fwd.z, fwd.x);
		//m_bossAngle = XMConvertToRadians(-90.0f);
		auto test = XMConvertToDegrees(m_bossAngle);

		auto playerPos = m_cameraManager->GetStage()->GetSharedGameObject<Player>(L"Player")->GetPosition();
		m_cameraManager->SetPushPos(m_cameraManager->GetCameraEye() - playerPos);

	}

	void CameraBossDieMovieState_First::Update(float deltaTime)
	{
		auto lockBoss = m_boss.lock();
		// ロック出来なかったらreturn
		if (!lockBoss) return;

		auto bossPos = lockBoss->GetPosition();

		//ボスの少し上の正面を見る
		m_cameraManager->SetCameraEye(bossPos + Vec3(cos(m_bossAngle) * 15.0f, 5.0f,sin(m_bossAngle) * 15.0f));
		m_cameraManager->SetCameraAt(bossPos + Vec3(0.0f, 0.0f, 0.0f));

		m_timeCount += deltaTime;

		if (m_timeCount > m_timeMax)
		{
			//元のステートに移行する
			m_cameraManager->ChangeState(L"Normal");
		}


		////カメラの位置更新
		//auto moveEnd = m_cameraManager->CameraPosUpdate(8, 0, 17.0f);
		//m_cameraManager->SetPushStart(false);

		//if (moveEnd)
		//{
		//	m_cameraManager->ChangeState(L"Normal");
		//}
	}

	void CameraBossDieMovieState_First::Exit()
	{
		//リセット
		m_timeCount = 0.0f;

		m_cameraManager->GetStage()->GetSharedGameObject<RT_MovieController>(L"MovieController")->SetCameraEnd(true);
	}
	//

}