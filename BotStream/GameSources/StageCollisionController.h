#pragma once

/*!
@file StageCollisionController.h
@brief �����蔻��̑���p
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	class StageCollisionController :public MyGameObject
	{
	private:
		bool m_CollisionSwhich;

		void CollisionActive(bool On);//�X�e�[�W��̗L���ȃR���W�����͈͂����߂�

	public:
		StageCollisionController(const shared_ptr<Stage>& stagePtr);
		~StageCollisionController();

		void OnCreate()override;
		void OnUpdate()override;
		void SetCollisionSwhich(bool On);//�R���W�����͈͂����߂邽�߂̃t���O���擾����֐�

	};

}