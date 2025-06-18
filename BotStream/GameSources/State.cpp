/*!
@file State.cpp
@brief ��Ԃ̏���
*/

#include "stdafx.h"
#include "Project.h"
#include "State.h"

namespace basecross {

	//�X�e�[�g���ꗗ�ɒǉ�����
	void StateMachineBase::AddState(wstring stateName, const shared_ptr<StateBase> state) {
		_stateTypes[stateName] = state;
	}

	//�X�e�[�g�ؑ�
	bool StateMachineBase::ChangeState(wstring stateName) {
		if (stateName == L"") {
			throw BaseException(
				L"�J�ڐ�X�e�[�g������͂��Ă��������B",
				L"if (stateName == L�h�h)",
				L"StateMachineBase::ChangeState(wstring stateName)"
			);
		}
		//�����L�[���̃X�e�[�g�����邩�ǂ����T��
		for (auto& it : _stateTypes) {
			if (it.first == stateName) {
				if (_currentState != nullptr) {
					_currentState->Exit();
				}
				//���X�e�[�g���w��̃X�e�[�g�ɁA�O�X�e�[�g�����X�e�[�g��
				_prevState = _currentState;
				_currentState = _stateTypes[stateName];
				_currentState->Enter();

				return true;
			}
		}
		//���̐��ʂ��@�����܂���ł����I�I
		return false;
	}

	//������I�u�W�F�N�g��OnUpdate�ɒu��
	void StateMachineBase::Update(float deltatime) {
		if (_currentState == nullptr) return;
		_currentState->Update(deltatime);
		
	}



}
//end basecross
