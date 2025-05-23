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
		//ptrColl->SetDrawActive(true);

		//タグをつける
		AddTag(L"HeadParts");


    }

    void HeadParts::OnUpdate()
    {

    }

	void HeadParts::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		if (Other->FindTag(L"Player"))
		{
			m_GetHeadParts = true;
			//GetStage()->RemoveGameObject<HeadParts>(GetThis<HeadParts>());
			SetDrawActive(false);
		}
		
	}

	void HeadParts::SetHeadParts(int setHeadParts)
	{
		m_GetHeadParts = setHeadParts;
	}

	bool HeadParts::GetHeadParts()
	{
		return m_GetHeadParts;
	}

	//void HeadParts::SetAttachHeadParts(int setAttachHeadParts)
	//{
	//	m_AttachHeadParts = setAttachHeadParts;
	//}

	//bool HeadParts::GetAttachHeadParts()
	//{
	//	return m_AttachHeadParts;
	//}


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
		//ptrColl->SetDrawActive(true);

		//タグをつける
		AddTag(L"BodyParts");


	}

	void BodyParts::OnUpdate()
	{

	}

	void BodyParts::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		if (Other->FindTag(L"Player"))
		{
			m_GetBodyParts = true;
			//GetStage()->RemoveGameObject<HeadParts>(GetThis<HeadParts>());
			SetDrawActive(false);
		}

	}

	void BodyParts::SetBodyParts(int setBodyParts)
	{
		m_GetBodyParts = setBodyParts;
	}

	bool BodyParts::GetBodyParts()
	{
		return m_GetBodyParts;
	}

	//void BodyParts::SetAttachBodyParts(int setAttachBodyParts)
	//{
	//	m_AttachBodyParts = setAttachBodyParts;
	//}

	//bool BodyParts::GetAttachBodyParts()
	//{
	//	return m_AttachBodyParts;
	//}



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
		//ptrColl->SetDrawActive(true);

		//タグをつける
		AddTag(L"LegParts");


	}

	void LegParts::OnUpdate()
	{

	}

	void LegParts::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		if (Other->FindTag(L"Player"))
		{
			m_GetLegParts = true;
			//GetStage()->RemoveGameObject<HeadParts>(GetThis<HeadParts>());
			SetDrawActive(false);
		}

	}

	void LegParts::SetLegParts(int setLegParts)
	{
		m_GetLegParts = setLegParts;
	}

	bool LegParts::GetLegParts()
	{
		return m_GetLegParts;
	}

	//void LegParts::SetAttachLegParts(int setAttachLegParts)
	//{
	//	m_AttachLegParts = setAttachLegParts;
	//}

	//bool LegParts::GetAttachLegParts()
	//{
	//	return m_AttachLegParts;
	//}

}