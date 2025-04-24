/*!
@file PauseUI.cpp
@brief �|�[�Y���
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//����
	void Pause::OnCreate()
	{
		auto stage = GetStage();

		////�X�e�[�W�Z���N�g
		//float selectSizeX = 250, selectSizeY = selectSizeX * 0.5f;
		//float selectPosX = 0, selectPosY = 200;
		//m_stageSelectUI = stage->AddGameObject<Sprite>(
		//	L"ClearOverText",
		//	Vec2(selectSizeX, selectSizeY),
		//	Vec3(selectPosX, selectPosY, 0)
		//);
		//m_stageSelectUI->SetUVRect(Vec2(0, 0.5f), Vec2(0.5f, 0.75f));

		//// �^�C�g��
		//m_titleUI = stage->AddGameObject<Sprite>(
		//	L"ClearOverText",
		//	Vec2(selectSizeX, selectSizeY),
		//	Vec3(selectPosX, selectPosY-100, 0)
		//);
		//m_titleUI->SetUVRect(Vec2(0, 0.75f), Vec2(0.5f, 1));

		//float selectFrameSizeX = 200, selectFrameSizeY = selectFrameSizeX * 0.8f/* * 0.375f*/;
		//auto m_selectPos = Vec3(selectPosX, selectPosY, 0);
		//// �I�����Ă���Ƃ���̂��[��]
		//m_selectSpriteL = stage->AddGameObject<Sprite>(
		//	L"Select",			//�e�N�X�`����
		//	Vec2(selectFrameSizeX, selectFrameSizeY),      // �T�C�Y
		//	m_selectPos + Vec3(-110, 0, 0) // �\���ʒu
		//);
		//m_selectSpriteL->SetUVRect(Vec2(0, 0), Vec2(0.5f, 1));

		//// �I�����Ă���Ƃ���̂��[�E]
		//m_selectSpriteR = stage->AddGameObject<Sprite>(
		//	L"Select",			//�e�N�X�`����
		//	Vec2(selectFrameSizeX, selectFrameSizeY),      // �T�C�Y
		//	m_selectPos + Vec3(110, 0, 0) // �\���ʒu
		//);
		//m_selectSpriteR->SetUVRect(Vec2(0.5f, 0), Vec2(1, 1));



	}
	//�X�V
	void Pause::OnUpdate()
	{
		// �C���v�b�g�f�o�C�X�I�u�W�F�N�g
		InputDevice inputDevice = App::GetApp()->GetInputDevice(); // �l�X�ȓ��̓f�o�C�X���Ǘ����Ă���I�u�W�F�N�g���擾
		//�R���g���[���[�̎擾
		auto m_controler = inputDevice.GetControlerVec()[0];
		auto m_contrloerVec = Vec2(m_controler.fThumbRX, m_controler.fThumbRY);

		//auto selectPos = m_stageSelectUI->GetPosition();

		////�I���ړ�
		//float selectPosX = 0, selectPosY = 200;
		//float selectSizeX = 250, selectSizeY = selectSizeX * 0.5f;
		//float selectFrameSizeX = 200, selectFrameSizeY = selectFrameSizeX * 0.8f/* * 0.375f*/;
		//auto m_selectPos = Vec3(selectPosX, selectPosY, 0);
		//if (m_contrloerVec.y <= -0.5f && !m_stick && m_select < 2)
		//{
		//	m_stick = true;
		//	m_select++;
		//	if (m_select == 2)
		//	{
		//		m_selectSpriteL->SetPosition(Vec3(selectSizeX-110+30, selectSizeY, 1.0f));
		//		m_selectSpriteR->SetPosition(Vec3(selectSizeX+110+30, selectSizeY, 1.0f));
		//	}
		//}
		
		
		
		
		if (!m_pauseFlag)
		{
		
			//�|�[�Y�J�n
			if (m_controler.wPressedButtons & XINPUT_GAMEPAD_START)
			{
				m_pauseFlag = true;//�|�[�Y���Ă����Ԃ̃t���O

				auto objVec = GetStage()->GetGameObjectVec();
				for (auto obj : objVec)
				{
					auto actor = dynamic_pointer_cast<Actor>(obj);
					if (actor)
					{
						actor->PoseSwitch(true);
					}
				}
			}

		}
		else if (m_pauseFlag)//�|�[�Y�I��
		{

			//�|�[�Y����
			if (m_controler.wPressedButtons & XINPUT_GAMEPAD_START)
			{
				m_pauseFlag = false;//�|�[�Y���Ă��Ȃ���Ԃ̃t���O

				auto objVec = GetStage()->GetGameObjectVec();
				for (auto obj : objVec)
				{
					auto actor = dynamic_pointer_cast<Actor>(obj);
					if (actor)
					{
						actor->PoseSwitch(false);
					}
				}
			}

		}

	}

}