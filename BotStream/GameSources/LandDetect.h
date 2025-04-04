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
		float m_CollideCount = 0;
		float m_CollideCountMax = .05f;
		//�n��ɂ��邩�ǂ���
		bool m_isLand = false;
	public:
		LandDetect(const shared_ptr<Stage>& stagePtr);
		LandDetect(const shared_ptr<Stage>& stagePtr, float collidecnt);
		~LandDetect();

		//�n�ʂɐڒn���Ă����true��Ԃ�
		bool GetLand() {
			return m_isLand;
		}

		void OnCreate() override;
		void OnUpdate() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
		virtual void OnCollisionExcute(shared_ptr<GameObject>& Other) override;
		virtual void OnCollisionExit(shared_ptr<GameObject>& Other) override;
	};

}
//end basecross

