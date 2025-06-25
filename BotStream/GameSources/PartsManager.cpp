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
		int drop = rand() % 3;
		auto stage = GetStage();
		//par = 10;//テスト用に絶対生成する
		//drop = 0;
		//35%ぐらいの確率でパーツドロップ
		if (par > 0 && par <= 15)
		{
			switch (drop)
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
			default:
				break;
			}
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