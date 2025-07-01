/*!
@file LandDetect.h
@brief �n�ʐڐG
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross{
	class LandDetect : public ObjectMove
	{
		shared_ptr<CollisionSphere> m_collision;
		//�󒆔���ɂȂ�܂ł̃J�E���g
		//float m_CollideCount = 0;
		int m_CollideCount = 0;
		//float m_CollideCountMax = .05f;
		int m_CollideCountMax = 5;
		Vec3 m_bindPos = Vec3(0, -.5f, 0);
		float m_defaultRadius = .1f;
		
		//�n��ɂ��邩�ǂ���
		bool m_isLand = false;
	public:
		LandDetect(const shared_ptr<Stage>& stagePtr);
		LandDetect(const shared_ptr<Stage>& stagePtr, float collidecnt);
		~LandDetect();

		bool GetLand();
		void SetBindPos(Vec3 pos);
		void SetCollScale(float scale);

		void OnCreate() override;
		void OnUpdate() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
		virtual void OnCollisionExcute(shared_ptr<GameObject>& Other) override;
		virtual void OnCollisionExit(shared_ptr<GameObject>& Other) override;
	};

}
//end basecross

