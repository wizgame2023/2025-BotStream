/*!
@file SoundManager.cpp
@brief ‰¹ŠyEŒø‰Ê‰¹‚ÌŠÇ—
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	SoundManager::SoundManager(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{
	}

	SoundManager::~SoundManager()
	{
	}

	void SoundManager::OnCreate()
	{

	}

	void SoundManager::OnUpdate()
	{

		//m_BGMVol = GetStage()->GetSharedGameObject<MyGameObject>(L"MyGameObject")->GetBGMVol();
		//m_SEVol = GetStage()->GetSharedGameObject<MyGameObject>(L"MyGameObject")->GetSEVol();


		auto StageObj = GetStage()->GetGameObjectVec();

		for (auto obj : StageObj)
		{
		    auto myGameObject = dynamic_pointer_cast<MyGameObject>(obj);

		    if (myGameObject)
		    {
		        myGameObject->SetBGMVol(m_BGMVol);
		        myGameObject->SetSEVol(m_SEVol);
		    }
		}

	}

	void SoundManager::PlaySE(int sfx)
	{
		switch (sfx)
		{
		case 0:
			m_SE = m_ptrSound->Start(L"Decision", 0, 0.5f);
			break;
		case 1:
			m_SE = m_ptrSound->Start(L"ArmorBreak", 0, 0.5f);
			break;
		case 2:
			m_SE = m_ptrSound->Start(L"Enemy_Slash", 0, 0.5f);
			break;
		case 3:
			m_SE = m_ptrSound->Start(L"Enemy_Spin", 0, 0.5f);
			break;
		case 4:
			m_SE = m_ptrSound->Start(L"Enemy_SpinFinish", 0, 0.5f);
			break;
		case 5:
			m_SE = m_ptrSound->Start(L"Enemy_Slam", 0, 0.5f);
			break;
		case 6:
			m_SE = m_ptrSound->Start(L"Beam", 0, 0.5f);
			break;
		case 7:
			m_SE = m_ptrSound->Start(L"Enemy_Defeat", 0, 0.5f);
			break;
		case 8:
			m_SE = m_ptrSound->Start(L"EnemyZako_Shot", 0, 0.5f);
			break;
		case 9:
			m_SE = m_ptrSound->Start(L"EnemyZako_Charge", 0, 0.5f);
			break;
		case 10:
			m_SE = m_ptrSound->Start(L"Damage", 0, 0.5f);
			break;
		case 11:
			m_SE = m_ptrSound->Start(L"Landing", 0, 0.5f);
			break;
		case 12:
			m_SE = m_ptrSound->Start(L"Dash", 0, 0.5f);
			break;
		case 13:
			m_SE = m_ptrSound->Start(L"Attack1", 0, 1.0f);
			break;
		case 14:
			m_SE = m_ptrSound->Start(L"Attack2", 0, 0.5f);
			break;
		case 15:
			m_SE = m_ptrSound->Start(L"Attack3", 0, 0.5f);
			break;
		case 16:
			m_SE = m_ptrSound->Start(L"HandGun", 0, 0.5f);
			break;
		case 17:
			m_SE = m_ptrSound->Start(L"Reload", 0, 0.5f);
			break;
		case 18:
			m_SE = m_ptrSound->Start(L"CantShotSE", 0, 0.5f);
			break;
		case 19:
			m_SE = m_ptrSound->Start(L"DamageVoiceSE", 0, 0.5f);
			break;
		case 20:
			m_SE = m_ptrSound->Start(L"StartVoiceSE", 0, 0.5f);
			break;
		case 21:
			m_SE = m_ptrSound->Start(L"GetPartsSE", 0, 0.5f);
			break;

		}
	}

	void SoundManager::PlayBGM(int sfx)
	{
		switch (sfx)
		{
		case 0:
			m_BGM = m_ptrSound->Start(L"Title", XAUDIO2_LOOP_INFINITE, 1.0f);
			break;
		case 1:
			m_BGM = m_ptrSound->Start(L"Diagnosis", XAUDIO2_LOOP_INFINITE, 1.0f);
			break;
		case 2:
			m_BGM = m_ptrSound->Start(L"SelectStage", XAUDIO2_LOOP_INFINITE, 1.0f);
			break;
		case 3:
			m_BGM = m_ptrSound->Start(L"EnemyWave", XAUDIO2_LOOP_INFINITE, 1.0f);
			break;
		case 4:
			m_BGM = m_ptrSound->Start(L"BossWave", XAUDIO2_LOOP_INFINITE, 1.0f);
			break;
		case 5:
			m_BGM = m_ptrSound->Start(L"Result", XAUDIO2_LOOP_INFINITE, 1.0f);
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