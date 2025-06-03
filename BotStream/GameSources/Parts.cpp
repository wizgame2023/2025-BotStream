/*!
@file Parts.cpp
@brief パーツオブジェクト
*/

#include "stdafx.h"
#include "Project.h"


namespace basecross {

	//パーツクラス
	Parts::Parts(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale) :
		ObjectMove(stagePtr),
		m_pos(pos),
		m_rot(rot),
		m_scale(scale)
	{

	}
	Parts::~Parts()
	{

	}

	void Parts::OnCreate()
	{
		//初期化
		auto trans = GetComponent<Transform>();
		trans->SetPosition(m_pos);
		trans->SetRotation(m_rot);
		trans->SetScale(m_scale);
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetDiffuse(Col4(1, 0, 0, 1));

		//OBB衝突判定を付ける
		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetAfterCollision(AfterCollision::None);

		auto stage = GetStage();

		//パーツポーチ取得
		m_partspoach = stage->GetSharedGameObject<EquippedParts>(L"PartsPoach");
		
		//ビルボード生成
		m_billBoard = stage->AddGameObject<BillBoard>(GetThis<GameObject>(),
			m_partsStatus.partsImagePass
			);
		m_billBoard->SetPushY(10.0f);
		
		
	}

	void Parts::OnUpdate()
	{
		auto stage = GetStage();
		auto player = stage->GetSharedGameObject<Player>(L"Player");
		auto playerPos = player->GetPosition();

		//プレイヤーとパーツの距離を取る
		auto distPos = playerPos - m_pos;

		//プレイヤーとの距離が一定以下ならパーツ説明ビルボードを出す
		if (distPos.length() <= 30.0f)
		{
			m_billBoard->SetScale(Vec3(10.0f, 1.0f, 10.0f));
		}
		else
		{
			m_billBoard->SetScale(Vec3(0.0f, 0.0f, 0.0f));
		}
	}

	void Parts::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		//パーツのデータを渡す
		if (Other->FindTag(L"Player"))
		{
			SetDrawActive(false);
			m_partspoach->AddParts(m_partsStatus);
		}

	}



	//頭パーツ用
    void HeadParts::OnCreate()
    {
		Parts::OnCreate();
    }

    void HeadParts::OnUpdate()
    {

	}

}