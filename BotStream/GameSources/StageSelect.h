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
		shared_ptr<Sprite> m_stagePhoto[6];
		shared_ptr<Sprite> m_tutorialSprite;

		// ����ƃL�����Z���B
		shared_ptr<Sprite> m_AButtonSp;
		shared_ptr<Sprite> m_BButtonSp;
		shared_ptr<Sprite> m_enterKeySp;
		shared_ptr<Sprite> m_backSpaceKeySp;
		shared_ptr<Sprite> m_textSp[2];


		shared_ptr<SoundItem> m_BGM;
		shared_ptr<SoundItem> m_SE;

		shared_ptr<Sprite> m_stageNum[3];

		shared_ptr<XAudio2Manager> m_BGMManager = nullptr;	// BGM���Đ����邽�߂̃}�l�[�W��
		shared_ptr<XAudio2Manager> m_SEManager = nullptr;	// SE�Ȃǂ��Đ����邽�߂̃}�l�[�W��

		weak_ptr<Scene> m_scene;

		Vec3 m_selectPos;

		// �؂�ւ��̎��Ɉ�C�ɐ؂�ւ��Ȃ��悤�ɂ��邽�߂̃t���O1
		bool m_selectOnceFlag1 = false;
		// �؂�ւ��̎��Ɉ�C�ɐ؂�ւ��Ȃ��悤�ɂ��邽�߂̃t���O2
		bool m_selectOnceFlag2 = false;

		// �`���[�g���A����I�����Ă���Ƃ��̃t���O
		bool m_tutorialFlag = false;

		// �`���[�g���A���ň�񂾂�����������t���O
		bool m_tutorialOnceFlag = false;

		// �I��ł���X�e�[�W��
		short int m_selectStageNum = 0;
		 // �X�e�[�W�I���t���O
		bool m_stageFlag = false;
		// time�؂�ւ��t���O
		bool m_timeFlag = false;

		float m_time = 0.0f;

		// BGM�̃{�����[��
		float m_BGMVol;
		// SE�̃{�����[��
		float m_SEVol;
	public:
		//�\�z�Ɣj��
		StageSelect() :Stage() {}
		virtual ~StageSelect() {}

		void CreateSprite();
		void CreateBGM();

		void IsContorollerConnect();

		//������
		virtual void OnCreate()override;
		virtual void OnUpdate()override;

	};
}