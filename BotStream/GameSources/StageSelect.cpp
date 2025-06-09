#pragma once
/*!
@file StageSelect.cpp
@brief �X�e�[�W�Z���N�g���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void StageSelect::CreateViewLight() {
		const Vec3 eye(0.0f, 5.0f, -10.0f);
		const Vec3 at(0.0f);
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(eye);
		PtrCamera->SetAt(at);
		//�}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		PtrMultiLight->SetDefaultLighting();
	}

	void StageSelect::OnCreate()
	{
		CreateViewLight();
		CreateSprite();
		CreateBGM();
	}

	void StageSelect::OnUpdate()
	{
		auto cntl = App::GetApp()->GetInputDevice().GetControlerVec();
		auto delta = App::GetApp()->GetElapsedTime();
		auto keybord = App::GetApp()->GetInputDevice().GetKeyState();
		auto ptrMana = App::GetApp()->GetXAudio2Manager();

		// �f�b�h�]�[��
		constexpr float dead = 0.3f;
		// �X�e�[�W�̍ő吔
		constexpr int stageMaxNum = 2;
		constexpr int stageMinNum = 0;
		// �R���g���[���[�̍��X�e�B�b�N�̔���
		Vec2 ret;
		if (cntl[0].bConnected)
		{
			ret.x = cntl[0].fThumbLX;
			ret.y = cntl[0].fThumbLY;
		}

		// ���E�����ꂩ�̃f�b�h�]�[�����A�Ńt���O�N���A
		if (fabs(ret.x) < dead)
			m_selectOnceFlag = false;

		// �E�ɓ|���ƃX�e�[�W���؂�ւ��(+��)
		if (ret.x >= dead && !m_selectOnceFlag && m_selectStageNum < stageMaxNum && !m_stageFlag)
		{

			// �O�̃X�e�[�W�ԍ��̐F��߂�
			m_stageNum[m_selectStageNum]->SetColor(Col4(1, 1, 1, 1));
			// �X�e�[�W�ԍ����X�V
			m_selectStageNum += 1;
			// �X�e�[�W�ԍ��̐F��ύX
			m_stageNum[m_selectStageNum]->SetColor(Col4(1, 1, 0, 1));

			m_selectOnceFlag = true;
		}
		// ���ɓ|���ƃX�e�[�W���؂�ւ��(-��)
		else if (ret.x <= -dead && !m_selectOnceFlag && m_selectStageNum > stageMinNum && !m_stageFlag)
		{
			// �O�̃X�e�[�W�ԍ��̐F��߂�
			m_stageNum[m_selectStageNum]->SetColor(Col4(1, 1, 1, 1));
			// �X�e�[�W�ԍ����X�V
			m_selectStageNum -= 1;
			// �X�e�[�W�ԍ��̐F��ύX
			m_stageNum[m_selectStageNum]->SetColor(Col4(1, 1, 0, 1));

			m_selectOnceFlag = true;
		}

		// A�{�^�����G���^�[�L�[�őI��
		if ((cntl[0].wPressedButtons & XINPUT_GAMEPAD_A || keybord.m_bPressedKeyTbl[VK_RETURN]) && !m_stageFlag)
		{
			m_stageFlag = true;
			m_selectOnceFlag = true;

			// �I�������X�e�[�W�̎ʐ^��\��(���u��)
			for (int i = 0; i < 2; i++)
			{
				m_stagePhoto[i]->OnClear(false);
			}

			//// �I�������X�e�[�W�̎ʐ^��\��(�{�Ԃ̕�)
			//for (int i = m_selectStageNum * 2; i < m_selectStageNum + 2; i++)
			//{
				//m_stagePhoto[i]->OnClear(false);
			//}
			

			// ���̃X�e�[�W���\��
			for (int i = 0; i < 3; i++)
			{
				if (i != m_selectStageNum)
				{
					m_stageNum[i]->OnClear(true);
				}
			}
			// �I�������X�e�[�W���ړ��A�g��
			m_stageNum[m_selectStageNum]->SetPosition(Vec3(-250, 0, 0));
			m_stageNum[m_selectStageNum]->SetScale(Vec3(2.0f, 2.0f, 1.0f));

			m_time = 0;
		}

		// B�{�^�����X�y�[�X�L�[�Ŗ߂�
		if ((cntl[0].wPressedButtons & XINPUT_GAMEPAD_B || keybord.m_bPressedKeyTbl[VK_SPACE]) && m_stageFlag)
		{
			m_stageFlag = false;
			m_selectOnceFlag = true;
			// �I�������X�e�[�W�ȊO��\��
			for (int i = 0; i < 3; i++)
			{
				m_stageNum[i]->OnClear(false);
			}

			// �I�������X�e�[�W��߂�
			m_stageNum[m_selectStageNum]->SetPosition(Vec3(-300 + (m_selectStageNum * 300), 100, 0));
			m_stageNum[m_selectStageNum]->SetScale(Vec3(1.0f, 1.0f, 1.0f));

			// �I�������X�e�[�W�̎ʐ^���\��(���u��)
			for (int i = 0; i < 2; i++)
			{
				m_stagePhoto[i]->OnClear(true);
			}

			//// �I�������X�e�[�W�̎ʐ^���\��(�{�Ԃ̕�)
			//for (int i = m_selectStageNum * 2; i < m_selectStageNum + 2; i++)
			//{
				//m_stagePhoto[i]->OnClear(false);
			//}


		}

		// �X�e�[�W�I�𒆂Ɏʐ^�𓧖��s�������������؂�ւ���
		if (m_stageFlag)
		{
			if (m_time < 1.2f && !m_timeFlag)
			{
				m_time += delta / 2;
				if(m_time > 1.0f) 
					m_timeFlag = true;

			}
			if (m_time > -0.2f && m_timeFlag)
			{
				m_time -= delta / 2;
				if(m_time < 0.0f)
					m_timeFlag = false;

			}

			// ���݂ɓ����x��ς���
			float alphaA = m_time;          
			float alphaB = 1.0f - m_time;

			m_stagePhoto[(m_selectStageNum * 2)]->SetColor(Col4(1, 1, 1, alphaA));
			m_stagePhoto[(m_selectStageNum * 2) + 1]->SetColor(Col4(1, 1, 1, alphaB));
		}

		// A�{�^�����G���^�[�L�[�ōŏI����
		if ((cntl[0].wPressedButtons & XINPUT_GAMEPAD_A || keybord.m_bPressedKeyTbl[VK_RETURN]) && m_stageFlag && !m_selectOnceFlag)
		{
			ptrMana->Stop(m_BGM);
			switch (m_selectStageNum)
			{
			case 0:
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToWaveStage");
				break;
			case 1:
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToWaveStage2");
				break;
			case 2:
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToWaveStage3");
				break;
			}
		}

	}

	void StageSelect::CreateBGM()
	{
		auto ptrMana = App::GetApp()->GetXAudio2Manager();
		m_BGM = ptrMana->Start(L"SelectStage", XAUDIO2_LOOP_INFINITE, 1.0f);


	}

	void StageSelect::CreateSprite()
	{
		float titleX = 1450;
		Vec2 titleSize(titleX, titleX * 0.5625);
		m_selectBackSprite = AddGameObject<Sprite>(
			L"SelectBack",
			titleSize,
			Vec3(0, 0, 0)
		);
		m_selectBackSprite->SetColor(Col4(0.49f, 0.49f, 0.49f, 1));

		Vec2 titleSize2(titleX * 0.25f, titleX * 0.125f);
		m_selectSprite = AddGameObject<Sprite>(
			L"StageSelectString",
			titleSize2,
			Vec3(0, 300, 0)
		);


		//m_textSprite = AddGameObject<Sprite>(
		//	L"TitleText",
		//	titleSize2,
		//	Vec3(0, -250, 0)
		//);

		Vec2 numSize(75, 150);


		for (int i = 0; i < 3; i++)
		{
			m_stageNum[i] = AddGameObject<Sprite>(
				L"Numbers",
				numSize,
				Vec3(-300.0f + (i * 300), 100, 0)
			);
			m_stageNum[i]->SetUVRect(Vec2(0.1f + (0.1f * i), 0), Vec2(0.2f + (0.1f * i), 1));

		}
		m_stageNum[0]->SetColor(Col4(1, 1, 0, 1));

		Vec3 photoPos(300, 100, 0);

		m_stagePhoto[0] = AddGameObject<Sprite>(
			L"Stage1-1Tex",
			Vec2(300, 200),
			photoPos
		);
		m_stagePhoto[1] = AddGameObject<Sprite>(
			L"Stage1-2Tex",
			Vec2(300, 200),
			photoPos
		);

		// �X�e�[�W�ʐ^�̑���
		constexpr int photoNum = 2;

		// �S�ẴX�e�[�W�ʐ^���\���ɂ���A���C���[�ݒ�
		for (int i = 0; i < photoNum; i++)
		{
			m_stagePhoto[i]->OnClear(true);
			m_stagePhoto[i]->SetDrawLayer(3);
		}
	}

}