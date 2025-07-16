/*!
@file PlayerUI.cpp
@brief プレイヤーに関するUI
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
#include "PlayerUI.h"

namespace basecross {

	void PlayerGaugeUI::OnCreate()
	{
		m_stage = GetStage();

		const Vec2 gaugeSize(300, 75);
		const Vec2 hpGaugeSize(gaugeSize.x * 0.8f, gaugeSize.y * 0.3f);
		const Vec2 spGaugeSize(gaugeSize.x * 0.47f, gaugeSize.y * 0.09f);
		const float gaugePosX = 0.0f, gaugePosY = -350;

		//Playerに関するバーUI生成
		m_gaugeFrameSprite = m_stage->AddGameObject<Sprite>(
			L"PLGauge", gaugeSize, Vec3(gaugePosX, gaugePosY, 0));
		m_gaugeFrameSprite->SetDrawLayer(1);

		m_plHPSprite = m_stage->AddGameObject<Sprite>(
			L"PLHP", hpGaugeSize,
			Vec3(gaugePosX + (gaugeSize.x * 0.066f), gaugePosY - 0.7f, 0));
		m_plHPSprite->SetDrawLayer(2);

		m_plSPSprite = m_stage->AddGameObject<Sprite>(
			L"PLSP", spGaugeSize,
			Vec3(gaugePosX - (gaugeSize.x * 0.098f), gaugePosY - 19.8f, 0));
		m_plSPSprite->SetDrawLayer(2);

		// UI非表示
		//AllClear(true);
	}

	void PlayerGaugeUI::OnUpdate()
	{
		auto player = m_player.lock();

		m_playerMaxHP = player->GetMaxHP();
		m_playerHP = player->GetHP();

		Vec3 framePos = m_gaugeFrameSprite->GetComponent<Transform>()->GetPosition();
		auto cntl = App::GetApp()->GetInputDevice().GetControlerVec();

		//// 仮：Yボタンでプレイヤーの(見かけ上の)HPが減る
		//if (cntl[0].wPressedButtons & XINPUT_GAMEPAD_Y)
		//{
		//	m_playerHP = max(0.0f, m_playerHP - 10.0f);  // ← 10ずつ減る想定
		//}

		// 比率でスケーリング(HP)
		float hpRatio = m_playerHP / m_playerMaxHP;
		hpRatio = clamp(hpRatio, 0.0f, 1.0f);

		auto hpTrans = m_plHPSprite->GetComponent<Transform>();
		hpTrans->SetScale(Vec3(hpRatio, 1.0f, 1.0f));

		// 左端固定
		const float gaugeWidth = 300.0f * 0.8f;
		float hpOffsetX = (hpRatio - 1.0f) * (gaugeWidth * 0.5f);
		hpTrans->SetPosition(Vec3(20.0f + hpOffsetX, -200.0f, 0.0f));

		// 枠の位置からの相対座標（プラマイ補正）
		Vec3 hpOffset(300.0f * 0.066f, -0.7f, 0.0f);
		hpTrans->SetPosition(framePos + hpOffset + Vec3(hpOffsetX, 0.0f, 0.0f));

		//// 仮：Bボタンで必殺技溜め
		//if (cntl[0].wPressedButtons & XINPUT_GAMEPAD_B)
		//{
		//	m_playerSP = min(m_playerSP + 10.0f, m_playerMaxSP); // 今の設定だと10回押すと最大になる
		//}

		// SPゲージの比率を計算（0.0?1.0）
		float spRatio = clamp(m_playerSP / m_playerMaxSP, 0.0f, 1.0f);

		auto spTrans = m_plSPSprite->GetComponent<Transform>();
		spTrans->SetScale(Vec3(spRatio, 1.0f, 1.0f));

		// 左端固定のための位置補正（中心をズラす）
		const float spGaugeWidth = 240.0f;
		float spOffsetX = (spRatio - 1.0f) * (spGaugeWidth * 0.3f);

		// 枠の位置からの相対座標
		Vec3 spOffset(-300.0f * 0.098f, -19.8f, 0.0f);
		spTrans->SetPosition(framePos + spOffset + Vec3(spOffsetX, 0.0f, 0.0f));

	}

	void PlayerGaugeUI::AllClear(bool clear)
	{
		m_gaugeFrameSprite->OnClear(clear);
		m_plHPSprite->OnClear(clear);
		m_plSPSprite->OnClear(clear);
	}

	//-----------------------------------------------
	//bullet
	//-----------------------------------------------
	void PlayerBulletUI::OnCreate()
	{
		m_stage = GetStage();

		m_bulletSprite = m_stage->AddGameObject<Sprite>(
			L"BulletUI", // テクスチャ名
			Vec2(m_digitSize, m_digitSize),     // サイズ
			Vec3(m_digitPos.x - 23.0f, m_digitPos.y + 2.0f, 0)  // 表示位置
		);

		//弾数関係------------------------------------------------------------
		for (int i = 0; i < 3; ++i)
		{
			auto digit = m_stage->AddGameObject<Sprite>(
				L"DamageNumbers",
				Vec2(m_digitSize, m_digitSize),
				Vec3(m_digitPos.x + i * m_digitSize, m_digitPos.y, 0.0f)
			);
			digit->SetDrawLayer(2); // UIの最前面に表示
			m_bulletDigits.push_back(digit);
		}
		//--------------------------------------------------------------------

	}

	void PlayerBulletUI::OnUpdate()
	{
		auto cntl = App::GetApp()->GetInputDevice().GetControlerVec();

		auto playerLock = m_player.lock();

		//Playerが居なくなったら自分も消える
		if (!playerLock)
		{
			GetStage()->RemoveGameObject<PlayerBulletUI>(GetThis<PlayerBulletUI>());
			return;
		}
		//プレイヤーの現在の球数によって数値が変わる
		m_bulletNum =  m_player.lock()->GetBulletNum();

		// 弾数を文字列に変換
		std::string bulletStr = std::to_string(m_bulletNum);
		size_t digitCount = bulletStr.size();

		// 表示位置(CreateSpriteの値と同じ)
		Vec2 bulletPos(-100, 0);               		

		// UVの幅
		float uvWidth = 1.0f / 10.0f;

		// 桁数に応じてスプライトを更新(UVだけ更新)
		for (size_t i = 0; i < m_bulletDigits.size(); ++i)
		{
			auto& digitSprite = m_bulletDigits[i];

			if (i < digitCount)
			{
				// 表示すべき数字を取り出す(文字をint型に)
				int digit = bulletStr[i] - '0';

				// UV範囲
				float u1 = digit * uvWidth;
				float u2 = u1 + uvWidth;

				// UVを設定して表示
				digitSprite->SetUVRect(Vec2(u1, 0.0f), Vec2(u2, 1.0f));
			}
			else
			{
				// 桁が足りない場合は非表示
				digitSprite->SetUVRect(Vec2(0.0f, 0.0f), Vec2(0.0f, 0.0f));
			}
		}

	}

	//-----------------------------------------------
	//button
	//-----------------------------------------------

	void PlayerButtonUI::OnCreate()
	{
		const Vec2
			AUV1(0.0f, 0.0f), AUV2(0.333f, 0.25f),
			XUV1(0.333f, 0.0f), XUV2(0.666f, 0.25f),
			RBUV1(0.0f, 0.25f), RBUV2(0.333f, 0.5f),
			RSUV1(0.333f, 0.25f), RSUV2(0.666f, 0.5f),
			LSUV1(0.666f, 0.25f), LSUV2(1.0f, 0.5f),
			HOLDAUV1(0.0f, 0.5f), HOLDAUV2(0.666f, 1.0f);

		//A
		auto sprite = m_stage->AddGameObject<Sprite>(
			L"Buttons",         // テクスチャ名
			m_buttonSize,      // サイズ
			Vec3(m_buttonPos.x, m_buttonPos.y, 0));        // 表示位置
		sprite->SetUVRect(AUV1, AUV2);

		//X
		sprite = m_stage->AddGameObject<Sprite>(
			L"Buttons",         // テクスチャ名
			m_buttonSize,      // サイズ
			Vec3(m_buttonPos.x - 75, m_buttonPos.y, 0));        // 表示位置
		sprite->SetUVRect(XUV1, XUV2);

		//RB
		sprite = m_stage->AddGameObject<Sprite>(
			L"Buttons",         // テクスチャ名
			m_buttonSize,      // サイズ
			Vec3(m_buttonPos.x + 75, m_buttonPos.y, 0));        // 表示位置
		sprite->SetUVRect(RBUV1, RBUV2);

		//RS
		sprite = m_stage->AddGameObject<Sprite>(
			L"Buttons",         // テクスチャ名
			m_buttonSize,      // サイズ
			Vec3(m_buttonPos.x, m_buttonPos.y + 75, 0));        // 表示位置
		sprite->SetUVRect(RSUV1, RSUV2);

		//LS
		sprite = m_stage->AddGameObject<Sprite>(
			L"Buttons",         // テクスチャ名
			m_buttonSize,      // サイズ
			Vec3(m_buttonPos.x + 75, m_buttonPos.y + 75, 0));        // 表示位置
		sprite->SetUVRect(LSUV1, LSUV2);

		//HOLD + A
		sprite = m_stage->AddGameObject<Sprite>(
			L"Buttons",         // テクスチャ名
			m_buttonSize,      // サイズ
			Vec3(m_buttonPos.x - 75, m_buttonPos.y + 75, 0));        // 表示位置
		sprite->SetUVRect(HOLDAUV1, HOLDAUV2);

	}

	//-----------------------------------------------
	//buttontext
	//-----------------------------------------------

	void PlayerButtonText::OnCreate()
	{
		const Vec2
			ATKUV1(0.0f, 0.0f), ATKUV2(0.5f, 0.333f),
			AVOIDUV1(0.5f, 0.0f), AVOIDUV2(1.0f, 0.333f),
			CAMERAUV1(0.0f, 0.333f), CAMERAUV2(0.5f, 0.666f),
			DASHUV1(0.5f, 0.333f), DASHUV2(1.0f, 0.666f),
			LOCKONUV1(0.0f, 0.666f), LOCKONUV2(0.5f, 1.0f),
			MOVEUV1(0.5f, 0.666f), MOVEUV2(1.0f, 1.0f);

		switch (m_textSwitch)
		{
		case 0:	//攻撃
			m_buttonText = m_stage->AddGameObject<Sprite>(
				L"Texts",         // テクスチャ名
				m_textSize,      // サイズ
				Vec3(m_textPos.x, m_textPos.y, 0));        // 表示位置
			m_buttonText->SetUVRect(ATKUV1, ATKUV2);
			break;

		case 1: // 回避
			m_buttonText = m_stage->AddGameObject<Sprite>(
				L"Texts",         // テクスチャ名
				m_textSize,      // サイズ
				Vec3(m_textPos.x, m_textPos.y, 0));        // 表示位置
			m_buttonText->SetUVRect(AVOIDUV1, AVOIDUV2);
			break;

		case 2: // カメラ
			m_buttonText = m_stage->AddGameObject<Sprite>(
				L"Texts",         // テクスチャ名
				m_textSize,      // サイズ
				Vec3(m_textPos.x, m_textPos.y, 0));        // 表示位置
			m_buttonText->SetUVRect(CAMERAUV1, CAMERAUV2);
			break;

		case 3: // ダッシュ
			m_buttonText = m_stage->AddGameObject<Sprite>(
				L"Texts",         // テクスチャ名
				m_textSize,      // サイズ
				Vec3(m_textPos.x, m_textPos.y, 0));        // 表示位置
			m_buttonText->SetUVRect(DASHUV1, DASHUV2);
			break;

		case 4: // ロックオン
			m_buttonText = m_stage->AddGameObject<Sprite>(
				L"Texts",         // テクスチャ名
				m_textSize,      // サイズ
				Vec3(m_textPos.x, m_textPos.y, 0));        // 表示位置
			m_buttonText->SetUVRect(LOCKONUV1, LOCKONUV2);
			break;

		case 5: // 移動
			m_buttonText = m_stage->AddGameObject<Sprite>(
				L"Texts",         // テクスチャ名
				m_textSize,      // サイズ
				Vec3(m_textPos.x, m_textPos.y, 0));        // 表示位置
			m_buttonText->SetUVRect(MOVEUV1, MOVEUV2);
			break;

		}
	}

	//-----------------------------------------------
	//weapon
	//-----------------------------------------------
	void PlayerWeaponUI::OnCreate()
	{
		m_stage = GetStage();
		Vec2 iconScl(80.0f, 80.0f);
		Vec3 pos(430.0f, -320.0f, 0.0f);
		float offsetX = 120.0f;
		int layer = 3;

		// アイコン -------------------------------------
		// 剣
		m_fightSprite[0] = m_stage->AddGameObject<Sprite>(
			L"KatanaTex",
			iconScl,
			pos,
			Vec3(0.0f),
			Col4(1.0f),
			layer
		);

		// 銃
		m_fightSprite[1] = m_stage->AddGameObject<Sprite>(
			L"GunTex",
			iconScl,
			Vec3(pos.x - offsetX, pos.y, pos.z),
			Vec3(0.0f),
			Col4(1.0f),
			layer
		);

		// 回避
		m_fightSprite[2] = m_stage->AddGameObject<Sprite>(
			L"AvoidTex",
			iconScl,
			Vec3(pos.x + offsetX, pos.y, pos.z),
			Vec3(0.0f),
			Col4(1.0f),
			layer
		);
		// ----------------------------------------------

		Vec2 buttonScl(35.0f, 35.0f);
		float buttonOffsetY = 60.0f;
		float gunOffsetX = 35.0f;
		// ボタン関係 -----------------------------------
		// RB / 剣----------------------
		m_fightSprite[3] = m_stage->AddGameObject<Sprite>(
			L"Buttons",
			buttonScl,
			Vec3(pos.x, pos.y - buttonOffsetY, pos.z),
			Vec3(0.0f),
			Col4(1.0f),
			layer
		);
		m_fightSprite[3]->SetUVRect(Vec2(0.0f, 0.25f), Vec2(0.333f, 0.5f));

		// 左クリック
		m_keyboardSprite[0] = m_stage->AddGameObject<Sprite>(
			L"SpaceMouse",
			buttonScl,
			Vec3(pos.x, pos.y - buttonOffsetY, pos.z),
			Vec3(0.0f),
			Col4(1.0f),
			layer
		);
		m_keyboardSprite[0]->SetUVRect(Vec2(0.0f, 0.28f), Vec2(0.5f, 1.0f));
		m_keyboardSprite[0]->OnClear(true);
		// 剣 --------------------------

		// LB / 銃----------------------
		m_fightSprite[4] = m_stage->AddGameObject<Sprite>(
			L"Buttons",
			buttonScl,
			Vec3(pos.x - offsetX - gunOffsetX, pos.y - buttonOffsetY, pos.z),
			Vec3(0.0f),
			Col4(1.0f),
			layer
		);
		m_fightSprite[4]->SetUVRect(Vec2(0.666f, 0.5f), Vec2(1.0f, 0.75f));
		
		// 右クリック
		m_keyboardSprite[1] = m_stage->AddGameObject<Sprite>(
			L"SpaceMouse",
			buttonScl,
			Vec3(pos.x - offsetX - gunOffsetX, pos.y - buttonOffsetY, pos.z),
			Vec3(0.0f),
			Col4(1.0f),
			layer
		);
		m_keyboardSprite[1]->SetUVRect(Vec2(0.5f, 0.28f), Vec2(1.0f, 1.0f));
		m_keyboardSprite[1]->OnClear(true);

		// プラスの部分
		m_fightSprite[5] = m_stage->AddGameObject<Sprite>(
			L"Buttons",
			Vec2(buttonScl.x - 5.0f, buttonScl.y - 5.0f),
			Vec3(pos.x - offsetX, pos.y - buttonOffsetY, pos.z),
			Vec3(0.0f),
			Col4(1.0f),
			layer
		);
		m_fightSprite[5]->SetUVRect(Vec2(0.666f, 0.75f), Vec2(1.0f, 1.0f));

		// RB 
		m_fightSprite[6] = m_stage->AddGameObject<Sprite>(
			L"Buttons",
			buttonScl,
			Vec3(pos.x - offsetX + gunOffsetX, pos.y - buttonOffsetY, pos.z),
			Vec3(0.0f),
			Col4(1.0f),
			layer
		);
		m_fightSprite[6]->SetUVRect(Vec2(0.0f, 0.25f), Vec2(0.333f, 0.5f));

		// 左クリック
		m_keyboardSprite[2] = m_stage->AddGameObject<Sprite>(
			L"SpaceMouse",
			buttonScl,
			Vec3(pos.x - offsetX + gunOffsetX, pos.y - buttonOffsetY, pos.z),
			Vec3(0.0f),
			Col4(1.0f),
			layer
		);
		m_keyboardSprite[2]->SetUVRect(Vec2(0.0f, 0.28f), Vec2(0.5f, 1.0f));
		m_keyboardSprite[2]->OnClear(true);

		// 銃 --------------------------

		// A / 回避----------------------------------------
		m_fightSprite[7] = m_stage->AddGameObject<Sprite>(
			L"Buttons",
			buttonScl,
			Vec3(pos.x + offsetX, pos.y - buttonOffsetY, pos.z),
			Vec3(0.0f),
			Col4(1.0f),
			layer
		);
		m_fightSprite[7]->SetUVRect(Vec2(0.0f, 0.0f), Vec2(0.333f, 0.25f));

		// SPACEキー
		m_keyboardSprite[3] = m_stage->AddGameObject<Sprite>(
			L"SpaceMouse",
			buttonScl + Vec2(10.0f,0.0f),
			Vec3(pos.x + offsetX, pos.y - buttonOffsetY, pos.z),
			Vec3(0.0f),
			Col4(1.0f),
			layer
		);
		m_keyboardSprite[3]->SetUVRect(Vec2(0.0f), Vec2(0.5f, 0.28f));
		m_keyboardSprite[3]->OnClear(true);

		// ----------------------------------------------

		// UI非表示(後でコメントアウト)
		//AllFightSpriteClear(true);
	}

	void PlayerWeaponUI::OnUpdate()
	{
		// ムービー中などはUIを非表示にするとかになったら多分使うと思うので書いておく
		//AllFightSpriteClear(m_movieFlag);
		constexpr int loopIOffset = 3;
		auto cntl = App::GetApp()->GetInputDevice().GetControlerVec();
		
		// コントローラーが接続されているかどうかでUIを変更
		if (cntl[0].bConnected)
		{
			for (int i = 0; i < 4; i++)
			{
				m_fightSprite[i + loopIOffset]->OnClear(false);
				if (i + loopIOffset == 6) m_fightSprite[i + loopIOffset + 1]->OnClear(false);
				m_keyboardSprite[i]->OnClear(true);
			}

			// なんかSetTextrureが反映されないから↑の方式にしたけど、まぁいいか！
			/*
			m_fightSprite[3]->SetTexture(L"Buttons");
			m_fightSprite[3]->SetUVRect(Vec2(0.0f, 0.25f), Vec2(0.333f, 0.5f));

			m_fightSprite[4]->SetTexture(L"Buttons");
			m_fightSprite[4]->SetUVRect(Vec2(0.666f, 0.5f), Vec2(1.0f, 0.75f));
			m_fightSprite[4]->SetRotate(Vec3(0.0f));

			m_fightSprite[6]->SetTexture(L"Buttons");
			m_fightSprite[6]->SetUVRect(Vec2(0.0f, 0.25f), Vec2(0.333f, 0.5f));

			m_fightSprite[7]->SetTexture(L"Buttons");
			m_fightSprite[7]->SetUVRect(Vec2(0.0f, 0.0f), Vec2(0.333f, 0.25f));
			*/
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				if(i + loopIOffset != 5) m_fightSprite[i + loopIOffset]->OnClear(true);
				if (i + loopIOffset == 6) m_fightSprite[i + loopIOffset + 1]->OnClear(true);
				m_keyboardSprite[i]->OnClear(false);
			}
			/*
			m_fightSprite[3]->SetTexture(L"SpaceMouse");
			m_fightSprite[3]->SetUVRect(Vec2(0.0f, 0.333f), Vec2(1.0f, 1.0f));

			m_fightSprite[4]->SetTexture(L"SpaceMouse");
			m_fightSprite[4]->SetUVRect(Vec2(0.0f, 0.333f), Vec2(1.0f, 1.0f));
			m_fightSprite[4]->SetRotate(Vec3(0.0f, XM_PI, 0.0f));

			m_fightSprite[6]->SetTexture(L"SpaceMouse");
			m_fightSprite[6]->SetUVRect(Vec2(0.0f, 0.333f), Vec2(1.0f, 1.0f));

			m_fightSprite[7]->SetTexture(L"SpaceMouse");
			m_fightSprite[7]->SetUVRect(Vec2(0.0f), Vec2(1.0f, 0.333f));
			*/
		}
	}

	// 戦闘用UIすべての表示非表示を設定する
	void PlayerWeaponUI::AllFightSpriteClear(bool clear)
	{
		constexpr int spriteNum = 8;
		for (int i = 0; i < spriteNum; i++)
		{
			m_fightSprite[i]->OnClear(clear);
		}
	}
	// --------------------------------------------------------------------------------------


	//================================================================
	// パーツ交換の処理
	//================================================================

	void PartsTextChange::OnCreate()
	{
		m_stage = GetStage();
		Vec2 scl(200.0f, 100.0f),numScl(30.0f,60.0f);
		int layer = 5;
		float offsetY = 50.0f;

		// SetUVRectの早見表的なもの
		// SetUVRectに適用するなら1を先に入れてください
		// 戦闘用パッチ
		Vec2 patch1(0.0f, 0.0f), patch2(0.5f, 0.25f);
		// 高性能モーター
		Vec2 motor1(0.0f, 0.25f), motor2(0.5f, 0.5f);
		// 試作パーツ
		Vec2 testParts1(0.0f, 0.5f), testParts2(0.5f, 0.75f);

		for (int i = 0; i < 3; i++)
		{
			m_partsTextSprite[i] = m_stage->AddGameObject<Sprite>(
				L"PartsText",
				scl,
				Vec3(-480,-250 - (i * offsetY),0.0f),
				Vec3(0.0f),
				Col4(1.0f),
				layer
			);
			m_partsTextSprite[i]->SetUVRect(testParts1, testParts2);
			m_partsTextSprite[i]->OnClear(true);

			m_num[i] = m_stage->AddGameObject<Sprite>(
				L"Numbers",
				numScl,
				Vec3(-600,-250 - (i * offsetY),0.0f),
				Vec3(0.0f),
				Col4(1.0f),
				layer
			);
			m_num[i]->SetColor(Col4(0.0f, 0.0f, 0.0f, 1.0f));
		}
		m_num[0]->SetUVRect(Vec2(0.1f, 0.0f), Vec2(0.2f, 1.0f));
		m_num[1]->SetUVRect(Vec2(0.2f, 0.0f), Vec2(0.3f, 1.0f));
		m_num[2]->SetUVRect(Vec2(0.3f, 0.0f), Vec2(0.4f, 1.0f));
		//最初は透明状態にする
		//m_partsTextSprite[0]->OnClear(true);
		//m_partsTextSprite[1]->OnClear(true);
		//m_partsTextSprite[2]->OnClear(true);

		// UI非表示(後でコメントアウト)
		//AllClear(true);
	}

	void PartsTextChange::OnUpdate()
	{
		// SetUVRectの早見表的なもの
		// SetUVRectに適用するなら1を先に入れてください
		// 戦闘用パッチ
		Vec2 patch1(0.0f, 0.0f), patch2(0.5f, 0.25f);
		// 高性能モーター
		Vec2 motor1(0.0f, 0.25f), motor2(0.5f, 0.5f);
		// 試作パーツ
		Vec2 testParts1(0.0f, 0.5f), testParts2(0.5f, 0.75f);


		auto stage = GetStage();
		auto equippedParts = stage->GetSharedGameObject<EquippedParts>(L"PartsPoach")->GetEquippedParts();
		int equippedPartsSize = 0;
		equippedPartsSize = equippedParts.size();
		auto a = 0;

		for (int i = 0; i < equippedPartsSize; i++)
		{
			int partsId = equippedParts[i].id;
			switch (partsId)
			{
			case 1:
				//試作パーツ
				m_partsTextSprite[i]->OnClear(false);
				m_partsTextSprite[i]->SetUVRect(testParts1, testParts2);
				break;
			case 2:
				//高性能モーター
				m_partsTextSprite[i]->OnClear(false);
				m_partsTextSprite[i]->SetUVRect(motor1, motor2);
				break;
			case 3:
				//戦闘用パッチ
				m_partsTextSprite[i]->OnClear(false);
				m_partsTextSprite[i]->SetUVRect(patch1, patch2);
				break;
			default:
				//装備していないもしくはその他の場合は透明にする
				m_partsTextSprite[i]->OnClear(true);
				break;
			}
		}
	}

	void PartsTextChange::AllClear(bool clear)
	{
		int parts = 3;
		for (int i = 0; i < parts; i++)
		{
			m_partsTextSprite[i]->OnClear(clear);
			m_num[i]->OnClear(clear);
		}
	}

	// --------------------------------------------------------------------------------------


	//================================================================
	// プレイヤーがやばいときに表示する奴
	//================================================================
	void PlayerEmergencyUI::OnCreate()
	{
		constexpr int layer = 4;
		// とりあえず赤色で表示
		Col4 color = { 1.0f, 0.0f, 0.0f, 0.0f };

		// 16:9のはずなんだけどなぁ…0.5625掛けるとちょっと足りない
		const Vec2 size(1300.0f, 800.0f);

		m_stage = GetStage();
		m_emergencySprite = m_stage->AddGameObject<Sprite>(
			L"EmergencyTex",
			size,
			Vec3(0, 0, 0),//画面中央に表示
			Vec3(0.0f), // 回転なし
			color,
			layer
		);
	}

	void PlayerEmergencyUI::OnUpdate()
	{
		shared_ptr<PauseSprite> pauseSp = m_stage->GetSharedGameObject<PauseSprite>(L"PauseUI");

		bool isPause = pauseSp->GetPauseFlag();

		// ポーズ中なら更新停止
		if (isPause)
		{
			return;
		}

		auto player = m_player.lock();
		if (!player)
		{
			GetStage()->RemoveGameObject<PlayerEmergencyUI>(GetThis<PlayerEmergencyUI>());
			return;
		}

		// プレイヤーの現在のHPと最大値を取得
		float currentHP = player->GetHPCurrent();
		float maxHP = player->GetMaxHP();

		// プレイヤーのHPが残り3割以下の時
		if ((currentHP / maxHP) < 0.3f)
		{
			m_time += App::GetApp()->GetElapsedTime() * 1.5f;
			m_emergencySprite->SetColor(Col4(1.0f, 0.0f, 0.0f, sinf(m_time)));
			if (sinf(m_time) < 0.0f)
			{
				m_time = 0.000001f;
			}
		}
		else
		{
			m_time = 0;
			m_emergencySprite->SetColor(Col4(1.0f, 0.0f, 0.0f, 0.0f)); // 非表示
		}
	}

	// --------------------------------------------------------------------------------------


	//================================================================
	// 砂嵐
	//================================================================
	void GameOverNoise::OnCreate()
	{
		constexpr int layer = 10;
		Col4 color = { 1.0f, 1.0f, 1.0f, 0.0f };
		const Vec2 size(1300.0f, 800.0f);
		m_stage = GetStage();
		m_noiseSprite = m_stage->AddGameObject<Sprite>(
			L"Noise",
			size,
			Vec3(0, 0, 0), //画面中央に表示
			Vec3(0.0f), // 回転なし
			color,
			layer
		);
		m_noiseSprite->SetUVRect(Vec2(0.0f, 0.0f), Vec2(0.333f, 0.333f));
		m_time = 1.0f;

		m_scene = App::GetApp()->GetScene<Scene>();
		float bgmVol = m_scene.lock()->GetBGMVolume();

		m_BGMMana = App::GetApp()->GetXAudio2Manager();

		m_BGM = m_BGMMana->Start(L"GameOverNoise", false, 0.9f * bgmVol);
	}

	void GameOverNoise::OnUpdate()
	{
		float alpha = m_noiseSprite->GetColor().w;
		m_time += App::GetApp()->GetElapsedTime() * 3.0f;
		if (alpha <= 1.0f)
		{
			m_noiseSprite->SetColor(Col4(1.0f, 1.0f, 1.0f, 1.0f - sinf(m_time)));
		}

		if((m_time / 3) >= 1.2f)
		{
			m_BGMMana->Stop(m_BGM);
		}

		// UVの幅
		constexpr float spriteUV = 1.0f / 3.0f; 

		m_noiseSprite->SetUVRect(
			Vec2(spriteUV * m_frameCount[0], spriteUV * m_frameCount[1]),
			Vec2(spriteUV * m_frameCount[0] + spriteUV, spriteUV * m_frameCount[1] + spriteUV)
		);
		
		m_frameCount[0]++;

		// 3x3のUVをループ
 		if (m_frameCount[0] > 2)
		{
			m_frameCount[0] = 0;
			m_frameCount[1]++;
			if (m_frameCount[1] > 2)
			{
				m_frameCount[1] = 0;
			}
		}
	}
}
//end basecross
