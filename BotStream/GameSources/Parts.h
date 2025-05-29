#pragma once
/*!
@file Parts.h
@brief パーツオブジェクト
*/

#include "stdafx.h"
#include "Project.h"
#include "ObjectMove.h"

namespace basecross {

	//パーツクラス
	class Parts : public ObjectMove
	{
		shared_ptr<Transform> m_trans;
		Vec3 m_pos;//位置
		Vec3 m_rot;//回転
		Vec3 m_scale;//大きさ

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

	//頭パーツ用
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