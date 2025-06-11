/*!
@file SoundManager.h
@brief âπäyÅEå¯â âπÇÃä«óù
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class SoundManager : public GameObject {
		shared_ptr<XAudio2Manager> m_ptrSound = App::GetApp()->GetXAudio2Manager();

		shared_ptr<SoundItem> m_BGM;
		shared_ptr<SoundItem> m_SE;

		weak_ptr<SoundItem> m_bgm;
		vector<weak_ptr<SoundItem>> m_seList;


		float m_bgmVolume;
		float m_seVolume;
		float m_bgmMaxVolume;
		float m_seMaxVolume;

	public:
		SoundManager(const shared_ptr<Stage>& StagePtr);
		virtual ~SoundManager();

		virtual void OnCreate() override;

		void PlaySE(int sfx);

		void PlayBGM(int sfx);

		void StopSE();

		void StopBGM();

		void SetBGMVolume(float setVolume);

		float GetBGMVolume();

		void SetSEVolume(float setVolume);

		float GetSEVolume();
	};

}