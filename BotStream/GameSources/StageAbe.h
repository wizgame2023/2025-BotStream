/*!
@file StageAbe.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class StageAbe : public Stage {
		//�r���[�̍쐬
		void CreateViewLight();
	public:
		//�\�z�Ɣj��
		StageAbe() :Stage() {}
		virtual ~StageAbe() {}
		//������
		virtual void OnCreate()override;
	};


}
//end basecross

