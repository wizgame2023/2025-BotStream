/*!
@file PartsManager.cpp
@brief パーツを管理するオブジェクト
*/

#include "stdafx.h"
#include "Project.h"


namespace basecross {

	void PartsManager::OnCreate()
	{

	}

	void PartsManager::OnUpdate()
	{

	}

	//パーツドロップ処理
	void PartsManager::PartsDrop(Vec3 dropPos)
	{
		// 空中に浮かんでいる敵からドロップしても手に入るようにする
		dropPos.y = 1.0f;

		float par = rand() % 100;
		int drop = rand() % 9;
		auto stage = GetStage();

		// デバッグ用のドロップパーツの番号(一種類ずつ生成)
		static int debug = 0;
		if (debug > 8) debug = 0;

		par = 10;//テスト用に絶対生成する
		//drop = 0;
		//35%ぐらいの確率でパーツドロップ
		if (par > 0 && par <= 15)
		{
			switch(debug)
			//switch (drop)
			{
			case 0:
				//戦闘用パッチ
				stage->AddGameObject<PartsBattlePatch>(dropPos,Vec3(0.0f,0.0f,0.0f),Vec3(2.0f,2.0f,2.0f));
				break;
			case 1:
				//試作パーツ
				stage->AddGameObject<HeadParts>(dropPos, Vec3(0.0f, 0.0f, 0.0f), Vec3(2.0f, 2.0f, 2.0f));
				break;
			case 2:
				//高性能モーター
				stage->AddGameObject<PartsHiMoter>(dropPos, Vec3(0.0f, 0.0f, 0.0f), Vec3(2.0f, 2.0f, 2.0f));
				break;
			case 3:
				//改造パーツ
				stage->AddGameObject<PartsPrototype2>(dropPos, Vec3(0.0f, 0.0f, 0.0f), Vec3(2.0f, 2.0f, 2.0f));
				break;
			case 4:
				//完全版パーツ
				stage->AddGameObject<PartsCompleteVersion>(dropPos, Vec3(0.0f, 0.0f, 0.0f), Vec3(2.0f, 2.0f, 2.0f));
				break;
			case 5:
				//改造モーター
				stage->AddGameObject<PartsModMoter>(dropPos, Vec3(0.0f, 0.0f, 0.0f), Vec3(2.0f, 2.0f, 2.0f));
				break;
			case 6:
				//旧式モーター
				stage->AddGameObject<PartsOldMoter>(dropPos, Vec3(0.0f, 0.0f, 0.0f), Vec3(2.0f, 2.0f, 2.0f));
				break;
			case 7:
				//攻撃特化パッチ
				stage->AddGameObject<PartsPowerPatch>(dropPos, Vec3(0.0f, 0.0f, 0.0f), Vec3(2.0f, 2.0f, 2.0f));
				break;
			case 8:
				//汎用パッチ
				stage->AddGameObject<PartsBalancePatch>(dropPos, Vec3(0.0f, 0.0f, 0.0f), Vec3(2.0f, 2.0f, 2.0f));
				break;
			default:
				break;
			}
			debug += 1;
		}
	}



	void EquippedParts::OnCreate()
	{
		
	}

	void EquippedParts::OnUpdate()
	{
		auto test = m_EquippedParts;
		auto a = 0;
	}
}