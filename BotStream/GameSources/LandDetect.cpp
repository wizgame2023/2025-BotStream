/*!
@file LandDetect.cpp
@brief 地面接触判定処理
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
		//押し合いしない判定にする
		m_collision->SetAfterCollision(AfterCollision::None);
		m_collision->SetMakedRadius(m_defaultRadius);
		m_collision->SetExcludeCollisionGroup(L"Actor");
		GetComponent<Transform>()->SetPosition(m_bindPos);

		//デバッグ
		m_collision->SetDrawActive(false);
	}

	void LandDetect::OnUpdate() {
		//離陸カウント
		if (m_isLand == true) {
			m_CollideCount -= App::GetApp()->GetElapsedTime();
			if (m_CollideCount <= 0) {
				//しばらく判定が取れなかったら離陸
				m_isLand = false;
			}
		} 
	}

	void LandDetect::OnCollisionEnter(shared_ptr<GameObject>& Other) {

	}
	
	void LandDetect::OnCollisionExcute(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"Floor")) {
			//カウントをリセットする
			m_isLand = true;
			m_CollideCount = m_CollideCountMax;
		}
	}

	void LandDetect::OnCollisionExit(shared_ptr<GameObject>& Other) {


	}

	//地面に接地していればtrueを返す
	bool LandDetect::GetLand() {
		return m_isLand;
	}

	//位置を変える
	void LandDetect::SetBindPos(Vec3 pos) {
		GetComponent<Transform>()->SetPosition(pos);
	}

	//判定の半径を変える
	void LandDetect::SetCollScale(float scale) {
		GetComponent<CollisionSphere>()->SetMakedRadius(scale);
	}

}
//end basecross
