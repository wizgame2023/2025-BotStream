/*!
@file GameClearScene.cpp
@brief �N���A���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------
	void GameClear::CreateViewLight() {
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

	void GameClear::OnCreate()
	{
		CreateViewLight();
		CreateSprite();
	}

	void GameClear::OnUpdate()
	{
		auto cntl = App::GetApp()->GetInputDevice().GetControlerVec();
		auto keybord = App::GetApp()->GetInputDevice().GetKeyState();

		Vec2 ret;
		if (cntl[0].bConnected)
		{
			ret.x = cntl[0].fThumbLX;
			ret.y = cntl[0].fThumbLY;
		}
		else if (!cntl[0].bConnected)
		{
			if (keybord.m_bPushKeyTbl[VK_UP])
				ret.y = 1;

			if (keybord.m_bPushKeyTbl[VK_LEFT])
				ret.x = -1;

			if (keybord.m_bPushKeyTbl[VK_DOWN])
				ret.y = -1;

			if (keybord.m_bPushKeyTbl[VK_RIGHT])
				ret.x = 1;

		}

		// L�X�e�B�b�N�����E�ɓ|���ƑI���ł���
		if (ret.x >= 0.3f && !m_selectFlag && m_select < 2)
		{
			m_select++;
			if (m_select == 1)
			{
				m_selectSprite[0]->SetPosition(Vec3((m_selectPos.x + m_select * 350) - 130, m_selectPos.y, m_selectPos.z));
				m_selectSprite[1]->SetPosition(Vec3((m_selectPos.x + m_select * 350) + 130, m_selectPos.y, m_selectPos.z));
			}
			else if (m_select == 2)
			{
				m_selectSprite[0]->SetPosition(Vec3((m_selectPos.x + m_select * 350) - 50, m_selectPos.y, m_selectPos.z));
				m_selectSprite[1]->SetPosition(Vec3((m_selectPos.x + m_select * 350) + 50, m_selectPos.y, m_selectPos.z));
			}
			m_selectFlag = true;
		}
		else if (ret.x <= -0.3f && !m_selectFlag && m_select > 0)
		{
			m_select--;
			if (m_select == 1)
			{
				m_selectSprite[0]->SetPosition(Vec3((m_selectPos.x + m_select * 350) - 130, m_selectPos.y, m_selectPos.z));
				m_selectSprite[1]->SetPosition(Vec3((m_selectPos.x + m_select * 350) + 130, m_selectPos.y, m_selectPos.z));
			}
			else if (m_select == 0)
			{
				m_selectSprite[0]->SetPosition(Vec3((m_selectPos.x + m_select * 350) - 110, m_selectPos.y, m_selectPos.z));
				m_selectSprite[1]->SetPosition(Vec3((m_selectPos.x + m_select * 350) + 110, m_selectPos.y, m_selectPos.z));
			}
			m_selectFlag = true;
		}
		else if ((ret.x <= 0.29f && ret.x >= -0.29f) && m_selectFlag)
		{
			m_selectFlag = false;
		}

		// A�{�^�����G���^�[�L�[�Ō���
		if (cntl[0].wPressedButtons & XINPUT_GAMEPAD_A || keybord.m_bPressedKeyTbl[VK_RETURN])
		{
			//switch (m_select)
			//{
			//// ���̃X�e�[�W
			//case 0:
			//	PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToWaveStage2");
			//	break;
			//// �Z���N�g�X�e�[�W
			//case 1:
			//	PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToSelectStage");
			//	break;
			//// �^�C�g��
			//case 2:
			//	PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage");
			//	break;

			////��O���N������^�C�g��
			//default:
			//	PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage");
			//	break;
			//}
		}
	}

	void GameClear::CreateSprite()
	{
		float overSizeX = 1000, overSizeY = overSizeX * 0.175f;
		auto sprite = AddGameObject<Sprite>(
			L"ClearOverText",
			Vec2(overSizeX, overSizeY),
			Vec3(0, 200, 0)
		);
		sprite->SetUVRect(Vec2(0, 0), Vec2(1, 0.25f));

		//�X�e�[�W�Z���N�g
		float selectSizeX = 300, selectSizeY = selectSizeX * 0.8f;
		float selectPosX = 0, selectPosY = -200;
		sprite = AddGameObject<Sprite>(
			L"ClearOverText",
			Vec2(selectSizeX, selectSizeY),
			Vec3(selectPosX, selectPosY, 0)
		);
		sprite->SetUVRect(Vec2(0, 0.5f), Vec2(0.5f, 0.75f));

		// ���X�^�[�g
		sprite = AddGameObject<Sprite>(
			L"ClearOverText",
			Vec2(selectSizeX, selectSizeY),
			Vec3(selectPosX - 350, selectPosY, 0)
		);
		sprite->SetUVRect(Vec2(0.5f, 0.75f), Vec2(1, 1));

		// �^�C�g��
		sprite = AddGameObject<Sprite>(
			L"ClearOverText",
			Vec2(selectSizeX, selectSizeY),
			Vec3(selectPosX + 350, selectPosY, 0)
		);
		sprite->SetUVRect(Vec2(0, 0.75f), Vec2(0.5f, 1));

		float selectFrameSizeX = 200, selectFrameSizeY = selectFrameSizeX * 0.8f/* * 0.375f*/;
		m_selectPos = Vec3(selectPosX - 350, selectPosY, 0);
		// �I�����Ă���Ƃ���̂��[0]
		m_selectSprite[0] = AddGameObject<Sprite>(
			L"Select",			//�e�N�X�`����
			Vec2(selectFrameSizeX, selectFrameSizeY),      // �T�C�Y
			m_selectPos + Vec3(-110, 0, 0) // �\���ʒu
		);
		m_selectSprite[0]->SetUVRect(Vec2(0, 0), Vec2(0.5f, 1));

		// �I�����Ă���Ƃ���̂��[1]
		m_selectSprite[1] = AddGameObject<Sprite>(
			L"Select",			//�e�N�X�`����
			Vec2(selectFrameSizeX, selectFrameSizeY),      // �T�C�Y
			m_selectPos + Vec3(110, 0, 0) // �\���ʒu
		);
		m_selectSprite[1]->SetUVRect(Vec2(0.5f, 0), Vec2(1, 1));

	}
}