/*!
@file StageSato.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class StageSato : public Stage {
		//�r���[�̍쐬
		void CreateViewLight();
		void CreateSprite();

		//--------�ʂ��Ƃ��͂������R�s�y����΂����Ǝv����--------

		// ���i�̃X�e�[�^�X
		struct PersonalState
		{
			int Good = 0;
			int Evil = 0;
			int Lawful = 0;
			int Chaos = 0;
		};


		enum PlayerType
		{
			Type_Speed,
			Type_Power,
			Type_Balance,
		};

		//shared_ptr<Sprite> m_plHPSprite;
		//shared_ptr<Sprite> m_plSPSprite;
		//shared_ptr<Sprite> m_gaugeFrameSprite;
		//shared_ptr<Sprite> m_katanaSprite;
		//shared_ptr<Sprite> m_gunSprite;
		//shared_ptr<Sprite> m_questionSprite[5];
		//shared_ptr<Sprite> m_answerSprite[5][3];
		//shared_ptr<Sprite> m_resultSprite;

		shared_ptr<Sprite> m_selectSprite;

		shared_ptr<Sprite> m_personalSelect;
		shared_ptr<Sprite> m_pauseBack;
		shared_ptr<Sprite> m_pauseTextSprite[6];
		shared_ptr<Sprite> m_speaker[2];
		shared_ptr<Sprite> m_audioMater[2];
		shared_ptr<Sprite> m_audioSelect[2];

		// �X�v���C�g�̃��X�g
		std::vector<std::shared_ptr<Sprite>> m_bulletDigits;
		// ����̓��e�����Ă������߂̃��X�g
		std::vector<int> m_questionOrder;

		float m_playerMaxHP = 1000.0f;		// HP�̍ő�l
		float m_playerHP = m_playerMaxHP;	// HP�̏����l

		float m_playerMaxSP = 100.0f;   // SP�̍ő�l
		float m_playerSP = 0.0f;        // �����lSP

		// ����؂�ւ��̃t���O
		bool m_weaponSwitchFlag = true;

		// �I��؂�ւ��t���O
		bool m_selectFlag = false;
		bool m_questionEndFlag = false;
		// Audio�p�̑I��؂�ւ��t���O
		bool m_audioSelectFlag = false;
		int m_select = 0;
		Vec3 m_selectPos;

		bool m_pauseFlag = false;
		bool m_pauseAudioFlag = false;
		float m_audioMax[2] = { 1.0f , 1.0f };

		int m_select2 = 0;

		PlayerType m_type = PlayerType::Type_Speed;

		// ���U���g�\���t���O
		bool m_resultFlag = false;

		// �e��
		int m_bulletNum = 90;
		// �e���̐����̑傫��
		float m_digitSize = 40;

		// �����m��Ȃ��I���[�J���ϐ��̃o�J�I
		int m_switchQues = 0;
		int m_questionNum = 0;

		PersonalState m_personality;
		//----------------------------------------------------------

	public:
		//�\�z�Ɣj��
		StageSato() :Stage() {}
		virtual ~StageSato() {}
		//������
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual void OnDraw()override;

		// �f�o�b�O�p������
		void DebugLog();

		// ���̐��K��
		float NormalizeAxis(float minusSide, float plusSide);

		// ���i�f�f�̃X�e�[�^�X�ϓ�
		//void PersonalStateChange(int questionID, int answerIndex);

		//void StateResult(int LawCha, int EvilGood);

		void ShowNumber(int value, Vec2 pos, float digitSize);

		// player�̍ő�HP���擾(sprite)
		float GetPLMaxHPSprite()
		{
			return m_playerMaxHP;
		}

		// player�̌��݂�HP���擾(sprite)
		float GetPLHPSprite()
		{
			return m_playerHP;
		}

		// player�̍ő�SP(�K�E�Z�Q�[�W)���擾(sprite)
		float GetPLMaxSPSprite()
		{
			return m_playerMaxSP;
		}

		// player�̌��݂�SP(�K�E�Z�Q�[�W)���擾(sprite)
		float GetPLSPSprite()
		{
			return m_playerSP;
		}

		// player�̍ő�HP��ݒ�(sprite)
		void SetPLMaxHPSprite(float value)
		{
			m_playerMaxHP = value;
		}

		// player�̍ő�SP(�K�E�Z�Q�[�W)��ݒ�(sprite)
		void SetPLMaxSPSprite(float value)
		{
			m_playerMaxSP = value;
		}

		// player�̍ő呕�U���̐ݒ�(sprite)
		void SetPLBulletNumSprite(float value)
		{
			m_bulletNum = value;
		}

		// player�̏������i(�^�C�v)���擾
		PlayerType GetPLType()
		{
			return m_type;
		}

		// �䗦�݂����Ȃ�����[�����[��������
		template <typename T>
		T clamp(T value, T minValue, T maxValue)
		{
			if (value < minValue) return minValue;
			if (value > maxValue) return maxValue;
			return value;
		}

	};


}
//end basecross

