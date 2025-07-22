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

	//パーツタイプ
	enum partsType
	{
		parts,
		MoterParts,
		PatchParts
	};

	//パーツのステータス一覧
	struct PartsStatus
	{
		//ID
		int id;
		//パーツタイプ
		int partsType;
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

		void SetPause(bool onOff)
		{
			m_pose = onOff;
		}

		void OnCollisionEnter(shared_ptr<GameObject>& Other)override;
	};

	//パーツ(試作パーツ)
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
			m_partsStatus.addSpeed = 0.1f;
			m_partsStatus.addAttack = 0;
			m_partsStatus.addAttribute = 10;

			// どのタイプのパーツか決める
			m_partsStatus.partsType = parts;

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
			m_partsStatus.addSpeed = 0.4f;
			m_partsStatus.addAttack = 0;
			m_partsStatus.addAttribute = 10;

			// どのタイプのパーツか決める
			m_partsStatus.partsType = MoterParts;

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

			// どのタイプのパーツか決める
			m_partsStatus.partsType = PatchParts;

			//名前
			m_partsStatus.partsName = L"戦闘用パッチ";
			//とりあえず適当なテクスチャにする
			m_partsStatus.partsImagePass = L"PartsBattlePatchTEX";
		}
		~PartsBattlePatch()
		{
		}

	};


	//パーツ(試作パーツ(強化版))
	class PartsPrototype2 : public Parts
	{

	public:
		PartsPrototype2(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale) :
			Parts(stagePtr, pos, rot, scale)
		{
			//どれぐらいステータスを上げるか決める
			m_partsStatus.id = 4;
			m_partsStatus.addHP = 15;
			m_partsStatus.addSpeed = 0.15;
			m_partsStatus.addAttack = 0;
			m_partsStatus.addAttribute = 15;

			// どのタイプのパーツか決める
			m_partsStatus.partsType = parts;

			//名前
			m_partsStatus.partsName = L"試作パーツ(強化版)";
			//とりあえず適当なテクスチャにする
			m_partsStatus.partsImagePass = L"Stage1-1Tex";
		}
		~PartsPrototype2()
		{
		}
	};


	//パーツ(完全版パーツ)
	class PartsCompleteVersion : public Parts
	{

	public:
		PartsCompleteVersion(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale) :
			Parts(stagePtr, pos, rot, scale)
		{
			//どれぐらいステータスを上げるか決める
			m_partsStatus.id = 5;
			m_partsStatus.addHP = 20;
			m_partsStatus.addSpeed = 0.2;
			m_partsStatus.addAttack = 0;
			m_partsStatus.addAttribute = 20;

			// どのタイプのパーツか決める
			m_partsStatus.partsType = parts;

			//名前
			m_partsStatus.partsName = L"完全版パーツ";
			//とりあえず適当なテクスチャにする
			m_partsStatus.partsImagePass = L"Stage1-2Tex";
		}
		~PartsCompleteVersion()
		{
		}
	};


	//パーツ(改造モーター)
	class PartsModMoter :public Parts
	{
	private:

	public:
		PartsModMoter(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale) :
			Parts(stagePtr, pos, rot, scale)
		{
			//どれぐらいステータスを上げるか決める
			m_partsStatus.id = 6;
			m_partsStatus.addHP = 0;
			m_partsStatus.addSpeed = 0.25f;
			m_partsStatus.addAttack = 2;
			m_partsStatus.addAttribute = -10;

			// どのタイプのパーツか決める
			m_partsStatus.partsType = MoterParts;

			//名前
			m_partsStatus.partsName = L"改造モーター";
			//とりあえず適当なテクスチャにする
			m_partsStatus.partsImagePass = L"Stage2-1Tex";
		}
		~PartsModMoter()
		{
		}
	};


	//パーツ(古びたモーター)
	class PartsOldMoter :public Parts
	{
	private:

	public:
		PartsOldMoter(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale) :
			Parts(stagePtr, pos, rot, scale)
		{
			//どれぐらいステータスを上げるか決める
			m_partsStatus.id = 7;
			m_partsStatus.addHP = 0;
			m_partsStatus.addSpeed = 0.15f;
			m_partsStatus.addAttack = 1;
			m_partsStatus.addAttribute = -20;

			// どのタイプのパーツか決める
			m_partsStatus.partsType = MoterParts;

			//名前
			m_partsStatus.partsName = L"古びたモーター";
			//とりあえず適当なテクスチャにする
			m_partsStatus.partsImagePass = L"Stage2-2Tex";
		}
		~PartsOldMoter()
		{
		}
	};


	//パーツ(パワー型パッチ)
	class PartsPowerPatch :public Parts
	{
	private:

	public:
		PartsPowerPatch(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale) :
			Parts(stagePtr, pos, rot, scale)
		{
			//どれぐらいステータスを上げるか決める
			m_partsStatus.id = 8;
			m_partsStatus.addHP = 0;
			m_partsStatus.addSpeed = 0;
			m_partsStatus.addAttack = 7;
			m_partsStatus.addAttribute = 5;

			// どのタイプのパーツか決める
			m_partsStatus.partsType = PatchParts;

			//名前
			m_partsStatus.partsName = L"パワー型パッチ";
			//とりあえず適当なテクスチャにする
			m_partsStatus.partsImagePass = L"Stage3-1Tex";
		}
		~PartsPowerPatch()
		{
		}

	};


	//パーツ(バランス型パッチ)
	class PartsBalancePatch :public Parts
	{
	private:

	public:
		PartsBalancePatch(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale) :
			Parts(stagePtr, pos, rot, scale)
		{
			//どれぐらいステータスを上げるか決める
			m_partsStatus.id = 9;
			m_partsStatus.addHP = 10;
			m_partsStatus.addSpeed = 0.2f;
			m_partsStatus.addAttack = 2;
			m_partsStatus.addAttribute = 20;

			// どのタイプのパーツか決める
			m_partsStatus.partsType = PatchParts;

			//名前
			m_partsStatus.partsName = L"バランス型パッチ";
			//とりあえず適当なテクスチャにする
			m_partsStatus.partsImagePass = L"Stage3-2Tex";
		}
		~PartsBalancePatch()
		{
		}

	};

}