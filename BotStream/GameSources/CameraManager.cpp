/*!
@file CameraManager.cpp
@brief カメラの制御処理
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	CameraManager::CameraManager(const shared_ptr<Stage>& stagePtr, float range, float targetRange,float meleeRange) :
		MyGameObject(stagePtr),
		m_cameraAngle(XMConvertToRadians(270.0f)),
		m_range(range),
		m_targetRange(targetRange),
		m_lockOnFlag(false),
		m_lockOnUse(false),
		m_lockOnNum(-1),
		m_meleeRange(meleeRange),
		m_stickFlag(false)
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
		m_stage->AddGameObject<LockOnRange>(m_targetRange, player);

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
		//shared_ptr<Stage> stage = GetStage();

		//もしステージ用のカメラを取得できなかったらreturnして自分を削除します
		if (!m_lockStageCamera)
		{
			GetStage()->RemoveGameObject<CameraManager>(GetThis<CameraManager>());
			return;
		}


		auto player = m_stage->GetSharedGameObject<Player>(L"Player");
		Vec3 playerPos = player->GetComponent<Transform>()->GetPosition();

		Vec3 hitPos;//当たった場所を保存する変数
		//当たり判定テスト
		TRIANGLE testTriangle;
		size_t testsize;
		//m_ptrDraw->HitTestStaticMeshSegmentTriangles(playerPos, m_lockStageCamera->GetEye(), hitPos,testTriangle,testsize);

		// インプットデバイスオブジェクト
		InputDevice inputDevice = App::GetApp()->GetInputDevice(); // 様々な入力デバイスを管理しているオブジェクトを取得
		//コントローラーの取得
		m_controler = inputDevice.GetControlerVec()[0];
		Vec2 contrloerVec = Vec2(m_controler.fThumbRX, m_controler.fThumbRY);

		//左スティックを傾けてカメラがPlayerの周りを回転する処理
		float addAngle = 3.0f * contrloerVec.x * m_delta;//追加する角度を決めて
		m_cameraAngle += -addAngle;//追加

		//ロックオン処理
		auto enemyManager = m_stage->GetSharedGameObject<EnemyManager>(L"EnemyManager");
		//ここのshared_ptrをweak_ptrにしたいんだけどどうすればいいんだろう？
		vector<shared_ptr<Enemy>> enemyVec = enemyManager->GetEnemyVec(true);//まず、見えている状態のEnemyを受け取る
		//vector<shared_ptr<Enemy>> targetVec;//ロックオン候補配列

		//ロックオン候補はどのオブジェクト達になるのか処理
		LockOnCandidate(enemyVec, playerPos);

		//ロックオン候補がいないならロックオンできない＆選択を初期化
		if (m_targets.size() <= 0)
		{
			LockOff(enemyVec);//ロックオンの解除
		}

		ObjectFactory::Create<Cube>(GetStage(), Vec3(-10.0f, 0.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Col4(0.0f, 1.0f, 0.0f, 1.0f));

		float playerAngle = player->GetAngle();
		//RBボタンを押す時の処理　
		//ロックオンが出来てロックオンのしてないなら使う、使ってたら使わない どちらでもなければそうでないならプレイヤーの向いている方向に向く
		if (m_controler.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			for (auto enemy : enemyVec)
			{
				enemy->RemoveTag(L"ロックオン対象");
			}

			//ロックオンするか決める処理
			if (m_lockOnFlag && !m_lockOnUse)
			{
				m_lockOnNum++;
				m_lockOnUse = true;//ロックオン使用
				m_targetObj = m_targets[m_lockOnNum];
				m_targets[m_lockOnNum]->AddTag(L"ロックオン対象");
			}
			else if (m_lockOnFlag && m_lockOnUse)
			{
				LockOff(enemyVec);//ロックオンの解除
			}
			else if (!m_lockOnFlag)
			{
				//Playerの向いている方向の鏡合わせになるように角度を変更する
				MovePlayerAngle(playerAngle);
			}
		}

		//角度リセット(デバック用)
		if (m_controler.wPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
			m_cameraAngle = XMConvertToRadians(270.0f);
		}


		//ロックオンするときの処理
		if (m_lockOnUse)
		{
			//ロックオン使用時に誰をロックオンするのか処理
			//ターゲット対象からスティックを傾けている方向のターゲット候補に変わる
			if (contrloerVec.x <= -0.9f && m_stickFlag)//ターゲットの左隣にいる候補に移す
			{
				//外角の差がマイナスで一番近いターゲット候補がターゲット対象となる
				auto targetAngle = m_targesDeta[m_lockOnNum].lockOnAngle;
				float targetdeg = XMConvertToDegrees(targetAngle);
				float min = 999999;
				for (int i = 0; i <= m_targesDeta.size()-1; i++)
				{		
					//今見ているターゲット候補が左側にいるわけでなければreturn
					if (m_targesDeta[i].leftOrRight != Left) continue;
					m_targets;
					float targetsDeg = XMConvertToDegrees(m_targesDeta[i].lockOnAngle);
					float difference = abs(m_targesDeta[i].lockOnAngle - targetAngle);

					if (min > difference && difference != 0.0f)
					{
						min = difference;
						m_targetObj->RemoveTag(L"ロックオン対象");
						m_lockOnNum = i;
					}
				}

				m_stickFlag = false;//スティックを受け取れないようにする
			}
			if (contrloerVec.x >= 0.9f && m_stickFlag)//ターゲットの右隣にいる候補に移す
			{
				//外角の差がプラスで一番近いターゲット候補がターゲット対象となる
				auto targetAngle = m_targesDeta[m_lockOnNum].lockOnAngle;
				float targetdeg = XMConvertToDegrees(targetAngle);
				float min = 999999;
				for (int i = 0; i <= m_targesDeta.size() - 1; i++)
				{
					//今見ているターゲット候補が左側にいるわけでなければreturn
					if (m_targesDeta[i].leftOrRight != Right) continue;

					float targetsDeg = XMConvertToDegrees(m_targesDeta[i].lockOnAngle);
					float difference = abs(m_targesDeta[i].lockOnAngle-targetAngle);

					if (min > difference && difference != 0.0f)
					{
						min = difference;
						m_targetObj->RemoveTag(L"ロックオン対象");
						m_lockOnNum = i;
					}
				}

				m_stickFlag = false;//スティックを受け取れないようにする
			}

			//スティックを傾けた後スティックを元に戻したら入力を受け入れる
			if (!m_stickFlag && contrloerVec.x == 0.0f)
			{
				m_stickFlag = true;
			}

			//ロックオンしていいか判断する
			if (m_lockOnNum >= 0)
			{
				m_lockOnUse = true;//ロックオン使用
				m_targetObj = m_targets[m_lockOnNum];
				m_targets[m_lockOnNum]->AddTag(L"ロックオン対象");
			}

			//カメラのPos
			auto cameraPos = m_lockStageCamera->GetEye();

			//ロックオンを切り替えるときにスティックを傾けてロックオン対象を変更する処理
			Vec3 targetPos = m_targetObj->GetComponent<Transform>()->GetPosition();
			//Playerから見てロックオン対象のいる方向ベクトルを求める
			Vec3 lockOnVec = targetPos - playerPos;
			
			m_targesDeta.clear();//初期化
			//その前に前提条件としてロックオン候補が見ている方向から右か左か確認する
			for (auto enemy : m_targets)
			{
				Vec3 enemyPos = enemy->GetComponent<Transform>()->GetPosition();

				//Playerから見てロックオン候補のいる方向ベクトルを求める
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
				float aSqrt = sqrt((lockOnVec.x*lockOnVec.x)+(lockOnVec.z*lockOnVec.z));
				float bSqrt = sqrt((lockOnCanVec.x*lockOnCanVec.x)+(lockOnCanVec.z*lockOnCanVec.z));
				//割られる数値
				float divide = (lockOnVec.x * lockOnCanVec.x) + (lockOnVec.z * lockOnCanVec.z);

				auto rad = divide / (aSqrt * bSqrt);
				auto deg = XMConvertToDegrees(rad);

				targetsDeta targetDeta = targetsDeta(deg, leftOrRight);
				//プレイヤーから見て敵のいる角度を配列に入れる
				m_targesDeta.push_back(targetDeta);
			}

			LockOn(m_targetObj, player);//ロックオン
		}//ロックオンできない時
		else if (!m_lockOnUse)
		{
			//注視点はPlayerの位置よりも少し先にしたい
			m_lockStageCamera->SetAt(playerPos);
		}






		//カメラをPlayerに追従
		m_lockStageCamera->SetEye(Vec3(playerPos.x + (cos(m_cameraAngle) * m_range), playerPos.y + 10.0f+10.0f, playerPos.z + (sin(m_cameraAngle) * m_range)));

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

			//if (m_lockOnNum >= 0)
			//{		
			//	auto targetAngle = m_lockOnAngle[m_lockOnNum];
			//	float a = targetAngle;
	
			//	wss << L"ロックオン角度 " << XMConvertToDegrees(targetAngle);
			//}

		scene->SetDebugString(wss.str());

	}

	//ロックオン候補を決める関数
	void CameraManager::LockOnCandidate(vector<shared_ptr<Enemy>> enemyVec,Vec3 playerPos)
	{
		m_targets.clear();//配列の初期化
		m_lockOnFlag = false;//フラグの初期化

		for (auto enemy : enemyVec)
		{
			Vec3 enemyPos = enemy->GetComponent<Transform>()->GetPosition();

			//ロックオン範囲の円の中にいるか計算する
			float targetRange = (enemyPos.x - playerPos.x) * (enemyPos.x - playerPos.x) +
				(enemyPos.z - playerPos.z) * (enemyPos.z - playerPos.z);
			float radiusRange = m_targetRange * m_targetRange;

			//ロックオン候補なら配列にオブジェクトとPosを入れる
			if (targetRange <= radiusRange)
			{
				if (!enemy->FindTag(L"ロックオン候補"))
				{
					m_stage->AddGameObject<LockOnLook>(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), enemy, Vec3(0.0f, 4.0f, 0.0f));
				}

				m_targetsPos.push_back(enemyPos);//ロックオン候補のPosを配列に入れる
				m_targets.push_back(enemy);
				enemy->AddTag(L"ロックオン候補");//ロックオン候補のタグ追加
				m_lockOnFlag = true;//ロックオン使用可能
			}
			else if (enemy->FindTag(L"ロックオン候補"))//ロックオン候補から外れたのがロックオン対象ならロックオンをやめる
			{
				enemy->RemoveTag(L"ロックオン候補");
				if (enemy->FindTag(L"ロックオン対象"))
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

		//ロックオン対象をPlayerの距離ベクトルを測って角度取得
		Vec3 lockOnVec = lockOnPos - originPos;
		auto m_targetRad = atan2(lockOnVec.z, lockOnVec.x);

		//計算した角度を入れてカメラを旋回させる
		m_cameraAngle = m_targetRad + XMConvertToRadians(180.0f);
		m_lockStageCamera->SetAt(lockOnPos);
	}

	//ロックオンの解除機能
	void CameraManager::LockOff(vector<shared_ptr<Enemy>> enemyVec)
	{
		for (auto enemy : enemyVec)
		{
			enemy->RemoveTag(L"ロックオン対象");
		}
		m_lockOnFlag = false;//ロックオンできない
		m_lockOnUse = false;//ロックオンしない
		m_lockOnNum = -1;
		m_targetObj = NULL;
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

		////影を付ける
		//auto m_ptrShadow = AddComponent<Shadowmap>();
		//m_ptrShadow->SetMeshResource(L"DEFAULT_CUBE");
		////m_ptrShadow->SetMeshToTransformMatrix(spanMat);


		//ptrDraw->HitTestSkinedMeshSegmentTriangles();

		//ptrDraw->HitT
	}



}
//end basecross
