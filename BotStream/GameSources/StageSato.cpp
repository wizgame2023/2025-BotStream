/*!
@file StageSato.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"
#include "EffectManager.h"
namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------
	void StageSato::CreateViewLight() {
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

	void StageSato::OnCreate() {
		try {
			//ビューとライトの作成
			CreateViewLight();

			CreateSharedObjectGroup(L"Actor");

			CreateSprite();
			//Player作成
			auto player = AddGameObject<Player>(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
			SetSharedGameObject(L"Player", player);


			auto ground = AddGameObject<Ground>();


		}
		catch (...) {
			throw;
		}

	}

	//スプライト関係、ステージでやっていいのかわからんから後で聞く
	void StageSato::CreateSprite()
	{
		//コントローラー関係--------------------------------------------------
		const float buttonPosX = 500, buttonPosY = -200;
		const Vec2 AUV1(0.0f, 0.0f), AUV2(0.333f, 0.25f), XUVX1(0.333f, 0.0f), XUVX2(0.666f, 0.25f),
			RBUV1(0.0f, 0.25f), RBUV2(0.333f, 0.5f), RSUV1(0.333f, 0.25f), RSUV2(0.666f, 0.5f),
			LSUV1(0.666f, 0.25f), LSUV2(1.0f, 0.5f), HOLDAUV1(0.0f, 0.5f), HOLDAUV2(0.666f, 1.0f),
			buttonSize(50.0f, 50.0f);
		//A
		auto sprite = AddGameObject<Sprite>(
			L"Buttons",         // テクスチャ名
			buttonSize,      // サイズ
			Vec3(buttonPosX, buttonPosY, 0));        // 表示位置
		sprite->SetUVRect(AUV1, AUV2);

		//X
		sprite = AddGameObject<Sprite>(
			L"Buttons",         // テクスチャ名
			buttonSize,      // サイズ
			Vec3(buttonPosX - 75, buttonPosY, 0));        // 表示位置
		sprite->SetUVRect(XUVX1, XUVX2);

		//RB
		sprite = AddGameObject<Sprite>(
			L"Buttons",         // テクスチャ名
			buttonSize,      // サイズ
			Vec3(buttonPosX + 75, buttonPosY, 0));        // 表示位置
		sprite->SetUVRect(RBUV1, RBUV2);

		//RS
		sprite = AddGameObject<Sprite>(
			L"Buttons",         // テクスチャ名
			buttonSize,      // サイズ
			Vec3(buttonPosX, buttonPosY + 75, 0));        // 表示位置
		sprite->SetUVRect(RSUV1, RSUV2);

		//LS
		sprite = AddGameObject<Sprite>(
			L"Buttons",         // テクスチャ名
			buttonSize,      // サイズ
			Vec3(buttonPosX + 75, buttonPosY + 75, 0));        // 表示位置
		sprite->SetUVRect(LSUV1, LSUV2);

		//HOLD + A
		sprite = AddGameObject<Sprite>(
			L"Buttons",         // テクスチャ名
			buttonSize,      // サイズ
			Vec3(buttonPosX - 75, buttonPosY + 75, 0));        // 表示位置
		sprite->SetUVRect(HOLDAUV1, HOLDAUV2);

		//--------------------------------------------------------------------

		//操作のテキスト関係--------------------------------------------------
		const Vec2 ATKUV1(0.0f, 0.0f), ATKUV2(0.5f, 0.333f),
			AVOIDUV1(0.5f, 0.0f), AVOIDUV2(1.0f, 0.333f),
			CAMERAUV1(0.0f, 0.333f), CAMERAUV2(0.5f, 0.666f),
			DASHUV1(0.5f, 0.333f), DASHUV2(1.0f, 0.666f),
			LOCKONUV1(0.0f, 0.666f), LOCKONUV2(0.5f, 1.0f),
		    MOVEUV1(0.5f, 0.666f), MOVEUV2(1.0f, 1.0f),
			textSize(100, 50);

		const float textPosX = -500, textPosY = -200;

		//攻撃
		sprite = AddGameObject<Sprite>(
			L"Texts",			//テクスチャ名
			textSize,      // サイズ
			Vec3(textPosX, buttonPosY, 0));        // 表示位置
		sprite->SetUVRect(ATKUV1, ATKUV2);

		//回避
		sprite = AddGameObject<Sprite>(
			L"Texts",			//テクスチャ名
			textSize,      // サイズ
			Vec3(textPosX + 75, buttonPosY, 0));        // 表示位置
		sprite->SetUVRect(AVOIDUV1, AVOIDUV2);

		// カメラ
		sprite = AddGameObject<Sprite>(
			L"Texts", 			//テクスチャ名
			textSize,      // サイズ
			Vec3(textPosX, textPosY + 75, 0));		//表示位置
		sprite->SetUVRect(CAMERAUV1, CAMERAUV2);

		// ダッシュ
		sprite = AddGameObject<Sprite>(
			L"Texts",  			//テクスチャ名
			textSize,       // サイズ
			Vec3(textPosX + 75, textPosY + 75, 0));		//表示位置
		sprite->SetUVRect(DASHUV1, DASHUV2);

		// ロックオン
		sprite = AddGameObject<Sprite>(
			L"Texts",  			//テクスチャ名
			textSize,       // サイズ
			Vec3(textPosX, textPosY + 150, 0));		//表示位置
		sprite->SetUVRect(LOCKONUV1, LOCKONUV2);

		// 移動
		sprite = AddGameObject<Sprite>(
			L"Texts",  			//テクスチャ名
			textSize,       // サイズ
			Vec3(textPosX + 75, textPosY + 150, 0));	//表示位置
		sprite->SetUVRect(MOVEUV1, MOVEUV2);

		//--------------------------------------------------------------------

		//プレイヤーのゲージ関係----------------------------------------------
		const Vec2 gaugeSize(300, 75),hpGaugeSize(gaugeSize.x * 0.8f,gaugeSize.y * 0.3f),
			spGaugeSize(gaugeSize.x * 0.47f, gaugeSize.y * 0.09f);

		//hpの初期位置
		const float gaugePosX = 0.0f, gaugePosY = -250;

		m_gaugeFrameSprite = AddGameObject<Sprite>(
			L"PLGauge",  			//テクスチャ名
			gaugeSize,       // サイズ
			Vec3(gaugePosX, gaugePosY, 0));	//表示位置
		m_gaugeFrameSprite->SetDrawLayer(1);

		m_plHPSprite = AddGameObject<Sprite>(
			L"PLHP",  			//テクスチャ名
			hpGaugeSize,       // サイズ
			Vec3(gaugePosX + (gaugeSize.x * 0.066f), gaugePosY - 0.7f, 0));	//表示位置
		m_plHPSprite->SetDrawLayer(2);

		m_plSPSprite = AddGameObject<Sprite>(
			L"PLSP",  			//テクスチャ名
			spGaugeSize,       // サイズ
			Vec3(gaugePosX - (gaugeSize.x * 0.098f), gaugePosY - 19.8f, 0));	//表示位置
		m_plSPSprite->SetDrawLayer(2);
		
		//--------------------------------------------------------------------

		//現在の攻撃方法関係--------------------------------------------------
		Vec2 weaponSize(40, 40);
		// 近接
		m_katanaSprite = AddGameObject<Sprite>(
			L"KatanaTex",  			//テクスチャ名
			weaponSize,       // サイズ
			Vec3(200, 150, 0));	//表示位置

		m_gunSprite = AddGameObject<Sprite>(
			L"GunTex",  			//テクスチャ名
			weaponSize,       // サイズ
			Vec3(200, 150, 0));	//表示位置
		m_gunSprite->OnClear(true);
		//--------------------------------------------------------------------
	}

	void StageSato::OnUpdate()
	{
		//エフェクト関係
		//EffectManager::Instance().InterfaceUpdate();

		Vec3 framePos = m_gaugeFrameSprite->GetComponent<Transform>()->GetPosition();

		auto cntl = App::GetApp()->GetInputDevice().GetControlerVec();

		// 仮：Yボタンでプレイヤーの(見かけ上の)HPが減る
		if (cntl[0].wPressedButtons & XINPUT_GAMEPAD_Y)
		{
			m_playerHP = max(0.0f, m_playerHP - 10.0f);  // ← 10ずつ減る想定
		}

		// 比率でスケーリング
		float hpRatio = m_playerHP / m_playerMaxHP;
		hpRatio = clamp(hpRatio, 0.0f, 1.0f);

		auto hpTrans = m_plHPSprite->GetComponent<Transform>();
		hpTrans->SetScale(Vec3(hpRatio, 1.0f, 1.0f));

		// 左端固定
		const float gaugeWidth = 300.0f * 0.8f;
		float hpOffsetX = (hpRatio - 1.0f) * (gaugeWidth * 0.5f);
		hpTrans->SetPosition(Vec3(20.0f + hpOffsetX, -200.0f, 0.0f));

		// 枠の位置からの相対座標（±補正）
		Vec3 hpOffset(300.0f * 0.066f, -0.7f, 0.0f);
		hpTrans->SetPosition(framePos + hpOffset + Vec3(hpOffsetX, 0.0f, 0.0f));

		// 仮：Bボタンで必殺技溜め
		if (cntl[0].wPressedButtons & XINPUT_GAMEPAD_B)
		{
			m_playerSP = min(m_playerSP + 10.0f, m_playerMaxSP); // 今の設定だと10回押すと最大になる
		}

		// SPゲージの比率を計算（0.0〜1.0）
		float spRatio = clamp(m_playerSP / m_playerMaxSP, 0.0f, 1.0f);

		auto spTrans = m_plSPSprite->GetComponent<Transform>();
		spTrans->SetScale(Vec3(spRatio, 1.0f, 1.0f));

		// 左端固定のための位置補正（中心をズラす）
		const float spGaugeWidth = 240.0f;
		float spOffsetX = (spRatio - 1.0f) * (spGaugeWidth * 0.3f);

		// 枠の位置からの相対座標
		Vec3 spOffset(-300.0f * 0.098f, -19.8f, 0.0f);
		spTrans->SetPosition(framePos + spOffset + Vec3(spOffsetX, 0.0f, 0.0f));

		// 仮：Xボタンで武器UI切り替え
		if (cntl[0].wPressedButtons & XINPUT_GAMEPAD_X)
		{
			m_gunSprite->OnClear(!m_weaponSwitchFlag);
			m_katanaSprite->OnClear(m_weaponSwitchFlag);
			m_weaponSwitchFlag = !m_weaponSwitchFlag;
		}
	}

	void StageSato::OnDraw()
	{
		EffectManager::Instance().InterfaceDraw();
	}
}
//end basecross
