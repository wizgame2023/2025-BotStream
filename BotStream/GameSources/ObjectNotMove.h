/*!
@file ObjectNotMove.h
@brief 動かないオブジェクト
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	class ObjectNotMove : public MyGameObject
	{
	private:
		bool m_pose;//ポーズフラグ
	public:
		ObjectNotMove(const shared_ptr<Stage>& stagePtr);
		~ObjectNotMove();
	protected:
		//void OnCreate()override;
		//void OnUpdate()override;
	};

	class DarkObj :public ObjectNotMove
	{
	private:
		bool m_pose;//ポーズフラグ

		Vec3 m_pos;//位置
		Vec3 m_rot;//回転
		Vec3 m_scale;//大きさ
		Col4 m_color;//色

		// コリジョン判定の有効化
		bool m_activateCollsion = false; 

	public:
		DarkObj(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, Col4 color = Col4(1.0f, 1.0f, 1.0f, 1.0f)) :
			ObjectNotMove(stagePtr),
			m_pos(pos),
			m_rot(rot),
			m_scale(scale),
			m_color(color)
		{

		}
		~DarkObj()
		{

		}

		void OnCreate()override;
		void OnUpdate()override;

		//m _activateCollsionのセッタ
		void SetActivateCollsion(bool onOff)
		{
			m_activateCollsion = onOff;
		}


		void OnCollisionEnter(shared_ptr<GameObject>& Other)override;

	protected:

	};

}
//end basecross