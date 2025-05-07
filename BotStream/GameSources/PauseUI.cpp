/*!
@file PauseUI.cpp
@brief ポーズ画面
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//生成
	void Pause::OnCreate()
	{
		auto stage = GetStage();
		//CreateSprite();
		////ステージセレクト
		//float selectSizeX = 250, selectSizeY = selectSizeX * 0.5f;
		//float selectPosX = 0, selectPosY = 200;
		//m_stageSelectUI = stage->AddGameObject<Sprite>(
		//	L"ClearOverText",
		//	Vec2(selectSizeX, selectSizeY),
		//	Vec3(selectPosX, selectPosY, 0)
		//);
		//m_stageSelectUI->SetUVRect(Vec2(0, 0.5f), Vec2(0.5f, 0.75f));

		//// タイトル
		//m_titleUI = stage->AddGameObject<Sprite>(
		//	L"ClearOverText",
		//	Vec2(selectSizeX, selectSizeY),
		//	Vec3(selectPosX, selectPosY-100, 0)
		//);
		//m_titleUI->SetUVRect(Vec2(0, 0.75f), Vec2(0.5f, 1));

		//float selectFrameSizeX = 200, selectFrameSizeY = selectFrameSizeX * 0.8f/* * 0.375f*/;
		//auto m_selectPos = Vec3(selectPosX, selectPosY, 0);
		//// 選択しているところのやつ[左]
		//m_selectSpriteL = stage->AddGameObject<Sprite>(
		//	L"Select",			//テクスチャ名
		//	Vec2(selectFrameSizeX, selectFrameSizeY),      // サイズ
		//	m_selectPos + Vec3(-110, 0, 0) // 表示位置
		//);
		//m_selectSpriteL->SetUVRect(Vec2(0, 0), Vec2(0.5f, 1));

		//// 選択しているところのやつ[右]
		//m_selectSpriteR = stage->AddGameObject<Sprite>(
		//	L"Select",			//テクスチャ名
		//	Vec2(selectFrameSizeX, selectFrameSizeY),      // サイズ
		//	m_selectPos + Vec3(110, 0, 0) // 表示位置
		//);
		//m_selectSpriteR->SetUVRect(Vec2(0.5f, 0), Vec2(1, 1));



	}
	//更新
	void Pause::OnUpdate()
	{
		// インプットデバイスオブジェクト
		InputDevice inputDevice = App::GetApp()->GetInputDevice(); // 様々な入力デバイスを管理しているオブジェクトを取得
		//コントローラーの取得
		auto m_controler = inputDevice.GetControlerVec()[0];
		auto m_contrloerVec = Vec2(m_controler.fThumbRX, m_controler.fThumbRY);

		//auto selectPos = m_stageSelectUI->GetPosition();

		////選択移動
		//float selectPosX = 0, selectPosY = 200;
		//float selectSizeX = 250, selectSizeY = selectSizeX * 0.5f;
		//float selectFrameSizeX = 200, selectFrameSizeY = selectFrameSizeX * 0.8f/* * 0.375f*/;
		//auto m_selectPos = Vec3(selectPosX, selectPosY, 0);
		//if (m_contrloerVec.y <= -0.5f && !m_stick && m_select < 2)
		//{
		//	m_stick = true;
		//	m_select++;
		//	if (m_select == 2)
		//	{
		//		m_selectSpriteL->SetPosition(Vec3(selectSizeX-110+30, selectSizeY, 1.0f));
		//		m_selectSpriteR->SetPosition(Vec3(selectSizeX+110+30, selectSizeY, 1.0f));
		//	}
		//}
		
		if (m_pauseFlag)
		{
			auto cntl = App::GetApp()->GetInputDevice().GetControlerVec();
			auto keybord = App::GetApp()->GetInputDevice().GetKeyState();

			Vec2 ret;
			if (cntl[0].bConnected)
			{
				ret.x = cntl[0].fThumbLX;
				ret.y = cntl[0].fThumbLY;
			}
			else if (!cntl[0].bConnected)
			{
				if (keybord.m_bPushKeyTbl[VK_UP])
					ret.y = 1;

				if (keybord.m_bPushKeyTbl[VK_LEFT])
					ret.x = -1;

				if (keybord.m_bPushKeyTbl[VK_DOWN])
					ret.y = -1;

				if (keybord.m_bPushKeyTbl[VK_RIGHT])
					ret.x = 1;

			}

			// Lスティックを上下に倒すと選択できる
			if (ret.y >= 0.3f && !m_selectFlag && m_select < 2)
			{
				m_select++;
				m_selectFlag = true;
			}
			else if (ret.y <= -0.3f && !m_selectFlag && m_select > 0)
			{
				m_select--;
				m_selectFlag = true;
			}
			else if ((ret.y <= 0.29f && ret.x >= -0.29f) && m_selectFlag)
			{
				m_selectFlag = false;
			}

		}
		
		
		if (!m_pauseFlag)
		{
		
			//ポーズ開始
			if (m_controler.wPressedButtons & XINPUT_GAMEPAD_START)
			{
				m_pauseFlag = true;//ポーズしている状態のフラグ

				auto objVec = GetStage()->GetGameObjectVec();
				for (auto obj : objVec)
				{
					auto actor = dynamic_pointer_cast<Actor>(obj);
					if (actor)
					{
						actor->PoseSwitch(true);
					}
				}
			}

		}
		else if (m_pauseFlag)//ポーズ終了
		{

			//ポーズ解除
			if (m_controler.wPressedButtons & XINPUT_GAMEPAD_START)
			{
				m_pauseFlag = false;//ポーズしていない状態のフラグ

				auto objVec = GetStage()->GetGameObjectVec();
				for (auto obj : objVec)
				{
					auto actor = dynamic_pointer_cast<Actor>(obj);
					if (actor)
					{
						actor->PoseSwitch(false);
					}
				}
			}

		}

	}


	//void Pause::CreateSprite()
	//{
	//	// pause
	//	float X = 300.0f;
	//	m_pauseTextSprite[0] = AddGameObject<Sprite>(
	//		L"PauseText",
	//		Vec2(500, 500 / 2),
	//		Vec3(0, 0, 0)
	//	);
	//	m_pauseTextSprite[0]->SetUVRect(Vec2(0.0f, 0.0f), Vec2(0.333f, 0.5f));

	//	for (int i = 0; i < 6; i++)
	//	{
	//		m_pauseTextSprite[i] = AddGameObject<Sprite>(
	//			L"PauseText",
	//			Vec2(X, X / 2),
	//			Vec3(-200, 300 - (i * 150), 0)
	//		);
	//	}
	//	m_pauseTextSprite[1] = AddGameObject<Sprite>(
	//		L"PauseText",
	//		Vec2(X, X / 2),
	//		Vec3(0, 0, 0)
	//	);
	//	m_pauseTextSprite[1]->SetUVRect(Vec2(0.333f, 0.0f), Vec2(0.666f,0.5f));

	//	// セレクト
	//	m_pauseTextSprite[2] = AddGameObject<Sprite>(
	//		L"PauseText",
	//		Vec2(X, X / 2),
	//		Vec3(0, 0, 0)
	//	);
	//	m_pauseTextSprite[2]->SetUVRect(Vec2(0.666f, 0.0f), Vec2(1.0f,0.5f));

	//	// オーディオ
	//	m_pauseTextSprite[3] = AddGameObject<Sprite>(
	//		L"PauseText",
	//		Vec2(X, X / 2),
	//		Vec3(0, 0, 0)
	//	);
	//	m_pauseTextSprite[3]->SetUVRect(Vec2(0.0f, 0.5f), Vec2(0.333f,1.0f));

	//	// BGM
	//	m_pauseTextSprite[4] = AddGameObject<Sprite>(
	//		L"PauseText",
	//		Vec2(X, X / 2),
	//		Vec3(0, 0, 0)
	//	);
	//	m_pauseTextSprite[4]->SetUVRect(Vec2(0.333f, 0.5f), Vec2(0.666f, 1.0f));

	//	// SE
	//	m_pauseTextSprite[5] = AddGameObject<Sprite>(
	//		L"PauseText",
	//		Vec2(X, X / 2),
	//		Vec3(0, 0, 0)
	//	);
	//	m_pauseTextSprite[5]->SetUVRect(Vec2(0.666f, 0.5f), Vec2(1.0f, 1.0f));
	//	//m_restartSprite = AddGameObject<Sprite>(
	//	//	L"PlayerType",
	//	//	Vec2(XY, XY),
	//	//	Vec3(0, 0, 0)
	//	//);
	//}
}