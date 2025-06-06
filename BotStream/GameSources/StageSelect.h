#pragma once
/*!
@file StageSelect.h
@brief �X�e�[�W�Z���N�g���
*/

#include "stdafx.h"

namespace basecross {
	class StageSelect : public Stage
	{
		//�r���[�̍쐬
		void CreateViewLight();
		shared_ptr<Sprite> m_selectSprite;
		shared_ptr<Sprite> m_selectBackSprite;
		shared_ptr<Sprite> m_textSprite;
		shared_ptr<Sprite> m_stagePhoto[2];
		shared_ptr<SoundItem> m_BGM;

		shared_ptr<Sprite> m_stageNum[3];

		Vec3 m_selectPos;

		// �؂�ւ��̎��Ɉ�C�ɐ؂�ւ��Ȃ��悤�ɂ��邽�߂̃t���O
		bool m_selectOnceFlag = false;

		// �I��ł���X�e�[�W��
		short int m_selectStageNum = 0;

		bool m_stageFlag = false; // �X�e�[�W�I���t���O

	public:
		//�\�z�Ɣj��
		StageSelect() :Stage() {}
		virtual ~StageSelect() {}

		void CreateSprite();
		void CreateBGM();

		//������
		virtual void OnCreate()override;
		virtual void OnUpdate()override;

	};
}