/*!
@file Enemy.h
@brief 敵など
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
#include "Actor.h"
#include "EnemyState.h"

namespace basecross{
	class EnemyStateMachine;
	class EnemyBase : public Actor
	{
	protected:
		bool m_used = false;
		shared_ptr<EnemyStateMachine> m_state;

	public:
		EnemyBase(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale);
		EnemyBase(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, bool use);
		~EnemyBase();

		void OnCreate() override;
		void OnUpdate() override;

		//初期化処理(敵をスポーンさせるとき使う？)
		void Initialize(Vec3 pos, Vec3 rot, Vec3 scale) {
			m_used = true;
			auto ptr = GetComponent<Transform>();
			ptr->SetPosition(pos);
			ptr->SetRotation(rot);
			ptr->SetScale(scale);
			SetDrawActive(true);
			SetUpdateActive(true);
			m_LandDetect->SetUpdateActive(true);
		}

		bool GetUsed() {
			return m_used;
		}
	};

	class BossFirst : public EnemyBase {
		BossFirst(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale) :
			EnemyBase(stagePtr, pos, rot, scale)
		{

		}
		~BossFirst() { }
		
		void RegisterAnim();
		void OnDamaged() override;

	public:
		void OnCreate() override;
		void OnUpdate() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
	};
}
//end basecross

