/*!
@file ObjectNotMove.cpp
@brief 動かないオブジェクト
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	ObjectNotMove::ObjectNotMove(const shared_ptr<Stage>& stagePtr) :
		MyGameObject(stagePtr)
	{

	}
	ObjectNotMove::~ObjectNotMove()
	{

	}


	//暗転用のオブジェクト
	void DarkObj::OnCreate()
	{
		//Transform設定
		auto m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 0.21f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XMConvertToRadians(-0.0f), 0.0f),
			Vec3(0.0f, 0.0f, +0.1f)
		);

		//ドローメッシュの設定
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetTextureResource(L"Fadeout");
		ptrDraw->SetDiffuse(m_color);
		ptrDraw->SetOwnShadowActive(false);//影は消す
		ptrDraw->SetDrawActive(true);
		ptrDraw->SetEmissive(m_color); // 自己発光カラー（ライティングによる陰影を消す効果がある）
		ptrDraw->SetOwnShadowActive(false); // 影の映り込みを反映させる
		ptrDraw->SetMeshToTransformMatrix(spanMat);

		//コリジョン作成
		auto ptrColl = AddComponent<CollisionObb>(); // コリジョンスフィアの方が壁にぶつかる判定に違和感がない
		ptrColl->SetAfterCollision(AfterCollision::None);
		ptrColl->SetDrawActive(false);
	}

	void DarkObj::OnUpdate()
	{

	}

	void DarkObj::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		if (!m_activateCollsion) return;

		auto player = dynamic_pointer_cast<Player>(Other);

		auto stage = GetWaveStage(true);
		auto firstStage = dynamic_pointer_cast<StageFirst>(stage);

		

		if (player)
		{
			//ステージ暗転開始
			firstStage->FadeOutStart();
		}


		//auto test = (StageFirst)stage;
	}

}
//end basecross
