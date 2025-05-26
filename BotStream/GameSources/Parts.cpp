/*!
@file Parts.cpp
@brief パーツオブジェクト
*/

#include "stdafx.h"
#include "Project.h"


namespace basecross {

	//頭パーツ用
	HeadParts::HeadParts(const shared_ptr<Stage>& stagePtr) :
		ObjectMove(stagePtr)
	{

	}
	HeadParts::~HeadParts()
	{

	}

    void HeadParts::OnCreate()
    {
		auto trans = GetComponent<Transform>();
		trans->SetPosition(Vec3(0.0f, -5.0f, -280.0f));
		trans->SetScale(Vec3(1.0f, 1.0f, 1.0f));
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetDiffuse(Col4(1, 0, 0, 1));

		//OBB衝突判定を付ける
		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetAfterCollision(AfterCollision::Auto);

		//タグをつける
		AddTag(L"HeadParts");


    }

    void HeadParts::OnUpdate()
    {

    }

	void HeadParts::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		//プレイヤーが触れたらパーツをゲットした判定になってオブジェクトが消える
		if (Other->FindTag(L"Player"))
		{
			m_GetHeadParts = true;
			SetDrawActive(false);
		}
		
	}

	//パーツをゲットしたらtrueになる
	void HeadParts::SetHeadParts(int setHeadParts)
	{
		m_GetHeadParts = setHeadParts;
	}

	bool HeadParts::GetHeadParts()
	{
		return m_GetHeadParts;
	}



	//体パーツ用
	BodyParts::BodyParts(const shared_ptr<Stage>& stagePtr) :
		ObjectMove(stagePtr)
	{

	}
	BodyParts::~BodyParts()
	{

	}

	void BodyParts::OnCreate()
	{
		auto trans = GetComponent<Transform>();
		trans->SetPosition(Vec3(15.0f, -5.0f, -280.0f));
		trans->SetScale(Vec3(1.0f, 1.0f, 1.0f));
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetDiffuse(Col4(0, 1, 0, 1));

		//OBB衝突判定を付ける
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetAfterCollision(AfterCollision::Auto);

		//タグをつける
		AddTag(L"BodyParts");


	}

	void BodyParts::OnUpdate()
	{

	}

	void BodyParts::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		//プレイヤーが触れたらパーツをゲットした判定になってオブジェクトが消える
		if (Other->FindTag(L"Player"))
		{
			m_GetBodyParts = true;
			SetDrawActive(false);
		}

	}

	//パーツをゲットしたらtrueになる
	void BodyParts::SetBodyParts(int setBodyParts)
	{
		m_GetBodyParts = setBodyParts;
	}

	bool BodyParts::GetBodyParts()
	{
		return m_GetBodyParts;
	}



	//足パーツ用
	LegParts::LegParts(const shared_ptr<Stage>& stagePtr) :
		ObjectMove(stagePtr)
	{

	}
	LegParts::~LegParts()
	{

	}

	void LegParts::OnCreate()
	{
		auto trans = GetComponent<Transform>();
		trans->SetPosition(Vec3(-15.0f, -5.0f, -280.0f));
		trans->SetScale(Vec3(1.0f, 1.0f, 1.0f));
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetDiffuse(Col4(0, 0, 1, 1));

		//OBB衝突判定を付ける
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetAfterCollision(AfterCollision::Auto);

		//タグをつける
		AddTag(L"LegParts");


	}

	void LegParts::OnUpdate()
	{

	}

	void LegParts::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		//プレイヤーが触れたらパーツをゲットした判定になってオブジェクトが消える
		if (Other->FindTag(L"Player"))
		{
			m_GetLegParts = true;
			SetDrawActive(false);
		}

	}

	//パーツをゲットしたらtrueになる
	void LegParts::SetLegParts(int setLegParts)
	{
		m_GetLegParts = setLegParts;
	}

	bool LegParts::GetLegParts()
	{
		return m_GetLegParts;
	}


}