/*!
@file State.h
@brief ��ԂȂ�
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	class StateBase;

	//--------------------------------------------------------------------
	// ���N���X
	//--------------------------------------------------------------------

	/// <summary>
	/// �X�e�[�g���܂Ƃ߂邽�߂̃I�u�W�F�N�g�B�p���p
	/// </summary>
	class StateMachineBase : enable_shared_from_this<StateBase> {
	protected:
		//�X�e�[�g�ꗗ
		unordered_map<wstring, shared_ptr<StateBase>> _stateTypes;
		//���݂̃X�e�[�g
		shared_ptr<StateBase> _currentState;
		//�O�̃X�e�[�g
		shared_ptr<StateBase> _prevState;

		void AddState(wstring stateName, const shared_ptr<StateBase> state);

	public:
		//AddState�͌p����̃R���X�g���N�^�ōs���Ă�������
		//�܂��A�K���ŏ���ChangeState���Ăяo����currentState�����������邱��
		StateMachineBase() {}
		~StateMachineBase() {}

		bool ChangeState(wstring stateName);

		void Update(float deltatime);
	};

	/// <summary>
	/// �X�e�[�g�̊��N���X
	/// </summary>
	class StateBase : enable_shared_from_this<StateBase> {
	protected:
		//�I�u�W�F�N�g
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

