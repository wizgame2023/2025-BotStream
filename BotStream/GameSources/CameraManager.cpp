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
		m_pauseFlag(false)
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

		Vec3 CameraPos = m_lockStageCamera->GetEye();
				
		//SE用のマネージャー取得
		m_SEManager = App::GetApp()->GetXAudio2Manager();
		//射撃用のクロスヘア用のテクスチャ追加
		m_spriteAiming = m_stage->AddGameObject<Sprite>(L"AimingTex", Vec2(50.0f, 50.0f));

		//カメラ用ステートマシンの作成まだステートは作成途中
		m_stateMashine = unique_ptr<CameraStateMachine>(new CameraStateMachine(GetThis<GameObject>()));

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
		// 今選択しているウィンドウハンドルを取得
		auto ActiveWindow = GetActiveWindow();
		// このゲームのウィンドウハンドルを取得
		auto MyWindowHandle = App::GetApp()->GetHWnd();

		m_delta = App::GetApp()->GetElapsedTime();
		m_lockStageCamera = m_stageCamera.lock();
		
		//もしステージ用のカメラを取得できなかったらreturnして自分を削除します
		if (!m_lockStageCamera)
		{
			GetStage()->RemoveGameObject<CameraManager>(GetThis<CameraManager>());
			return;
		}


		//プレイヤーを取得
		auto player = m_stage->GetSharedGameObject<Player>(L"Player");
		m_playerPos = player->GetComponent<Transform>()->GetPosition();

		// インプットデバイスオブジェクト
		InputDevice inputDevice = App::GetApp()->GetInputDevice(); // 様々な入力デバイスを管理しているオブジェクトを取得
		// コントローラーの取得
		m_controler = inputDevice.GetControlerVec()[0];
		m_contrloerVec = Vec2(m_controler.fThumbRX, m_controler.fThumbRY);
		// キーマウの取得
		KEYBOARD_STATE keyState = App::GetApp()->GetInputDevice().GetKeyState();
		
		//マウスの移動ベクトル取得
		MouseCameraMove();
		//ステートマシン更新(ステート更新)
		m_stateMashine->Update(m_delta);

		// Y軸の角度の調整0~360度までしか出ないようにする
		m_cameraAngleY = AdjustmentAngle(m_cameraAngleY);

	}

	// カメラのX軸回転の制限
	// 第一引数：上限角度(ラジアン) 第二引数：下限角度(ラジアン)
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
	// 第一引数：カメラ移動時のスピード 第二引数：カメラ減速時のスピード
	void CameraManager::InertialRotation(float moveSpeed,float decelerationSpeed)
	{
		//コントローラーが接続されているときはコントローラーのみで入力
		if (m_controler.bConnected)
		{
			//Y軸回転
			if (m_contrloerVec.x != 0.0f)
			{
				//左スティックをX方面に傾けてカメラがPlayerのY軸方向に回転する処理
				m_addAngleYAxis = (m_speedYAxis * m_contrloerVec.x) * moveSpeed;
			}
			//コントローラーを傾けていなければだんだん移動スピードがなくなる
			else if (m_contrloerVec.x == 0.0f)
			{
				if (m_addAngleYAxis > 0)
				{
					m_addAngleYAxis -= decelerationSpeed * m_delta;
					if (m_addAngleYAxis <= 0)
					{
						m_addAngleYAxis = 0.0f;
					}
				}
				else if (m_addAngleYAxis < 0)
				{
					m_addAngleYAxis += decelerationSpeed * m_delta;
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
				m_addAngleXAxis = (m_speedXAxis * m_contrloerVec.y) * moveSpeed;
			}
			//コントローラーを傾けていなければだんだん移動スピードがなくなる
			else if (m_contrloerVec.y == 0.0f)
			{
				if (m_addAngleXAxis > 0)
				{
					m_addAngleXAxis -= decelerationSpeed / 2 * m_delta;
					if (m_addAngleXAxis <= 0)
					{
						m_addAngleXAxis = 0.0f;
					}
				}
				else if (m_addAngleXAxis < 0)
				{
					m_addAngleXAxis += decelerationSpeed / 2 * m_delta;
					if (m_addAngleXAxis >= 0)
					{
						m_addAngleXAxis = 0.0f;
					}
				}
			}
		}
		else
		{
			//コントローラーが接続されていないときはキーマウで操作できるようにする

			// 今選択しているウィンドウハンドルを取得
			auto ActiveWindow = GetActiveWindow();
			// このゲームのウィンドウハンドルを取得
			auto MyWindowHandle = App::GetApp()->GetHWnd();

			// 選択しているウィンドウがこのゲームの時はマウスポインタ固定する,そうでない場合は解除
			if (ActiveWindow != MyWindowHandle) return;
			// ポーズしているときはカメラ回転処理はしない
			if (m_pauseFlag) return;


			auto speedMouseYAixs = (m_speedYAxis * 0.05f);
			//Y軸回転
			if (m_mouseMoveVec.x != 0.0f)
			{
				//マウスをX方面移動してカメラがPlayerのY軸方向に回転する処理
				m_addAngleYAxis = ((speedMouseYAixs * m_mouseMoveVec.x) * moveSpeed);
			}
			//コントローラーを傾けていなければだんだん移動スピードがなくなる
			else if (m_mouseMoveVec.x == 0.0f)
			{
				if (m_addAngleYAxis > 0)
				{
					m_addAngleYAxis -= decelerationSpeed * m_delta;
					if (m_addAngleYAxis <= 0)
					{
						m_addAngleYAxis = 0.0f;
					}
				}
				else if (m_addAngleYAxis < 0)
				{
					m_addAngleYAxis += decelerationSpeed * m_delta;
					if (m_addAngleYAxis >= 0)
					{
						m_addAngleYAxis = 0.0f;
					}
				}
			}

			auto addMouseAngleXAxis = (m_speedXAxis * 0.05f);
			//X軸回転
			if (m_mouseMoveVec.y != 0.0f)
			{
				//左スティックをY方面に傾けてカメラがPlayerのX軸方向に回転する処理
				m_addAngleXAxis = -(addMouseAngleXAxis * m_mouseMoveVec.y) * moveSpeed;
			}
			//コントローラーを傾けていなければだんだん移動スピードがなくなる
			else if (m_mouseMoveVec.y <= 3.0f)
			{
				if (m_addAngleXAxis > 0)
				{
					m_addAngleXAxis -= decelerationSpeed / 2 * m_delta;
					if (m_addAngleXAxis <= 0)
					{
						m_addAngleXAxis = 0.0f;
					}
				}
				else if (m_addAngleXAxis < 0)
				{
					m_addAngleXAxis += decelerationSpeed / 2 * m_delta;
					if (m_addAngleXAxis >= 0)
					{
						m_addAngleXAxis = 0.0f;
					}
				}
			}
		}
		
		m_cameraAngleX += m_addAngleXAxis * m_delta;//追加
		m_cameraAngleY += -m_addAngleYAxis * m_delta;//追加
	}

	//通常モード時のカメラ操作処理
	void CameraManager::CameraControlNomalMode()
	{
		auto pushMaxAtPos = -5.0f;
		//移動処理
		m_pushAtPos.y = MoveToDestination(m_pushAtPos.y, pushMaxAtPos, 120.0f);
		m_pushAtPos.z = MoveToDestination(m_pushAtPos.z, pushMaxAtPos, 120.0f);
		m_pushAtPos.x = MoveToDestination(m_pushAtPos.x, pushMaxAtPos, 120.0f);

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
		auto pushMaxAtPos = -15.0f;
		auto pushMaxAtPosY = -15.0f;
		//移動処理
		m_pushAtPos.x = MoveToDestination(m_pushAtPos.x, pushMaxAtPos, 16.0f*5);
		m_pushAtPos.y = MoveToDestination(m_pushAtPos.y, pushMaxAtPosY, 16.0f*5);
		m_pushAtPos.z = MoveToDestination(m_pushAtPos.z, pushMaxAtPos, 16.0f*5);

		//ここはUIを出す
		m_spriteAiming->OnClear(false);

		//注視点の変更(普段よりも先に見たい)
		m_lockStageCamera->SetAt(m_playerPos + Vec3(cosf(m_cameraAngleY) * sin(m_cameraAngleX) * m_pushAtPos.x,
			cos(m_cameraAngleX) * m_pushAtPos.y,
			sinf(m_cameraAngleY) * sin(m_cameraAngleX) * m_pushAtPos.z));
	}

	//プレイヤーの方向に回転するカメラ操作処理
	void CameraManager::CameraControlTransitionMode(bool firstFlag)
	{
		CameraControlNomalMode();

		if (firstFlag)
		{
			//プレイヤーを取得
			auto player = m_stage->GetSharedGameObject<Player>(L"Player");
			m_resetPlayerAngle = player->GetAngle();
			m_resetPlayerAngle = AdjustmentAngle(m_resetPlayerAngle);
		}

		//Playerの向いている方向に移動するフラグをオンにする
		m_movePlayerAngleFlag = true;
		//向く座標を決める
		m_targetAngleY = -m_resetPlayerAngle + XMConvertToRadians(180.0f);
		m_targetAngleY = AdjustmentAngle(m_targetAngleY);

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

	//開始ムービー用のカメラ操作処理
	void CameraManager::CameraControlStartMovieMode()
	{

		auto pushMaxAtPos = -5.0f;
		float speed = 1.0f;
		//移動処理
		m_pushAtPos.z = MoveToDestination(m_pushAtPos.z, pushMaxAtPos, speed);
		m_pushAtPos.y = MoveToDestination(m_pushAtPos.y, pushMaxAtPos, speed);
		m_pushAtPos.x = MoveToDestination(m_pushAtPos.x, pushMaxAtPos, speed);

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

	// マウスでの移動ベクトル取得処理
	void CameraManager::MouseCameraMove()
	{

		// 今選択しているウィンドウハンドルを取得
		auto ActiveWindow = GetActiveWindow();
		// このゲームのウィンドウハンドルを取得
		auto MyWindowHandle = App::GetApp()->GetHWnd();

		//マウス位置更新
		if (ActiveWindow != MyWindowHandle)
		{
			GetCursorPos(&m_mouseBeforPos);
			return;
		}

		// マウスの位置取得
		GetCursorPos(&m_mouseCurrentPos);

		// キーマウの取得
		KEYBOARD_STATE keyState = App::GetApp()->GetInputDevice().GetKeyState();

		// マウスの位置取得
		GetCursorPos(&m_mouseCurrentPos);

		// マウスの移動ベクトル計算
		m_mouseMoveVec = Vec2(m_mouseCurrentPos.x - m_mouseBeforPos.x,
			m_mouseCurrentPos.y - m_mouseBeforPos.y);

		//カーソルが端まで行ったらループさせる
		int maxPosX = 1500;	int minPosX = 0;
		int maxPosY = 850;	int minPosY = 0;

		//// 移動ループした後は移動量が大きくなるので調整  マウスポインタを固定しているのでこの処理使わなくて問題ない
		// だけど必要になる可能性はあるので残しておきます。
		//if (m_cursorFlagX)
		//{
		//	// 右端から左端へ
		//	if (m_mouseMoveVec.x <= -1)
		//	{
		//		m_mouseMoveVec.x = (m_mouseCurrentPos.x + maxPosX) - m_mouseBeforPos.x;
		//	}
		//	// 左端から右端へ
		//	else if (m_mouseMoveVec.x >= +1)
		//	{
		//		m_mouseMoveVec.x = m_mouseCurrentPos.x - (m_mouseBeforPos.x + maxPosX);
		//	}

		//	m_cursorFlagX = false;
		//}

		//if (m_cursorFlagY)
		//{
		//	//上端から下端へ
		//	if (m_mouseMoveVec.y <= -1)
		//	{
		//		m_mouseMoveVec.y = (m_mouseCurrentPos.y + maxPosY) - m_mouseBeforPos.y;
		//	}
		//	//下端から上端へ
		//	else if (m_mouseMoveVec.y >= 1)
		//	{
		//		m_mouseMoveVec.y = m_mouseCurrentPos.y - (m_mouseBeforPos.y + maxPosY);
		//	}

		//	m_cursorFlagY = false;
		//}

		//// 今選択しているウィンドウハンドルを取得
		//auto ActiveWindow = GetActiveWindow();
		//// このゲームのウィンドウハンドルを取得
		//auto MyWindowHandle = App::GetApp()->GetHWnd();

		SetCursorPos(750, 420);
		GetCursorPos(&m_mouseBeforPos);

		// X座標
		if (m_mouseCurrentPos.x >= maxPosX)
		{
			m_mouseCurrentPos.x = minPosX + 1;
			SetCursorPos(minPosX + 1, m_mouseCurrentPos.y);
			m_cursorFlagX = true;
		}
		if (m_mouseCurrentPos.x <= minPosX)
		{
			m_mouseCurrentPos.x = minPosX - 1;
			SetCursorPos(maxPosX - 1, m_mouseCurrentPos.y);
			m_cursorFlagX = true;
		}
		// Y座標
		if (m_mouseCurrentPos.y >= maxPosY)
		{
			m_mouseCurrentPos.y = minPosY + 1;
			SetCursorPos(m_mouseCurrentPos.x, minPosY + 1);
			m_cursorFlagY = true;
		}
		if (m_mouseCurrentPos.y <= minPosY)
		{
			m_mouseCurrentPos.y = minPosY - 1;
			SetCursorPos(m_mouseCurrentPos.x, maxPosY - 1);
			m_cursorFlagY = true;
		}
	}
	

	// 現在地から目的地までの移動処理
	// 第一引数：現在地　第二引数：目的地　第三引数：移動スピード
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

	// ステート変更処理 引数に入れたステートに変更する
	// 第一引数：変更したいステート名
	void CameraManager::ChangeState(wstring stateName)
	{
		m_stateMashine->ChangeState(stateName);
	}

	// カメラのポジションを決める関数
	// 第一引数：Y座標をどの位ずらすか 第二引数：銃モードでのプレイヤーから離れる追加距離 
	// 第三引数：通常時のプレイヤーから離れる距離 第四引数：カメラの移動モード
	bool CameraManager::CameraPosUpdate(float maxPushPosY,float maxLength,float CameraLenght,float cameraSpeed,int moveMode)
	{
		auto objVec = m_stage->GetGameObjectVec();
		m_cameraPos = m_lockStageCamera->GetEye();


		Vec3 hitPos;			// 出力用：レイの交差地点(衝突点)
		TRIANGLE triangle;		// レイが交差したポリゴンを構成する頂点の座標
		size_t triangleNumber;	// レイが交差したポリゴンの番号
		float min = 9999999.9f;	//Playerから見てカメラの障害となる距離の最小値
		bool moveEnd = false;	//移動処理が終わったかを保存する変数

		// プレイヤーからどのくらい離れるのかのベクトルの計算
		Vec3 CameraPushGoalPos = Vec3((cos(m_cameraAngleY) * sin(m_cameraAngleX) * CameraLenght),
			(maxPushPosY) + cos(m_cameraAngleX) * CameraLenght,
			(sin(m_cameraAngleY) * sin(m_cameraAngleX) * CameraLenght));
		// 銃モード用の離れる距離をプラスする
		CameraPushGoalPos += Vec3(cos(m_cameraAngleY + XMConvertToRadians(45.0f)) * maxLength,
			0.0f,
			sin(m_cameraAngleY + XMConvertToRadians(45.0f)) * maxLength);

		// 現在の位置と目的地の方向ベクトルの計算
		Vec3 directionVec = CameraPushGoalPos - m_pushPos;	

		// 通常移動モード
		if (moveMode == NormalMove)
		{
			//ある程度カメラの位置が目的地に近かったら目的地にたどり着いたとみなす
			if (directionVec.length() <= 1.0f)
			{
				m_pushPos = CameraPushGoalPos;
				moveEnd = true; // 移動終了
			}
			// ステージ開始時にカメラの移動が始まらないようにする例外処理
			if (m_pushStart)
			{
				m_pushPos = CameraPushGoalPos;
				m_pushStart = false;
			}

			// 正規化
			directionVec = directionVec.normalize();

			// カメラの位置と目的地が一緒でなければ移動する
			if (m_pushPos != CameraPushGoalPos)
			{
				//auto cameraSpeed = 120.0f;
				m_pushPos += directionVec * cameraSpeed * m_delta;
			}
		}
		else if (DirectMove)
		{
			m_pushPos = CameraPushGoalPos;
		}

		m_cameraPos = m_playerPos + m_pushPos;

		// 障害物になりえるオブジェクト達にカメラの機能を邪魔していないか見る
		for (auto obj : objVec)
		{
			auto obstacles = dynamic_pointer_cast<GameObject>(obj);// 当たり判定の対象
			float hitLength = min;// Playerと障害物の距離の長さ

			// 障害物になりえそうならカメラの表示に邪魔をしていないか確認をする
			if (obstacles)
			{
				// カメラの障害になりえるオブジェクトしかカメラを邪魔をしているか評価しない
				if (!obstacles->FindTag(L"CameraObstacles")) continue;

				auto ptrDraw = obstacles->GetComponent<SmBaseDraw>();
				ptrDraw->HitTestStaticMeshSegmentTriangles(m_playerPos, m_cameraPos, hitPos, triangle, triangleNumber);
				Vec3 playerorObstaclesVec = hitPos - m_playerPos;
				hitLength = abs(playerorObstaclesVec.x) + abs(playerorObstaclesVec.y) + abs(playerorObstaclesVec.z);
			}

			// minよりhitLengthが短かったら位置更新する
			if (hitPos != Vec3(0.0f, 0.0f, 0.0f) && min > hitLength)
			{
				min = hitLength;
				//hitPos.y = m_cameraPos.y;//Y座標は変えないようにする
				m_cameraPos = hitPos;
			}
		}

		// カメラの位置更新
		m_lockStageCamera->SetEye(m_cameraPos);
		
		// 移動処理が終わったか伝える
		return moveEnd;
	}

	// Playerの向いている角度の鏡合わせになるように角度を変更する
	// 第一引数：プレイヤーの向いている角度
	void CameraManager::MovePlayerAngle(float playerAngle)
	{
		bool Movechage = false;
		Movechage = MoveAngle(playerAngle, 1);//角度を移動させる

		//移動が終わったのでフラグをfalseにする
		if (Movechage) m_movePlayerAngleFlag = false;
	}

	// 注視点の移動処理
	// 第一引数：目標の位置
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
	//第一引数：向きたい目標角度 第二引数：どの軸で移動するか(Xが0Yが1) 戻り値 移動処理が終わったか
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

		// 移動したい角度が同じなら移動はしない
		if (angleDifference == 0)
		{
			return true;
		}

		//移動完了してないことを知らせる
		return false;
	}

	// 角度の調整0~360度までしか出ないようにする
	// 第一引数：現在角度
	float CameraManager::AdjustmentAngle(float angle)
	{
		if (angle >= XMConvertToRadians(360.0f))
		{
			angle -= XMConvertToRadians(360.0f);
		}
		else if (angle < XMConvertToRadians(0.0f))
		{
			angle += XMConvertToRadians(360.0f);
		}

		return angle;
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
		m_pauseFlag = onOff;
	}

	//近距離攻撃をするかの処理のゲッタ
	bool CameraManager::GetMeleeFlag()
	{
		return m_meleeFlag;
	}

	//近距離攻撃をするかの処理のセッタ
	void CameraManager::SetMeleeFlag(bool onOff)
	{
		m_meleeFlag = onOff;
	}

	//ロックオン範囲を知らせるためのオブジェクト 現在はロックオン機能は実装から省いています
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
		DefAttack(.5f, tmp);
		GetAttackPtr()->SetPos(Vec3(0, 0, 10));
		auto AttackPtr = GetAttackPtr();
		AttackPtr->SetCollScale(1.0f);
		
		DefAttack(999.0f, tmp);

	}

	void EnemyCube::OnUpdate()
	{
		Actor::OnUpdate();

	}


}
//end basecross
