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

		// �I��؂�ւ��t���O
		bool m_selectFlag = false;
		int m_select = 0;

		Vec3 m_selectPos;

	public:
		//�\�z�Ɣj��
		GameClear() :Stage() {}
		virtual ~GameClear() {}

		void CreateSprite();
		//������
		virtual void OnCreate()override;
		virtual void OnUpdate()override;

	};
}