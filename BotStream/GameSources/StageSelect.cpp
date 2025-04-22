#pragma once
/*!
@file StageSelect.cpp
@brief ステージセレクト画面
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void StageSelect::CreateViewLight() {
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

	void StageSelect::OnCreate()
	{
		CreateViewLight();
		CreateSprite();
	}

	void StageSelect::OnUpdate()
	{
		auto cntl = App::GetApp()->GetInputDevice().GetControlerVec();
		auto keybord = App::GetApp()->GetInputDevice().GetKeyState();

		// Aボタンかエンターキーで決定
		if (cntl[0].wPressedButtons & XINPUT_GAMEPAD_A || keybord.m_bPressedKeyTbl[VK_RETURN])
		{
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToWaveStage");
		}

	}

	void StageSelect::CreateSprite()
	{
		float titleX = 1450;
		Vec2 titleSize(titleX, titleX * 0.5625);
		m_selectBackSprite = AddGameObject<Sprite>(
			L"TitleBack",
			titleSize,
			Vec3(0, 0, 0)
		);
		m_selectBackSprite->SetColor(Col4(0.49f,0.49f,0.49f,1));

		Vec2 titleSize2(titleX * 0.25f, titleX * 0.125f);
		m_selectSprite = AddGameObject<Sprite>(
			L"StageSelectString",
			titleSize2,
			Vec3(0, 300, 0)
		);


		m_textSprite = AddGameObject<Sprite>(
			L"TitleText",
			titleSize2,
			Vec3(0, -250, 0)
		);

		Vec2 titleSize3(titleX * 0.125f, titleX * 0.25f);

		m_stageNum = AddGameObject<Sprite>(
			L"Numbers",
			titleSize3,
			Vec3(0, 0, 0)
		);
		m_stageNum->SetUVRect(Vec2(0.1f, 0), Vec2(0.2f, 1));
	}

}