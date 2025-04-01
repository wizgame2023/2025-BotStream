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
		shared_ptr<EfkEffect> m_EfkLaser;
		shared_ptr<EfkEffect> m_EfkSword;
		shared_ptr<EfkEffect> m_EfkLanding;

		shared_ptr<EfkPlay> m_EfkPlay;
		shared_ptr<Player> m_Player;

		shared_ptr<EfkInterface> m_ShEfkInterface;

	public:
		//�\�z�Ɣj��
		StageSato() :Stage() {}
		virtual ~StageSato() {}
		//������
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual void OnDraw()override;

		shared_ptr<EfkInterface> GetEfkInterface()
		{
			return m_ShEfkInterface;
		}

	};


}
//end basecross

