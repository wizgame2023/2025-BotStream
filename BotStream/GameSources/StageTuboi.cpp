/*!
@file GameTuboi.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------
	void StageTuboi::CreateViewLight() {
		const Vec3 eye(0.0f, 5.0f, -5.0f);
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



	void StageTuboi::OnCreate() {
		try {
			//ビューとライトの作成
			CreateViewLight();
		}
		catch (...) {
			throw;
		}

		CreateSprite();
		AddGameObject<Ground>();

		CreateSharedObjectGroup(L"Actor");

		auto player = AddGameObject<Player>(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		SetSharedGameObject(L"Player", player);

		auto enemyMgr = AddGameObject<EnemyManager>();
		SetSharedGameObject(L"EnemyManager", enemyMgr);

		auto boss = AddGameObject<BossFirst>(Vec3(1, 10 ,1), Vec3(0), Vec3(1));
		SetSharedGameObject(L"Boss", boss);
		enemyMgr->InstBoss(dynamic_pointer_cast<EnemyBase>(boss));

		//test
		enemyMgr->InstEnemy(Vec3(1), Vec3(0), Vec3(1));

		//カメラマネージャ作成
		auto cameraMgr = AddGameObject<CameraManager>();
		SetSharedGameObject(L"CameraManager", cameraMgr);
	}

	//スプライト関係、ステージでやっていいのかわからんから後で聞く
	void StageTuboi::CreateSprite()
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
	}

}
//end basecross
