/*!
@file CameraManager.cpp
@brief カメラの制御処理
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	CameraManager::CameraManager(const shared_ptr<Stage>& stagePtr, float range, float targetRange,float meleeRange,float speedXAxis,float speedYAxis) :
		MyGameObject(stagePtr),
		m_cameraAngleY(XMConvertToRadians(270.0f)),
		m_cameraAngleX(XMConvertToRadians(105.0f)),
		m_range(range),
		m_targetRange(targetRange),
		m_lockOnFlag(false),
		m_lockOnUse(false),
		m_lockOnChangeFlag(false),
		m_lockOnNum(-1),
		m_meleeRange(meleeRange),
		m_stickFlag(false),
		m_speedXAxis(speedXAxis),
		m_speedYAxis(speedYAxis),
		m_contrloerVec(Vec2(0.0f,0.0f)),
		m_movePlayerAngleFlag(false),
		m_poseFlag(false)
	{

	}
	CameraManager::~CameraManager()
	{

	}

	//作成
	void CameraManager::OnCreate()
	{
		//ステージ上で使われるカメラを取得する
		m_stage = GetStage();
		m_stageCamera = OnGetDrawCamera();//ステージのカメラ取得
		m_lockStageCamera = m_stageCamera.lock();	

		//カメラとPlayerの距離ベクトルを測って大きさを決める
		auto player = m_stage->GetSharedGameObject<Player>(L"Player");
		Vec3 playerPos = player->GetComponent<Transform>()->GetPosition();
		
		//カメラをPlayerに追従
		m_lockStageCamera->SetEye(Vec3(playerPos.x + (cos(m_cameraAngleY) * m_range), playerPos.y + 10.0f, playerPos.z + (sin(m_cameraAngleY) * m_range)));
		//注視点はPlayerの位置よりも少し先にしたい
		m_lockStageCamera->SetAt(playerPos);

		//スプライト追加
		//m_spriteAttack = m_stage->AddGameObject<Sprite>(L"KatanaTex", Vec2(100.0f, 100.0f), Vec3(570.0f, -350.0f, 0));

		Vec3 CameraPos = m_lockStageCamera->GetEye();
				
		//ロックオンの有効範囲を可視化
		//m_stage->AddGameObject<LockOnRange>(m_targetRange, player);

		//SE用のマネージャー取得
		m_SEManager = App::GetApp()->GetXAudio2Manager();
		//射撃用のクロスヘア用のテクスチャ追加
		m_spriteAiming = m_stage->AddGameObject<Sprite>(L"AimingTex", Vec2(50.0f, 50.0f));


		//カメラ用ステートマシンの作成まだステートは作成途中
		m_stateMashine = shared_ptr<CameraStateMachine>(new CameraStateMachine(GetThis<GameObject>()));


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
		m_delta = App::GetApp()->GetElapsedTime();
		m_lockStageCamera = m_stageCamera.lock();

		//もしステージ用のカメラを取得できなかったらreturnして自分を削除します
		if (!m_lockStageCamera)
		{
			GetStage()->RemoveGameObject<CameraManager>(GetThis<CameraManager>());
			return;
		}
		//ポーズフラグがオンならカメラ移動はできない
		if (m_poseFlag)
		{
			return;
		}

		//プレイヤーを取得
		auto player = m_stage->GetSharedGameObject<Player>(L"Player");
		m_playerPos = player->GetComponent<Transform>()->GetPosition();

		// インプットデバイスオブジェクト
		InputDevice inputDevice = App::GetApp()->GetInputDevice(); // 様々な入力デバイスを管理しているオブジェクトを取得
		//コントローラーの取得
		m_controler = inputDevice.GetControlerVec()[0];
		m_contrloerVec = Vec2(m_controler.fThumbRX, m_controler.fThumbRY);

		////慣性付きの回転処理
		//InertialRotation();
		////X軸回転の制限処理
		//CameraAngleXLimit();

		//ステートマシン更新(ステート更新)
		m_stateMashine->Update();

		////ロックオン処理
		//auto enemyManager = m_stage->GetSharedGameObject<EnemyManager>(L"EnemyManager");
		////ここのshared_ptrをweak_ptrにしたいんだけどどうすればいいんだろう？
		//vector<shared_ptr<EnemyBase>> enemyVec = enemyManager->GetEnemyVec(true);//まず、見えている状態のEnemyを受け取る

		//近遠どちらの攻撃をするかの処理
		//MeleeFlagUpdate();

		//ロックオンを解除する条件処理
		//ConditionsLockOff(enemyVec);

		//ObjectFactory::Create<Cube>(GetStage(), Vec3(-10.0f, 0.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Col4(0.0f, 1.0f, 0.0f, 1.0f));

		float playerAngle = player->GetAngle();
		//ロックオン対象の敵を決める処理
		//SelectTargetObj(enemyVec, playerAngle);
		
		////右スティック押し込みでプレイヤーの向いている方向に回転する
		//if (m_controler.wPressedButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
		//{
		//	//if (!m_lockOnFlag && !m_movePlayerAngleFlag)
		//	//{
		//		//Playerの向いている方向に移動するフラグをオンにする
		//		m_movePlayerAngleFlag = true;	
		//		//向く座標を決める
		//		m_targetAngleY = -playerAngle + XMConvertToRadians(180.0f);
		//	//}
		//}
		////フラグがオンになったらPlayerの向きに移動する
		//if (m_movePlayerAngleFlag)
		//{
		//	//Playerの向いている方向の鏡合わせになるように角度を変更する
		//	MovePlayerAngle(m_targetAngleY);
		//}

		////LBを押している最中は射撃モードに移行する
		//if (m_controler.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		//{
		//	//射撃状態
		//	//Playerとの距離を縮めて狙いを定めているっぽくする
		//	m_range = 10.0f;

		//	//銃使うフラグにした
		//	m_meleeFlag = false;

		//	//その後に射撃用のUIも出したい
		//	m_spriteAiming->OnClear(false);

		//	m_spriteAttack->SetTexture(L"GunTex");

		//	//注視点の変更(普段よりも先に見たい)
		//	m_lockStageCamera->SetAt(m_playerPos + Vec3(cosf(m_cameraAngleY) * sin(m_cameraAngleX) * -15.0f,
		//		cos(m_cameraAngleX) * -15.0f,
		//		sinf(m_cameraAngleY) * sin(m_cameraAngleX) * -15.0f));

		//}
		//else
		//{
		//	//通常状態
		//	//Playerとの距離を縮めて狙いを定めているっぽくする
		//	m_range = 15.0f;

		//	//銃を使わないフラグ
		//	m_meleeFlag = true;

		//	//ここはUIを出さない
		//	m_spriteAiming->OnClear(true);

		//	m_spriteAttack->SetTexture(L"KatanaTex");

		//	//注視点の変更
		//	m_lockStageCamera->SetAt(m_playerPos + Vec3(cosf(m_cameraAngleY) * sin(m_cameraAngleX) * -5.0f,
		//		cos(m_cameraAngleX) * -5.0f,
		//		sinf(m_cameraAngleY) * sin(m_cameraAngleX) * -5.0f));
		//}


		//////現在の注視点を見れるようにする(デバック用)
		////if (m_controler.wPressedButtons & XINPUT_GAMEPAD_B)
		////{
		////	player->GetStage()->AddGameObject<Cube>(m_lockStageCamera->GetAt(), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f),Col4(1.0f,0.0f,0.0f,1.0f));
		////}


		////ロックオンするときの処理
		////LockOn(player);


		////LockOnCanはどのオブジェクト達になるのか処理
		////LockOnCandidate(enemyVec, m_playerPos);
		////角度の調整0~360度までしか出ないようにする
		//AdjustmentAngle();
		////カメラの位置更新
		//CameraPosUpdate();


		//デバック用
		wstringstream wss(L"");
		auto scene = App::GetApp()->GetScene<Scene>();

		//ロックオン対象との距離を計算
		if (m_targetObj)
		{
			Vec3 targetVec = m_targetObj->GetComponent<Transform>()->GetPosition() - m_playerPos;
			m_targetDis = (targetVec.x*targetVec.x) + (targetVec.z*targetVec.z);
		}

		
		wss /* << L"デバッグ用文字列 "*/
			<< L"\nPlayerから見てカメラの角度Y軸: " << XMConvertToDegrees(m_cameraAngleY)
			<< L"\nPlayerから見てカメラの角度X軸: " << XMConvertToDegrees(m_cameraAngleX)
			<< L"\nPlayerの向いている角度: " << XMConvertToDegrees(-playerAngle)
			<< L"\nターゲット対象の距離: " << m_targetDis
			<< L"\nFPS: " << 1.0f/m_delta
			//<< L"\n当たった場所x: " << hitPos.x
			//<< L"\n当たった場所y: " << hitPos.y
			//<< L"\n当たった場所z: " << hitPos.z
			//<<L"\nコントローラーの入力 x:"<<contrloerVec.x<<L" y:"<<contrloerVec.y
			//<<L"\nFPS:"<< 1.0f/delta
			<< endl;

			//if (m_lockOnNum >= 0)
			//{		
			//	auto targetAngle = m_lockOnAngle[m_lockOnNum];
			//	float a = targetAngle;
			//	wss << L"ロックオン角度 " << XMConvertToDegrees(targetAngle);
			//}

		scene->SetDebugString(wss.str());

	}

	//カメラのX軸回転の制限
	void CameraManager::CameraAngleXLimit(float maxRad,float minRad)
	{
		//もし今のX軸回転が最小値よりも大きかったら最小値にする
		if (m_cameraAngleX < minRad)
		{
			m_cameraAngleX = minRad;
		}
		//もし、今のX軸回転が最大よりも大きかったら最大値にする
		if (m_cameraAngleX > maxRad)
		{
			m_cameraAngleX = maxRad;
		}
	}

	//カメラの慣性回転の処理
	void CameraManager::InertialRotation(float decelerationSpeed,float magnificationSpeed)
	{
		//Y軸回転
		if (m_contrloerVec.x != 0.0f)
		{
			//左スティックをX方面に傾けてカメラがPlayerのY軸方向に回転する処理
			m_addAngleYAxis = (m_speedYAxis * m_contrloerVec.x) * decelerationSpeed;
		}
		//コントローラーを傾けていなければだんだん移動スピードがなくなる
		else if(m_contrloerVec.x == 0.0f)
		{
			if (m_addAngleYAxis > 0)
			{
				m_addAngleYAxis -= magnificationSpeed * m_delta;
				if (m_addAngleYAxis <= 0)
				{
					m_addAngleYAxis = 0.0f;
				}
			}
			else if (m_addAngleYAxis < 0)
			{
				m_addAngleYAxis += magnificationSpeed * m_delta;
				if (m_addAngleYAxis >= 0)
				{
					m_addAngleYAxis = 0.0f;
				}
			}
		}

		//X軸回転
		if (m_contrloerVec.y != 0.0f)
		{
			//左スティックをY方面に傾けてカメラがPlayerのX軸方向に回転する処理
			m_addAngleXAxis = (m_speedXAxis * m_contrloerVec.y) * decelerationSpeed;
		}
		//コントローラーを傾けていなければだんだん移動スピードがなくなる
		else if (m_contrloerVec.y == 0.0f)
		{
			if (m_addAngleXAxis > 0)
			{
				m_addAngleXAxis -= magnificationSpeed / 2 * m_delta;
				if (m_addAngleXAxis <= 0)
				{
					m_addAngleXAxis = 0.0f;
				}
			}
			else if (m_addAngleXAxis < 0)
			{
				m_addAngleXAxis += magnificationSpeed / 2 * m_delta;
				if (m_addAngleXAxis >= 0)
				{
					m_addAngleXAxis = 0.0f;
				}
			}
		}

		//右スティック入力がされていない方向はスピードが落ちていく
		float maxAddAngleXAxis = 0.025f;
		float maxAddAngleYAxis = 0.08f;
		
		m_cameraAngleX += m_addAngleXAxis * m_delta;//追加
		m_cameraAngleY += -m_addAngleYAxis * m_delta;//追加

	}

	//通常モード時のカメラ操作処理
	void CameraManager::CameraControlNomalMode()
	{
		//通常状態
		//m_range = 15.0f;

		//m_spriteAttack->SetTexture(L"KatanaTex");

		auto pushMaxAtPos = -5.0f;
		//移動処理
		m_pushAtPos.y = MoveToDestination(m_pushAtPos.y, pushMaxAtPos, 120.0f);
		m_pushAtPos.z = MoveToDestination(m_pushAtPos.z, pushMaxAtPos, 120.0f);
		m_pushAtPos.x = MoveToDestination(m_pushAtPos.x, pushMaxAtPos, 120.0f);

		//銃を使わないフラグ
		m_meleeFlag = true;
		//ここはUIを出さない
		m_spriteAiming->OnClear(true);

		//注視点の変更
		m_lockStageCamera->SetAt
		(
			m_playerPos + Vec3(cosf(m_cameraAngleY) * sin(m_cameraAngleX) * m_pushAtPos.x,
			cos(m_cameraAngleX) * m_pushAtPos.y,
			sinf(m_cameraAngleY) * sin(m_cameraAngleX) * m_pushAtPos.z)
		);
	}

	//射撃モード時のカメラ操作処理
	void CameraManager::CameraControlShotMode()
	{
		//射撃状態
		//Playerとの距離を縮めて狙いを定めているっぽくする
		//m_range = 10.0f;

		//m_spriteAttack->SetTexture(L"GunTex");


		auto pushMaxAtPos = -15.0f;
		auto pushMaxAtPosY = -15.0f;
		//移動処理
		m_pushAtPos.x = MoveToDestination(m_pushAtPos.x, pushMaxAtPos, 16.0f*5);
		m_pushAtPos.y = MoveToDestination(m_pushAtPos.y, pushMaxAtPosY, 16.0f*5);
		m_pushAtPos.z = MoveToDestination(m_pushAtPos.z, pushMaxAtPos, 16.0f*5);

		//銃使うフラグにした
		m_meleeFlag = false;
		//ここはUIを出す
		m_spriteAiming->OnClear(false);

		//注視点の変更(普段よりも先に見たい)
		m_lockStageCamera->SetAt(m_playerPos + Vec3(cosf(m_cameraAngleY) * sin(m_cameraAngleX) * m_pushAtPos.x,
			cos(m_cameraAngleX) * m_pushAtPos.y,
			sinf(m_cameraAngleY) * sin(m_cameraAngleX) * m_pushAtPos.z));
	}

	//プレイヤーの方向に回転するカメラ操作処理
	void CameraManager::CameraControlTransitionMode()
	{
		CameraControlNomalMode();

		//プレイヤーを取得
		auto player = m_stage->GetSharedGameObject<Player>(L"Player");
		float playerAngle = player->GetAngle();

		//Playerの向いている方向に移動するフラグをオンにする
		m_movePlayerAngleFlag = true;
		//向く座標を決める
		m_targetAngleY = -playerAngle + XMConvertToRadians(180.0f);


		//フラグがオンになったらPlayerの向きに移動する
		if (m_movePlayerAngleFlag)
		{
			//Playerの向いている方向の鏡合わせになるように角度を変更する
			MovePlayerAngle(m_targetAngleY);

			//カメラ移動処理が終わったら通常ステートに戻る
			if (!m_movePlayerAngleFlag)
			{
				ChangeState(L"Normal");
			}
		}
	}

	//現在地から目的地までの移動処理
	float CameraManager::MoveToDestination(float nowPos, float destinationPos,float speed)
	{
		//移動する距離の差
		float difference = destinationPos - nowPos;

		//プラスする位置になるまで縮ませる
		if (nowPos > destinationPos && difference < 0)
		{
			nowPos-= m_delta * speed;
		}
		//プラスする位置になるまで縮ませる
		if (nowPos < destinationPos && difference > 0)
		{
			nowPos += m_delta * speed;
		}
		//プラスする位置が既定から越えないようにする
		if (nowPos < destinationPos && difference < 0)
		{
			nowPos = destinationPos;
		}
		//プラスする位置が既定から越えないようにする
		if (nowPos > destinationPos && difference > 0)
		{
			nowPos = destinationPos;
		}

		return nowPos;
	}

	//ステート変更処理 引数に入れたステートに変更する
	void CameraManager::ChangeState(wstring stateName)
	{
		m_stateMashine->ChangeState(stateName);
	}

	//カメラのポジションを決める関数
	void CameraManager::CameraPosUpdate(float maxPushPosY,float maxLength,float CameraLenght)
	{
		auto objVec = m_stage->GetGameObjectVec();
		m_cameraPos = m_lockStageCamera->GetEye();

		Vec3 hitPos; // 出力用：レイの交差地点(衝突点)
		TRIANGLE triangle; // レイが交差したポリゴンを構成する頂点の座標
		size_t triangleNumber; // レイが交差したポリゴンの番号
		float min = 9999999.9f;//Playerから見てカメラの障害となる距離の最小値

		//移動する距離の差
		float difference = maxPushPosY - m_pushPos.y;

		//プラスする位置になるまで縮ませる
		if (m_pushPos.y > maxPushPosY && difference < 0)
		{
			m_pushPos.y -= m_delta * 20.0f;
		}
		//プラスする位置になるまで縮ませる
		if (m_pushPos.y < maxPushPosY && difference > 0)
		{
			m_pushPos.y += m_delta * 20.0f;
		}
		//プラスする位置が既定から越えないようにする
		if (m_pushPos.y < maxPushPosY && difference < 0)
		{
			m_pushPos.y = maxPushPosY;
		}
		//プラスする位置が既定から越えないようにする
		if (m_pushPos.y > maxPushPosY && difference > 0)
		{
			m_pushPos.y = maxPushPosY;
		}


		//移動する距離の差(カメラとプレイヤーの半径)
		difference = CameraLenght - m_range;

		//プラスする位置になるまで縮ませる
		if (m_range > CameraLenght && difference < 0)
		{
			m_range -= m_delta * 30.0f;
		}
		//プラスする位置になるまで縮ませる
		if (m_range < CameraLenght && difference > 0)
		{
			m_range += m_delta * 30.0f;
		}
		//プラスする位置が既定から越えないようにする
		if (m_range < CameraLenght && difference < 0)
		{
			m_range = CameraLenght;
		}
		//プラスする位置が既定から越えないようにする
		if (m_range > CameraLenght && difference > 0)
		{
			m_range = CameraLenght;
		}



		m_cameraPos = Vec3(m_playerPos.x + (cos(m_cameraAngleY) * sin(m_cameraAngleX) * m_range),
			(m_playerPos.y + m_pushPos.y) + cos(m_cameraAngleX) * m_range,
			m_playerPos.z + (sin(m_cameraAngleY) * sin(m_cameraAngleX) * m_range));


		//移動する距離の差
		difference = maxLength - m_gunShiftLength;
			
		//ずらす位置を既定の長さになるまで伸ばす
		if (m_gunShiftLength < maxLength && difference > 0)
		{
			m_gunShiftLength += m_delta * 20.0f;
		}
		//ずらす位置を既定の長さになるまで伸ばす
		if (m_gunShiftLength > maxLength && difference < 0)
		{
			m_gunShiftLength -= m_delta * 20.0f;
		}
		//ずらす長さを既定から越えないようにする
		if (m_gunShiftLength > maxLength && difference > 0)
		{
			m_gunShiftLength = maxLength;
		}
		//ずらす長さを既定から越えないようにする
		if (m_gunShiftLength < maxLength && difference < 0)
		{
			m_gunShiftLength = maxLength;
		}

		//射撃モードは少し通常の状態から位置をずらす
		m_cameraPos += Vec3(cos(m_cameraAngleY + XMConvertToRadians(45.0f)) * m_gunShiftLength,
			0.0f,
			sin(m_cameraAngleY + XMConvertToRadians(45.0f)) * m_gunShiftLength);


		//障害物になりえるオブジェクト達にカメラの機能を邪魔していないか見る
		for (auto obj : objVec)
		{
			auto obstacles = dynamic_pointer_cast<GameObject>(obj);//当たり判定の対象
			float hitLength = min;//Playerと障害物の距離の長さ

			//障害物になりえそうならカメラの表示に邪魔をしていないか確認をする
			if (obstacles)
			{
				//カメラの障害になりえるオブジェクトしかカメラを邪魔をしているか評価しない
				if (!obstacles->FindTag(L"CameraObstacles")) continue;

				auto ptrDraw = obstacles->GetComponent<PNTStaticDraw>();//Bc対応にしない
				ptrDraw->HitTestStaticMeshSegmentTriangles(m_playerPos, m_cameraPos, hitPos, triangle, triangleNumber);
				Vec3 playerorObstaclesVec = hitPos - m_playerPos;
				hitLength = abs(playerorObstaclesVec.x) + abs(playerorObstaclesVec.y) + abs(playerorObstaclesVec.z);
			}

			//minよりhitLengthが短かったら位置更新する
			if (hitPos != Vec3(0.0f, 0.0f, 0.0f) && min > hitLength)
			{
				min = hitLength;
				//hitPos.y = m_cameraPos.y;//Y座標は変えないようにする
				m_cameraPos = hitPos;
			}
		}

		//カメラの位置更新
		m_lockStageCamera->SetEye(m_cameraPos);

	}

	//LockOnCanを決める関数
	void CameraManager::LockOnCandidate(vector<shared_ptr<EnemyBase>> enemyVec,Vec3 playerPos)
	{
		m_targets.clear();//配列の初期化
		m_lockOnFlag = false;//フラグの初期化
		vector<shared_ptr<EnemyBase>> kariTargetsVec;

		for (auto enemy : enemyVec)
		{
			Vec3 enemyPos = enemy->GetComponent<Transform>()->GetPosition();

			//ロックオン範囲の円の中にいるか計算する
			float targetRange = (enemyPos.x - playerPos.x) * (enemyPos.x - playerPos.x) +
				(enemyPos.z - playerPos.z) * (enemyPos.z - playerPos.z);
			float radiusRange = m_targetRange * m_targetRange;

			//LockOnCanなら配列にオブジェクトとPosを入れる
			if (targetRange <= radiusRange)
			{
				if (!enemy->FindTag(L"LockOnCan"))
				{
					m_stage->AddGameObject<LockOnLook>(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), enemy, Vec3(0.0f, 4.0f, 0.0f));
				}

				m_targetsPos.push_back(enemyPos);//LockOnCanのPosを配列に入れる
				m_targets.push_back(enemy);
				enemy->AddTag(L"LockOnCan");//LockOnCanのタグ追加
				m_lockOnFlag = true;//ロックオン使用可能
			}
			else if (enemy->FindTag(L"LockOnCan"))//LockOnCanから外れたのがLockOnTargetならロックオンをやめる
			{
				enemy->RemoveTag(L"LockOnCan");
				if (enemy->FindTag(L"LockOnTarget"))
				{
					LockOff(enemyVec);//ロックオンの解除
				}
			}

			//auto  = cameraPos - enemyPos

		}

	}

	//ロックオン機能 対象のオブジェクトを中心点とする
	void CameraManager::LockOn(shared_ptr<GameObject> lockOnObj,shared_ptr<Player> originObj)
	{
		Vec3 lockOnPos = lockOnObj->GetComponent<Transform>()->GetPosition();
		Vec3 originPos = originObj->GetComponent<Transform>()->GetPosition();

		//LockOnTargetをPlayerの距離ベクトルを測って角度取得
		Vec3 lockOnVec = lockOnPos - originPos;
		auto m_targetRad = atan2(lockOnVec.z, lockOnVec.x);

		//計算した角度を入れてカメラを旋回させる
		//m_cameraAngleY = m_targetRad + XMConvertToRadians(180.0f);
		MoveAngle(m_targetRad + XMConvertToRadians(180.0f), 1);
		//m_lockStageCamera->SetAt(lockOnPos);
		MoveLockAt(lockOnPos);//注視点移動
	}

	//ロックオンの解除機能
	void CameraManager::LockOff(vector<shared_ptr<EnemyBase>> enemyVec)
	{
		m_targetDis = 0.0f;
		m_targetObj->RemoveTag(L"LockOnTarget");
		m_lockOnFlag = false;//ロックオンできない
		m_lockOnUse = false;//ロックオンしない
		m_lockOnNum = -1;
		m_targetObj = NULL;
	}

	//LockOnCanのデータを更新する処理
	void CameraManager::UpdateTargesDeta(Vec3 playerPos)
	{
		m_targesDeta.clear();//初期化

		//ロックオンを切り替えるときにスティックを傾けてLockOnTargetを変更する処理
		Vec3 targetPos = m_targetObj->GetComponent<Transform>()->GetPosition();
		//Playerから見てLockOnTargetのいる方向ベクトルを求める
		Vec3 lockOnVec = targetPos - playerPos;

		for (auto enemy : m_targets)
		{
			Vec3 enemyPos = enemy->GetComponent<Transform>()->GetPosition();

			//Playerから見てLockOnCanのいる方向ベクトルを求める
			Vec3 lockOnCanVec = enemyPos - playerPos;

			//外積を求める
			Vec3 outsideCorner = Vec3(lockOnVec.y * 0.0f - lockOnCanVec.y * 0.0f,//x
				lockOnVec.z * lockOnCanVec.x - lockOnCanVec.z * lockOnVec.x,//y
				lockOnVec.x * 0.0f - lockOnCanVec.x * 0.0f//z
			);
			//左がマイナス右がプラス
			auto cross = outsideCorner.y;
			//crossが0より大きかったらfalse逆に小さかったらtrue
			int leftOrRight;
			if (cross < 0) leftOrRight = Left;
			if (cross > 0) leftOrRight = Right;
			if (cross == 0) leftOrRight = Middle;

			//なす角を求める
			float aSqrt = sqrt((lockOnVec.x * lockOnVec.x) + (lockOnVec.z * lockOnVec.z));
			float bSqrt = sqrt((lockOnCanVec.x * lockOnCanVec.x) + (lockOnCanVec.z * lockOnCanVec.z));
			//割られる数値
			float divide = (lockOnVec.x * lockOnCanVec.x) + (lockOnVec.z * lockOnCanVec.z);

			auto rad = divide / (aSqrt * bSqrt);
			auto deg = XMConvertToDegrees(rad);

			targetsDeta targetDeta = targetsDeta(deg, leftOrRight);
			//プレイヤーから見て敵のいる角度を配列に入れる
			m_targesDeta.push_back(targetDeta);
		}
	}

	//LockOnTargetを変更する処理
	//第一引数 右か左か 第二引数 ターゲット対象のなす角
	void CameraManager::ChangeLockOn(int leftOrRight,float targetAngle)
	{
		float min = 99999.9f;//最小値

		//外角の差がマイナスで一番近いターゲット候補がターゲット対象となる
		for (int i = 0; i <= m_targesDeta.size() - 1; i++)
		{
			//今見ているターゲット候補が左側にいるわけでなければreturn
			if (m_targesDeta[i].leftOrRight != leftOrRight) continue;

			float targetsDeg = XMConvertToDegrees(m_targesDeta[i].lockOnAngle);
			float difference = abs(m_targesDeta[i].lockOnAngle - targetAngle);

			if (min > difference && difference != 0.0f)
			{
				min = difference;
				m_targetObj->RemoveTag(L"LockOnTarget");
				m_lockOnNum = i;
				m_lockOnChangeFlag = true;
			}
		}
	}

	//Playerの向いている方向の鏡合わせになるように角度を変更する
	void CameraManager::MovePlayerAngle(float playerAngle)
	{
		//m_cameraAngleY = -playerAngle + XMConvertToRadians(180.0f);
		//auto targetAngleY = -playerAngle + XMConvertToRadians(180.0f);
		bool Movechage = false;
		Movechage = MoveAngle(playerAngle, 1);//角度を移動させる

		//移動が終わったのでフラグをfalseにする
		if (Movechage) m_movePlayerAngleFlag = false;
	}

	//注視点の移動処理
	void CameraManager::MoveLockAt(Vec3 targetPos)
	{
		auto lockAt = m_lockStageCamera->GetAt();
		auto lockAtVec = targetPos - lockAt;
		float lockAtSpeed = 30.0f;

		//注視点が既に目標を示していたら動かない
		if (lockAtVec == Vec3(0.0f,0.0f,0.0f)) return;

		//X座標
		if (lockAtVec.x > 0)//プラス方向に行くとき
		{
			lockAt.x += lockAtSpeed * m_delta;
			//プラス方向に行きすぎたらターゲットPosと同じにする
			if (lockAt.x > targetPos.x)
			{
				lockAt.x = targetPos.x;
			}
		}
		else if (lockAtVec.x < 0)
		{
			lockAt.x -= lockAtSpeed * m_delta;
			//マイナス方向に行きすぎたらターゲットPosと同じにする
			if (lockAt.x < targetPos.x)
			{
				lockAt.x = targetPos.x;
			}
		}

		//Y座標
		if (lockAtVec.y > 0)//プラス方向に行くとき
		{
			lockAt.y += lockAtSpeed * m_delta;
			//プラス方向に行きすぎたらターゲットPosと同じにする
			if (lockAt.y > targetPos.y)
			{
				lockAt.y = targetPos.y;
			}
		}
		else if (lockAtVec.y < 0)
		{
			lockAt.y -= lockAtSpeed * m_delta;
			//マイナス方向に行きすぎたらターゲットPosと同じにする
			if (lockAt.y < targetPos.y)
			{
				lockAt.y = targetPos.y;
			}
		}

		//Z座標
		if (lockAtVec.z > 0)//プラス方向に行くとき
		{
			lockAt.z += lockAtSpeed * m_delta;
			//プラス方向に行きすぎたらターゲットPosと同じにする
			if (lockAt.z > targetPos.z)
			{
				lockAt.z = targetPos.z;
			}
		}
		else if (lockAtVec.z < 0)
		{
			lockAt.z -= lockAtSpeed * m_delta;
			//マイナス方向に行きすぎたらターゲットPosと同じにする
			if (lockAt.z < targetPos.z)
			{
				lockAt.z = targetPos.z;
			}
		}

		//注視点移動処理
		m_lockStageCamera->SetAt(lockAt);
	}

	//回転度の移動処理//ここを作業する
	//第一引数 向きたい目標角度 第二引数　どの軸で移動するか(Xが0Yが1) 戻り値 移動処理が終わったか
	bool CameraManager::MoveAngle(float targetAngle,int XorY)
	{
		//Y軸
		auto angleNow = m_cameraAngleY;
		auto addAngleSpeed = 10.0f;
		auto angleDifference = targetAngle - angleNow;
		//角度の差が181以上ならマイナスにして計算したほうが進む方向として早い
		if (angleDifference >= XMConvertToRadians(181.0f))
		{
			targetAngle -= XMConvertToRadians(360.0f);
			angleDifference = targetAngle - angleNow;
		}
		if (angleDifference <= XMConvertToRadians(-181.0f))
		{
			targetAngle += XMConvertToRadians(360.0f);
			angleDifference = targetAngle - angleNow;
		}

		if (angleDifference > 0)//プラス方向に行くとき
		{
			m_cameraAngleY += addAngleSpeed * m_delta;
			//プラス方向に行きすぎたらターゲットの角度と同じにする
			if (m_cameraAngleY > targetAngle)
			{
				m_cameraAngleY = targetAngle;
				return true;//移動完了したことを知らせる
			}
		}
		else if (angleDifference < 0)//マイナス方向に行くとき
		{
			m_cameraAngleY -= addAngleSpeed * m_delta;
			//プラス方向に行きすぎたらターゲットの角度と同じにする
			if (m_cameraAngleY < targetAngle)
			{
				m_cameraAngleY = targetAngle;
				return true;//移動完了したことを知らせる
			}
		}

		//移動完了してないことを知らせる
		return false;
	}

	//角度の調整0~360度までしか出ないようにする
	void CameraManager::AdjustmentAngle()
	{
		if (m_cameraAngleY >= XMConvertToRadians(360.0f))
		{
			m_cameraAngleY -= XMConvertToRadians(360.0f);
		}
		else if (m_cameraAngleY < XMConvertToRadians(0.0f))
		{
			m_cameraAngleY += XMConvertToRadians(360.0f);
		}
	}

	//ロックオンを解除する処理
	void CameraManager::ConditionsLockOff(vector<shared_ptr<EnemyBase>> enemyVec)
	{
		//LockOnCanがいないならロックオンできない＆選択を初期化
		if (m_targets.size() <= 0 && m_targetObj)
		{
			LockOff(enemyVec);//ロックオンの解除
		}
		//ロックオン中にLockOnTargetが居なくなったらロックオン解除する
		if (m_lockOnUse)
		{
			auto test = m_targetObj->GetUsed();
			if (!test)
			{
				LockOff(enemyVec);//ロックオンの解除
			}
		}
	}

	//ロックオンする敵を決める処理
	void CameraManager::SelectTargetObj(vector<shared_ptr<EnemyBase>> enemyVec,float playerAngle)
	{
		//ロックオンが出来てロックオンのしてないなら使う、使ってたら使わない どちらでもなければそうでないならプレイヤーの向いている方向に向く
		if (m_controler.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			//ロックオンするか決める処理
			if (m_lockOnFlag && !m_lockOnUse)
			{
				//ロックオンする敵を決める
				float min = 99999;
				//距離を測ってPlayerから一番近い敵をロックオンする
				for (auto enemy : m_targets)
				{
					Vec3 enemyPos = enemy->GetComponent<Transform>()->GetPosition();
					Vec3 distanceVec = enemyPos - m_playerPos;//敵とプレイヤーとの距離ベクトル
					float ditance = abs(distanceVec.x) + abs(distanceVec.z);
					if (min > ditance)
					{
						min = ditance;
						m_targetDis = abs(distanceVec.x) + abs(distanceVec.z);//対象との距離を保存する
						m_targetObj = enemy;//LockOnTargetを決める
					}
				}

				m_lockOnUse = true;//ロックオン使用
				m_targetObj->AddTag(L"LockOnTarget");
				m_SEManager->Start(L"LockOnSE", 0, 0.9f);//ロックオン用SE再生
			}
			else if (m_lockOnFlag && m_lockOnUse)
			{
				m_targetDis = 0.0f;
				LockOff(enemyVec);//ロックオンの解除
			}
			else if (!m_lockOnFlag && !m_movePlayerAngleFlag)
			{
				//Playerの向いている方向に移動するフラグをオンにする
				m_movePlayerAngleFlag = true;
				//向く座標を決める
				m_targetAngleY = -playerAngle + XMConvertToRadians(180.0f);
				auto a = 0;
			}
		}

	}

	//ロックオン処理
	void CameraManager::LockOn(shared_ptr<Player> player)
	{
		//ロックオンするときの処理
		if (m_lockOnUse)
		{
			//ロックオンするときの材料作成
			UpdateTargesDeta(m_playerPos);

			float targetAngle;
			//ターゲット対象のなす角を受け取る
			for (auto enemy : m_targesDeta)
			{
				if (enemy.leftOrRight == Middle)
				{
					targetAngle = enemy.lockOnAngle;
				}
			}

			//ターゲット対象からスティックを傾けている方向のターゲット候補に変える処理
			if (m_contrloerVec.x <= -0.9f && m_stickFlag)//対象の左隣にいる候補に移す
			{
				//現在の対象の方向と一番近い候補がターゲット対象になる
				ChangeLockOn(Left, targetAngle);
				m_stickFlag = false;//入力を受け付けない
			}
			if (m_contrloerVec.x >= 0.9f && m_stickFlag)//対象の右隣にいる候補に移す
			{
				//現在の対象の方向と一番近い候補がターゲット対象になる
				ChangeLockOn(Right, targetAngle);
				m_stickFlag = false;//入力を受け付けない
			}

			//スティックを傾けた後スティックを元に戻したら入力を受け入れる
			if (!m_stickFlag && m_contrloerVec.x == 0.0f)
			{
				m_stickFlag = true;//入力を受け付ける
			}

			//ロックオンしていいか判断する
			if (m_lockOnChangeFlag)
			{
				m_lockOnChangeFlag = false;
				m_lockOnUse = true;//ロックオン使用
				m_targetObj = m_targets[m_lockOnNum];
				m_targets[m_lockOnNum]->AddTag(L"LockOnTarget");
			}

			LockOn(m_targetObj, player);//ロックオン
		}//ロックオンできない時
		else if (!m_lockOnUse)
		{
			//注視点はPlayerの位置よりも少し先にしたい
			m_lockStageCamera->SetAt(m_playerPos + Vec3(0.0f, 3.0f, 0.0f));
		}

	}

	//ターゲット対象を渡す関数
	shared_ptr<Actor> CameraManager::GetTargetObj()
	{
		return m_targetObj;
	}

	//ターゲット対象との距離を渡す
	float CameraManager::GetTargetDis()
	{
		return m_targetDis;
	}

	//ポーズ処理のオンオフ
	void CameraManager::PoseSwitch(bool onOff)
	{
		m_poseFlag = onOff;
	}

	//近遠どちらの攻撃をするかの処理
	void CameraManager::MeleeFlagUpdate()
	{
		//近距離攻撃の有効範囲
		float meleeRange = 200.0f;
		if (m_targetDis >= meleeRange)
		{
			m_meleeFlag = true;
		}
		if (m_targetDis < meleeRange)
		{
			m_meleeFlag = false;
		}
	}

	//近距離攻撃をするかの処理のゲッター
	bool CameraManager::GetMeleeFlag()
	{
		return m_meleeFlag;
	}



	//ロックオン範囲を知らせるためのオブジェクト
	//第一引数　ロックオン有効範囲,第二引数　ロックオン範囲の中心となるオブジェクト
	LockOnRange::LockOnRange(const shared_ptr<Stage>& stagePtr,float range,shared_ptr<Player> player):
		ObjectMove(stagePtr),
		m_pos(Vec3(0.0f)),
		m_rot(Vec3(0.0f)),
		m_scale(Vec3(range*2, 1.0f, range*2)),
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
		m_ptrDraw->SetTextureResource(L"BarTex");
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
		m_ptrDraw->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 0.3f));
		m_ptrDraw->SetEmissive(Col4(1.0f, 1.0f, 1.0f, 0.3f));
		SetAlphaActive(true);


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

		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(XMConvertToRadians(180.0f), 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//Transform設定
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(parentPos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

		//ドローメッシュの設定
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CONE");
		ptrDraw->SetDiffuse(Col4(0.0f,1.0f,0.0f,1.0f));
		ptrDraw->SetMeshToTransformMatrix(spanMat);
		ptrDraw->SetOwnShadowActive(false);//影は消す
		ptrDraw->SetDrawActive(true);
		ptrDraw->SetEmissive(Col4(0.0f, 1.0f, 0.0f, 1.0f)); // 自己発光カラー（ライティングによる陰影を消す効果がある）
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
		//ここで追跡対象のタグにLockOnTargetやLockOnCanなどのタグがなければ消去される
		if (!parentLock->FindTag(L"LockOnCan"))
		{
			GetStage()->RemoveGameObject<LockOnLook>(GetThis<LockOnLook>());
			return;
		}


		if (parentLock->FindTag(L"LockOnTarget"))
		{
			auto ptrDraw = GetComponent<PNTStaticDraw>();
			ptrDraw->SetDiffuse(Col4(1.0f, 0.0f, 0.0f, 1.0f));
			ptrDraw->SetEmissive(Col4(1.0f, 0.0f, 0.0f, 1.0f)); // 自己発光カラー（ライティングによる陰影を消す効果がある）
		}
		else
		{
			auto ptrDraw = GetComponent<PNTStaticDraw>();
			ptrDraw->SetDiffuse(Col4(0.0f, 1.0f, 0.0f, 1.0f));
			ptrDraw->SetEmissive(Col4(0.0f, 1.0f, 0.0f, 1.0f)); // 自己発光カラー（ライティングによる陰影を消す効果がある）
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
		ptrDraw->SetEmissive(m_color); // 自己発光カラー（ライティングによる陰影を消す効果がある）
		ptrDraw->SetOwnShadowActive(true); // 影の映り込みを反映させる
	}

	void Cube::OnUpdate()
	{
	}


	//デバック用 敵の攻撃オブジェクト
	EnemyCube::EnemyCube(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, Col4 color) :
		Actor(stagePtr,pos,rot,scale),
		m_color(color)
	{

	}
	EnemyCube::~EnemyCube()
	{

	}

	void EnemyCube::OnCreate()
	{
		Actor::OnCreate();
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
		ptrDraw->SetEmissive(m_color); // 自己発光カラー（ライティングによる陰影を消す効果がある）
		ptrDraw->SetOwnShadowActive(true); // 影の映り込みを反映させる

		//コリジョン生成
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetAfterCollision(AfterCollision::Auto);
		ptrColl->SetDrawActive(false);

		
		//攻撃判定の定義
		auto tmp = GetAttackPtr()->GetHitInfo();
		tmp.Type = AttackType::Enemy;//攻撃のタイプは敵
		tmp.HitOnce = true;//一回しかヒットしないか
		tmp.Damage = 10;//ダメージ
		tmp.HitVel_Stand = Vec3(-30, 1, 0);//ヒットバック距離
		tmp.HitTime_Stand = 3.0f;//のけぞり時間
		//tmp.PauseTime = 5.0f;
		//tmp.ForceRecover = true;
		DefAttack(.5f, tmp);
		GetAttackPtr()->SetPos(Vec3(0, 0, 10));
		auto AttackPtr = GetAttackPtr();
		AttackPtr->SetCollScale(1.0f);
		
		DefAttack(999.0f, tmp);

	}

	void EnemyCube::OnUpdate()
	{
		Actor::OnUpdate();

		//auto tmp = GetAttackPtr()->GetHitInfo();
		////tmp.HitSound = L"Attack1";
		//tmp.Type = AttackType::Enemy;//攻撃のタイプは敵
		//tmp.HitOnce = true;//一回しかヒットしないか
		//tmp.Damage = 10;//ダメージ
		//tmp.HitVel_Stand = Vec3(-2, 5, 0);//ヒットバック距離
		//tmp.HitTime_Stand = 3.0f;//のけぞり時間
		////tmp.ForceRecover = true;
		//DefAttack(.5f, tmp);
		//GetAttackPtr()->SetPos(Vec3(0, 1, 0));
		//auto AttackPtr = GetAttackPtr();
		//AttackPtr->SetCollScale(20.0f);


	}


}
//end basecross
