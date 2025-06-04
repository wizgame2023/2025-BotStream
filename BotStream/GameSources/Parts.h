#pragma once
/*!
@file Parts.h
@brief パーツオブジェクト
*/

#include "stdafx.h"
#include "Project.h"
#include "ObjectMove.h"

namespace basecross {
	class EquippedParts;
	class BillBoard;

	//パーツのステータス一覧
	struct PartsStatus
	{
		//ID
		int id;
		//攻撃力
		int addAttack;
		//速度
		float addSpeed;
		//善か悪
		int addAttribute;
		//追加HP
		int addHP;
		//パーツの名前
		wstring partsName;
		//パーツの写真のパス名
		wstring partsImagePass;
	};

	//パーツクラス
	class Parts : public ObjectMove
	{
	protected:
		shared_ptr<Transform> m_trans;
		Vec3 m_pos;//位置
		Vec3 m_rot;//回転
		Vec3 m_scale;//大きさ

		//パーツのステータス
		PartsStatus m_partsStatus;
		//パーツを入れるポーチ
		shared_ptr<EquippedParts> m_partspoach;

		//ビルボード
		shared_ptr<BillBoard> m_billBoard;

		bool m_pose = false;

	public:
		Parts(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale);
		~Parts();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void SetPose(bool onOff)
		{
			m_pose = onOff;
		}

		void OnCollisionEnter(shared_ptr<GameObject>& Other)override;
	};

	//頭パーツ用
	class HeadParts : public Parts
	{
		//shared_ptr<Stage> m_stage;


	public:
		HeadParts(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale) :
			Parts(stagePtr,pos,rot,scale)
		{
			//どれぐらいステータスを上げるか決める
			m_partsStatus.id = 1;
			m_partsStatus.addHP = 10;
			m_partsStatus.addSpeed = 0.05f;
			m_partsStatus.addAttack = 0;
			m_partsStatus.addAttribute = 10;

			//名前
			m_partsStatus.partsName = L"テスト用パーツ";
			//とりあえず適当なテクスチャにする
			m_partsStatus.partsImagePass = L"PartsTestTEX";
		}
		~HeadParts()
		{
		}
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};


	//パーツ(高性能モーター)
	class PartsHiMoter :public Parts
	{
	private:

	public:
		PartsHiMoter(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale) :
			Parts(stagePtr, pos, rot, scale)
		{
			//どれぐらいステータスを上げるか決める
			m_partsStatus.id = 2;
			m_partsStatus.addHP = 0;
			m_partsStatus.addSpeed = 0.3f;
			m_partsStatus.addAttack = 0;
			m_partsStatus.addAttribute = 10;

			//名前
			m_partsStatus.partsName = L"高性能モーター";
			//とりあえず適当なテクスチャにする
			m_partsStatus.partsImagePass = L"PartsHiMoterTEX";
		}
		~PartsHiMoter()
		{
		}
	};


	//パーツ(戦闘用パッチ)
	class PartsBattlePatch :public Parts
	{
	private:

	public:
		PartsBattlePatch(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale) :
			Parts(stagePtr, pos, rot, scale)
		{
			//どれぐらいステータスを上げるか決める
			m_partsStatus.id = 3;
			m_partsStatus.addHP = 20;
			m_partsStatus.addSpeed = 0.0f;
			m_partsStatus.addAttack = 3;
			m_partsStatus.addAttribute = -20;

			//名前
			m_partsStatus.partsName = L"戦闘用パッチ";
			//とりあえず適当なテクスチャにする
			m_partsStatus.partsImagePass = L"PartsBattlePatchTEX";
		}
		~PartsBattlePatch()
		{
		}

	};
}