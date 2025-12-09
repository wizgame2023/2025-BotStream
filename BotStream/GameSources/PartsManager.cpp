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

		//par = 10;//テスト用に絶対生成する
		//drop = 0;
		//40%ぐらいの確率でパーツドロップ
		if (par > 0 && par <= 40)
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
		auto test = m_equippedParts;
		auto a = 0;
	}

	//ポーチにパーツを入れる処理
	void EquippedParts::AddParts(PartsStatus addParts)
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

		// 最大可能装備数
		int equippedPartsMax = 3;
		//パーツを３つ以上付けていないときは自動的に装備される
		if (m_equippedParts.size() < equippedPartsMax)
		{
			m_equippedParts.push_back(addParts);
		}
		//そうでなければ、パーツを古いやつを捨てて新しいやつに変える
		else if (m_equippedParts.size() >= equippedPartsMax)
		{
			//一番最初に取ったものを削除して新しく手に入れたものを入れる
			m_equippedParts.erase(m_equippedParts.begin());
			m_equippedParts.push_back(addParts);
		}

		int roop = 0;
		//装備している合計のステータスを計算する
		for (auto parts : m_equippedParts)
		{
			allParts.id = 0;
			allParts.addAttack += m_equippedParts[roop].addAttack;
			allParts.addAttribute += m_equippedParts[roop].addAttribute;
			allParts.addHP += m_equippedParts[roop].addHP;
			allParts.addSpeed += m_equippedParts[roop].addSpeed;
			allParts.partsImagePass = L"使わないデータ";
			allParts.partsName = L"使わないデータ";

			roop++;
		}

		//パーツ入手SE再生
		auto m_SEManager = App::GetApp()->GetXAudio2Manager();
		m_SEManager->Start(L"GetPartsSE", 0, 0.4f * m_SEVol);

		//プレイヤーに装備した合計のステータス値を渡す
		player->SetEquippedParts(allParts);
	}

}