#pragma once
/*!
@file Parts.h
@brief �p�[�c�I�u�W�F�N�g
*/

#include "stdafx.h"
#include "Project.h"
#include "ObjectMove.h"

namespace basecross {

	//���p�[�c�p
	class HeadParts : public ObjectMove
	{
		bool m_GetHeadParts = false;
		bool m_AttachHeadParts = false;

	public:
		HeadParts(const shared_ptr<Stage>& stagePtr);
		~HeadParts();
		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		bool GetHeadParts();
		void SetHeadParts(int setHeadParts);


		void OnCollisionEnter(shared_ptr<GameObject>& Other)override;

	};

	// �̃p�[�c�p
	class BodyParts : public ObjectMove
	{
		bool m_GetBodyParts = false;
		bool m_AttachBodyParts = false;

	public:
		BodyParts(const shared_ptr<Stage>& stagePtr);
		~BodyParts();
		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		bool GetBodyParts();
		void SetBodyParts(int setBodyParts);


		void OnCollisionEnter(shared_ptr<GameObject>& Other)override;

	};

	//���p�[�c�p
	class LegParts : public ObjectMove
	{
		bool m_GetLegParts = false;
		bool m_AttachLegParts = false;

	public:
		LegParts(const shared_ptr<Stage>& stagePtr);
		~LegParts();
		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		bool GetLegParts();
		void SetLegParts(int setLegParts);


		void OnCollisionEnter(shared_ptr<GameObject>& Other)override;

	};


	struct PlusStatus
	{
		float Speed;
		int Attack;
		int Bad;
		int Good;
	};


}