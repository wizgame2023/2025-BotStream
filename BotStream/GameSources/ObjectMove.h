/*!
@file ObjectMove.h
@brief �����I�u�W�F�N�g
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	class ObjectMove : public MyGameObject
	{
	private:
		bool m_pose;//�|�[�Y�t���O
	public:
		ObjectMove(const shared_ptr<Stage>& stagePtr);
		~ObjectMove();
	protected:
		//void OnCreate()override;
		//void OnUpdate()override;
	};

}
//end basecross#pragma once
