/*!
@file State.h
@brief 状態など
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	class StateBase;

	//--------------------------------------------------------------------
	// 基底クラス
	//--------------------------------------------------------------------

	/// <summary>
	/// ステートをまとめるためのオブジェクト。継承用
	/// </summary>
	class StateMachineBase : enable_shared_from_this<StateBase> {
	protected:
		//ステート一覧
		unordered_map<wstring, shared_ptr<StateBase>> _stateTypes;
		//現在のステート
		shared_ptr<StateBase> _currentState;
		//前のステート
		shared_ptr<StateBase> _prevState;

		void AddState(wstring stateName, const shared_ptr<StateBase> state);

	public:
		//AddStateは継承先のコンストラクタで行ってください
		//また、必ず最初にChangeStateを呼び出してcurrentStateを初期化すること
		StateMachineBase() {}
		~StateMachineBase() {}

		bool ChangeState(wstring stateName);

		void Update(float deltatime);
	};

	/// <summary>
	/// ステートの基底クラス
	/// </summary>
	class StateBase : enable_shared_from_this<StateBase> {
	protected:
		//オブジェクト
		weak_ptr<GameObject> _obj;

	public:
		StateBase(const shared_ptr<GameObject>& obj) {
			_obj = obj;
		}
		~StateBase() {}

		virtual void Enter() {}
		virtual void Update(float deltatime) {}
		virtual void Exit() {}
	};
}
//end basecross

