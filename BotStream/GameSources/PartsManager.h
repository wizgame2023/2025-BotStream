#pragma once
/*!
@file PartsManager.h
@brief パーツを管理するオブジェクト
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	class PartsManager : public MyGameObject
	{

	public:
		PartsManager(const shared_ptr<Stage>& stagePtr) :
			MyGameObject(stagePtr)
		{
		}
		~PartsManager(){}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void PartsDrop(Vec3 dropPos);

	};

	// パーツの装備処理
	class EquippedParts : public MyGameObject
	{
	private:
		vector<PartsStatus> m_partspoach;
		vector<PartsStatus> m_equippedParts;

	public:
		EquippedParts(const shared_ptr<Stage>& stagePtr) :
			MyGameObject(stagePtr)
		{
		}
		~EquippedParts() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		//ポーチにパーツを入れる処理
		void AddParts(PartsStatus addParts);

		//装備パーツを受け取る処理
		vector<PartsStatus> GetEquippedParts()
		{
			return m_equippedParts;
		}

		//ポーチを受け取る処理
		vector<PartsStatus> GetParts()
		{
			return m_partspoach;
		}

	};
}