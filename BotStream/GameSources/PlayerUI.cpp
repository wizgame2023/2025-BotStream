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
		const float gaugePosX = 0.0f, gaugePosY = -250;

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
	}

	void PlayerGaugeUI::OnUpdate()
	{
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

	//-----------------------------------------------
	//bullet
	//-----------------------------------------------
	void PlayerBulletUI::OnCreate()
	{
		m_stage = GetStage();

		//弾数関係------------------------------------------------------------
		for (int i = 0; i < 3; ++i)
		{
			auto digit = m_stage->AddGameObject<Sprite>(
				L"Numbers",
				Vec2(m_digitSize, m_digitSize),
				Vec3(m_digitPos.x + i * m_digitSize, m_digitPos.y, 0.0f)
			);
			digit->SetDrawLayer(3); // UIの最前面に表示
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

		// 仮：AボタンでUIの数字が下がる
		//if (cntl[0].wPressedButtons & XINPUT_GAMEPAD_A)
		//{
		//	m_bulletNum = max(0, m_bulletNum - 1);
		//}
		//else if (m_bulletNum <= 0)
		//{
		//	m_bulletNum = 90;
		//}

		// 弾数を文字列に変換
		std::string bulletStr = std::to_string(m_bulletNum);
		size_t digitCount = bulletStr.size();
		Vec2 bulletPos(-100, 0);               // 表示位置(CreateSpriteの値と同じ)
		float uvWidth = 1.0f / 10.0f;          // UVの幅

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
		/*
		switch (m_buttonSwitch)
		{
		case 0:	//A
			m_button = m_stage->AddGameObject<Sprite>(
				L"Buttons",         // テクスチャ名
				m_buttonSize,      // サイズ
				Vec3(m_buttonPos.x, m_buttonPos.y, 0));        // 表示位置
			m_button->SetUVRect(AUV1, AUV2);
			break;

		case 1: // X
			m_button = m_stage->AddGameObject<Sprite>(
				L"Buttons",         // テクスチャ名
				m_buttonSize,      // サイズ
				Vec3(m_buttonPos.x, m_buttonPos.y, 0));        // 表示位置
			m_button->SetUVRect(XUV1, XUV2);
			break;

		case 2: // RB
			m_button = m_stage->AddGameObject<Sprite>(
				L"Buttons",         // テクスチャ名
				m_buttonSize,      // サイズ
				Vec3(m_buttonPos.x, m_buttonPos.y, 0));        // 表示位置
			m_button->SetUVRect(RBUV1, RBUV2);
			break;

		case 3: // RS
			m_button = m_stage->AddGameObject<Sprite>(
				L"Buttons",         // テクスチャ名
				m_buttonSize,      // サイズ
				Vec3(m_buttonPos.x, m_buttonPos.y, 0));        // 表示位置
			m_button->SetUVRect(RSUV1, RSUV2);
			break;

		case 4: // LS
			m_button = m_stage->AddGameObject<Sprite>(
				L"Buttons",         // テクスチャ名
				m_buttonSize,      // サイズ
				Vec3(m_buttonPos.x, m_buttonPos.y, 0));        // 表示位置
			m_button->SetUVRect(LSUV1, LSUV2);
			break;

		case 5: // Hold
			m_button = m_stage->AddGameObject<Sprite>(
				L"Buttons",         // テクスチャ名
				m_buttonSize,      // サイズ
				Vec3(m_buttonPos.x, m_buttonPos.y, 0));        // 表示位置
			m_button->SetUVRect(HOLDAUV1, HOLDAUV2);
			break;

		}
		*/

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
		//現在の攻撃方法関係--------------------------------------------------
		// 近接
		m_weaponSprite[0] = m_stage->AddGameObject<Sprite>(
			L"KatanaTex",  			//テクスチャ名
			m_weaponSize,       // サイズ
			Vec3(m_weaponPos.x, m_weaponPos.y, 0));	//表示位置

		m_weaponSprite[1] = m_stage->AddGameObject<Sprite>(
			L"GunTex",  			//テクスチャ名
			m_weaponSize,       // サイズ
			Vec3(m_weaponPos.x, m_weaponPos.y, 0));	//表示位置
		m_weaponSprite[1]->OnClear(true);
		//--------------------------------------------------------------------

	}

	void PlayerWeaponUI::OnUpdate()
	{
		auto cntl = App::GetApp()->GetInputDevice().GetControlerVec();

		// 仮：Xボタンで武器UI切り替え
		if (cntl[0].wPressedButtons & XINPUT_GAMEPAD_X)
		{
			m_weaponSprite[0]->OnClear(!m_weaponSwitchFlag);
			m_weaponSprite[1]->OnClear(m_weaponSwitchFlag);
			m_weaponSwitchFlag = !m_weaponSwitchFlag; // m_weaponSwitchFlagがtrueであればfalseを返す、falseであればtrueを返す。
		}


	}


	void BossGaugeUI::OnCreate()
	{
		// ステージ取得
		m_stage = GetStage();

		// ゲージサイズ (PlayerGaugeUI と同じ)
		const Vec2 gaugeSize(300.0f, 75.0f);
		const Vec2 hpGaugeSize(gaugeSize.x * 0.8f, gaugeSize.y * 0.3f);
		const Vec2 ampGaugeSize = hpGaugeSize;  // アーマーゲージも同じ高さに

		// 画面上部に表示
		const float gaugePosX = 0.0f;
		const float gaugePosY = 200.0f;

		// 枠
		m_gaugeFrameSprite = m_stage->AddGameObject<Sprite>(
			L"BossGaugeFrame",
			gaugeSize, 
			Vec3(gaugePosX, gaugePosY, 0));
		m_gaugeFrameSprite->SetDrawLayer(1);

		// HP ゲージ
		auto m_plHPSprite = m_stage->AddGameObject<Sprite>(
			L"BossHP",
			hpGaugeSize,
			Vec3(gaugePosX - (gaugeSize.x - hpGaugeSize.x) * 0.5f,
				gaugePosY - (gaugeSize.y * 0.2f),
				0));
		m_plHPSprite->SetDrawLayer(2);

	}

	void BossGaugeUI::OnUpdate()
	{
		// 枠の位置を基準に
		Vec3 framePos = m_gaugeFrameSprite->GetComponent<Transform>()->GetPosition();

		// HP 比率
		float hpRatio = clamp(m_HP / m_MaxHP, 0.0f, 1.0f);
		auto hpTrans = m_HPSprite->GetComponent<Transform>();
		hpTrans->SetScale(Vec3(hpRatio, 1.0f, 1.0f));
		// 左端固定のための X オフセット
		{
			const float gaugeWidth = 300.0f * 0.8f;
			float offsetX = (hpRatio - 1.0f) * (gaugeWidth * 0.5f);
			hpTrans->SetPosition(framePos + Vec3(offsetX, -(300.0f * 0.2f), 0));
		}

		//// アーマー比率
		//float ampRatio = clamp(m_AMP / m_MaxAMP, 0.0f, 1.0f);
		//auto ampTrans = m_AMPSprite->GetComponent<Transform>();
		//ampTrans->SetScale(Vec3(ampRatio, 1.0f, 1.0f));
		//// アーマーゲージも同様に左端固定
		//{
		//	const float gaugeWidth = 300.0f * 0.8f;
		//	float offsetX = (ampRatio - 1.0f) * (gaugeWidth * 0.5f);
		//	// HPゲージ下に 5px 分ずらした位置
		//	ampTrans->SetPosition(framePos + Vec3(offsetX, -(300.0f * 0.2f) - (300.0f * 0.3f) - 5.0f, 0));
		//}

	}
}
//end basecross
