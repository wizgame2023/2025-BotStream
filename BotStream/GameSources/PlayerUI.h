#pragma once
/*!
@file PlayerUI.h
@brief �v���C���[�Ɋւ���UI
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {

	class PlayerGaugeUI : public MyGameObject {

		shared_ptr<Sprite> m_gaugeFrameSprite;
		shared_ptr<Sprite> m_plHPSprite;
		shared_ptr<Sprite> m_plSPSprite;
		shared_ptr<Stage> m_stage;

		weak_ptr<Player> m_player;

		float m_playerMaxHP = 1000.0f;		// HP�̍ő�l
		float m_playerHP = m_playerMaxHP;	// HP�̏����l

		float m_playerMaxSP = 100.0f;   // SP�̍ő�l
		float m_playerSP = 0.0f;        // �����lSP

		void AllClear(bool clear);
	public:
		PlayerGaugeUI(const shared_ptr<Stage>& stagePtr,shared_ptr<Player> player) :
			MyGameObject(stagePtr),
			m_player(player)
		{
		}
		virtual ~PlayerGaugeUI() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

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

		// player��HP�̍X�V(sprite)
		void SetPLHPSprite(float value)
		{
			m_playerHP = value;
		}

		// player�̍ő�SP(�K�E�Z�Q�[�W)��ݒ�(sprite)
		void SetPLMaxSPSprite(float value)
		{
			m_playerMaxSP = value;
		}

		// player�̍ő�SP(�K�E�Z�Q�[�W)��ݒ�(sprite)
		void SetPLSPSprite(float value)
		{
			m_playerSP = value;
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

	class PlayerBulletUI : public MyGameObject
	{
		// �X�v���C�g�̃��X�g
		vector<std::shared_ptr<Sprite>> m_bulletDigits;

		shared_ptr<Stage> m_stage;
		weak_ptr<Player> m_player;

		// �e��
		int m_bulletNum = 90;
		// �e���̐����̑傫��
		float m_digitSize = 40;
		// �����̈ʒu
		Vec2 m_digitPos;

	public:
		// digitPos:�ʒu�ݒ�
		// bulletNum:�ő呕�U��
		// digitSize:�����̑傫��
		PlayerBulletUI(const shared_ptr<Stage>& stagePtr, shared_ptr<Player> player, Vec2 digitPos, float digitSize = 40.0f) :
			MyGameObject(stagePtr),
			m_digitPos(digitPos),
			m_digitSize(digitSize),
			m_player(player)
		{
		}
		virtual ~PlayerBulletUI() {}

		//�e�̐��̃Z�b�^�[
		void SetBullet(int setBullet)
		{
			m_bulletNum = setBullet;
		}
		//�e�̐��̃Q�b�^�[
		int GetBullet()
		{
			return m_bulletNum;
		}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

	};


	//==============================================================================
	// �R���g���[���{�^���A�C�R���\��UI
	//==============================================================================
	class PlayerButtonUI : public MyGameObject {
		Vec2 m_buttonPos;
		Vec2 m_buttonSize;
		//int  m_buttonSwitch;
		shared_ptr<Sprite> m_button;

		shared_ptr<Stage> m_stage;
	public:
		// stagePtr: �X�e�[�W, buttonPos: �A�C�R���ʒu, buttonSize: �T�C�Y, switch: �I���C���f�b�N�X
		PlayerButtonUI(const std::shared_ptr<Stage>& stagePtr,
			const Vec2& buttonPos,
			const Vec2& buttonSize/*,
			int buttonSwitch*/
		) :
			MyGameObject(stagePtr),
			m_buttonPos(buttonPos),
			m_buttonSize(buttonSize)//,
			//m_buttonSwitch(buttonSwitch)
		{
		}

		virtual ~PlayerButtonUI() {}

		virtual void OnCreate() override;

	};

	//==============================================================================
	// �R���g���[���{�^���̃e�L�X�g�\��UI
	//==============================================================================
	class PlayerButtonText : public MyGameObject {
		Vec2 m_textPos;
		Vec2 m_textSize;
		int  m_textSwitch;
		shared_ptr<Sprite> m_buttonText;

		shared_ptr<Stage> m_stage;

	public:
		// stagePtr: �X�e�[�W, textPos: �����ʒu, textSize: �T�C�Y, switch: �I���C���f�b�N�X
		PlayerButtonText(const std::shared_ptr<Stage>& stagePtr,
			const Vec2& textPos,
			const Vec2& textSize,
			int textSwitch
		) :
			MyGameObject(stagePtr),
			m_textPos(textPos),
			m_textSize(textSize),
			m_textSwitch(textSwitch)
		{
		}
		virtual ~PlayerButtonText() {}

		virtual void OnCreate() override;
	};

	//==============================================================================
	// ����ؑ�UI
	//==============================================================================
	class PlayerWeaponUI : public MyGameObject {

		shared_ptr<Stage> m_stage;

		shared_ptr<Sprite> m_fightSprite[8];

	public:
		// stagePtr: �X�e�[�W
		PlayerWeaponUI(const std::shared_ptr<Stage>& stagePtr) :
			MyGameObject(stagePtr)
		{
		}

		virtual ~PlayerWeaponUI() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void AllFightSpriteClear(bool clear);
	};

	//==============================================================================
	// �����p�[�cUI
	//==============================================================================
	class PartsTextChange : public MyGameObject {
		shared_ptr<Stage> m_stage;
		shared_ptr<Sprite> m_partsTextSprite[3];
		shared_ptr<Sprite> m_num[3];
	public:
		PartsTextChange(const std::shared_ptr<Stage>& stagePtr) :
			MyGameObject(stagePtr)
		{
		}

		virtual ~PartsTextChange() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void AllClear(bool clear);
	};
}
// namespace basecross
