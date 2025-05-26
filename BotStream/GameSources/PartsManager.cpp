/*!
@file PartsManager.cpp
@brief パーツを管理するオブジェクト
*/

#include "stdafx.h"
#include "Project.h"


namespace basecross {

	void PartsManager::OnCreate()
	{
		m_stage = GetStage();

	}

	void PartsManager::OnUpdate()
	{
		auto headParts = m_stage->GetSharedGameObject<HeadParts>(L"HeadParts");
		auto bodyParts = m_stage->GetSharedGameObject<BodyParts>(L"BodyParts");
		auto legParts = m_stage->GetSharedGameObject<LegParts>(L"LegParts");

		headParts->GetHeadParts();
		m_GetHeadParts = headParts->GetHeadParts();
		bodyParts->GetBodyParts();
		m_GetBodyParts = bodyParts->GetBodyParts();
		legParts->GetLegParts();
		m_GetLegParts = legParts->GetLegParts();

		//パーツをゲットしたら装備する
		//装備したら取得フラグをfalseにする
		//m_Attach○○Partsがtrueの時に同じ部位の別のパーツを取得したら選択させるようにする
		if (m_GetHeadParts == true)
		{
			m_AttachHeadParts = true;
			m_GetHeadParts = false;
		}

		if (m_GetBodyParts == true)
		{
			m_AttachBodyParts = true;
			m_GetBodyParts = false;
		}

		if (m_GetLegParts == true)
		{
			m_AttachLegParts = true;
			m_GetLegParts = false;
		}

	}

	//パーツを付けているときはtrueになる
	void PartsManager::SetAttachHeadParts(int setAttachHeadParts)
	{
		m_AttachHeadParts = setAttachHeadParts;
	}

	bool PartsManager::GetAttachHeadParts()
	{
		return m_AttachHeadParts;
	}

	void PartsManager::SetAttachBodyParts(int setAttachBodyParts)
	{
		m_AttachBodyParts = setAttachBodyParts;
	}

	bool PartsManager::GetAttachBodyParts()
	{
		return m_AttachBodyParts;
	}

	void PartsManager::SetAttachLegParts(int setAttachLegParts)
	{
		m_AttachLegParts = setAttachLegParts;
	}

	bool PartsManager::GetAttachLegParts()
	{
		return m_AttachLegParts;
	}

}