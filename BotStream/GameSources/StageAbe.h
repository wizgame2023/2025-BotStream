/*!
@file StageAbe.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class SoundManager : public GameObject {
		shared_ptr<XAudio2Manager> m_ptrSound = App::GetApp()->GetXAudio2Manager();
	
		shared_ptr<SoundItem> m_BGM;
		shared_ptr<SoundItem> m_SE;

	public:
		SoundManager(const shared_ptr<Stage>& StagePtr);
		virtual ~SoundManager();

		virtual void OnCreate() override;

		void PlaySE(int sfx);

		void PlayBGM(int sfx);

		void StopSE();

		void StopBGM();
	
	};


}
//end basecross

