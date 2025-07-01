/*!
@file SoundManager.h
@brief âπäyÅEå¯â âπÇÃä«óù
*/

#pragma once
#include "Project.h"
#include "stdafx.h"

namespace basecross {

	class SoundManager : public GameObject {
		shared_ptr<XAudio2Manager> m_ptrSound = App::GetApp()->GetXAudio2Manager();

		shared_ptr<SoundItem> m_BGM;
		shared_ptr<SoundItem> m_SE;


		float m_BGMVol = 1.0f;
		float m_SEVol = 1.0f;

		float m_audioMax[2];

	public:
		SoundManager(const shared_ptr<Stage>& StagePtr);
		virtual ~SoundManager();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;


		void PlaySE(int sfx);

		void PlayBGM(int sfx);

		void StopSE();

		void StopBGM();

	};

}