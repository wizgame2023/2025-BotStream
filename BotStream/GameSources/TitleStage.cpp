/*!
@file TitleStage.cpp
@brief タイトル画面
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
//	ゲームステージクラス実体
//--------------------------------------------------------------------------------------

	void TitleStage::CreateViewLight() {
		const Vec3 eye(0.0f, 5.0f, -10.0f);
		const Vec3 at(0.0f);
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(eye);
		PtrCamera->SetAt(at);
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}

	void TitleStage::OnCreate()
	{
		CreateViewLight();
		CreateSprite();
		CreateBGM();
	}

	void TitleStage::OnUpdate()
	{
		auto cntl = App::GetApp()->GetInputDevice().GetControlerVec();
		auto keybord = App::GetApp()->GetInputDevice().GetKeyState();
		auto delta = App::GetApp()->GetElapsedTime();
		auto ptrMana = App::GetApp()->GetXAudio2Manager();

		m_time += delta;

		if (sinf(m_time) < 0.0f)
		{
			m_time = 0;
		}
		
		m_textSprite->SetColor(Col4(1.0f, 1.0f, 1.0f, sinf(m_time)));

		//BGMのボリュームの更新
		auto BGMVol = App::GetApp()->GetScene<Scene>()->GetBGMVolume();
		auto BGMVoice = m_BGM->m_SourceVoice;
		if (BGMVoice)
		{
			BGMVoice->SetVolume(BGMVol);
		}

		// Aボタンかエンターキーで決定
		if (cntl[0].wPressedButtons & XINPUT_GAMEPAD_A || keybord.m_bPressedKeyTbl[VK_RETURN])
		{
			ptrMana->Stop(m_BGM);
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToStageSelect");
		}

	}

	void TitleStage::CreateSprite()
	{
		float titleX = 1450;
		Vec2 titleSize(titleX, titleX * 0.5625);
		m_titleBackSprite = AddGameObject<Sprite>(
			L"TitleBack",
			titleSize,
			Vec3(0, 0, 0)
		);

		m_titleSprite = AddGameObject<Sprite>(
			L"TitleLogo",
			titleSize,
			Vec3(0, 0, 0)
		);

		Vec2 titleSize2(titleX * 0.5f, titleX * 0.25f);
		
		m_textSprite = AddGameObject<Sprite>(
			L"TitleText",
			titleSize2,
			Vec3(0, -200, 0)
		);

	}

	void TitleStage::CreateBGM()
	{
		auto ptrMana = App::GetApp()->GetXAudio2Manager();
		m_BGM = ptrMana->Start(L"Title", XAUDIO2_LOOP_INFINITE, 1.0f);

	}
}