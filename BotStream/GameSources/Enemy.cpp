/*!
@file Enemy.cpp
@brief 敵の処理
*/

#include "stdafx.h"
#include "Project.h"
#include "Actor.h"
#include "Enemy.h"

namespace basecross {
	Enemy::Enemy(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale) :
		Actor(stagePtr, pos, rot, scale)
	{

	}
	Enemy::~Enemy()
	{

	}

	void Enemy::OnCreate() {
		Actor::OnCreate();
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
		auto ptrDraw = GetComponent<PNTBoneModelDraw>();
		ptrDraw->SetMultiMeshResource(L"Spearmen");//仮のメッシュ
		ptrDraw->AddAnimation(L"Idle", 0, 1, true, 60.0f);//歩き状態
		ptrDraw->AddAnimation(L"Walk", 0, 100, true, 60.0f);//歩き状態
		ptrDraw->SetDiffuse(Col4(0));
		ptrDraw->SetSamplerState(SamplerState::LinearWrap);
		ptrDraw->SetMeshToTransformMatrix(spanMat);
		ptrDraw->SetTextureResource(L"SpearmenTexture");

		//コリジョン作成
		auto ptrColl = AddComponent<CollisionSphere>();//コリジョンスフィアの方が壁にぶつかる判定に違和感がない
		ptrColl->SetAfterCollision(AfterCollision::Auto);

		AddTag(L"Player");//Player用のタグ

		AddTag(L"Enemy");
	}

	void Enemy::OnUpdate() {
		//m_used=falseなら表示を消してUpdateをreturn
		if (GetDrawActive() != m_used) {
			SetDrawActive(m_used);
		}
		if (m_used == false) {
			return;
		}

		//なんやかんや


	}

}
//end basecross
