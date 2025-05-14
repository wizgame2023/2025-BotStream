/*!
@file PauseUI.cpp
@brief �|�[�Y���
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//����
	void PauseSprite::OnCreate()
	{
		auto stage = GetStage();
		CreateSprite();
	}
	//�X�V
	void PauseSprite::OnUpdate()
	{
		// �R���g���[���[�ƃL�[�{�[�h�̓��͎�t
		auto cntl = App::GetApp()->GetInputDevice().GetControlerVec();
		auto keybord = App::GetApp()->GetInputDevice().GetKeyState();

		// �R���g���[���[�̍��X�e�B�b�N�̔���
		Vec2 ret;
		if (cntl[0].bConnected)
		{
			ret.x = cntl[0].fThumbLX;
			ret.y = cntl[0].fThumbLY;
		}
		// �R���g���[���[���Ȃ����Ă��Ȃ��ꍇ�͏\���L�[�Ή�
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

		// �|�[�Y������Ȃ����ɃX�^�[�g�{�^���������ƃ|�[�Y�ɂȂ�
		if (!m_pauseFlag &&
			(cntl[0].wPressedButtons & XINPUT_GAMEPAD_START ||
				keybord.m_bPressedKeyTbl[VK_SPACE]))
		{
			m_pauseFlag = true;
			m_pauseAudioFlag = false;
			m_select2 = 0;     // ���j���[�C���f�b�N�X 0 ����X�^�[�g
			m_selectFlag = false; // �f�b�h�]�[���t���O�N���A
		}

		// --- �萔��` ------------------------------------
		constexpr int MAIN_MENU_COUNT = 4; // �ĊJ/�X�e�[�W�I��/Audio/�I�� �� 0�`3
		constexpr int AUDIO_MENU_COUNT = 2; // BGM/SE �� 0�`1
		const float dead = 0.3f;            // �f�b�h�]�[��

		// --- Pause OFF �� ���S��\�� ------------------
		if (!m_pauseFlag)
		{
			m_pauseBack->OnClear(true);
			m_selectSprite->OnClear(true);
			MoveSwitchActor(m_pauseFlag);//�A�N�^�[�N���X���ꎞ��~���畜��������
			for (int i = 0; i < MAIN_MENU_COUNT + AUDIO_MENU_COUNT; ++i)
				m_pauseTextSprite[i]->OnClear(true);
			// �i���� m_BGMMater �ȂǕʔz�񂪂���΂����őS�� OnClear(true)�j
		}
		// --- Pause ON �� �K�v�v�f�����\�� -------------
		else
		{
			// �w�i�ƑI�����Ă�Ƃ���͏�ɕ\��
			m_pauseBack->OnClear(false);
			m_selectSprite->OnClear(false);
			MoveSwitchActor(m_pauseFlag);//�A�N�^�[�N���X���ꎞ��~

			// ���C�����j���[ or �I�[�f�B�I���j���[�̐؂�ւ�
			if (!m_pauseAudioFlag)
			{
				// ���C��4�����\��
				for (int i = 0; i < MAIN_MENU_COUNT; ++i)
					m_pauseTextSprite[i]->OnClear(false);
				// Audio���ڂ͉B��
				AudioUIClear(true);
			}
			else
			{
				// ���C�����ڂ��B����
				for (int i = 0; i < MAIN_MENU_COUNT; ++i)
					m_pauseTextSprite[i]->OnClear(true);
				// Audio���ڂ����\��
				AudioUIClear(false);
			}

			// --- �X�e�B�b�N�㉺�Ń��j���[�ړ� -----------
			// �f�b�h�]�[�����畜�A�����玟�̓|����������Z�b�g
			if (fabs(ret.y) < dead)
				m_selectFlag = false;

			// ���̃��j���[���ɉ����čő�C���f�b�N�X����
			int maxIndex = m_pauseAudioFlag
				? (AUDIO_MENU_COUNT - 1)
				: (MAIN_MENU_COUNT - 1);

			// ���|���F++m_select2
			if (ret.y <= -dead && !m_selectFlag)
			{
				m_select2 = clamp(m_select2 + 1, 0, maxIndex - 1);
				m_selectFlag = true;
			}
			// ��|���F--m_select2
			else if (ret.y >= dead && !m_selectFlag)
			{
				m_select2 = clamp(m_select2 - 1, 0, maxIndex);
				m_selectFlag = true;
			}

			// --- �I��`��ʒu�X�V --------------------
			Vec3 base = m_selectPos;
			if (m_pauseAudioFlag)
				base.x += 600;  // Audio���j���[�����E��

			float offsetY = m_pauseAudioFlag
				? (m_select2 * 200 + 50)
				: (m_select2 * 150);
			m_selectSprite->SetPosition(
				Vec3(base.x, base.y - offsetY, base.z)
			);

			// --- Audio�ݒ�̍X�V���� --------------------
			if (m_pauseAudioFlag)
			{
				// �f�b�h�]�[����`
				constexpr float dead = 0.3f;
				constexpr float change = 50.0f;

				// ���E�����ꂩ�̃f�b�h�]�[�����A�Ńt���O�N���A
				if (fabs(ret.x) < dead)
					m_audioSelectFlag = false;

				// �X�e�B�b�N���E�ɓ|������ +0.1f
				if (ret.x >= dead && !m_audioSelectFlag && m_audioMax[m_select2] < 1.0f)
				{
					auto selectPos = m_audioSelect[m_select2]->GetPosition();
					m_audioSelect[m_select2]->SetPosition(Vec3(selectPos.x + change, selectPos.y, selectPos.z));
					m_audioMax[m_select2] = clamp(m_audioMax[m_select2] + 0.1f, 0.0f, 1.0f);
					if (!m_select2)
					{
						m_BGMMater[m_audioMaxSetCol[m_select2]]->SetColor(Col4(0.59f, 0.98f, 0.59f, 1.0f));
					}
					else
					{
						m_SEMater[m_audioMaxSetCol[m_select2]]->SetColor(Col4(0.59f, 0.98f, 0.59f, 1.0f));
					}
					m_audioMaxSetCol[m_select2] += 1;
					m_audioSelectFlag = true;
				}
				// �X�e�B�b�N�����ɓ|������ -0.1f
				else if (ret.x <= -dead && !m_audioSelectFlag && m_audioMax[m_select2] > 0.1f)
				{
					auto selectPos = m_audioSelect[m_select2]->GetPosition();
					m_audioSelect[m_select2]->SetPosition(Vec3(selectPos.x - change, selectPos.y, selectPos.z));
					m_audioMax[m_select2] = clamp(m_audioMax[m_select2] - 0.1f, 0.0f, 1.0f);
					m_audioMaxSetCol[m_select2] -= 1;
					if (!m_select2)
					{
						m_BGMMater[m_audioMaxSetCol[m_select2]]->SetColor(Col4(1.0f, 1.0f, 1.0f, 1.0f));
					}
					else
					{
						m_SEMater[m_audioMaxSetCol[m_select2]]->SetColor(Col4(1.0f, 1.0f, 1.0f, 1.0f));
					}

					m_audioSelectFlag = true;
				}
			}


			// --- ����iB�{�^�� or Enter�j ----------------
			if ((cntl[0].wPressedButtons & XINPUT_GAMEPAD_B) ||
				keybord.m_bPressedKeyTbl[VK_RETURN])
			{
				if (!m_pauseAudioFlag)
				{
					switch (m_select2)
					{
					case 0: // �ĊJ
						m_pauseFlag = false;
						break;
					case 1: // �X�e�[�W�Z���N�g�֑J��
						PostEvent(0.0f,
							GetThis<ObjectInterface>(),
							App::GetApp()->GetScene<Scene>(),
							L"ToStageSelect");
						m_pauseFlag = false;
						break;
					case 2: // �I�[�f�B�I�ݒ��
						// ���C�����B����Audio�\���Ɉڍs
						m_pauseAudioFlag = true;
						m_select2 = 0;
						m_selectFlag = false;
						break;
					case 3: // �Q�[���I���i�K�v�Ȃ�����j
					default:
						break;
					}
				}
				else
				{
					// Audio���j���[���̌��菈��
					if ((cntl[0].wPressedButtons & XINPUT_GAMEPAD_B) ||
						keybord.m_bPressedKeyTbl[VK_RETURN])
					{
						m_select2 = 0;
						m_pauseAudioFlag = false;
					}
				}
			}
		}

	}

	// END UPDATE-------------------

	void PauseSprite::AudioUIClear(bool clear)
	{
		// --- �萔��` ------------------------------------
		constexpr int MAIN_MENU_COUNT = 4; // �ĊJ/�X�e�[�W�I��/Audio/�I�� �� 0�`3
		constexpr int AUDIO_MENU_COUNT = 2; // BGM/SE �� 0�`1
		constexpr int AUDIO_MATER = 10;

		for (int i = MAIN_MENU_COUNT; i < MAIN_MENU_COUNT + AUDIO_MENU_COUNT; ++i)
		{
			m_pauseTextSprite[i]->OnClear(clear);
			m_audioSelect[i - MAIN_MENU_COUNT]->OnClear(clear);
			m_speaker[i - MAIN_MENU_COUNT]->OnClear(clear);
		}

		for (int i = 0; i < AUDIO_MATER; i++)
		{
			m_BGMMater[i]->OnClear(clear);
			m_SEMater[i]->OnClear(clear);
		}
	}



	void PauseSprite::CreateSprite()
	{
		m_stage = GetStage();
		constexpr float XY1 = 1450, textPosX = -400, textPosY = 300;
		// pause�w�i
		m_pauseBack = m_stage->AddGameObject<Sprite>(
			L"PauseBack",
			Vec2(XY1, XY1 * 0.5625f),
			Vec3(0, 0, 0)
		);
		m_pauseBack->OnClear(true);
		m_pauseBack->SetDrawLayer(0);

		// �|�[�Y���̃e�L�X�g(UV���W�͂����ł͐ݒ肵�Ă��Ȃ��̂ŉ��ɏ���)
		for (int i = 0; i < 4; i++)
		{
			m_pauseTextSprite[i] = m_stage->AddGameObject<Sprite>(
				L"PauseText",
				Vec2(250, 250 / 2),
				Vec3(textPosX, textPosY - (i * 150), 0)
			);
			m_pauseTextSprite[i]->SetDrawLayer(1);
			m_pauseTextSprite[i]->OnClear(true);

		}

		// �I�[�f�B�I�ݒ�̃e�L�X�g
		for (int i = 4; i < 6; i++)
		{
			m_pauseTextSprite[i] = m_stage->AddGameObject<Sprite>(
				L"PauseText",
				Vec2(200, 200 / 2),
				Vec3(200, 900 - (i * 200), 0)
			);
			m_pauseTextSprite[i]->SetDrawLayer(1);
			m_pauseTextSprite[i]->OnClear(true);
		}

		// pause
		m_pauseTextSprite[0]->SetUVRect(Vec2(0.0f, 0.0f), Vec2(0.333f, 0.5f));
		// �ĊJ
		m_pauseTextSprite[1]->SetUVRect(Vec2(0.333f, 0.0f), Vec2(0.666f, 0.5f));
		// �Z���N�g
		m_pauseTextSprite[2]->SetUVRect(Vec2(0.666f, 0.0f), Vec2(1.0f, 0.5f));
		// �I�[�f�B�I
		m_pauseTextSprite[3]->SetUVRect(Vec2(0.0f, 0.5f), Vec2(0.333f, 1.0f));
		// BGM
		m_pauseTextSprite[4]->SetUVRect(Vec2(0.333f, 0.5f), Vec2(0.666f, 1.0f));
		// SE
		m_pauseTextSprite[5]->SetUVRect(Vec2(0.666f, 0.5f), Vec2(1.0f, 1.0f));

		// BGMTextPos
		auto BGMPos = m_pauseTextSprite[4]->GetPosition();
		// SETextPos
		auto SEPos = m_pauseTextSprite[5]->GetPosition();

		// BGM�̃��[�^�[
		for (int i = 0; i < 10; i++)
		{
			auto audioPos = /*i ? SEPos : */BGMPos;
			audioPos.x -= 200;
			m_BGMMater[i] = m_stage->AddGameObject<Sprite>(
				L"AudioOther",
				Vec2(50, 100),
				Vec3(audioPos.x + (50.0f * i), audioPos.y - 100, audioPos.z)
			);
			m_BGMMater[i]->SetDrawLayer(1);
			m_BGMMater[i]->OnClear(true);
			m_BGMMater[i]->SetUVRect(Vec2(0.5f, 0.0f), Vec2(0.75f, 1.0f));
			m_BGMMater[i]->SetColor(Col4(0.59f, 0.98f, 0.59f, 1.0f));
		}

		// SE�̃��[�^�[
		for (int i = 0; i < 10; i++)
		{
			auto sePos = SEPos;
			sePos.x -= 200;
			m_SEMater[i] = m_stage->AddGameObject<Sprite>(
				L"AudioOther",
				Vec2(50, 100),
				Vec3(sePos.x + (50.0f * i), sePos.y - 100, sePos.z)
			);
			m_SEMater[i]->SetDrawLayer(1);
			m_SEMater[i]->OnClear(true);
			m_SEMater[i]->SetUVRect(Vec2(0.5f, 0.0f), Vec2(0.75f, 1.0f));
			m_SEMater[i]->SetColor(Col4(0.59f, 0.98f, 0.59f, 1.0f));
		}

		// �X�s�[�J�[�̐}�`
		for (int i = 0; i < 2; i++)
		{
			auto audioPos = i ? SEPos : BGMPos;
			m_speaker[i] = m_stage->AddGameObject<Sprite>(
				L"AudioOther",
				Vec2(80, 80),
				Vec3(audioPos.x - 300, audioPos.y - 100, audioPos.z)
			);
			m_speaker[i]->SetDrawLayer(1);
			m_speaker[i]->SetUVRect(Vec2(0.0f, 0.0f), Vec2(0.5f, 1.0f));
			m_speaker[i]->OnClear(true);
		}

		// �I�����Ă�Ƃ���
		for (int i = 0; i < 2; i++)
		{
			auto bgmMax = m_BGMMater[9]->GetPosition();
			auto seMax = m_SEMater[9]->GetPosition();
			auto audioPos = i ? seMax : bgmMax;
			m_audioSelect[i] = m_stage->AddGameObject<Sprite>(
				L"AudioOther",
				Vec2(50, 100),
				Vec3(audioPos)
			);
			m_audioSelect[i]->SetDrawLayer(1);
			m_audioSelect[i]->SetUVRect(Vec2(0.75f, 0.0f), Vec2(1.0f, 1.0f));
			m_audioSelect[i]->OnClear(true);
			m_audioSelect[i]->SetColor(Col4(0.52f, 0.8f, 0.92f, 1.0f));
		}

		Vec2 selectSize(400, 300);
		m_selectPos = Vec3(textPosX, textPosY - 150, 0);
		// �I�����Ă�Ƃ���̂��
		m_selectSprite = m_stage->AddGameObject<Sprite>(
			L"Select",			//�e�N�X�`����
			selectSize,      // �T�C�Y
			m_selectPos);        // �\���ʒu
		m_selectSprite->OnClear(true);
		m_selectSprite->SetDrawLayer(2);

	}

	//Actor���ꎞ��~�������蓮�������肷�鏈��(�|�[�Y���)
	void PauseSprite::MoveSwitchActor(bool OnOff)
	{
		//�X�C�b�`���I�����Ǝ~�܂�
		if (OnOff)
		{
			auto objVec = GetStage()->GetGameObjectVec();
			for (auto obj : objVec)
			{
				auto actor = dynamic_pointer_cast<Actor>(obj);
				auto cameraManager = dynamic_pointer_cast<CameraManager>(obj);

				if (actor)
				{
					actor->SetPose(true);
				}
				if (cameraManager)
				{
					cameraManager->SetPose(true);
				}
			}
		}
		//�I�t�Ȃ瓮��
		if (!OnOff)
		{
			auto objVec = GetStage()->GetGameObjectVec();
			for (auto obj : objVec)
			{
				auto actor = dynamic_pointer_cast<Actor>(obj);	
				auto cameraManager = dynamic_pointer_cast<CameraManager>(obj);

				if (actor)
				{
					actor->SetPose(false);
				}
				if (cameraManager)
				{
					cameraManager->SetPose(false);
				}
			}
		}
	}
}