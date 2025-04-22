
/*!
@file StageCollisionController.cpp
@brief “–‚½‚è”»’è‚Ì‘€ì—p
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	StageCollisionController::StageCollisionController(shared_ptr<Stage>& stagePtr) :
		MyGameObject(stagePtr),
		m_CollisionSwhich(true)
	{

	}
	StageCollisionController::~StageCollisionController()
	{

	}

	void StageCollisionController::OnCreate()
	{

	}

	void StageCollisionController::OnUpdate()
	{
		CollisionActive(m_CollisionSwhich);
	}

	void StageCollisionController::CollisionActive(bool On)
	{
		auto stage = GetStage();
		auto player = stage->GetSharedGameObject<Player>(L"Player");
		auto playerPos = player->GetPosition();

		if (On == true)
		{
			AABB CollisionActiveArea = AABB(playerPos + Vec3(-120.0f, -10.0f, -100.0f), playerPos + Vec3(120.0f, 10.0f, 100.0f));
			stage->GetCollisionManager()->SetRootAABB(CollisionActiveArea);
		}
		if (On == false)
		{
			AABB CollisionActiveArea = AABB(Vec3(0.0f, 100.0f, 0.0f), Vec3(0.0f, 100.0f, 0.0f));
			stage->GetCollisionManager()->SetRootAABB(CollisionActiveArea);
		}

	}

	void StageCollisionController::SetCollisionSwhich(bool On)
	{
		m_CollisionSwhich = On;
	}

}