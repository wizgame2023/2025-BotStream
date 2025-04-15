/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	BillBoard::BillBoard(const shared_ptr<Stage>& StagePtr,
		shared_ptr<GameObject>& actorPtr, size_t Number, float pushY, Vec3 scale) :
		MyGameObject(StagePtr),
		m_actor(actorPtr),
		m_Number(Number),
		m_textureName(L"Clear"),
		m_pushY(pushY),
		m_scale(scale),
		m_color(Col4(1.0f, 1.0f, 1.0f, 1.0f)),
		m_layer(2)
	{}
	BillBoard::BillBoard(const shared_ptr<Stage>& StagePtr,
		shared_ptr<GameObject>& actorPtr, wstring spriteName, int layer, float pushY, Vec3 scale, Col4 color) :
		MyGameObject(StagePtr),
		m_actor(actorPtr),
		m_Number(0),
		m_textureName(spriteName),
		m_pushY(pushY),
		m_scale(scale),
		m_color(color),
		m_layer(layer)
	{}
	BillBoard::~BillBoard() {}

	//初期化
	void BillBoard::OnCreate() {

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

			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Squareの作成(ヘルパー関数を利用)
			MeshUtill::CreateSquare(1.0f, m_vertices, indices);
			//UV値の変更
			//左上頂点
			m_vertices[0].textureCoordinate = Vec2(0, 0);
			//右上頂点
			m_vertices[1].textureCoordinate = Vec2(1, 0);
			//左下頂点
			m_vertices[2].textureCoordinate = Vec2(0, 1.0f);
			//右下頂点
			m_vertices[3].textureCoordinate = Vec2(1, 1.0f);

			//頂点の型を変えた新しい頂点を作成(こうしないと色がつかない)
			vector<VertexPositionColorTexture> new_vertices;
			for (auto& v : m_vertices) {
				VertexPositionColorTexture nv;
				nv.position = v.position;
				nv.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
				nv.textureCoordinate = v.textureCoordinate;
				new_vertices.push_back(nv);
			}
			//新しい頂点を使ってメッシュリソースの作成
			m_SquareMeshResource = MeshResource::CreateMeshResource<VertexPositionColorTexture>(new_vertices, indices, true);

			auto DrawComp = AddComponent<PCTStaticDraw>();
			DrawComp->SetMeshResource(m_SquareMeshResource);
			DrawComp->SetTextureResource(m_textureName);
			SetAlphaActive(true);
			SetDrawLayer(m_layer);
		}

	}
	void BillBoard::OnUpdate() {
		if (m_actor.expired())
		{
			GetStage()->RemoveGameObject<BillBoard>(GetThis<BillBoard>());
		}
		if (!m_actor.expired()) {
			auto SeekPtr = m_actor.lock();
			auto SeekTransPtr = SeekPtr->GetComponent<Transform>();

			//トランスフォーム取得
			auto PtrTransform = GetComponent<Transform>();
			auto Pos = SeekTransPtr->GetPosition();
			Pos.y += m_pushY;
			PtrTransform->SetPosition(Pos);
			PtrTransform->SetScale(m_scale);

			auto DrawComp = GetComponent<PCTStaticDraw>();
			DrawComp->SetTextureResource(m_textureName);//テクスチャ更新

			auto PtrCamera = GetStage()->GetView()->GetTargetCamera();

			Quat Qt;
			//向きをカメラ目線にする
			Qt = Billboard(PtrCamera->GetAt() - PtrCamera->GetEye());

			PtrTransform->SetQuaternion(Qt);

		}

	}

	//ビルボードのテクスチャ変更
	void BillBoard::ChangeTexture(wstring textureName)
	{
		m_textureName = textureName;
	}

	//サイズのセッター
	void BillBoard::SetScale(Vec3 scale)
	{
		m_scale = scale;
	}

	//出現する高さのセッター
	void BillBoard::SetPushY(float pushY)
	{
		m_pushY = pushY;
	}


}
//end basecross
