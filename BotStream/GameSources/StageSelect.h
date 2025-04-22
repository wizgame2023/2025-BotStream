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
		shared_ptr<Sprite> m_stageNum;

		Vec3 m_selectPos;

	public:
		//�\�z�Ɣj��
		StageSelect() :Stage() {}
		virtual ~StageSelect() {}

		void CreateSprite();

		//������
		virtual void OnCreate()override;
		virtual void OnUpdate()override;

	};
}