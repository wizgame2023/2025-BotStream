/*!
@file CameraManager.cpp
@brief カメラの制御処理
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	CameraManager::CameraManager(const shared_ptr<Stage>& stagePtr, float range, float targetRange) :
		MyGameObject(stagePtr),
		m_cameraAngle(XMConvertToRadians(270.0f)),
		m_range(range),
		m_targetRange(targetRange),
		m_lockOn(false),
		m_lockOnNum(-1)
	{

	}
	CameraManager::~CameraManager()
	{

	}

	//作成
	void CameraManager::OnCreate()
	{
		//ステージ上で使われるカメラを取得する
		auto stage = GetStage();
		m_stageCamera = OnGetDrawCamera();//ステージのカメラ取得
		m_lockStageCamera = m_stageCamera.lock();	

		//カメラとPlayerの距離ベクトルを測って大きさを決める
		auto player = stage->GetSharedGameObject<Player>(L"Player");
		Vec3 playerPos = player->GetComponent<Transform>()->GetPosition();
		
		//カメラをPlayerに追従
		m_lockStageCamera->SetEye(Vec3(playerPos.x + (cos(m_cameraAngle) * m_range), playerPos.y + 10.0f, playerPos.z + (sin(m_cameraAngle) * m_range)));
		//注視点はPlayerの位置よりも少し先にしたい
		m_lockStageCamera->SetAt(playerPos);

		//Vec3 CameraPos = m_lockStageCamera->GetEye();
		
		////クォータニオンの値を取得してカメラとPlayerの差を見えるようにするオブジェクトを出そう！
		//Vec3 CameraVec = CameraPos - playerPos;
		//Vec3 CameraScale = CameraVec = Vec3(abs(CameraVec.x), abs(CameraVec.y), abs(CameraVec.z));
		//Vec3 up = Vec3(0, 1, 0);
		//Mat4x4 CameraQt = (Mat4x4)XMMatrixLookAtLH(CameraPos, playerPos, -up);
		
		////レイキャスト生成
		//auto raycast = stage->AddGameObject<CameraRayCast>(Vec3(CameraVec.x/2, CameraVec.y / 2, -CameraVec.z / 2), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f,1.0f,CameraScale.z));
		//raycast->GetComponent<Transform>()->SetQuaternion((Quat)CameraQt);


		//ロックオンの有効範囲を可視化
		stage->AddGameObject<LockOnRange>(m_targetRange * 2, player);

		//もしステージ用のカメラを取得できなかったらreturnして自分を削除します
		if (!m_lockStageCamera)
		{
			GetStage()->RemoveGameObject<CameraManager>(GetThis<CameraManager>());
			return;
		}
	}

	//更新
	void CameraManager::OnUpdate()
	{
		auto delta = App::GetApp()->GetElapsedTime();
		m_lockStageCamera = m_stageCamera.lock();
		auto stage = GetStage();

		//もしステージ用のカメラを取得できなかったらreturnして自分を削除します
		if (!m_lockStageCamera)
		{
			GetStage()->RemoveGameObject<CameraManager>(GetThis<CameraManager>());
			return;
		}


		auto player = stage->GetSharedGameObject<Player>(L"Player");
		Vec3 playerPos = player->GetComponent<Transform>()->GetPosition();

		Vec3 hitPos;//当たった場所を保存する変数
		//当たり判定テスト
		TRIANGLE testTriangle;
		size_t testsize;
		//m_ptrDraw->HitTestStaticMeshSegmentTriangles(playerPos, m_lockStageCamera->GetEye(), hitPos,testTriangle,testsize);


		// インプットデバイスオブジェクト
		InputDevice inputDevice = App::GetApp()->GetInputDevice(); // 様々な入力デバイスを管理しているオブジェクトを取得
		//コントローラーの取得
		CONTROLER_STATE m_controler = inputDevice.GetControlerVec()[0];
		Vec2 contrloerVec = Vec2(m_controler.fThumbRX, m_controler.fThumbRY);

		//左スティックを傾けてカメラがPlayerの周りを回転する処理
		float addAngle = 3.0f * contrloerVec.x * delta;//追加する角度を決めて
		m_cameraAngle += addAngle;//追加


		//ロックオン処理
		auto enemyManager = stage->GetSharedGameObject<EnemyManager>(L"EnemyManager");
		//ここのshared_ptrをweak_ptrにしたいんだけどどうすればいいんだろう？
		vector<shared_ptr<Enemy>> enemyVec = enemyManager->GetEnemyVec(true);//まず、見えている状態のEnemyを受け取る
		vector<shared_ptr<Enemy>> targetVec;//ロックオン候補配列
		//shared_ptr<Enemy> targetObj;//ターゲット対象

		//EnemyとPlayerとの距離を測ってロックオン候補になるのか確認する
		for (auto enemy : enemyVec)
		{
			Vec3 enemyPos = enemy->GetComponent<Transform>()->GetPosition();
			
			//ロックオン範囲の円の中にいるか計算する
			float targetRange = (enemyPos.x - playerPos.x) * (enemyPos.x - playerPos.x) +
								(enemyPos.z - playerPos.z) * (enemyPos.z - playerPos.z);
			float radiusRange = m_targetRange * m_targetRange;

			//ロックオン対象だったら配列に格納する
			if (targetRange <= radiusRange)
			{
				if (!enemy->FindTag(L"ロックオン候補"))
				{
					stage->AddGameObject<LockOnLook>(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), enemy, Vec3(0.0f, 4.0f, 0.0f));
				}

				targetVec.push_back(enemy);
				enemy->AddTag(L"ロックオン候補");//ロックオン候補のタグ追加
			}
			else if(enemy->FindTag(L"ロックオン候補"))
			{
				enemy->RemoveTag(L"ロックオン候補");
			}
		}

		//ロックオン候補がいないならロックオンできない＆選択を初期化
		if (targetVec.size() <= 0)
		{
			for (auto enemy : enemyVec)
			{
				enemy->RemoveTag(L"ロックオン対象");
			}
			m_lockOn = false;
			m_lockOnNum = -1;
			m_targetObj = NULL;
		}

		float playerAngle = player->GetAngle();
		//RBボタンを押すと範囲内に対象がいるならロックオンそうでなければPlayerが向いている方向に回転する
		if (m_controler.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			for (auto enemy : enemyVec)
			{
				enemy->RemoveTag(L"ロックオン対象");
			}

			//ここで誰をロックオン対象にするか決める
			if (targetVec.size() > 0)
			{
				m_lockOnNum++;

				//選択している数値がロックオン候補の数より大きくならないようにする
				if (m_lockOnNum > targetVec.size() - 1)
				{
					m_lockOnNum = targetVec.size() - 1;
				}
			}
			if (targetVec.size() <= 0)
			{
				//Playerの向いている方向の鏡合わせになるように角度を変更する
				MovePlayerAngle(playerAngle);
			}

			//ロックオンしていいか判断する
			if (m_lockOnNum >= targetVec.size() - 1 && m_lockOnNum >= 0)
			{
				m_lockOn = true;
				m_targetObj = targetVec[m_lockOnNum];
				targetVec[m_lockOnNum]->AddTag(L"ロックオン対象");
			}

		}
		//角度リセット(デバック用)
		if (m_controler.wPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
			m_cameraAngle = XMConvertToRadians(270.0f);
		}

		//ロックオン
		if (m_lockOn)
		{
			LockOn(m_targetObj, player);
		}//ロックオンできない時
		else if (!m_lockOn)
		{
			//注視点はPlayerの位置よりも少し先にしたい
			m_lockStageCamera->SetAt(playerPos);
		}






		//カメラをPlayerに追従
		m_lockStageCamera->SetEye(Vec3(playerPos.x + (cos(m_cameraAngle) * m_range), playerPos.y + 10.0f, playerPos.z + (sin(m_cameraAngle) * m_range)));

		//角度の調整0~360度までしか出ないようにする
		AdjustmentAngle();


		////デバック用
		wstringstream wss(L"");
		auto scene = App::GetApp()->GetScene<Scene>();

		wss /* << L"デバッグ用文字列 "*/
			<< L"\nPlayerから見てカメラの角度: " << XMConvertToDegrees(m_cameraAngle)
			<< L"\nPlayerの向いている角度: " << XMConvertToDegrees(-playerAngle)
			<< L"\n当たった場所x: " << hitPos.x
			<< L"\n当たった場所y: " << hitPos.y
			<< L"\n当たった場所z: " << hitPos.z
			//<<L"\nコントローラーの入力 x:"<<contrloerVec.x<<L" y:"<<contrloerVec.y
			//<<L"\nFPS:"<< 1.0f/delta
			<< endl;

		scene->SetDebugString(wss.str());

	}

	//ロックオン機能 対象のオブジェクトを中心点とする
	void CameraManager::LockOn(shared_ptr<GameObject> lockOnObj,shared_ptr<Player> originObj)
	{
		Vec3 lockOnPos = lockOnObj->GetComponent<Transform>()->GetPosition();
		Vec3 originPos = originObj->GetComponent<Transform>()->GetPosition();

		//ロックオン対象をPlayerの距離ベクトルを測って角度取得
		Vec3 lockOnVec = lockOnPos - originPos;
		auto m_targetRad = atan2(lockOnVec.z, lockOnVec.x);

		//計算した角度を入れてカメラを旋回させる
		m_cameraAngle = m_targetRad + XMConvertToRadians(180.0f);

		m_lockStageCamera->SetAt(lockOnPos);
	}

	//Playerの向いている方向の鏡合わせになるように角度を変更する
	void CameraManager::MovePlayerAngle(float playerAngle)
	{
		m_cameraAngle = -playerAngle + XMConvertToRadians(180.0f);
	}

	//角度の調整0~360度までしか出ないようにする
	void CameraManager::AdjustmentAngle()
	{
		if (m_cameraAngle >= XMConvertToRadians(360.0f))
		{
			m_cameraAngle -= XMConvertToRadians(360.0f);
		}
		else if (m_cameraAngle < XMConvertToRadians(0.0f))
		{
			m_cameraAngle += XMConvertToRadians(360.0f);
		}
	}



	//カメラのレイキャスト用
	CameraRayCast::CameraRayCast(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale) :
		ObjectMove(stagePtr),
		m_pos(pos),
		m_rot(rot),
		m_scale(scale)
	{

	}
	CameraRayCast::~CameraRayCast()
	{

	}

	void CameraRayCast::OnCreate()
	{
		//Transform設定
		auto m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

		//ドローメッシュの設定
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetTextureResource(L"StoneBrick");
		//ptrDraw->SetDiffuse(m_color);
		//ptrDraw->SetOwnShadowActive(false);//影は消す
		ptrDraw->SetDrawActive(true);

	}
	void CameraRayCast::OnUpdate()
	{

	}

	//ロックオン範囲を知らせるためのオブジェクト
	//第一引数　ロックオン有効範囲,第二引数　ロックオン範囲の中心となるオブジェクト
	LockOnRange::LockOnRange(const shared_ptr<Stage>& stagePtr,float range,shared_ptr<Player> player):
		ObjectMove(stagePtr),
		m_pos(Vec3(0.0f)),
		m_rot(Vec3(0.0f)),
		m_scale(Vec3(range, 0.1f, range)),
		m_range(range),
		m_player(player)
	{

	}
	LockOnRange::~LockOnRange()
	{

	}

	void LockOnRange::OnCreate()
	{
		//Transform設定
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

		//ドローメッシュの設定
		m_ptrDraw = AddComponent<PNTStaticDraw>();
		m_ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		m_ptrDraw->SetOwnShadowActive(false);//影は消す
		m_ptrDraw->SetDrawActive(true);

	}

	void LockOnRange::OnUpdate()
	{
		float delta = App::GetApp()->GetElapsedTime();
		auto stage = GetStage();
		auto playerLock = m_player.lock();

		//Playerが消えていたら自分も消える
		if (!playerLock)
		{
			GetStage()->RemoveGameObject<LockOnRange>(GetThis<LockOnRange>());
			return;
		}

		//いつもPlayerについていくようにする
		Vec3 PlayerPos = playerLock->GetComponent<Transform>()->GetPosition();
		m_pos = PlayerPos;
		m_pos.y = PlayerPos.y - 0.5f;
		m_trans->SetPosition(m_pos);
		
	}

	//ロックオンしている対象を分かるようにする処理
	LockOnLook::LockOnLook(const shared_ptr<Stage>& stagePtr,Vec3 rot, Vec3 scale,weak_ptr<Actor> parentObj,Vec3 pushPos) :
		ObjectMove(stagePtr),
		m_rot(rot),
		m_scale(scale),
		m_parentObj(parentObj),
		m_pushPos(pushPos)
	{

	}
	LockOnLook::~LockOnLook()
	{

	}

	void LockOnLook::OnCreate()
	{
		auto parentLock = m_parentObj.lock();
		auto parentPos = parentLock->GetComponent<Transform>()->GetPosition();

		//Transform設定
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(parentPos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

		//ドローメッシュの設定
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CONE");
		ptrDraw->SetDiffuse(Col4(0.0f,1.0f,0.0f,1.0f));
		ptrDraw->SetOwnShadowActive(false);//影は消す
		ptrDraw->SetDrawActive(true);
	}
	void LockOnLook::OnUpdate()
	{
		auto parentLock = m_parentObj.lock();
		//追跡対象が消えていたら自分も消える
		if (!parentLock)
		{
			GetStage()->RemoveGameObject<LockOnLook>(GetThis<LockOnLook>());
			return;
		}
		//ここで追跡対象のタグにロックオン対象やロックオン候補などのタグがなければ消去される
		if (!parentLock->FindTag(L"ロックオン候補"))
		{
			GetStage()->RemoveGameObject<LockOnLook>(GetThis<LockOnLook>());
			return;
		}


		if (parentLock->FindTag(L"ロックオン対象"))
		{
			auto ptrDraw = GetComponent<PNTStaticDraw>();
			ptrDraw->SetDiffuse(Col4(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else
		{
			auto ptrDraw = GetComponent<PNTStaticDraw>();
			ptrDraw->SetDiffuse(Col4(0.0f, 1.0f, 0.0f, 1.0f));
		}
		//追跡対象と同じ座標にいる(PushPosという例外あり)
		auto parentPos = parentLock->GetComponent<Transform>()->GetPosition();
		m_trans->SetPosition(parentPos + m_pushPos);
	}


	//デバック用のただの四角い奴
	Cube::Cube(const shared_ptr<Stage>& stagePtr,Vec3 pos,Vec3 rot, Vec3 scale,Col4 color) :
		ObjectNotMove(stagePtr),
		m_pos(pos),
		m_rot(rot),
		m_scale(scale),
		m_color(color)
	{

	}
	Cube::~Cube()
	{

	}

	void Cube::OnCreate()
	{
		//Transform設定
		auto m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

		//ドローメッシュの設定
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetDiffuse(m_color);
		ptrDraw->SetOwnShadowActive(false);//影は消す
		ptrDraw->SetDrawActive(true);
		//ptrDraw->HitTestSkinedMeshSegmentTriangles();

		//ptrDraw->HitT
	}



}
//end basecross
