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

	class EquippedParts : public MyGameObject
	{
	private:
		vector<PartsStatus> m_partspoach;
		vector<PartsStatus> m_EquippedParts;

	public:
		EquippedParts(const shared_ptr<Stage>& stagePtr) :
			MyGameObject(stagePtr)
		{
		}
		~EquippedParts() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		//ポーチにパーツを入れる処理
		void AddParts(PartsStatus addParts)
		{
			auto player = GetStage()->GetSharedGameObject<Player>(L"Player");

			//装備している全てのパーツのステータス
			PartsStatus allParts;
			allParts.addAttack = 0;
			allParts.addAttribute = 0;
			allParts.addHP = 0;
			allParts.addSpeed = 0.0f;

			//パーツポーチに入れる
			m_partspoach.push_back(addParts);

			//パーツを３つ以上付けていないときは自動的に装備される
			if (m_EquippedParts.size() < 3)
			{
				m_EquippedParts.push_back(addParts);
			}
			auto test = m_partspoach.size();
			//そうでなければ、パーツを古いやつを捨てて新しいやつに変える
			if (m_EquippedParts.size() >= 3)
			{
				//一番最初に取ったものを削除して新しく手に入れたものを入れる
				m_EquippedParts.erase(m_EquippedParts.begin());
				m_EquippedParts.push_back(addParts);
			}

			int roop = 0;
			//装備している合計のステータスを計算する
			for (auto parts : m_EquippedParts)
			{
				allParts.addAttack += m_EquippedParts[roop].addAttack;
				allParts.addAttribute += m_EquippedParts[roop].addAttribute;
				allParts.addHP += m_EquippedParts[roop].addHP;
				allParts.addSpeed += m_EquippedParts[roop].addSpeed;
				allParts.partsImagePass = L"使わないデータ";
				allParts.partsName = L"使わないデータ";

				roop++;
			}

			//プレイヤーに装備した合計のステータス値を渡す
			player->SetEquippedParts(allParts);
		}

		//ポーチを受け取る処理
		vector<PartsStatus> GetParts()
		{
			return m_partspoach;
		}

	};
}