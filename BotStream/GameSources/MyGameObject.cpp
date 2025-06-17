/*!
@file MyGameObject.cpp
@brief ���ꂩ��쐬���镨�̌��ƂȂ�N���X
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	MyGameObject::MyGameObject(const shared_ptr<Stage>& stagePtr):
		GameObject(stagePtr)
	{

	}
	MyGameObject::~MyGameObject()
	{

	}

	shared_ptr<WaveStageBase> MyGameObject::GetWaveStage(bool ExceptionActive) const {
		shared_ptr<Stage> shptr = GetStage();
		shared_ptr<WaveStageBase> wave = dynamic_pointer_cast<WaveStageBase>(shptr);
		if (wave) {
			return wave;
		}
		else {
			if (ExceptionActive) {
				throw BaseException(
					L"�����X�e�[�W��null���AWaveStageBase�̌p���N���X�ł͂���܂���",
					L"if (!wave)",
					L"MyGameObject::GetWaveStage()"
				);
			}
			else {
				return nullptr;
			}
		}
		// �����X�e�[�W��null������
		return nullptr;
	}

	void MyGameObject::OnCreate()
	{
		m_BGMVol = GetStage()->GetSharedGameObject<PauseSprite>(L"PauseUI")->GetAudioMax(m_audioMax[0]);
		m_SEVol = GetStage()->GetSharedGameObject<PauseSprite>(L"PauseUI")->GetAudioMax(m_audioMax[1]);

	}

	void MyGameObject::OnUpdate()
	{

	}

	float MyGameObject::GetBGMVol()
	{
		return m_BGMVol;
	}

	void MyGameObject::SetBGMVol(int setBGM)
	{
		m_BGMVol = setBGM;
	}

	float MyGameObject::GetSEVol()
	{
		return m_SEVol;
	}

	void MyGameObject::SetSEVol(int setSE)
	{
		m_SEVol = setSE;

	}
}
//end basecross
