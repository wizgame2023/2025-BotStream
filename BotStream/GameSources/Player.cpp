/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	Player::Player(const shared_ptr<Stage>& stagePtr,Vec3 pos,Vec3 rot, Vec3 scale) :
		Actor(stagePtr),
		m_pos(pos),
		m_rot(rot),
		m_scale(scale)
	{

	}
	Player::~Player()
	{

	}

	//作成
	void Player::OnCreate()
	{
		//Transform設定
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//ドローメッシュの設定
		auto ptrDraw = AddComponent<PNTBoneModelDraw>();
		ptrDraw->SetMultiMeshResource(L"Spearmen");//仮のメッシュ
		ptrDraw->AddAnimation(L"Walk", 0, 100, true, 60.0f);//歩き状態
		ptrDraw->SetSamplerState(SamplerState::LinearWrap);
		ptrDraw->SetMeshToTransformMatrix(spanMat);
		ptrDraw->SetTextureResource(L"SpearmenTexture");

		//コリジョン作成
		auto ptrColl = AddComponent<CollisionSphere>();//コリジョンスフィアの方が壁にぶつかる判定に違和感がない
		ptrColl->SetAfterCollision(AfterCollision::Auto);

		AddTag(L"Player");//Player用のタグ
	}

	void Player::OnUpdate()
	{
		float delta = App::GetApp()->GetElapsedTime();
		
		//動く処理(仮)
		PlayerMove();

		//デバック用文字列
		//DebagLog();
	}

	void Player::PlayerMove()
	{
		auto Delta = App::GetApp()->GetElapsedTime();
		float m_speed = 3.0f;//仮のスピード

		// インプットデバイスオブジェクト
		auto inputDevice = App::GetApp()->GetInputDevice(); // 様々な入力デバイスを管理しているオブジェクトを取得
		//コントローラーの取得
		auto m_controler = inputDevice.GetControlerVec()[0];
		Vec2 contrloerVec = Vec2(m_controler.fThumbLX, m_controler.fThumbLY);


		//左ステックの向きにプレイヤーが進む
		if (m_controler.bConnected)
		{
			m_pos.z += (m_controler.fThumbLY * m_speed * Delta) * 2;
			m_pos.x += (m_controler.fThumbLX * m_speed * Delta) * 2;

			m_trans->SetPosition(m_pos);//ポジション更新
		}

		//ステックを傾けるとアニメーションが再生される
		if (!m_controler.fThumbLX == 0 || !m_controler.fThumbLY == 0)
		{
			auto Name = GetComponent<PNTBoneModelDraw>()->GetCurrentAnimation();
			if (Name != L"Walk")
			{
				//アニメーション変更
				GetComponent<PNTBoneModelDraw>()->ChangeCurrentAnimation(L"Walk");
			}

			//アニメーション再生
			GetComponent<PNTBoneModelDraw>()->UpdateAnimation(Delta*5);
		}


		//左ステックの向きにプレイヤーも向く
		if (m_controler.bConnected)
		{
			//ゲームパットの傾きが無ければ回転度は更新しない
			if (m_controler.fThumbLY != 0.0f && m_controler.fThumbLX != 0.0f)
			{		
				//スティックの傾きをラジアンにする
				m_angle = -atan2(m_controler.fThumbLY, m_controler.fThumbLX);
				m_rot.y = m_angle;

				m_trans->SetRotation(m_rot);
				float playerRad = atan2f(contrloerVec.y, contrloerVec.x);//角度を取得
				float playerdeg = XMConvertToDegrees(playerRad);//角度をでぃ
			}
		}

	}

	//Playerの向いている方向のゲッター
	float Player::GetAngle()
	{
		return m_angle;
	}

	//Playerの向いている方向のセッター
	void Player::SetAngle(float angle)
	{
		m_angle = angle;
	}

	//デバック用文字列表示関数
	void Player::DebagLog()
	{
		// インプットデバイスオブジェクト
		auto inputDevice = App::GetApp()->GetInputDevice(); // 様々な入力デバイスを管理しているオブジェクトを取得

		////デバック用
		wstringstream wss(L"");
		auto scene = App::GetApp()->GetScene<Scene>();

		wss /* << L"デバッグ用文字列 "*/
			<< L"\nPos.x " << m_pos.x << "\nPos.z " << m_pos.z
			<< endl;

		scene->SetDebugString(wss.str());
	}
}
//end basecross

