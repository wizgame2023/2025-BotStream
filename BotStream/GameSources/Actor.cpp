/*!
@file Actor.cpp
@brief Player��Enemy�Ȃǂ̃L�����N�^�[�p�̃N���X
*/

#include "stdafx.h"
#include "Project.h"
#include "Actor.h"

namespace basecross {
	Actor::Actor(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale) :
		ObjectMove(stagePtr),
		m_pos(pos),
		m_rot(rot),
		m_scale(scale)
	{

	}
	Actor::~Actor()
	{

	}

	void Actor::OnCreate() {
		AddTag(L"Actor");

		auto group = GetStage()->GetSharedObjectGroup(L"Actor");
		if (group) {
			group->IntoGroup(GetThis<GameObject>());
		}

		AddComponent<PNTBoneModelDraw>();

		//���n����̐����A�q�I�u�W�F�N�g�ɂ���
		m_LandDetect = GetStage()->AddGameObject<LandDetect>();
		m_LandDetect->GetComponent<Transform>()->SetParent(dynamic_pointer_cast<GameObject>(GetThis<Actor>()));
	}

	void Actor::OnUpdate() {
		
	}

}
//end basecross
