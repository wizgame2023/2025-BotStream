/*!
@file AttackCollision.cpp
@brief çUåÇîªíËÇÃèàóù
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	AttackCollision::AttackCollision(const shared_ptr<Stage>& stagePtr) :
		MyGameObject(stagePtr), m_info()
	{

	}
	AttackCollision::~AttackCollision()
	{

	}

	void AttackCollision::OnCreate() {
		m_collision = AddComponent<CollisionCapsule>();
		m_collision->SetAfterCollision(AfterCollision::None);

		m_collision->SetMakedRadius(10);
	}

	void AttackCollision::OnUpdate() {
		const auto delta = App::GetApp()->GetElapsedTime();

		if (m_ActiveTime > 0) {
			m_collision->SetUpdateActive(true);
			//m_collision->SetDrawActive(true);
			m_ActiveTime -= delta;
		}
		else {
			m_collision->SetUpdateActive(false);
			//m_collision->SetDrawActive(false);
		}
	}

	bool AttackCollision::GetMoveContact() {
		return m_moveContact;
	}

	void AttackCollision::SetMoveContact(bool mc) {
		m_moveContact = mc;
	}

}
//end basecross
