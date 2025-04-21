/*!
@file PlayerUI.cpp
@brief �v���C���[�Ɋւ���UI
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
#include "PlayerUI.h"

namespace basecross {

	void PlayerGaugeUI::OnCreate()
	{
		m_stage = GetStage();

        const Vec2 gaugeSize(300, 75);
        const Vec2 hpGaugeSize(gaugeSize.x * 0.8f, gaugeSize.y * 0.3f);
        const Vec2 spGaugeSize(gaugeSize.x * 0.47f, gaugeSize.y * 0.09f);
        const float gaugePosX = 0.0f, gaugePosY = -250;

		//Player�Ɋւ���o�[UI����
        m_gaugeFrameSprite = m_stage->AddGameObject<Sprite>(
            L"PLGauge", gaugeSize, Vec3(gaugePosX, gaugePosY, 0));
        m_gaugeFrameSprite->SetDrawLayer(1);

        m_plHPSprite = m_stage->AddGameObject<Sprite>(
            L"PLHP", hpGaugeSize,
            Vec3(gaugePosX + (gaugeSize.x * 0.066f), gaugePosY - 0.7f, 0));
        m_plHPSprite->SetDrawLayer(2);

        m_plSPSprite = m_stage->AddGameObject<Sprite>(
            L"PLSP", spGaugeSize,
            Vec3(gaugePosX - (gaugeSize.x * 0.098f), gaugePosY - 19.8f, 0));
        m_plSPSprite->SetDrawLayer(2);
	}

    void PlayerGaugeUI::OnUpdate()
    {
        Vec3 framePos = m_gaugeFrameSprite->GetComponent<Transform>()->GetPosition();
		auto cntl = App::GetApp()->GetInputDevice().GetControlerVec();

		//// ���FY�{�^���Ńv���C���[��(���������)HP������
		//if (cntl[0].wPressedButtons & XINPUT_GAMEPAD_Y)
		//{
		//	m_playerHP = max(0.0f, m_playerHP - 10.0f);  // �� 10������z��
		//}

		// �䗦�ŃX�P�[�����O(HP)
		float hpRatio = m_playerHP / m_playerMaxHP;
		hpRatio = clamp(hpRatio, 0.0f, 1.0f);

		auto hpTrans = m_plHPSprite->GetComponent<Transform>();
		hpTrans->SetScale(Vec3(hpRatio, 1.0f, 1.0f));

		// ���[�Œ�
		const float gaugeWidth = 300.0f * 0.8f;
		float hpOffsetX = (hpRatio - 1.0f) * (gaugeWidth * 0.5f);
		hpTrans->SetPosition(Vec3(20.0f + hpOffsetX, -200.0f, 0.0f));

		// �g�̈ʒu����̑��΍��W�i�v���}�C�␳�j
		Vec3 hpOffset(300.0f * 0.066f, -0.7f, 0.0f);
		hpTrans->SetPosition(framePos + hpOffset + Vec3(hpOffsetX, 0.0f, 0.0f));

		//// ���FB�{�^���ŕK�E�Z����
		//if (cntl[0].wPressedButtons & XINPUT_GAMEPAD_B)
		//{
		//	m_playerSP = min(m_playerSP + 10.0f, m_playerMaxSP); // ���̐ݒ肾��10�񉟂��ƍő�ɂȂ�
		//}

		// SP�Q�[�W�̔䗦���v�Z�i0.0?1.0�j
		float spRatio = clamp(m_playerSP / m_playerMaxSP, 0.0f, 1.0f);

		auto spTrans = m_plSPSprite->GetComponent<Transform>();
		spTrans->SetScale(Vec3(spRatio, 1.0f, 1.0f));

		// ���[�Œ�̂��߂̈ʒu�␳�i���S���Y�����j
		const float spGaugeWidth = 240.0f;
		float spOffsetX = (spRatio - 1.0f) * (spGaugeWidth * 0.3f);

		// �g�̈ʒu����̑��΍��W
		Vec3 spOffset(-300.0f * 0.098f, -19.8f, 0.0f);
		spTrans->SetPosition(framePos + spOffset + Vec3(spOffsetX, 0.0f, 0.0f));

    }

	//-----------------------------------------------
	//bullet
	//-----------------------------------------------
	void PlayerBulletUI::OnCreate()
	{
		m_stage = GetStage();

		//�e���֌W------------------------------------------------------------
		for (int i = 0; i < 3; ++i)
		{
			auto digit = m_stage->AddGameObject<Sprite>(
				L"Numbers",
				Vec2(m_digitSize, m_digitSize),
				Vec3(m_digitPos.x + i * m_digitSize, m_digitPos.y, 0.0f)
			);
			digit->SetDrawLayer(3); // UI�̍őO�ʂɕ\��
			m_bulletDigits.push_back(digit);
		}
		//--------------------------------------------------------------------

	}

	void PlayerBulletUI::OnUpdate()
	{
		auto cntl = App::GetApp()->GetInputDevice().GetControlerVec();

		auto playerLock = m_player.lock();

		//Player�����Ȃ��Ȃ����玩����������
		if (!playerLock)
		{
			GetStage()->RemoveGameObject<PlayerBulletUI>(GetThis<PlayerBulletUI>());
			return;
		}
		//�v���C���[�̌��݂̋����ɂ���Đ��l���ς��
		m_bulletNum =  m_player.lock()->GetBulletNum();

		// ���FA�{�^����UI�̐�����������
		//if (cntl[0].wPressedButtons & XINPUT_GAMEPAD_A)
		//{
		//	m_bulletNum = max(0, m_bulletNum - 1);
		//}
		//else if (m_bulletNum <= 0)
		//{
		//	m_bulletNum = 90;
		//}

		// �e���𕶎���ɕϊ�
		std::string bulletStr = std::to_string(m_bulletNum);
		size_t digitCount = bulletStr.size();
		Vec2 bulletPos(-100, 0);               // �\���ʒu(CreateSprite�̒l�Ɠ���)
		float uvWidth = 1.0f / 10.0f;          // UV�̕�

		// �����ɉ����ăX�v���C�g���X�V(UV�����X�V)
		for (size_t i = 0; i < m_bulletDigits.size(); ++i)
		{
			auto& digitSprite = m_bulletDigits[i];

			if (i < digitCount)
			{
				// �\�����ׂ����������o��(������int�^��)
				int digit = bulletStr[i] - '0';

				// UV�͈�
				float u1 = digit * uvWidth;
				float u2 = u1 + uvWidth;

				// UV��ݒ肵�ĕ\��
				digitSprite->SetUVRect(Vec2(u1, 0.0f), Vec2(u2, 1.0f));
			}
			else
			{
				// ��������Ȃ��ꍇ�͔�\��
				digitSprite->SetUVRect(Vec2(0.0f, 0.0f), Vec2(0.0f, 0.0f));
			}
		}

	}
}
//end basecross
