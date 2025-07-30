#pragma once
/*!
@file GameClearScene.h
@brief �N���A���
*/

#include "stdafx.h"

namespace basecross {
	class GameClear : public Stage
	{
		//�r���[�̍쐬
		void CreateViewLight();
		shared_ptr<Sprite> m_selectSprite[2];
		shared_ptr<Sprite> m_fadeInSprite;

		shared_ptr<SoundItem> m_BGM;
		shared_ptr<SoundItem> m_SE;

		shared_ptr<XAudio2Manager> m_BGMMana;
		shared_ptr<XAudio2Manager> m_SEMana;

		weak_ptr<Scene> m_scene;

		// �I��؂�ւ��t���O
		bool m_selectFlag = false;
		// �t�F�[�h�C���t���O
		bool m_fadeInFlag = false;

		int m_select = 0;

		Vec3 m_selectPos;
		int m_stageNum;

		float m_time;

		float m_SEVol;

		wstring m_strStage;
	public:
		//�\�z�Ɣj��
		GameClear() :Stage() {}
		virtual ~GameClear() {}

		void CreateSprite();
		//������
		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		virtual void OnDestroy() override;
	};
}