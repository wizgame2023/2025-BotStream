#pragma once
/*!
@file TitleStage.h
@brief �^�C�g�����
*/

#include "stdafx.h"

namespace basecross {
	class TitleStage : public Stage
	{
		//�r���[�̍쐬
		void CreateViewLight();
		shared_ptr<Sprite> m_titleSprite;
		shared_ptr<Sprite> m_titleBackSprite;
		shared_ptr<Sprite> m_textSprite;
		shared_ptr<Sprite> m_textSpriteKeyBoard;

		shared_ptr<SoundItem> m_BGM;

		Vec3 m_selectPos;
		float m_time = 0;
	public:
		//�\�z�Ɣj��
		TitleStage() :Stage() {}
		virtual ~TitleStage() {}

		void CreateSprite();
		void CreateBGM();
		//������
		virtual void OnCreate()override;
		virtual void OnUpdate()override;

	};
}