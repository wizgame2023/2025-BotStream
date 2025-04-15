/*!
@file Actor.cpp
@brief PlayerやEnemyなどのキャラクター用のクラス
*/

#include "stdafx.h"
#include "Project.h"
#include "Actor.h"

namespace basecross {
	Actor::Actor(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale) :
		ObjectMove(stagePtr),
		m_pos(pos),
		m_rot(rot),
		m_scale(scale)
	{

	}
	Actor::~Actor()
	{

	}

	void Actor::OnCreate() {
		auto stage = GetStage();

		//Actorグループに登録する
		auto group = stage->GetSharedObjectGroup(L"Actor");
		if (group) {
			group->IntoGroup(GetThis<GameObject>());
		}

		//描画コンポーネントの追加
		AddComponent<PNTBoneModelDraw>();

		//着地判定の生成、子オブジェクトにする
		m_LandDetect = stage->AddGameObject<LandDetect>();
		m_LandDetect->GetComponent<Transform>()->SetParent(dynamic_pointer_cast<GameObject>(GetThis<Actor>()));
	}

	void Actor::OnUpdate() {
		_delta = App::GetApp()->GetElapsedTime();
	}

	//最高速度
	void Actor::SpeedLimit(float multiply) {
		float limit = m_speedMax * multiply;
		auto angle = m_velocity;
		angle.y = 0;
		if (angle.length() > 0) {
			angle.normalize();
			if (angle.x > 0) {
				if (m_velocity.x > angle.x * limit) m_velocity.x = angle.x * limit;
			}
			else {
				if (m_velocity.x < angle.x * limit) m_velocity.x = angle.x * limit;
			}
			if (angle.z > 0) {
				if (m_velocity.z > angle.z * limit) m_velocity.z = angle.z * limit;
			}
			else {
				if (m_velocity.z < angle.z * limit) m_velocity.z = angle.z * limit;
			}
		}
		//落下の終端速度
		if (m_velocity.y < m_fallTerminal) m_velocity.y = m_fallTerminal;
	}

	//摩擦(地上のみ)
	void Actor::Friction() {
		//静摩擦
		if (m_accel == Vec3(0)) {
			m_velocity.x -= m_velocity.x * m_friction * (1000.0f / 60.0f) * _delta;
			m_velocity.z -= m_velocity.z * m_friction * (1000.0f / 60.0f) * _delta;
			if (m_velocity.length() > m_frictionThreshold) m_velocity.x = 0;
			return;
		}
		//動摩擦
		if (m_accel != Vec3(0)) {
			m_velocity.x -= m_velocity.x * m_frictionDynamic * (1000.0f / 60.0f) * _delta;
			m_velocity.z -= m_velocity.z * m_frictionDynamic * (1000.0f / 60.0f) * _delta;
		}
	}

	//重力
	void Actor::Gravity() {
		m_velocity.y += m_gravity * _delta;
	}

}
//end basecross
