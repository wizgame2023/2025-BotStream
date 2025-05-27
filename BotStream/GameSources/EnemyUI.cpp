#pragma once
/*!
@file EnemyUI.cpp
@brief 敵用のUI処理
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	BillBoardGauge::BillBoardGauge(const shared_ptr<Stage>& StagePtr,
		shared_ptr<GameObject>& actorPtr, wstring spriteName, int layer, float pushY, Vec3 scale) :
		BillBoard(StagePtr, actorPtr, spriteName, layer, pushY, scale),
		m_indices(vector<uint16_t>())
	{

	}

	BillBoardGauge::~BillBoardGauge()
	{

	}

	void BillBoardGauge::OnCreate()
	{
		m_parsecond = 0;//全体の何パーセント出すか決める
		auto PtrTransform = GetComponent<Transform>();
		if (!m_actor.expired()) {
			auto SeekPtr = m_actor.lock();
			auto SeekTransPtr = SeekPtr->GetComponent<Transform>();
			auto Pos = SeekTransPtr->GetPosition();
			Pos.y += m_pushY;
			PtrTransform->SetPosition(Pos);
			PtrTransform->SetScale(m_scale);
			PtrTransform->SetQuaternion(SeekTransPtr->GetQuaternion());
			//変更できるスクエアリソースを作成

			//Squareの作成(ヘルパー関数を利用)
			MeshUtill::CreateSquare(1.0f, m_vertices, m_indices);

			//UV値の変更
			//左上頂点
			m_vertices[0].textureCoordinate = Vec2(0.0f, 0.0f);
			//右上頂点
			m_vertices[1].textureCoordinate = Vec2(1.0f * m_parsecond, 0.0f);
			//左下頂点
			m_vertices[2].textureCoordinate = Vec2(0.0f, 1.0f);
			//右下頂点
			m_vertices[3].textureCoordinate = Vec2(1.0f * m_parsecond, 1.0f);

			//頂点の型を変えた新しい頂点を作成
			vector<VertexPositionColorTexture> new_vertices;
			for (auto& v : m_vertices) {
				VertexPositionColorTexture nv;
				nv.position = v.position;
				nv.color = Col4(1.0f, 0.0f, 0.0f, 1.0f);//赤
				nv.textureCoordinate = v.textureCoordinate;
				new_vertices.push_back(nv);
			}
			//新しい頂点を使ってメッシュリソースの作成
			m_SquareMeshResource = MeshResource::CreateMeshResource<VertexPositionColorTexture>(new_vertices, m_indices, true);

			auto DrawComp = AddComponent<PCTStaticDraw>();
			DrawComp->SetMeshResource(m_SquareMeshResource);
			DrawComp->SetTextureResource(m_textureName);
			SetAlphaActive(true);
			SetDrawLayer(m_layer);
		}

	}

	void BillBoardGauge::OnUpdate()
	{
		if (m_actor.expired())
		{
			GetStage()->RemoveGameObject<BillBoard>(GetThis<BillBoard>());
		}
		if (!m_actor.expired()) {
			auto SeekPtr = m_actor.lock();
			auto SeekTransPtr = SeekPtr->GetComponent<Transform>();

			//アニメーション処理/////////////////////////////////////////
			m_vertices[1].position.x = -0.5f + (1.0f * m_parsecond);
			m_vertices[3].position.x = -0.5f + (1.0f * m_parsecond);
			m_vertices[1].position.y = 0.5f;
			m_vertices[3].position.y = -0.5f;
			//UV座標の設定		
			auto move = (1.0f * m_parsecond);
			m_vertices[1].textureCoordinate.x = move;
			m_vertices[3].textureCoordinate.x = move;
			////////////////////////////////////////////////////////////

			//新しい頂点に更新
			vector<VertexPositionColorTexture> new_vertices;
			for (auto& v : m_vertices) {
				VertexPositionColorTexture nv;
				nv.position = v.position;
				nv.color = Col4(1.0f, 0.0f, 0.0f, 1.0f);//赤
				nv.textureCoordinate = v.textureCoordinate;
				new_vertices.push_back(nv);
			}
			//新しい頂点を使ってメッシュリソースの作成
			m_SquareMeshResource = MeshResource::CreateMeshResource<VertexPositionColorTexture>(new_vertices, m_indices, true);

			auto PtrTransform = GetComponent<Transform>();
			auto Pos = SeekTransPtr->GetPosition();
			Pos.y += m_pushY;
			PtrTransform->SetPosition(Pos);
			PtrTransform->SetScale(m_scale);

			//メッシュの更新
			auto DrawComp = GetComponent<PCTStaticDraw>();
			DrawComp->SetMeshResource(m_SquareMeshResource);
			DrawComp->SetTextureResource(m_textureName);

			auto PtrCamera = GetStage()->GetView()->GetTargetCamera();

			Quat Qt;
			//向きをカメラ目線にする
			Qt = Billboard(PtrCamera->GetAt() - PtrCamera->GetEye());

			PtrTransform->SetQuaternion(Qt);

		}
		//auto verticesVec = m_SquareMeshResource->GetBackupVerteces<VertexPositionColorTexture>();
		////verticesVec[1].position
		//
		////ワールドマトリックス取得
		//auto world = GetComponent<Transform>()->GetWorldMatrix();
		//world.transpose();//ワールド座標できるように変換

	}

	//何パーセントテクスチャを出すか決める
	void BillBoardGauge::SetPercent(float parcent)
	{
		m_parsecond = parcent;
	}


	//--------------------------------------------------
	//  ボスのHPゲージ
	//--------------------------------------------------

	void BossGaugeUI::OnCreate()
	{
		m_stage = GetStage();
		//m_hitPointMax = m
		CreateSprite();
		ClearBossGaugeUI(true);
	}

	void BossGaugeUI::OnUpdate()
	{
		Vec3 framePos = m_gaugeFrameSp->GetComponent<Transform>()->GetPosition();
		auto cntl = App::GetApp()->GetInputDevice().GetControlerVec();
		auto keybord = App::GetApp()->GetInputDevice().GetKeyState();

		// 比率でスケーリング(HP)
		float hpRatio = m_hitPoint / m_hitPointMax;
		hpRatio = clamp(hpRatio, 0.0f, 1.0f);

		auto hpTrans = m_hitPointSp->GetComponent<Transform>();
		hpTrans->SetScale(Vec3(hpRatio, 1.0f, 1.0f));

		// 左端固定
		const float gaugeWidth = 300.0f * 0.8f;
		float hpOffsetX = (hpRatio - 1.0f) * (gaugeWidth * 0.965f);
		hpTrans->SetPosition(Vec3(20.0f + hpOffsetX, -200.0f, 0.0f));

		// 枠の位置からの相対座標（プラマイ補正）
		Vec3 hpOffset(0.0f * 0.066f, 0.7f, 0.0f);
		hpTrans->SetPosition(framePos + hpOffset + Vec3(hpOffsetX, 0.0f, 0.0f));

	}

	void BossGaugeUI::CreateSprite()
	{
		float frameSizeX = 500, frameSizeY = 75;
		m_gaugeFrameSp = m_stage->AddGameObject<Sprite>(
			L"BossGaugeFrame",
			Vec2(frameSizeX, frameSizeY),
			Vec3(0.0f, 350.0f, 0.0f)
		);

		auto framePos = m_gaugeFrameSp->GetPosition();

		m_hitPointSp = m_stage->AddGameObject<Sprite>(
			L"BossHPMater",
			Vec2(frameSizeX * 0.966f, frameSizeY * 0.826f),
			Vec3(framePos.x, framePos.y + 0.7f, framePos.z)
		);
	}
	// END ---------------------------------------------------

	//--------------------------------------------------
	//  敵にダメージを与えた時のUIみたいなやつ
	//--------------------------------------------------

	void EnemyDamageBill::OnCreate()
	{
		auto PtrTransform = GetComponent<Transform>();
		if (!m_actor.expired()) {
			auto SeekPtr = m_actor.lock();
			auto SeekTransPtr = SeekPtr->GetComponent<Transform>();
			auto Pos = SeekTransPtr->GetPosition();
			Pos.y += m_pushY;
			PtrTransform->SetPosition(Pos);
			PtrTransform->SetScale(m_scale);
			PtrTransform->SetQuaternion(SeekTransPtr->GetQuaternion());
			//変更できるスクエアリソースを作成

			//Squareの作成(ヘルパー関数を利用)
			MeshUtill::CreateSquare(1.0f, m_vertices, m_indices);

			//SetBillUV(Vec2(0.0f, 0.0f), Vec2(0.1f, 1.0f));

			//頂点の型を変えた新しい頂点を作成
			vector<VertexPositionColorTexture> new_vertices;
			for (auto& v : m_vertices) {
				VertexPositionColorTexture nv;
				nv.position = v.position;
				nv.color = Col4(1.0f, 0.0f, 0.0f, 1.0f);//赤
				nv.textureCoordinate = v.textureCoordinate;
				new_vertices.push_back(nv);
			}
			//新しい頂点を使ってメッシュリソースの作成
			m_SquareMeshResource = MeshResource::CreateMeshResource<VertexPositionColorTexture>(new_vertices, m_indices, true);

			auto DrawComp = AddComponent<PCTStaticDraw>();
			DrawComp->SetMeshResource(m_SquareMeshResource);
			DrawComp->SetTextureResource(m_textureName);
			SetAlphaActive(true);
			SetDrawLayer(m_layer);
		}

	}

	void EnemyDamageBill::OnUpdate()
	{
		auto cntl = App::GetApp()->GetInputDevice().GetControlerVec();

		auto PtrCamera = GetStage()->GetView()->GetTargetCamera();
		auto PtrTransform = GetComponent<Transform>();

		Quat Qt;
		//向きをカメラ目線にする
		Qt = Billboard(PtrCamera->GetAt() - PtrCamera->GetEye());

		PtrTransform->SetQuaternion(Qt);
	}


	// ダメージ表示
	void EnemyDamageBill::ShowDamage(int value, float digitSize/*, BillBoard bill*/)
	{
		// 数値を文字列に変換
		std::string str = std::to_string(value);
		// UVの幅
		float uvWidth = 1.0f / 10.0f;

		for (size_t i = 0; i < str.size(); ++i)
		{
			char setValue = str[i];

			if (setValue < '0' || setValue > '9') continue;

			int digit = setValue - '0';
			float u1 = digit * uvWidth;
			float u2 = u1 + uvWidth;

			SetBillUV(Vec2(u1, 0.0f), Vec2(u2, 1.0f));
		}
	}
	// END ---------------------------------------------------
}