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
		
	public:
		//�\�z�Ɣj��
		StageSato() :Stage() {}
		virtual ~StageSato() {}
		//������
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual void OnDraw()override;

	};


}
//end basecross

