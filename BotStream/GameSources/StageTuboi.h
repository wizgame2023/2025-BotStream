/*!
@file GameTuboi.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class StageTuboi : public Stage {
		//�r���[�̍쐬
		void CreateViewLight();
	public:
		//�\�z�Ɣj��
		StageTuboi() :Stage() {}
		virtual ~StageTuboi() {}
		//������
		virtual void OnCreate()override;
	};


}
//end basecross

