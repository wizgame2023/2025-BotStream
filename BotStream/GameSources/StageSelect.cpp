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
		m_scene = App::GetApp()->GetScene<Scene>();
		m_BGMManager = App::GetApp()->GetXAudio2Manager();
		m_SEManager = App::GetApp()->GetXAudio2Manager();
		m_BGMVol = m_scene.lock()->GetBGMVolume();
		m_SEVol = m_scene.lock()->GetSEVolume();

		CreateViewLight();
		CreateSprite();
		CreateBGM();
	}

	void StageSelect::OnUpdate()
	{
		auto cntl = App::GetApp()->GetInputDevice().GetControlerVec();
		auto delta = App::GetApp()->GetElapsedTime();
		auto keybord = App::GetApp()->GetInputDevice().GetKeyState();

		//BGMのボリュームの更新
		auto BGMVol = App::GetApp()->GetScene<Scene>()->GetBGMVolume();
		auto BGMVoice = m_BGM->m_SourceVoice;
		if (BGMVoice)
		{
			BGMVoice->SetVolume(BGMVol);
		}

		// デッドゾーン
		constexpr float dead = 0.6f;
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
			if (keybord.m_bPushKeyTbl[VK_UP] || keybord.m_bPushKeyTbl['W'])
				ret.y = 1;

			if (keybord.m_bPushKeyTbl[VK_LEFT] || keybord.m_bPushKeyTbl['A'])
				ret.x = -1;

			if (keybord.m_bPushKeyTbl[VK_DOWN] || keybord.m_bPushKeyTbl['S'])
				ret.y = -1;

			if (keybord.m_bPushKeyTbl[VK_RIGHT] || keybord.m_bPushKeyTbl['D'])
				ret.x = 1;
		}

		IsContorollerConnect();

		if (fabs(ret.y) < dead)
			m_selectOnceFlag2 = false;

		// 上に倒すとステージが切り替わる(+側)
		if (ret.y >= dead && !m_selectOnceFlag2 && !m_stageFlag && m_tutorialFlag)
		{
			m_SE = m_SEManager->Start(L"StageSelectSE", 0);

			// ステージ番号を更新
			m_selectStageNum = 0;

			// チュートリアルを白に戻して、stage1に移動
			m_tutorialSprite->SetColor(Col4(1.0f, 1.0f, 1.0f, 1.0f));

			// ステージ番号の色を変更
			m_stageNum[m_selectStageNum]->SetColor(Col4(1, 1, 0, 1));

			m_selectOnceFlag2 = true;
			m_tutorialFlag = false;
		}
		// 下に倒すとステージが切り替わる(-側)
		else if (ret.y <= -dead && !m_selectOnceFlag2 && !m_stageFlag && !m_tutorialFlag)
		{
			m_SE = m_SEManager->Start(L"StageSelectSE", 0);

			// ステージ番号を更新
			m_selectStageNum = 3;

			// チュートリアル以外のステージ番号の色を戻す
			for (int i = 0; i < 3; i++)
			{
				m_stageNum[i]->SetColor(Col4(1, 1, 1, 1));
			}

			// ステージ番号の色を変更
			m_tutorialSprite->SetColor(Col4(1, 1, 0, 1));

			m_selectOnceFlag2 = true;
			m_tutorialFlag = true;
		}


		if (!m_tutorialFlag)
		{
			// 左右いずれかのデッドゾーン復帰でフラグクリア
			if (fabs(ret.x) < dead)
				m_selectOnceFlag1 = false;

			// 右に倒すとステージが切り替わる(+側)
			if (ret.x >= dead && !m_selectOnceFlag1 && m_selectStageNum < stageMaxNum && !m_stageFlag)
			{
				m_SE = m_SEManager->Start(L"StageSelectSE", 0);
				// 前のステージ番号の色を戻す
				m_stageNum[m_selectStageNum]->SetColor(Col4(1, 1, 1, 1));
				// ステージ番号を更新
				m_selectStageNum += 1;
				// ステージ番号の色を変更
				m_stageNum[m_selectStageNum]->SetColor(Col4(1, 1, 0, 1));

				m_selectOnceFlag1 = true;
			}
			// 左に倒すとステージが切り替わる(-側)
			else if (ret.x <= -dead && !m_selectOnceFlag1 && m_selectStageNum > stageMinNum && !m_stageFlag)
			{
				m_SE = m_SEManager->Start(L"StageSelectSE", 0);

				// 前のステージ番号の色を戻す
				m_stageNum[m_selectStageNum]->SetColor(Col4(1, 1, 1, 1));
				// ステージ番号を更新
				m_selectStageNum -= 1;
				// ステージ番号の色を変更
				m_stageNum[m_selectStageNum]->SetColor(Col4(1, 1, 0, 1));

				m_selectOnceFlag1 = true;
			}
		}


		//==========================================================================================================
		// チュートリアルにジャンプ(仮)
		//if ((cntl[0].wPressedButtons & XINPUT_GAMEPAD_START || keybord.m_bPressedKeyTbl[VK_TAB]) && !m_stageFlag)
		//{
		//	m_BGMManager->Stop(m_BGM);
		//	PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"Tutorial");
		//}
		//==========================================================================================================
		
		// チュートリアル用の最終決定(贅沢だね)
		if ((cntl[0].wPressedButtons & XINPUT_GAMEPAD_A || keybord.m_bPressedKeyTbl[VK_RETURN]) && m_stageFlag && m_tutorialFlag && m_tutorialOnceFlag)
		{
			m_BGMManager->Stop(m_BGM);
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"Tutorial");
		}


		// Aボタンかエンターキーで選択
		if ((cntl[0].wPressedButtons & XINPUT_GAMEPAD_A || keybord.m_bPressedKeyTbl[VK_RETURN]) && !m_stageFlag)
		{
			m_SE = m_SEManager->Start(L"SelectionSE", 0);

			m_stageFlag = true;
			m_selectOnceFlag1 = true;

			// 選択したステージの写真を表示(チュートリアルは写真なし)
			if (m_selectStageNum != 3)
			{
				m_tutorialSprite->OnClear(true);
				for (int i = m_selectStageNum * 2; i < (m_selectStageNum * 2) + 2; i++)
				{
					m_stagePhoto[i]->OnClear(false);
				}

				// 他のステージを非表示
				for (int i = 0; i < 3; i++)
				{
					m_stageNum[i]->OnClear(true);
				}
				// 選択したステージを移動、拡大
				m_stageNum[m_selectStageNum]->SetPosition(Vec3(-250, 0, 0));
				m_stageNum[m_selectStageNum]->SetScale(Vec3(2.0f, 2.0f, 1.0f));
				m_stageNum[m_selectStageNum]->OnClear(false);

				m_time = 0;
			}

			if (m_selectStageNum == 3)
			{
				m_stageFlag = true;
				m_tutorialOnceFlag = true;
				auto pos = m_tutorialSprite->GetPosition();
				// 文字を大きくして表示
				m_tutorialSprite->SetScale(Vec3(1.5f, 1.5f, 1.0f));
				m_tutorialSprite->SetPosition(pos + Vec3(0.0f, 200.0f, 0.0f));

				// 他のステージを非表示
				for (int i = 0; i < 3; i++)
				{
					m_stageNum[i]->OnClear(true);
				}
			}
		}

		// 何も選ばずにBボタンかバックスペースを押すとタイトルに行く
		if ((cntl[0].wPressedButtons & XINPUT_GAMEPAD_B || keybord.m_bPressedKeyTbl[VK_BACK]) && !m_stageFlag)
		{
			m_BGMManager->Stop(m_BGM);
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage");
		}

		// Bボタンかバックスペースで戻る
		if ((cntl[0].wPressedButtons & XINPUT_GAMEPAD_B || keybord.m_bPressedKeyTbl[VK_BACK]) && m_stageFlag && !m_tutorialFlag)
		{
			m_SE = m_SEManager->Start(L"SelectionCancelSE", 0);

			m_stageFlag = false;
			m_selectOnceFlag1 = true;
			// ステージ番号の表示
			for (int i = 0; i < 3; i++)
			{
				m_stageNum[i]->OnClear(false);
			}
			m_tutorialSprite->OnClear(false);

			// 選択したステージを戻す
			m_stageNum[m_selectStageNum]->SetPosition(Vec3(-300 + (m_selectStageNum * 300), 100, 0));
			m_stageNum[m_selectStageNum]->SetScale(Vec3(1.0f, 1.0f, 1.0f));

			// ステージの写真を非表示
			for (int i = 0; i < 6; i++)
			{
				m_stagePhoto[i]->OnClear(true);
			}


		}
		// チュートリアルキャンセル
		if ((cntl[0].wPressedButtons & XINPUT_GAMEPAD_B || keybord.m_bPressedKeyTbl[VK_BACK]) && m_stageFlag && m_tutorialFlag)
		{
			m_SE = m_SEManager->Start(L"SelectionCancelSE", 0);

			m_stageFlag = false;

			// 他のステージを表示
			for (int i = 0; i < 3; i++)
			{
				m_stageNum[i]->OnClear(false);
			}

			// チュートリアルの文字を小さくして元の位置へ
			m_tutorialSprite->SetScale(Vec3(1.0f, 1.0f, 1.0f));
			auto pos = m_tutorialSprite->GetPosition();
			m_tutorialSprite->SetPosition(pos + Vec3(0.0f, -200.0f, 0.0f));

		}

		// ステージ選択中に写真を透明不透明をゆっくり切り替える
		if (m_stageFlag && !m_tutorialFlag)
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
			float alphaA = 1.0f - m_time;          
			float alphaB = m_time;

			m_stagePhoto[(m_selectStageNum * 2)]->SetColor(Col4(1, 1, 1, alphaA));
			m_stagePhoto[(m_selectStageNum * 2) + 1]->SetColor(Col4(1, 1, 1, alphaB));
		}

		// Aボタンかエンターキーで最終決定
		if ((cntl[0].wPressedButtons & XINPUT_GAMEPAD_A || keybord.m_bPressedKeyTbl[VK_RETURN]) && m_stageFlag && !m_selectOnceFlag1 && !m_tutorialFlag)
		{
			m_BGMManager->Stop(m_BGM);
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

			//なんか反応しないので他に書く
			//case 3:
			//	PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"Tutorial");
			//	break;
			}
		}

	}

	void StageSelect::CreateBGM()
	{
		m_BGM = m_BGMManager->Start(L"SelectStage", XAUDIO2_LOOP_INFINITE, m_BGMVol);
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

		auto stageTwoPos = m_stageNum[1]->GetPosition();
		Vec2 buttonSize(60.0f);


		m_AButtonSp = AddGameObject<Sprite>(
			L"Buttons",
			buttonSize,
			Vec3(280.0f, -350.0f, 0.0f)
		);
		m_AButtonSp->SetUVRect(Vec2(0.0f), Vec2(0.333f, 0.25f));

		auto AButtPos = m_AButtonSp->GetPosition();
		m_enterKeySp = AddGameObject<Sprite>(
			L"EnterBackSpace",
			Vec2(80.0f),
			AButtPos + Vec3(-10.0f, 0.0f, 0.0f)
		);
		m_enterKeySp->SetUVRect(Vec2(0.0f), Vec2(0.5f, 1.0f));

		m_BButtonSp = AddGameObject<Sprite>(
			L"Buttons",
			buttonSize,
			Vec3(480.0f, -350.0f, 0.0f)
		);
		m_BButtonSp->SetUVRect(Vec2(0.333f, 0.0f), Vec2(0.666f, 0.25f));

		auto BButtPos = m_BButtonSp->GetPosition();
		m_backSpaceKeySp = AddGameObject<Sprite>(
			L"EnterBackSpace",
			Vec2(70.0f,100.0f),
			BButtPos + Vec3(-10.0f, 0.0f, 0.0f)
		);
		m_backSpaceKeySp->SetUVRect(Vec2(0.5f,0.0f), Vec2(1.0f));

		for (int i = 0; i < 2; i++)
		{
			auto buttonPos = i ? BButtPos : AButtPos;

			m_textSp[i] = AddGameObject<Sprite>(
				L"Texts",
				Vec2(100, 75),
				buttonPos + Vec3(80.0f, 0.0f, 0.0f)
			);
			m_textSp[i]->SetUVRect(Vec2(0.0f, 0.333f * i), Vec2(0.5f, 0.333f * (i + 1)));
		}


		m_tutorialSprite = AddGameObject<Sprite>(
			L"Texts",
			Vec2(300, 150),
			stageTwoPos + Vec3(0, -300, 0)
		);
		m_tutorialSprite->SetUVRect(Vec2(0.5f, 0.0f), Vec2(1.0f, 0.333f));

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
			L"Stage2-1Tex",
			Vec2(300, 200),
			photoPos
		);
		m_stagePhoto[3] = AddGameObject<Sprite>(
			L"Stage2-2Tex",
			Vec2(300, 200),
			photoPos
		);

		m_stagePhoto[4] = AddGameObject<Sprite>(
			L"Stage3-1Tex",
			Vec2(300, 200),
			photoPos
		);
		m_stagePhoto[5] = AddGameObject<Sprite>(
			L"Stage3-2Tex",
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

	// コントローラーがつながっているときに表示するスプライトを判定する関数みたいな
	void StageSelect::IsContorollerConnect()
	{
		bool clear = App::GetApp()->GetInputDevice().GetControlerVec()[0].bConnected;

		// つながっているとき
		m_AButtonSp->OnClear(!clear);
		m_BButtonSp->OnClear(!clear);
		
		m_enterKeySp->OnClear(clear);
		m_backSpaceKeySp->OnClear(clear);
	}
}