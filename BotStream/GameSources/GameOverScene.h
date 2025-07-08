#pragma once
/*!
@file GameOverScene.cpp
@brief �Q�[���I�[�o�[���
*/

#include "stdafx.h"

namespace basecross {
	class GameOver : public Stage
	{
		//�r���[�̍쐬
		void CreateViewLight();
		shared_ptr<Sprite> m_selectSprite[2];
		shared_ptr<Sprite> m_fadeInSprite;
		shared_ptr<Sprite> m_noiseSprite;
		weak_ptr<Scene> m_scene;

		// �I��؂�ւ��t���O
		bool m_selectFlag = false;
		// �t�F�[�h�C���t���O
		bool m_fadeInFlag = false;

		int m_select = 0;

		int m_frameCount[2] = { 0 };

		Vec3 m_selectPos;
		int m_stageNum;

		float m_time;

		wstring m_strStage;
	public:
		//�\�z�Ɣj��
		GameOver() :Stage() {}
		virtual ~GameOver() {}

		void CreateSprite();

		//������
		virtual void OnCreate()override;
		virtual void OnUpdate()override;

	};
}