/*!
@file StageAbe.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	SoundManager::SoundManager(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{}

	SoundManager::~SoundManager()
	{
	}

	void SoundManager::OnCreate()
	{
	}

	void SoundManager::PlaySE(int sfx)
	{
		switch (sfx)
		{
		case 0:
			m_SE = m_ptrSound->Start(L"Decision", 0, 0.3f);
			break;
		case 1:
			m_SE = m_ptrSound->Start(L"Decision2", 0, 0.3f);
			break;
		case 2:
			m_SE = m_ptrSound->Start(L"Landing", 0, 0.3f);
			break;
		case 3:
			m_SE = m_ptrSound->Start(L"Dash", 0, 0.5f);
			break;
		case 4:
			m_SE = m_ptrSound->Start(L"ArmorBreak", 0, 0.3f);
			break;
		case 5:
			m_SE = m_ptrSound->Start(L"Attack1", 0, 0.3f);
			break;
		case 6:
			m_SE = m_ptrSound->Start(L"Attack2", 0, 0.3f);
			break;
		case 7:
			m_SE = m_ptrSound->Start(L"Attack3", 0, 0.3f);
			break;
		case 8:
			m_SE = m_ptrSound->Start(L"HandGun", 0, 0.3f);
			break;
		case 9:
			m_SE = m_ptrSound->Start(L"Reload", 0, 0.3f);
			break;
		case 10:
			m_SE = m_ptrSound->Start(L"AssaultRifle", 0, 0.3f);
			break;

		}
	}

	void SoundManager::PlayBGM(int sfx)
	{
		switch(sfx)
		{
		case 0:
			m_BGM = m_ptrSound->Start(L"Title", XAUDIO2_LOOP_INFINITE, 0.5f);
			break;
		case 1:
			m_BGM = m_ptrSound->Start(L"Diagnosis", XAUDIO2_LOOP_INFINITE, 0.5f);
			break;
		case 2:
			m_BGM = m_ptrSound->Start(L"SelectStage", XAUDIO2_LOOP_INFINITE, 0.5f);
			break;
		case 3:
			m_BGM = m_ptrSound->Start(L"EnemyWave", XAUDIO2_LOOP_INFINITE, 0.5f);
			break;
		case 4:
			m_BGM = m_ptrSound->Start(L"BossWave", XAUDIO2_LOOP_INFINITE, 0.5f);
			break;
		case 5:
			m_BGM = m_ptrSound->Start(L"Result", XAUDIO2_LOOP_INFINITE, 0.5f);
			break;

		}
	}

	void SoundManager::StopSE()
	{
		auto XAPtr = App::GetApp()->GetXAudio2Manager();
		XAPtr->Stop(m_SE);
	}

	void SoundManager::StopBGM()
	{
		auto XAPtr = App::GetApp()->GetXAudio2Manager();
		XAPtr->Stop(m_BGM);
	}
}
//end basecross
