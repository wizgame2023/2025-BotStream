/*!
@file ObjectNotMove.h
@brief �����Ȃ��I�u�W�F�N�g
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	class ObjectNotMove : public MyGameObject
	{
	private:
		bool m_pose;//�|�[�Y�t���O
	public:
		ObjectNotMove(const shared_ptr<Stage>& stagePtr);
		~ObjectNotMove();
	protected:
		//void OnCreate()override;
		//void OnUpdate()override;
	};

}
//end basecross