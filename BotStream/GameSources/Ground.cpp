/*!
@file Ground.cpp
@brief �n�ʃI�u�W�F�N�g����
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

	//������
	void Ground::OnCreate() {
		auto trans = GetComponent<Transform>();
		trans->SetPosition(Vec3(0, -2, 0));
		trans->SetScale(Vec3(50.0f, 1, 50.0f));
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");

		//OBB�Փ˔����t����
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
		//�^�O������
		AddTag(L"Floor");

	}

}
//end basecross

