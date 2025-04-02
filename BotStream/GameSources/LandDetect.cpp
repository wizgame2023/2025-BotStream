/*!
@file LandDetect.cpp
@brief �n�ʐڐG���菈��
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	LandDetect::LandDetect(const shared_ptr<Stage>& stagePtr) :
		ObjectMove(stagePtr)
	{

	}
	LandDetect::LandDetect(const shared_ptr<Stage>& stagePtr, float collidecnt) :
		ObjectMove(stagePtr)
	{
		m_CollideCountMax = collidecnt;
	}
	LandDetect::~LandDetect()
	{

	}

	void LandDetect::OnCreate() {
		m_collision = AddComponent<CollisionSphere>();
		//�����������Ȃ�����ɂ���
		m_collision->SetAfterCollision(AfterCollision::None);
		m_collision->SetMakedRadius(.25f);
		m_collision->SetExcludeCollisionGroup(L"Actor");
		GetComponent<Transform>()->SetPosition(Vec3(0, -.3f, 0));

		//�f�o�b�O
		m_collision->SetDrawActive(true);
	}

	void LandDetect::OnUpdate() {
		if (m_isLand == true) {
			m_CollideCount -= App::GetApp()->GetElapsedTime();
			if (m_CollideCount <= 0) {
				m_isLand = false;
			}
		} 
	}

	void LandDetect::OnCollisionEnter(shared_ptr<GameObject>& Other) {

	}
	
	void LandDetect::OnCollisionExcute(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"Floor")) {
			//�J�E���g�����Z�b�g����
			m_isLand = true;
			m_CollideCount = m_CollideCountMax;
		}
	}

	void LandDetect::OnCollisionExit(shared_ptr<GameObject>& Other) {


	}

}
//end basecross
