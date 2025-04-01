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
		AddComponent<PNTBoneModelDraw>();
	}

}
//end basecross
