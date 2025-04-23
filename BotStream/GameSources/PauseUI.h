#pragma once
/*!
@file PauseUI.h
@brief ポーズ画面
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {

	class Pause :public MyGameObject
	{
	private:
		//ポーズされているかのフラグ
		bool m_pauseFlag = false;

		//スティック傾きを受け入れるフラグ
		bool m_stick = false;
		//選択しているものを保存するフラグ
		int m_select = 1;

		//ステージセレクトのUI
		shared_ptr<Sprite> m_stageSelectUI;
		//タイトルのUI
		shared_ptr<Sprite> m_titleUI;

		//選択右
		shared_ptr<Sprite> m_selectSpriteR;
		//選択左
		shared_ptr<Sprite> m_selectSpriteL;

	public:
		Pause(shared_ptr<Stage>& StagePtr) :
			MyGameObject(StagePtr)
		{

		}
		~Pause()
		{
		}

		void OnCreate()override;
		void OnUpdate()override;
	};

}