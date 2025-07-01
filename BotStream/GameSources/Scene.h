/*!
@file Scene.h
@brief シーン
*/
#pragma once

#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	class Scene : public SceneBase{
		int m_stageNum = 1;

		// BGM.SEの音量
		float m_BGMVolume = 1.0f;
		float m_SEVolume = 1.0f;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		Scene() :SceneBase(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Scene();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief イベント取得
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnEvent(const shared_ptr<Event>& event) override;
	
		//ゲームに必要な物のパスを渡す
		void GameResourses();

		// ステージ数を取得する
		int GetStageNum()
		{
			return m_stageNum;
		}	
		
		//BGM.SEのボリュームのゲッタ
		float GetBGMVolume()
		{
			return m_BGMVolume;
		}
		float GetSEVolume()
		{
			return m_SEVolume;
		}

		//BGM.SEのボリュームをセットする
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
