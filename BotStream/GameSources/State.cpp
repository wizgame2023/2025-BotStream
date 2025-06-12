/*!
@file State.cpp
@brief 状態の処理
*/

#include "stdafx.h"
#include "Project.h"
#include "State.h"

namespace basecross {

	//ステートを一覧に追加する
	void StateMachineBase::AddState(wstring stateName, const shared_ptr<StateBase> state) {
		_stateTypes[stateName] = state;
	}

	//ステート切替
	bool StateMachineBase::ChangeState(wstring stateName) {
		if (stateName == L"") {
			throw BaseException(
				L"遷移先ステート名を入力してください。",
				L"if (stateName == L””)",
				L"StateMachineBase::ChangeState(wstring stateName)"
			);
		}
		//同じキー名のステートがあるかどうか探す
		for (auto& it : _stateTypes) {
			if (it.first == stateName) {
				if (_currentState != nullptr) {
					_currentState->Exit();
				}
				//現ステートを指定のステートに、前ステートを現ステートに
				_prevState = _currentState;
				_currentState = _stateTypes[stateName];
				_currentState->Enter();

				return true;
			}
		}
		//何の成果も　得られませんでした！！
		return false;
	}

	//これをオブジェクトのOnUpdateに置く
	void StateMachineBase::Update(float deltatime) {
		if (_currentState == nullptr) return;
		_currentState->Update(deltatime);
		
	}



}
//end basecross
