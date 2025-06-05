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
		auto keybord = App::GetApp()->GetInputDevice().GetKeyState();
		auto ptrMana = App::GetApp()->GetXAudio2Manager();

		// �f�b�h�]�[��
		constexpr float dead = 0.3f;
		// �X�e�[�W�̍ő吔
		constexpr int stageMaxNum = 3;
		constexpr int stageMinNum = 1;
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
		if (ret.x >= dead && !m_selectOnceFlag && m_selectStageNum < stageMaxNum)
		{
			
			m_selectStageNum += 1;
			m_selectOnceFlag = true;
		}
		// ���ɓ|���ƃX�e�[�W���؂�ւ��(-��)
		else if (ret.x <= -dead && !m_selectOnceFlag && m_selectStageNum > stageMinNum)
		{
			m_selectStageNum -= 1;
			m_selectOnceFlag = true;
		}
		// A�{�^�����G���^�[�L�[�Ō���
		if (cntl[0].wPressedButtons & XINPUT_GAMEPAD_A || keybord.m_bPressedKeyTbl[VK_RETURN])
		{
			ptrMana->Stop(m_BGM);
			switch (m_selectStageNum)
			{
			case 1:
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToWaveStage");
				break;
			case 2:
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToWaveStage2");
				break;
			case 3:
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
		m_selectBackSprite->SetColor(Col4(0.49f,0.49f,0.49f,1));

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
	}

}