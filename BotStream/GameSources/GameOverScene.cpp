/*!
@file GameOverScene.cpp
@brief ゲームオーバー画面
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------
	void GameOver::CreateViewLight() {
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

	void GameOver::OnCreate()
	{
		CreateViewLight();
		CreateSprite();
		m_scene = App::GetApp()->GetScene<Scene>();
		m_stageNum = m_scene.lock()->GetStageNum();
		m_strStage = L"ToWaveStage";
		m_strStage += to_wstring(m_stageNum);
	}

	void GameOver::OnUpdate()
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

		// Lスティックを左右に倒すと選択できる
		if (ret.x >= 0.3f && !m_selectFlag && m_select < 2)
		{
			m_select++;
			if (m_select == 1)
			{
				m_selectSprite[0]->SetPosition(Vec3((m_selectPos.x + m_select * 350) - 130, m_selectPos.y, m_selectPos.z));
				m_selectSprite[1]->SetPosition(Vec3((m_selectPos.x + m_select * 350) + 130, m_selectPos.y, m_selectPos.z));
			}
			else if (m_select == 2)
			{
				m_selectSprite[0]->SetPosition(Vec3((m_selectPos.x + m_select * 350) - 50, m_selectPos.y, m_selectPos.z));
				m_selectSprite[1]->SetPosition(Vec3((m_selectPos.x + m_select * 350) + 50, m_selectPos.y, m_selectPos.z));
			}
			m_selectFlag = true;
		}
		else if (ret.x <= -0.3f && !m_selectFlag && m_select > 0)
		{
			m_select--;
			if (m_select == 1)
			{
				m_selectSprite[0]->SetPosition(Vec3((m_selectPos.x + m_select * 350) - 130, m_selectPos.y, m_selectPos.z));
				m_selectSprite[1]->SetPosition(Vec3((m_selectPos.x + m_select * 350) + 130, m_selectPos.y, m_selectPos.z));
			}
			else if (m_select == 0)
			{
				m_selectSprite[0]->SetPosition(Vec3((m_selectPos.x + m_select * 350) - 80, m_selectPos.y, m_selectPos.z));
				m_selectSprite[1]->SetPosition(Vec3((m_selectPos.x + m_select * 350) + 80, m_selectPos.y, m_selectPos.z));
			}
			m_selectFlag = true;
		}
		else if ((ret.x <= 0.29f && ret.x >= -0.29f) && m_selectFlag)
		{
			m_selectFlag = false;
		}

		// Aボタンかエンターキーで決定
		if (cntl[0].wPressedButtons & XINPUT_GAMEPAD_A || keybord.m_bPressedKeyTbl[VK_RETURN])
		{
			switch (m_select)
			{
			// リスタート
			case 0:
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), m_strStage);
				break;
			// セレクトステージ
			case 1:
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToStageSelect");
				break;
			// タイトル
			case 2:
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage");
				break;

			//例外が起きたらタイトル
			default:
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage");
				break;
			}
		}

	}

	void GameOver::CreateSprite()
	{
		float titleX = 1450;
		Vec2 titleSize(titleX, titleX * 0.5625);
		auto m_selectBackSprite = AddGameObject<Sprite>(
			L"GameOverBack",
			titleSize,
			Vec3(0, 0, 0)
		);
		//m_selectBackSprite->SetColor(Col4(0.49f, 0.49f, 0.49f, 1));

		float overSizeX = 1000, overSizeY = overSizeX * 0.175f;
		auto sprite = AddGameObject<Sprite>(
			L"ClearOverText",
			Vec2(overSizeX, overSizeY),
			Vec3(0, 200, 0)
		);
		sprite->SetUVRect(Vec2(0, 0.25f), Vec2(1, 0.5f));

		//ステージセレクト
		float selectSizeX = 300, selectSizeY = selectSizeX * 0.25f;
		float selectPosX = 0, selectPosY = -200;
		sprite = AddGameObject<Sprite>(
			L"ClearOverText",
			Vec2(selectSizeX, selectSizeY),
			Vec3(selectPosX, selectPosY, 0)
		);
		sprite->SetUVRect(Vec2(0, 0.5f), Vec2(0.5f, 0.75f));

		// リスタート
		sprite = AddGameObject<Sprite>(
			L"ClearOverText",
			Vec2(selectSizeX, selectSizeY),
			Vec3(selectPosX - 350, selectPosY, 0)
		);
		sprite->SetUVRect(Vec2(0.5f, 0.5f), Vec2(1, 0.75f));

		// タイトル
		sprite = AddGameObject<Sprite>(
			L"ClearOverText",
			Vec2(selectSizeX, selectSizeY),
			Vec3(selectPosX + 350, selectPosY, 0)
		);
		sprite->SetUVRect(Vec2(0, 0.75f), Vec2(0.5f, 1));

		float selectFrameSizeX = 200, selectFrameSizeY = selectFrameSizeX * 0.8f/* * 0.375f*/;
		m_selectPos = Vec3(selectPosX - 350, selectPosY, 0);
		// 選択しているところのやつ[0]
		m_selectSprite[0] = AddGameObject<Sprite>(
			L"Select",			//テクスチャ名
			Vec2(selectFrameSizeX, selectFrameSizeY),      // サイズ
			m_selectPos + Vec3(-80, 0, 0) // 表示位置
		);
		m_selectSprite[0]->SetUVRect(Vec2(0, 0), Vec2(0.5f, 1));

		// 選択しているところのやつ[1]
		m_selectSprite[1] = AddGameObject<Sprite>(
			L"Select",			//テクスチャ名
			Vec2(selectFrameSizeX, selectFrameSizeY),      // サイズ
			m_selectPos + Vec3(80, 0, 0) // 表示位置
		);
		m_selectSprite[1]->SetUVRect(Vec2(0.5f, 0), Vec2(1, 1));

	}
}