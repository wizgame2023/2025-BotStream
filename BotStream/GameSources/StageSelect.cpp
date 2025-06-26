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
		CreateBGM();
	}

	void StageSelect::OnUpdate()
	{
		auto cntl = App::GetApp()->GetInputDevice().GetControlerVec();
		auto delta = App::GetApp()->GetElapsedTime();
		auto keybord = App::GetApp()->GetInputDevice().GetKeyState();
		auto ptrMana = App::GetApp()->GetXAudio2Manager();

		float time = 0;

		// デッドゾーン
		constexpr float dead = 0.3f;
		// ステージの最大数
		constexpr int stageMaxNum = 2;
		constexpr int stageMinNum = 0;
		// コントローラーの左スティックの判定
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

		// 左右いずれかのデッドゾーン復帰でフラグクリア
		if (fabs(ret.x) < dead)
			m_selectOnceFlag = false;

		// 右に倒すとステージが切り替わる(+側)
		if (ret.x >= dead && !m_selectOnceFlag && m_selectStageNum < stageMaxNum && !m_stageFlag)
		{

			// 前のステージ番号の色を戻す
			m_stageNum[m_selectStageNum]->SetColor(Col4(1, 1, 1, 1));
			// ステージ番号を更新
			m_selectStageNum += 1;
			// ステージ番号の色を変更
			m_stageNum[m_selectStageNum]->SetColor(Col4(1, 1, 0, 1));

			m_selectOnceFlag = true;
		}
		// 左に倒すとステージが切り替わる(-側)
		else if (ret.x <= -dead && !m_selectOnceFlag && m_selectStageNum > stageMinNum && !m_stageFlag)
		{
			// 前のステージ番号の色を戻す
			m_stageNum[m_selectStageNum]->SetColor(Col4(1, 1, 1, 1));
			// ステージ番号を更新
			m_selectStageNum -= 1;
			// ステージ番号の色を変更
			m_stageNum[m_selectStageNum]->SetColor(Col4(1, 1, 0, 1));

			m_selectOnceFlag = true;
		}

		//==========================================================================================================
		// チュートリアルにジャンプ(仮)
		if ((cntl[0].wPressedButtons & XINPUT_GAMEPAD_START || keybord.m_bPressedKeyTbl[VK_TAB]) && !m_stageFlag)
		{
			ptrMana->Stop(m_BGM);
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"Tutorial");
		}
		//==========================================================================================================
		
		// Aボタンかエンターキーで選択
		if ((cntl[0].wPressedButtons & XINPUT_GAMEPAD_A || keybord.m_bPressedKeyTbl[VK_RETURN]) && !m_stageFlag)
		{
			m_stageFlag = true;
			m_selectOnceFlag = true;

			// 選択したステージの写真を表示
			for (int i = m_selectStageNum * 2; i < (m_selectStageNum * 2) + 2; i++)
			{
				m_stagePhoto[i]->OnClear(false);
			}
			

			// 他のステージを非表示
			for (int i = 0; i < 3; i++)
			{
				if (i != m_selectStageNum)
				{
					m_stageNum[i]->OnClear(true);
				}
			}
			// 選択したステージを移動、拡大
			m_stageNum[m_selectStageNum]->SetPosition(Vec3(-250, 0, 0));
			m_stageNum[m_selectStageNum]->SetScale(Vec3(2.0f, 2.0f, 1.0f));

			m_time = 0;
		}

		// Bボタンかスペースキーで戻る
		if ((cntl[0].wPressedButtons & XINPUT_GAMEPAD_B || keybord.m_bPressedKeyTbl[VK_SPACE]) && m_stageFlag)
		{
			m_stageFlag = false;
			m_selectOnceFlag = true;
			// ステージ番号の表示
			for (int i = 0; i < 3; i++)
			{
				m_stageNum[i]->OnClear(false);
			}

			// 選択したステージを戻す
			m_stageNum[m_selectStageNum]->SetPosition(Vec3(-300 + (m_selectStageNum * 300), 100, 0));
			m_stageNum[m_selectStageNum]->SetScale(Vec3(1.0f, 1.0f, 1.0f));

			// ステージの写真を非表示
			for (int i = 0; i < 6; i++)
			{
				m_stagePhoto[i]->OnClear(true);
			}


		}

		// ステージ選択中に写真を透明不透明をゆっくり切り替える
		if (m_stageFlag)
		{
			if (m_time < 1.2f && !m_timeFlag)
			{
				m_time += delta / 2;
				if(m_time > 1.0f) 
					m_timeFlag = true;

			}
			if (m_time > -0.2f && m_timeFlag)
			{
				m_time -= delta / 2;
				if(m_time < 0.0f)
					m_timeFlag = false;

			}

			// 交互に透明度を変える
			float alphaA = m_time;          
			float alphaB = 1.0f - m_time;

			m_stagePhoto[(m_selectStageNum * 2)]->SetColor(Col4(1, 1, 1, alphaA));
			m_stagePhoto[(m_selectStageNum * 2) + 1]->SetColor(Col4(1, 1, 1, alphaB));
		}

		// Aボタンかエンターキーで最終決定
		if ((cntl[0].wPressedButtons & XINPUT_GAMEPAD_A || keybord.m_bPressedKeyTbl[VK_RETURN]) && m_stageFlag && !m_selectOnceFlag)
		{
			ptrMana->Stop(m_BGM);
			switch (m_selectStageNum)
			{
			case 0:
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToWaveStage1");
				break;
			case 1:
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToWaveStage2");
				break;
			case 2:
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToWaveStage3");
				break;
			}
		}

	}

	void StageSelect::CreateBGM()
	{
		auto ptrMana = App::GetApp()->GetXAudio2Manager();
		m_BGM = ptrMana->Start(L"SelectStage", XAUDIO2_LOOP_INFINITE, 1.0f);


	}

	void StageSelect::CreateSprite()
	{
		float titleX = 1450;
		Vec2 titleSize(titleX, titleX * 0.5625);
		m_selectBackSprite = AddGameObject<Sprite>(
			L"SelectBack",
			titleSize,
			Vec3(0, 0, 0)
		);
		m_selectBackSprite->SetColor(Col4(0.49f, 0.49f, 0.49f, 1));

		Vec2 titleSize2(titleX * 0.25f, titleX * 0.125f);
		m_selectSprite = AddGameObject<Sprite>(
			L"StageSelectString",
			titleSize2,
			Vec3(0, 300, 0)
		);


		//m_textSprite = AddGameObject<Sprite>(
		//	L"TitleText",
		//	titleSize2,
		//	Vec3(0, -250, 0)
		//);

		Vec2 numSize(75, 150);


		for (int i = 0; i < 3; i++)
		{
			m_stageNum[i] = AddGameObject<Sprite>(
				L"Numbers",
				numSize,
				Vec3(-300.0f + (i * 300), 100, 0)
			);
			m_stageNum[i]->SetUVRect(Vec2(0.1f + (0.1f * i), 0), Vec2(0.2f + (0.1f * i), 1));

		}
		m_stageNum[0]->SetColor(Col4(1, 1, 0, 1));

		Vec3 photoPos(300, 100, 0);

		m_stagePhoto[0] = AddGameObject<Sprite>(
			L"Stage1-1Tex",
			Vec2(300, 200),
			photoPos
		);
		m_stagePhoto[1] = AddGameObject<Sprite>(
			L"Stage1-2Tex",
			Vec2(300, 200),
			photoPos
		);

		m_stagePhoto[2] = AddGameObject<Sprite>(
			L"Stage1-1Tex",
			Vec2(300, 200),
			photoPos
		);
		m_stagePhoto[3] = AddGameObject<Sprite>(
			L"Stage1-2Tex",
			Vec2(300, 200),
			photoPos
		);

		m_stagePhoto[4] = AddGameObject<Sprite>(
			L"Stage1-1Tex",
			Vec2(300, 200),
			photoPos
		);
		m_stagePhoto[5] = AddGameObject<Sprite>(
			L"Stage1-2Tex",
			Vec2(300, 200),
			photoPos
		);

		// ステージ写真の総数
		constexpr int photoNum = 6;

		// 全てのステージ写真を非表示にする、レイヤー設定
		for (int i = 0; i < photoNum; i++)
		{
			m_stagePhoto[i]->OnClear(true);
			m_stagePhoto[i]->SetDrawLayer(3);
		}
	}

}