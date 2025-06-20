/*!
@file CameraState.h
@brief �J���������̃X�e�[�g
*/

#pragma once
#include "stdafx.h"
//#include "State.h"
//#include "CameraManager.h"

namespace basecross {
	class CameraManager;

	//�J�����X�e�[�g�̌��ƂȂ�N���X
	class CameraStateBase :public StateBase
	{
	protected:
		shared_ptr<CameraManager> m_cameraManager;//�v���C���[�̎���
		CONTROLER_STATE m_controller;//�R���g���[���[
		Vec3 m_controllerVec;
		float m_timeOfPushAttackButton = 0.0f;//�U���{�^���������Ă��鎞��
		shared_ptr<Actor> m_targetObj = nullptr;//���b�N�I�����̑Ώ�
		float m_targetDistance;//�^�[�Q�b�g�ΏۂƂ̋���

		shared_ptr<Stage> m_stage;

	public:
		CameraStateBase(shared_ptr<GameObject>& obj) :
			StateBase(obj),
			m_cameraManager(dynamic_pointer_cast<CameraManager>(obj))
		{

		}
		~CameraStateBase()
		{
		}


		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();
	};



	//�J�����̒ʏ��Ԃ̃X�e�[�g(�ߐ�)
	class CameraNomalState :public CameraStateBase
	{
	private:

	public:
		CameraNomalState(shared_ptr<GameObject>& obj) :
			CameraStateBase(obj)
		{

		}
		~CameraNomalState()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();
	};
	//


	//�J�����̏e���[�h�̃X�e�[�g
	class CameraGunState :public CameraStateBase
	{
	private:

	public:
		CameraGunState(shared_ptr<GameObject>& obj) :
			CameraStateBase(obj)
		{

		}
		~CameraGunState()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();
	};
	//

	//�J�������v���C�������Ɍ������X�e�[�g
	class CameraResetState :public CameraStateBase
	{
	private:

	public:
		CameraResetState(shared_ptr<GameObject>& obj) :
			CameraStateBase(obj)
		{

		}
		~CameraResetState()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();
	};

	//�J������ʏ탂�[�h����e���[�h�ɐ؂�ւ���Ƃ��̈ړ��X�e�[�g
	class CameraNormalToGunState :public CameraStateBase
	{
	private:

	public:
		CameraNormalToGunState(shared_ptr<GameObject>& obj) :
			CameraStateBase(obj)
		{

		}
		~CameraNormalToGunState()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();

	};

	//���[�r�[�p�̃J�����X�e�[�g(�J�n��)
	class CameraStartMovieState_First :public CameraStateBase
	{
	private:

	public:
		CameraStartMovieState_First(shared_ptr<GameObject>& obj) :
			CameraStateBase(obj)
		{

		}
		~CameraStartMovieState_First()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();

	};

	//���[�r�[�p�̃J�����X�e�[�g(�J�n��)
	class CameraStartMovieState_Second :public CameraStateBase
	{
	private:

	public:
		CameraStartMovieState_Second(shared_ptr<GameObject>& obj) :
			CameraStateBase(obj)
		{

		}
		~CameraStartMovieState_Second()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();

	};

	//�{�X���[�r�[�p�̃J�����X�e�[�g
	class CameraBossMovieState :public CameraStateBase
	{
	private:
		// �{�X
		//weak_ptr<EnemyBase> m_boss;

	public:
		CameraBossMovieState(shared_ptr<GameObject>& obj) :
			CameraStateBase(obj)
		{

		}
		~CameraBossMovieState()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();

	};


	//�J�����X�e�[�g�}�V��
	class CameraStateMachine :public StateMachineBase
	{
	public:
		CameraStateMachine(shared_ptr<GameObject>& obj)
		{
			//�X�e�[�g�̓o�^
			//�ʏ���
			AddState(L"Normal", shared_ptr<CameraNomalState>(new CameraNomalState(obj)));
			//�e���
			AddState(L"Gun", shared_ptr<CameraGunState>(new CameraGunState(obj)));
			//�J�������v���C���[�̕����Ɍ��������
			AddState(L"Reset", shared_ptr<CameraResetState>(new CameraResetState(obj)));
			//���[�r�[�p�̃J�����X�e�[�g(�J�n����i�K��)
			AddState(L"StartMovie_First", shared_ptr<CameraStartMovieState_First>(new CameraStartMovieState_First(obj)));
			//���[�r�[�p�̃J�����X�e�[�g(�J�n����i�K��)
			AddState(L"StartMovie_Second", shared_ptr<CameraStartMovieState_Second>(new CameraStartMovieState_Second(obj)));

			ChangeState(L"Normal");
		}
	};

}