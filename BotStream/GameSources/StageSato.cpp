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

			//auto playerUIButton = AddGameObject<PlayerButtonUI>(Vec2(100,100),Vec2(50,50));
			//SetSharedGameObject(L"PlayerButton", playerUIButton);

			////Player作成
			//auto player = AddGameObject<Player>(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
			//SetSharedGameObject(L"Player", player);

			////エネミーマネージャー
			//auto enemyMgr = AddGameObject<EnemyManager>();
			//SetSharedGameObject(L"EnemyManager", enemyMgr);

			////カメラマネージャ作成
			//auto cameraManager = AddGameObject<CameraManager>();
			//SetSharedGameObject(L"CameraManager", cameraManager);

			//Player関係のUI生成
			//auto playerGauge = AddGameObject<PlayerGaugeUI>(100);
			//SetSharedGameObject(L"PlayerGauge", playerGauge);
			//auto playerUI = AddGameObject<PlayerGaugeUI>(100);
			//SetSharedGameObject(L"PlayerUI", playerUI);

			//playerUI->SetPLMaxHPSprite(player->GetHPMax());//

			auto ground = AddGameObject<Ground>();


		}
		catch (...) {
			throw;
		}

	}

	//スプライト関係、ステージでやっていいのかわからんから後で聞く
	void StageSato::CreateSprite()
	{
		float XY = 300;
		m_personalSelect = AddGameObject<Sprite>(
			L"PlayerType",
			Vec2(XY, XY),
			Vec3(0, 0, 0)
		);
		m_personalSelect->SetUVRect(Vec2(0.0f, 0.0f), Vec2(0.333f, 1.0f));

		/*
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
		*/

		/*
		auto sprite = AddGameObject<Sprite>(
			L"Select",
			Vec2(40, 80),
			Vec3(0, 0, 0)
		);
		const float buttonPosX = 500, buttonPosY = -200;

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
		//const Vec2 gaugeSize(300, 75), hpGaugeSize(gaugeSize.x * 0.8f, gaugeSize.y * 0.3f),
		//	spGaugeSize(gaugeSize.x * 0.47f, gaugeSize.y * 0.09f);

		////hpの初期位置
		//const float gaugePosX = 0.0f, gaugePosY = -250;

		//m_gaugeFrameSprite = AddGameObject<Sprite>(
		//	L"PLGauge",  			//テクスチャ名
		//	gaugeSize,       // サイズ
		//	Vec3(gaugePosX, gaugePosY, 0));	//表示位置
		//m_gaugeFrameSprite->SetDrawLayer(1);

		//m_plHPSprite = AddGameObject<Sprite>(
		//	L"PLHP",  			//テクスチャ名
		//	hpGaugeSize,       // サイズ
		//	Vec3(gaugePosX + (gaugeSize.x * 0.066f), gaugePosY - 0.7f, 0));	//表示位置
		//m_plHPSprite->SetDrawLayer(2);

		//m_plSPSprite = AddGameObject<Sprite>(
		//	L"PLSP",  			//テクスチャ名
		//	spGaugeSize,       // サイズ
		//	Vec3(gaugePosX - (gaugeSize.x * 0.098f), gaugePosY - 19.8f, 0));	//表示位置
		//m_plSPSprite->SetDrawLayer(2);

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
		*/
		//質問表示関係--------------------------------------------------------

		//const float quesSizeY = 120, quesSizeX = quesSizeY * 10, answerX = 150, answerY = answerX * 0.5625f;
		//const Vec3 questionPos(0, 200, 0), answerPos(questionPos.x - 350, questionPos.y - 400, questionPos.z);
		//const Vec2 questionSize(quesSizeX, quesSizeY), answerSize(answerX, answerY);
		//int questionIndex = 0;
		//// 重複しない乱数を格納するセット
		//std::set<int> uniqueNumbers;

		//// 乱数生成器の初期化
		//std::default_random_engine engine(static_cast<unsigned int>(std::time(nullptr)));

		//std::uniform_int_distribution<int> distribution(0, 9); // 1から10までの乱数

		//// 5つの重複しない乱数を生成
		//while (uniqueNumbers.size() < 5) {
		//	int randomNumber = distribution(engine); // 乱数を生成
		//	uniqueNumbers.insert(randomNumber); // セットに追加
		//}

		//for (const auto& number : uniqueNumbers) {
		//	// ランダムに選ばれた質問を記憶
		//	m_questionOrder.push_back(number);

		//	//質問
		//	m_questionSprite[questionIndex] = AddGameObject<Sprite>(
		//		L"Questions",  			//テクスチャ名
		//		questionSize,       // サイズ
		//		questionPos);	//表示位置
		//	m_questionSprite[questionIndex]->SetUVRect(Vec2(0.0f, number * 0.1f), Vec2(1.0f, (number * 0.1f) + 0.1f));

		//	//回答
		//	for (int i = 0; i < 3; i++)
		//	{
		//		m_answerSprite[questionIndex][i] = AddGameObject<Sprite>(
		//			L"Answer",  			//テクスチャ名
		//			answerSize,       // サイズ
		//			answerPos + Vec3(i * 375, 0.0f, 0.0f));	//表示位置
		//		m_answerSprite[questionIndex][i]->SetUVRect(Vec2(0.333f * i, number * 0.1f), Vec2(0.333f * (i + 1), (number * 0.1f) + 0.1f));
		//	}

		//	if (questionIndex != 0)
		//	{
		//		m_questionSprite[questionIndex]->OnClear(true);
		//		for (int i = 0; i < 3; i++)
		//		{
		//			m_answerSprite[questionIndex][i]->OnClear(true);
		//		}
		//	}
		//	questionIndex++;
		//}
		//m_switchQues = 0;

		//Vec2 selectSize(275, 200);
		//m_selectPos = answerPos;
		//// 選択してるところのやつ
		//m_selectSprite = AddGameObject<Sprite>(
		//	L"Select",			//テクスチャ名
		//	selectSize,      // サイズ
		//	m_selectPos);        // 表示位置

		//Vec2 resultSize(400,100);
		//Vec3 resultPos(0, 0, 0);
		//m_resultSprite = AddGameObject<Sprite>(
		//	L"ResultText",   // テクスチャ名
		//	resultSize,      // サイズ
		//	resultPos        // 位置
		//);
		//m_resultSprite->OnClear(true);
		//--------------------------------------------------------------------
		

	}

	void StageSato::OnUpdate()
	{
		//エフェクト関係
		EffectManager::Instance().InterfaceUpdate();

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
			//m_selectSprite->SetPosition(Vec3(m_selectPos.x + m_select * 375, m_selectPos.y, m_selectPos.z));
			m_personalSelect->SetUVRect(Vec2(0.333f * m_select, 0.0f), Vec2((0.333f * m_select) + 0.333f, 1.0f));
			m_selectFlag = true;
		}
		else if (ret.x <= -0.3f && !m_selectFlag && m_select > 0)
		{
			m_select--;
			//m_selectSprite->SetPosition(Vec3(m_selectPos.x + m_select * 375, m_selectPos.y, m_selectPos.z));
			m_personalSelect->SetUVRect(Vec2(0.333f * m_select, 0.0f), Vec2((0.333f * m_select) + 0.333f, 1.0f));
			m_selectFlag = true;
		}
		else if ((ret.x <= 0.29f && ret.x >= -0.29f) && m_selectFlag)
		{
			m_selectFlag = false;
		}

		// 仮：LBボタン,キーボードのSPACEで次の質問表示
		//if (cntl[0].wPressedButtons & XINPUT_GAMEPAD_A || keybord.m_bPressedKeyTbl[VK_SPACE])
		//{
			//if (m_switchQues < m_questionOrder.size())
			//{
			//	int questionID = m_questionOrder[m_switchQues];
			//	// 選択でステータス変動
			//	PersonalStateChange(questionID, m_select);
			//}
			//m_select = 0;
			//m_selectSprite->SetPosition(Vec3(m_selectPos.x + m_select * 375, m_selectPos.y, m_selectPos.z));

			//if (m_switchQues == 5)
			//{
			//	m_switchQues = 0;
			//}

			//// ここはちゃんと動くかのテストみたいな側面があるため要らないです
			//if (m_switchQues == 4)
			//{
			//	m_questionSprite[m_switchQues]->OnClear(true);
			//	m_switchQues = 0;
			//	m_questionSprite[m_switchQues]->OnClear(false);
			//}
			////-------------------
			//else 
			//if (m_switchQues != 4)
			//{
			//	m_questionSprite[m_switchQues]->OnClear(true);
			//	m_questionSprite[m_switchQues + 1]->OnClear(false);
			//	for (int i = 0; i < 3; i++)
			//	{
			//		m_answerSprite[m_switchQues][i]->OnClear(true);
			//		m_answerSprite[m_switchQues + 1][i]->OnClear(false);
			//	}
			//	m_switchQues++;
			//}
			//else
			//if(m_switchQues == 4)
			//{
			//	m_questionSprite[m_switchQues]->OnClear(true);
			//	m_selectSprite->OnClear(true);
			//	for (int i = 0; i < 3; i++)
			//	{
			//		m_answerSprite[m_switchQues][i]->OnClear(true);
			//	}
			//	float lawChaosIndex = NormalizeAxis(m_personality.Lawful, m_personality.Chaos);
			//	float evilGoodIndex = NormalizeAxis(m_personality.Evil, m_personality.Good);
			//	int checkLawCha, checkEvilGood;

			//	// 中立:0
			//	// 秩序:1
			//	// 混沌:2
			//	if (lawChaosIndex > -1.4 && lawChaosIndex < 1.4) checkLawCha = 0;
			//	else if (lawChaosIndex <= -1.4) checkLawCha = 1;
			//	else if (lawChaosIndex >= 1.4) checkLawCha = 2;

			//	// 中立:0
			//	// 悪:1
			//	// 善:2
			//	if (evilGoodIndex > -1.4 && lawChaosIndex < 1.4) checkEvilGood = 0;
			//	else if (evilGoodIndex <= -1.4) checkEvilGood = 1;
			//	else if (evilGoodIndex >= 1.4) checkEvilGood = 2;
			//	
			//	StateResult(checkLawCha, checkEvilGood);
			//	m_resultFlag = true;
			//}
		//}

		// 結果が出た状態でAボタン、もしくはEnterを押したら次のシーンに移行
		if ((cntl[0].wPressedButtons & XINPUT_GAMEPAD_A || keybord.m_bPressedKeyTbl[VK_RETURN])/* && m_resultFlag == true*/)
		{
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToStageSelect");
		}

		//// 仮：Xボタンで武器UI切り替え
		//if (cntl[0].wPressedButtons & XINPUT_GAMEPAD_X)
		//{
		//	m_gunSprite->OnClear(!m_weaponSwitchFlag);
		//	m_katanaSprite->OnClear(m_weaponSwitchFlag);
		//	m_weaponSwitchFlag = !m_weaponSwitchFlag; // m_weaponSwitchFlagがtrueであればfalseを返す、falseであればtrueを返す。
		//}

		//// 仮：AボタンでUIの数字が下がる
		//if (cntl[0].wPressedButtons & XINPUT_GAMEPAD_A)
		//{
		//	m_bulletNum = max(0, m_bulletNum - 1);
		//}
		//else if (m_bulletNum <= 0)
		//{
		//	m_bulletNum = 90;
		//}


		//// 弾数を文字列に変換
		//std::string bulletStr = std::to_string(m_bulletNum);
		//size_t digitCount = bulletStr.size();
		//Vec2 bulletPos(-100, 0);               // 表示位置(CreateSpriteの値と同じ)
		//float uvWidth = 1.0f / 10.0f;          // UVの幅

		//// 桁数に応じてスプライトを更新(UVだけ更新)
		//for (size_t i = 0; i < m_bulletDigits.size(); ++i)
		//{
		//	auto& digitSprite = m_bulletDigits[i];

		//	if (i < digitCount)
		//	{
		//		// 表示すべき数字を取り出す(文字をint型に)
		//		int digit = bulletStr[i] - '0';

		//		// UV範囲
		//		float u1 = digit * uvWidth;
		//		float u2 = u1 + uvWidth;

		//		// UVを設定して表示
		//		digitSprite->SetUVRect(Vec2(u1, 0.0f), Vec2(u2, 1.0f));
		//	}
		//	else
		//	{
		//		// 桁が足りない場合は非表示
		//		digitSprite->SetUVRect(Vec2(0.0f, 0.0f), Vec2(0.0f, 0.0f));
		//	}
		//}
		//DebugLog();

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

	// minusSide : Lawful,Evil
	// plusSide  : Chaos,Good   の想定
	float StageSato::NormalizeAxis(float minusSide, float plusSide)
	{
		// どちらが大きいかの判断
		if (plusSide >= minusSide) {
			return  3.0f - (minusSide / plusSide) * 3;
		}
		else {
			return -(3.0f - (plusSide / minusSide) * 3);
		}

	}

	// デバッグ用文字列表示
	void StageSato::DebugLog()
	{
		wstringstream wss(L"");
		auto scene = App::GetApp()->GetScene<Scene>();
		wss /* << L"デバッグ用文字列 "*/
			<< "Chaos  : " << m_personality.Chaos  << "\n"
			<< "Lawful : " << m_personality.Lawful << "\n"
			<< "Good   : " << m_personality.Good << "\n"
			<< "Evil   : " << m_personality.Evil << "\n" 
			<< "QuesID : " << m_questionOrder[m_switchQues] << "\n"
			<< "Answer : " << m_select << "\n"
			<< endl;
		scene->SetDebugString(wss.str());

	}

	//質問表示関係(超長い)
	//void StageSato::PersonalStateChange(int questionID, int answerIndex)
	//{
	//	int addNum = 5;
	//	switch (questionID)
	//	{
	//	case 0: // 買い物するときによく使うものは
	//		switch (answerIndex)
	//		{
	//		case 0: // 現金
	//			m_personality.Lawful += addNum * 2;
	//			break;
	//		case 1: // キャッシュレス
	//			m_personality.Evil += addNum * 2;
	//			break;
	//		case 2: // クレジットカード(これもキャッシュレスだろって言われると私は死ぬ)
	//			m_personality.Chaos += addNum * 2;
	//			break;
	//		}
	//		break;

	//	case 1: // 生まれ変わるとしたら
	//		switch (answerIndex)
	//		{
	//		case 0: // 人間
	//			m_personality.Evil += addNum;
	//			break;
	//		case 1: // 動物
	//			m_personality.Good += addNum;
	//			break;
	//		case 2: // 植物
	//			m_personality.Lawful += addNum;
	//			break;
	//		}
	//		break;

	//	case 2: // アウトドア派かインドア派か
	//		switch (answerIndex)
	//		{
	//		case 0: // アウトドア派
	//			m_personality.Chaos += addNum * 2;
	//			break;
	//		case 1: // インドア派
	//			m_personality.Evil += addNum * 2;
	//			break;
	//		case 2: // 半々(ここに関して本当に思いつかなかった。気になるなら質問の代案、もしくは3つめの代案求ム)
	//			m_personality.Lawful += addNum * 2;
	//			break;
	//		}
	//		break;

	//	case 3: // ネットの情報に左右されやすいか
	//		switch (answerIndex)
	//		{
	//		case 0: // はい
	//			m_personality.Good += addNum;
	//			break;
	//		case 1: // いいえ
	//			m_personality.Chaos += addNum;
	//			break;
	//		case 2: // どちらでもない(ここに関しては回答が本当に思いつかなかった。気になるなら質問の代案(ry
	//			m_personality.Lawful += addNum;
	//			break;
	//		}
	//		break;

	//	case 4: // 人助けのためなら、法を犯しても構わない
	//		switch (answerIndex)
	//		{
	//		case 0: // はい
	//			m_personality.Good += addNum * 2;
	//			m_personality.Chaos += addNum * 2;
	//			break;
	//		case 1: // いいえ
	//			m_personality.Lawful += addNum * 2;
	//			m_personality.Evil += addNum * 2;
	//			break;
	//		case 2: // どちらでもない(ここに関しては関しては回答が本当に思いつかなかった。気にな(ry
	//			m_personality.Evil += addNum * 2;
	//			break;
	//		}
	//		break;

	//	case 5: // 好きな映画のジャンルは
	//		switch (answerIndex)
	//		{
	//		case 0: // ダークヒーロー
	//			m_personality.Good += addNum * 2;
	//			m_personality.Lawful += addNum * 2;
	//			break;
	//		case 1: // ラブコメ
	//			m_personality.Chaos += addNum * 2;
	//			m_personality.Good += addNum * 2;
	//			break;
	//		case 2: // ドキュメンタリー
	//			m_personality.Lawful += addNum * 2;
	//			m_personality.Evil += addNum * 2;
	//			break;
	//		}
	//		break;

	//	case 6: // 筋肉はすべてを解決すると思うか
	//		switch (answerIndex)
	//		{
	//		case 0: // はい
	//			m_personality.Chaos += addNum;
	//			m_personality.Good += addNum;
	//			break;
	//		case 1: // いいえ
	//			m_personality.Lawful += addNum;
	//			m_personality.Evil += addNum;
	//			break;
	//		case 2: // どちらでもない(ここに関しては関しては回答が本当に思いつか(ry
	//			m_personality.Chaos += addNum;
	//			m_personality.Evil += addNum;
	//			break;
	//		}
	//		break;

	//	case 7: // クリスマスは誰と過ごすか
	//		switch (answerIndex)
	//		{
	//		case 0: // 家族
	//			m_personality.Good += addNum * 2;
	//			m_personality.Lawful += addNum * 2;
	//			break;
	//		case 1: // 恋人
	//			m_personality.Good += addNum * 2;
	//			m_personality.Chaos += addNum * 2;
	//			break;
	//		case 2: // 孤高
	//			m_personality.Evil += addNum * 2;
	//			m_personality.Lawful += addNum * 2;
	//			break;
	//		}
	//		break;

	//	case 8: // 検索エンジンでよく使うのは
	//		switch (answerIndex)
	//		{
	//		case 0: // Chrome
	//			m_personality.Lawful += addNum;
	//			break;
	//		case 1: // Edge
	//			m_personality.Chaos += addNum;
	//			break;
	//		case 2: // Safari
	//			m_personality.Evil += addNum;
	//			break;
	//		}
	//		break;

	//	case 9: // コンビニでよく買うものは
	//		switch (answerIndex)
	//		{
	//		case 0: // 食用品
	//			m_personality.Lawful += addNum * 2;
	//			m_personality.Good += addNum * 2;
	//			break;
	//		case 1: // 日用品
	//			m_personality.Lawful += addNum * 2;
	//			m_personality.Evil += addNum * 2;
	//			break;
	//		case 2: // 書籍類
	//			m_personality.Chaos += addNum * 2;
	//			m_personality.Good += addNum * 2;
	//			break;
	//		}
	//		break;
	//	}
	//}

	//診断結果表示関係(結構長い)
	//void StageSato::StateResult(int LawCha, int EvilGood)
	//{
	//	switch (LawCha)
	//	{
	//	case 0:  // 中立軸
	//		switch (EvilGood)
	//		{
	//		case 0:
	//			// 真なる中立
	//			m_resultSprite->OnClear(false);
	//			m_resultSprite->SetUVRect(Vec2(0, 0.6f), Vec2(1, 0.7f));
	//			break;

	//		case 1:
	//			// 中立にして悪
	//			m_resultSprite->OnClear(false);
	//			m_resultSprite->SetUVRect(Vec2(0, 0.3f), Vec2(1, 0.4f));
	//			break;

	//		case 2:
	//			// 中立にして善
	//			m_resultSprite->OnClear(false);
	//			m_resultSprite->SetUVRect(Vec2(0, 0.0f), Vec2(1, 0.1f));
	//			break;
	//		}
	//		break;

	//	case 1:  // 秩序軸
	//		switch (EvilGood)
	//		{
	//		case 0:
	//			// 秩序にして中立
	//			m_resultSprite->OnClear(false);
	//			m_resultSprite->SetUVRect(Vec2(0, 0.8f), Vec2(1, 0.9f));
	//			break;

	//		case 1:
	//			// 秩序にして悪
	//			m_resultSprite->OnClear(false);
	//			m_resultSprite->SetUVRect(Vec2(0, 0.5f), Vec2(1, 0.6f));
	//			break;

	//		case 2:
	//			// 秩序にして善
	//			m_resultSprite->OnClear(false);
	//			m_resultSprite->SetUVRect(Vec2(0, 0.2f), Vec2(1, 0.3f));
	//			break;
	//		}
	//		break;

	//	case 2:  // 混沌軸
	//		switch (EvilGood)
	//		{
	//		case 0:
	//			// 混沌にして中立
	//			m_resultSprite->OnClear(false);
	//			m_resultSprite->SetUVRect(Vec2(0, 0.7f), Vec2(1, 0.8f));
	//			break;

	//		case 1:
	//			// 混沌にして悪
	//			m_resultSprite->OnClear(false);
	//			m_resultSprite->SetUVRect(Vec2(0, 0.4f), Vec2(1, 0.5f));
	//			break;

	//		case 2:
	//			// 混沌にして善
	//			m_resultSprite->OnClear(false);
	//			m_resultSprite->SetUVRect(Vec2(0, 0.1f), Vec2(1, 0.2f));
	//			break;
	//		}
	//		break;
	//	}
	//}
}
	//end basecross
