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

	private:
		//void AttackCollision();	// �U������
		//enum State;				// ���			(enum�ŗǂ����͂킩��Ȃ��̂ŋC�ɓ���Ȃ�������ς��Ă�������)
		//void LandDetect();		//				(����悭�킩��Ȃ��̂łƂ肠����void�̊֐��ɂ��Ă����܂�)
	public:
		Actor(const shared_ptr<Stage>& stagePtr);
		~Actor();

	};
	
}
//end basecross
