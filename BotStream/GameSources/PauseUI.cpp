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

}