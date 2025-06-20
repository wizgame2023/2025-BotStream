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
		m_cameraManager->CameraPosUpdate(8, 0, 17.0f);

		//通常モード時のカメラ操作処理
		m_cameraManager->CameraControlNomalMode();
		//慣性付きの回転処理
		m_cameraManager->InertialRotation();

		//もし,LBボタンを押していたら銃ステートに移行する
		if (m_controller.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
			m_cameraManager->ChangeState(L"Gun");
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
		//もし,LBボタンを押していなかったら通常モードに戻る
		if (!(m_controller.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER))
		{
			m_cameraManager->ChangeState(L"Normal");
			return;
		}

		CameraStateBase::Update(deltaTime);

		//カメラの位置更新
		m_cameraManager->CameraPosUpdate(3.0f, 3.0f, 8.0f);
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

		//カメラの位置更新
		//m_cameraManager->CameraPosUpdate(8, 0, 17.0f);

		//m_cameraManager

		//移動モード時のカメラ操作処理
		m_cameraManager->CameraControlTransitionMode();
	}

	void CameraNormalToGunState::Exit()
	{

	}
	//


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
		auto moveEnd = m_cameraManager->CameraPosUpdate(8, 0, 17.0f);

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

}