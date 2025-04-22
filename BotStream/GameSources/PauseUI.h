#pragma once
/*!
@file PauseUI.h
@brief �|�[�Y���
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {

	class Pause :public MyGameObject
	{
	private:
		//�|�[�Y����Ă��邩�̃t���O
		bool m_pauseFlag = false;

		//�X�e�B�b�N�X�����󂯓����t���O
		bool m_stick = false;
		//�I�����Ă�����̂�ۑ�����t���O
		int m_select = 1;

		//�X�e�[�W�Z���N�g��UI
		shared_ptr<Sprite> m_stageSelectUI;
		//�^�C�g����UI
		shared_ptr<Sprite> m_titleUI;

		//�I���E
		shared_ptr<Sprite> m_selectSpriteR;
		//�I����
		shared_ptr<Sprite> m_selectSpriteL;

	public:
		Pause(shared_ptr<Stage>& StagePtr) :
			MyGameObject(StagePtr)
		{

		}
		~Pause()
		{
		}

		void OnCreate()override;
		void OnUpdate()override;
	};

}