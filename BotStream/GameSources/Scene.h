/*!
@file Scene.h
@brief �V�[��
*/
#pragma once

#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	�Q�[���V�[��
	//--------------------------------------------------------------------------------------
	class Scene : public SceneBase{
		int m_stageNum = 1;

		// BGM.SE�̉���
		float m_BGMVolume = 1.0f;
		float m_SEVolume = 1.0f;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		Scene() :SceneBase(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Scene();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �C�x���g�擾
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnEvent(const shared_ptr<Event>& event) override;
	
		//�Q�[���ɕK�v�ȕ��̃p�X��n��
		void GameResourses();

		// �X�e�[�W�����擾����
		int GetStageNum()
		{
			return m_stageNum;
		}	
		
		//BGM.SE�̃{�����[���̃Q�b�^
		float GetBGMVolume()
		{
			return m_BGMVolume;
		}
		float GetSEVolume()
		{
			return m_SEVolume;
		}

		//BGM.SE�̃{�����[�����Z�b�g����
		void SetBGMVolume(float BGMVolume)
		{
			m_BGMVolume = BGMVolume;
		}
		void SetSEVolume(float SEVolume)
		{
			m_SEVolume = SEVolume;
		}

	};

}

//end basecross
