/*!
@file Actor.h
@brief �v���C���[�Ȃ�
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
//#include "MyGameObject.h"

namespace basecross {
	class Actor : public ObjectMove
	{

	protected:
		shared_ptr<Transform> m_trans;
		Vec3 m_pos;//�ʒu
		Vec3 m_rot;//��]
		Vec3 m_scale;//�傫��

		//shared_ptr<AttackCollision> m_AttackCol;
		//shared_ptr<State> m_State;
		//shared_ptr<LandDetect> m_LandDetect;
	public:
		Actor(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale);
		~Actor();

		void OnCreate() override;
	};
	
}
//end basecross
