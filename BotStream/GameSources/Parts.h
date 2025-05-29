#pragma once
/*!
@file Parts.h
@brief �p�[�c�I�u�W�F�N�g
*/

#include "stdafx.h"
#include "Project.h"
#include "ObjectMove.h"

namespace basecross {

	//�p�[�c�N���X
	class Parts : public ObjectMove
	{
		shared_ptr<Transform> m_trans;
		Vec3 m_pos;//�ʒu
		Vec3 m_rot;//��]
		Vec3 m_scale;//�傫��

		int AddAttack = 0.0f;
		int AddHp = 0.0f;
		float AddSpeed = 0.0f;

		int AddGood = 0.0f;
		int AddBad = 0.0f;

	public:
		Parts(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale);
		~Parts();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

	};

	//���p�[�c�p
	class HeadParts : public Parts
	{

		int AddAttack = 3.0f;
		int AddGood = 2.0f;


		shared_ptr<Stage> m_stage;

		vector<HeadParts> Partspoach;

	public:
		HeadParts(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale);
		~HeadParts();
		virtual void OnCreate() override;
		virtual void OnUpdate() override;



		void OnCollisionEnter(shared_ptr<GameObject>& Other)override;

	};





}