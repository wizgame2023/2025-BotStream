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


	};

	class PartsPoach : public MyGameObject
	{

		vector<shared_ptr<HeadParts>> m_partspoach;

	public:
		PartsPoach(const shared_ptr<Stage>& stagePtr) :
			MyGameObject(stagePtr)
		{
		}
		~PartsPoach() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

	};
}