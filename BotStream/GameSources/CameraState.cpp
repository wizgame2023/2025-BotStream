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
		m_cameraManager->CameraPosUpdate(8,0,17.0f);

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

		//移動モード時のカメラ操作処理
		m_cameraManager->CameraControlTransitionMode();
	}

	void CameraNormalToGunState::Exit()
	{

	}
	//

}