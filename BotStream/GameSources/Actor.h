/*!
@file Actor.h
@brief プレイヤーなど
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
//#include "MyGameObject.h"

namespace basecross {
	class Actor : public ObjectMove
	{

	private:
		//void AttackCollision();	// 攻撃判定
		//enum State;				// 状態			(enumで良いかはわからないので気に入らなかったら変えてください)
		//void LandDetect();		//				(これよくわからないのでとりあえずvoidの関数にしておきます)
	public:
		Actor(const shared_ptr<Stage>& stagePtr);
		~Actor();

	};
	
}
//end basecross
