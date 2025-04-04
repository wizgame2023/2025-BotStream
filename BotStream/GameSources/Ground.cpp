/*!
@file Ground.cpp
@brief 地面オブジェクト実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	Ground::Ground(const shared_ptr<Stage>& stagePtr) :
		ObjectNotMove(stagePtr)
	{

	}
	Ground::~Ground()
	{

	}

	//初期化
	void Ground::OnCreate() {
		auto trans = GetComponent<Transform>();
		trans->SetPosition(Vec3(0, -2, 0));
		trans->SetScale(Vec3(50.0f, 1, 50.0f));
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");

		//OBB衝突判定を付ける
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
		//タグをつける
		AddTag(L"Floor");

	}

}
//end basecross

