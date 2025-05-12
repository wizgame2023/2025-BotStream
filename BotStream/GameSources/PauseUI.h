#pragma once
/*!
@file PauseUI.h
@brief �|�[�Y���
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {

	class PauseSprite :public MyGameObject
	{
	private:
		// �|�[�Y��ʂ̃��C��
		shared_ptr<Sprite> m_selectSprite;	// �I�����Ă�Ƃ���
		shared_ptr<Sprite> m_pauseBack;		// �|�[�Y�̔w�i
		shared_ptr<Sprite> m_pauseTextSprite[6];	// �|�[�Y��ʂ̃e�L�X�g(0:Pause 1:�ĊJ 2:�X�e�[�W�Z���N�g 3:�I�[�f�B�I 4:BGM 5:SE)

		// �I�[�f�B�I�ݒ���
		shared_ptr<Sprite> m_speaker[2];	// �X�s�[�J�[�̐}
		shared_ptr<Sprite> m_BGMMater[10];	// BGM�̃��[�^�[
		shared_ptr<Sprite> m_SEMater[10];	// SE�̃��[�^�[
		shared_ptr<Sprite> m_audioSelect[2];	// �I�[�f�B�I�ݒ�̑I�����Ă���Ƃ���

		shared_ptr<Stage> m_stage;
		// �I��؂�ւ��t���O
		bool m_selectFlag = false;
		// Audio�p�̑I��؂�ւ��t���O
		bool m_audioSelectFlag = false;

		bool m_pauseFlag = false;		// �|�[�Y�̃t���O
		bool m_pauseAudioFlag = false;	// �I�[�f�B�I�ݒ�̃t���O

		float m_audioMax[2] = { 1.0f , 1.0f };	// ���ʂ̎����l(0:BGM 1:SE)
		int m_audioMaxSetCol[2] = { 10, 10 };	// ���ʂ̐F�ς��邽�߂����ɑ��݂������

		int m_select2 = 0;	// �I�����Ă���Ƃ���̎����l

		// ���C���̑I�����Ă���Ƃ���̃|�W�V����
		Vec3 m_selectPos;

	public:
		PauseSprite(shared_ptr<Stage>& StagePtr) :
			MyGameObject(StagePtr)
		{

		}
		~PauseSprite()
		{
		}
		void CreateSprite();

		// BGM,SE�֌W�̕\����\���̐���
		void AudioUIClear(bool clear);

		template <typename T>
		T clamp(T value, T minValue, T maxValue)
		{
			if (value < minValue) return minValue;
			if (value > maxValue) return maxValue;
			return value;
		}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};

}