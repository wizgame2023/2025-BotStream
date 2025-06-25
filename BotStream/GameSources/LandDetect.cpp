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
		m_collision->SetMakedRadius(m_defaultRadius);
		m_collision->SetExcludeCollisionGroup(L"Actor");
		GetComponent<Transform>()->SetPosition(m_bindPos);

		//�f�o�b�O
		m_collision->SetDrawActive(false);
	}

	void LandDetect::OnUpdate() {
		//�����J�E���g
		if (m_isLand == true) {
			m_CollideCount -= App::GetApp()->GetElapsedTime();
			if (m_CollideCount <= 0) {
				//���΂炭���肪���Ȃ������痣��
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

	//�n�ʂɐڒn���Ă����true��Ԃ�
	bool LandDetect::GetLand() {
		return m_isLand;
	}

	//�ʒu��ς���
	void LandDetect::SetBindPos(Vec3 pos) {
		GetComponent<Transform>()->SetPosition(pos);
	}

	//����̔��a��ς���
	void LandDetect::SetCollScale(float scale) {
		GetComponent<CollisionSphere>()->SetMakedRadius(scale);
	}

}
//end basecross
