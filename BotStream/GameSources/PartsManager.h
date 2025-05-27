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
		shared_ptr<Stage> m_stage;

		bool m_GetHeadParts = false;
		bool m_AttachHeadParts = false;
		bool m_GetBodyParts = false;
		bool m_AttachBodyParts = false;
		bool m_GetLegParts = false;
		bool m_AttachLegParts = false;

	public:
		PartsManager(const shared_ptr<Stage>& stagePtr) :
			MyGameObject(stagePtr)
		{
		}
		~PartsManager(){}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		bool GetAttachHeadParts();
		void SetAttachHeadParts(int setAttachHeadParts);

		bool GetAttachBodyParts();
		void SetAttachBodyParts(int setAttachBodyParts);

		bool GetAttachLegParts();
		void SetAttachLegParts(int setAttachLegParts);

	};
}