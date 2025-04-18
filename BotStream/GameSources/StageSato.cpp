/*!
@file StageSato.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"
#include "EffectManager.h"
#include "time.h"
#include <random>
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
		const Vec2 gaugeSize(300, 75), hpGaugeSize(gaugeSize.x * 0.8f, gaugeSize.y * 0.3f),
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

		//弾数関係------------------------------------------------------------
		const Vec2 numberPos(-120, -200); // 表示開始位置

		for (int i = 0; i < 3; ++i)
		{
			auto digit = AddGameObject<Sprite>(
				L"Numbers",
				Vec2(m_digitSize, m_digitSize),
				Vec3(numberPos.x + i * m_digitSize, numberPos.y, 0.0f)
			);
			digit->SetDrawLayer(3); // UIの最前面に表示
			m_bulletDigits.push_back(digit);
		}
		//--------------------------------------------------------------------

		//質問表示関係--------------------------------------------------------

		const float quesSizeY = 150, quesSizeX = quesSizeY * 10,answerX = 150,answerY = answerX * 0.5625f;
		const Vec3 questionPos(0, 200, 0),answerPos(questionPos.x, questionPos.y - 400, questionPos.z);
		const Vec2 questionSize(quesSizeX, quesSizeY),answerSize(answerX, answerY);

		// 重複しない乱数を格納するセット
		std::set<int> uniqueNumbers;

		// 乱数生成器の初期化
		std::default_random_engine engine(static_cast<unsigned int>(std::time(nullptr)));

		std::uniform_int_distribution<int> distribution(0, 9); // 1から10までの乱数

		// 5つの重複しない乱数を生成
		while (uniqueNumbers.size() < 5) {
			int randomNumber = distribution(engine); // 乱数を生成
			uniqueNumbers.insert(randomNumber); // セットに追加
		}

		for (const auto& number : uniqueNumbers) {
			//質問
			m_questionSprite[m_switchQues] = AddGameObject<Sprite>(
				L"Questions",  			//テクスチャ名
				questionSize,       // サイズ
				questionPos);	//表示位置
			m_questionSprite[m_switchQues]->SetUVRect(Vec2(0.0f, number * 0.1f), Vec2(1.0f, (number * 0.1f) + 0.1f));

			for (int i = 0; i < 3; i++)
			{
				//回答
				m_answerSprite[m_switchQues][i] = AddGameObject<Sprite>(
					L"Answer",  			//テクスチャ名
					answerSize,       // サイズ
					answerPos);	//表示位置
				m_answerSprite[m_switchQues][i]->SetUVRect(Vec2(0.333f * i, number * 0.1f), Vec2(0.333f * (i + 1), (number * 0.1f) + 0.1f));
			}

			if (m_switchQues != 0)
			{
				m_questionSprite[m_switchQues]->OnClear(true);
				for (int i = 0; i < 3; i++)
				{
					m_answerSprite[m_switchQues][i]->OnClear(true);
				}
			}
			m_switchQues++;

		}

		//--------------------------------------------------------------------
	}

	void StageSato::OnUpdate()
	{
		//エフェクト関係
		EffectManager::Instance().InterfaceUpdate();


		Vec3 framePos = m_gaugeFrameSprite->GetComponent<Transform>()->GetPosition();

		auto cntl = App::GetApp()->GetInputDevice().GetControlerVec();
		auto keybord = App::GetApp()->GetInputDevice().GetKeyState();
		int a;
		// 仮：LBボタン,キーボードのSPACEで次の質問表示
		if (cntl[0].wPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER || keybord.m_bPressedKeyTbl[VK_SPACE])
		{
			if (m_switchQues == 5)
			{
				m_switchQues = 0;
			}

			if (m_switchQues == 4)
			{
				m_questionSprite[m_switchQues]->OnClear(true);
				m_switchQues = 0;
				m_questionSprite[m_switchQues]->OnClear(false); 
			}
			else if (m_switchQues < 5)
			{
				m_questionSprite[m_switchQues]->OnClear(true);
				m_questionSprite[m_switchQues + 1]->OnClear(false); 
				m_switchQues++;
			}
		}

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

		// 仮：AボタンでUIの数字が下がる
		if (cntl[0].wPressedButtons & XINPUT_GAMEPAD_A)
		{
			m_bulletNum = max(0, m_bulletNum - 1);
		}
		else if (m_bulletNum <= 0)
		{
			m_bulletNum = 90;
		}

		if (cntl[0].wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			//EfkPlaying(L"Beam", angle + XM_PIDIV2, Vec3(0, 1, 0));
			//EfkPlaying(L"Dodge", angle + XM_PI, Vec3(0, 1, 0));
			//EfkPlaying(L"Dash", angle + XM_PIDIV2, Vec3(0, 1, 0));
			//EfkPlaying(L"PathBullet", angle + XM_PIDIV2, Vec3(0, 1, 0));
			//EfkPlaying(L"Slap", angle + XM_PIDIV2, Vec3(0, 1, 0));
		}

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

	void StageSato::OnDraw()
	{
		EffectManager::Instance().InterfaceDraw();
	}

	// 使わないかもだけど一応残す
	void StageSato::ShowNumber(int value, Vec2 pos, float digitSize)
	{
		// 数値を文字列に変換
		std::string str = std::to_string(value);

		float digitWidth = digitSize;
		// UVの幅
		float uvWidth = 1.0f / 10.0f;

		for (size_t i = 0; i < str.size(); ++i)
		{
			char setValue = str[i];
			//無いとは思うが、数字以外の文字が入っていたら無視
			if (setValue < '0' || setValue > '9') continue;

			int digit = setValue - '0';
			float u1 = digit * uvWidth;
			float u2 = u1 + uvWidth;

			auto sprite = AddGameObject<Sprite>(
				L"Numbers",
				Vec2(digitWidth, digitSize),
				Vec3(pos.x + i * digitWidth, pos.y, 0.0f)
			);
			sprite->SetUVRect(Vec2(u1, 0.0f), Vec2(u2, 1.0f));
			sprite->SetDrawLayer(3);  // 前面に表示
		}
	}

}
//end basecross
