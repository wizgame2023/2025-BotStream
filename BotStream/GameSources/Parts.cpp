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

	}

	void Parts::OnUpdate()
	{

	}


	//頭パーツ用
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


    }

    void HeadParts::OnUpdate()
    {

    }

	void HeadParts::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		if (Other->FindTag(L"Player"))
		{
			SetDrawActive(false);
			//PartsPoach->Partspoach.push_back(GetThis<HeadParts>());
		}
		
	}






}