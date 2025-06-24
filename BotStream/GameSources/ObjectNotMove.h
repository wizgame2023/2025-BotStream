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

	class DarkObj :public ObjectNotMove
	{
	private:
		bool m_pose;//�|�[�Y�t���O

		Vec3 m_pos;//�ʒu
		Vec3 m_rot;//��]
		Vec3 m_scale;//�傫��
		Col4 m_color;//�F

		// �R���W��������̗L����
		bool m_activateCollsion = false; 

	public:
		DarkObj(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, Col4 color = Col4(1.0f, 1.0f, 1.0f, 1.0f)) :
			ObjectNotMove(stagePtr),
			m_pos(pos),
			m_rot(rot),
			m_scale(scale),
			m_color(color)
		{

		}
		~DarkObj()
		{

		}

		void OnCreate()override;
		void OnUpdate()override;

		//m _activateCollsion�̃Z�b�^
		void SetActivateCollsion(bool onOff)
		{
			m_activateCollsion = onOff;
		}


		void OnCollisionEnter(shared_ptr<GameObject>& Other)override;

	protected:

	};

}
//end basecross