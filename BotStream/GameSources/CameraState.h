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
	class EnemyBase;

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

		bool m_meleeFlag = true;
		bool m_meleeNow = false;
		bool m_gunNow = false;

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

	//�J�������e���[�h����ʏ탂�[�h�ɐ؂�ւ���Ƃ��̈ړ��X�e�[�g
	class CameraGunToNormalState :public CameraStateBase
	{
	private:

	public:
		CameraGunToNormalState(shared_ptr<GameObject>& obj) :
			CameraStateBase(obj)
		{

		}
		~CameraGunToNormalState()
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

	//�{�X���[�r�[�p�̃J�����X�e�[�g(��Ԗ�)
	class CameraBossMovieState_First :public CameraStateBase
	{
	private:
		// �{�X
		weak_ptr<EnemyBase> m_boss;

		// ���̃X�e�[�g�̑؍ݎ���
		float m_timeCount = 0.0f;
		// ���̃X�e�[�g�̑؍݉\����
		float m_timeMax = 2.0f;

	public:
		CameraBossMovieState_First(shared_ptr<GameObject>& obj) :
			CameraStateBase(obj)
		{

		}
		~CameraBossMovieState_First()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();

	};

	//�{�X���[�r�[�p�̃J�����X�e�[�g(��Ԗ�)
	class CameraBossMovieState_Second :public CameraStateBase
	{
	private:
		// �{�X
		weak_ptr<EnemyBase> m_boss;

		// ���̃X�e�[�g�̑؍ݎ���
		float m_timeCount = 0.0f;
		// ���̃X�e�[�g�̑؍݉\����
		float m_timeMax = 2.2f;

	public:
		CameraBossMovieState_Second(shared_ptr<GameObject>& obj) :
			CameraStateBase(obj)
		{

		}
		~CameraBossMovieState_Second()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();

	};


	//�{�X���[�r�[�p�̃J�����X�e�[�g(�O�Ԗ�)
	class CameraBossMovieState_Three :public CameraStateBase
	{
	private:
		// �{�X
		weak_ptr<EnemyBase> m_boss;

		// ���̃X�e�[�g�̑؍ݎ���
		float m_timeCount = 0.0f;
		// ���̃X�e�[�g�̑؍݉\����
		float m_timeMax = 0.5f;

	public:
		CameraBossMovieState_Three(shared_ptr<GameObject>& obj) :
			CameraStateBase(obj)
		{

		}
		~CameraBossMovieState_Three()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();

	};


	//�{�X���[�r�[�p�̃J�����X�e�[�g(�l�Ԗ�)
	class CameraBossMovieState_Fourth :public CameraStateBase
	{
	private:
		// �{�X
		weak_ptr<EnemyBase> m_boss;


		// ���̃X�e�[�g�̑؍ݎ���
		float m_timeCount = 0.0f;
		// ���̃X�e�[�g�̑؍݉\����
		float m_timeMax = 2.0f;

	public:
		CameraBossMovieState_Fourth(shared_ptr<GameObject>& obj) :
			CameraStateBase(obj)
		{

		}
		~CameraBossMovieState_Fourth()
		{
		}

		virtual void Enter();
		virtual void Update(float deltaTime);
		virtual void Exit();

	};


	//�{�X���[�r�[�p�̃J�����X�e�[�g
	class CameraBossDieMovieState_First :public CameraStateBase
	{
	private:
		// �{�X
		weak_ptr<EnemyBase> m_boss;

		//�{�X�̌����Ă������
		float m_bossAngle = 0.0f;

		// ���̃X�e�[�g�̑؍ݎ���
		float m_timeCount = 0.0f;
		// ���̃X�e�[�g�̑؍݉\����
		float m_timeMax = 2.0f;

	public:
		CameraBossDieMovieState_First(shared_ptr<GameObject>& obj) :
			CameraStateBase(obj)
		{

		}
		~CameraBossDieMovieState_First()
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
			//�ʏ��ԁ��e���
			AddState(L"NormalToGun", shared_ptr<CameraNormalToGunState>(new CameraNormalToGunState(obj)));
			//�e��ԁ��ʏ���
			AddState(L"GunToNormal", shared_ptr<CameraGunToNormalState>(new CameraGunToNormalState(obj)));
			//�J�������v���C���[�̕����Ɍ��������
			AddState(L"Reset", shared_ptr<CameraResetState>(new CameraResetState(obj)));
			//���[�r�[�p�̃J�����X�e�[�g(�J�n����i�K��)
			AddState(L"StartMovie_First", shared_ptr<CameraStartMovieState_First>(new CameraStartMovieState_First(obj)));
			//���[�r�[�p�̃J�����X�e�[�g(�J�n����i�K��)
			AddState(L"StartMovie_Second", shared_ptr<CameraStartMovieState_Second>(new CameraStartMovieState_Second(obj)));
			//�{�X���[�r�[�p�̃J�����X�e�[�g
			AddState(L"BossMovie_First", shared_ptr<CameraBossMovieState_First>(new CameraBossMovieState_First(obj)));
			//�{�X���[�r�[�p�̃J�����X�e�[�g
			AddState(L"BossMovie_Second", shared_ptr<CameraBossMovieState_Second>(new CameraBossMovieState_Second(obj)));
			//�{�X���[�r�[�p�̃J�����X�e�[�g
			AddState(L"BossMovie_Three", shared_ptr<CameraBossMovieState_Three>(new CameraBossMovieState_Three(obj)));
			//�{�X���[�r�[�p�̃J�����X�e�[�g
			AddState(L"BossMovie_Fourth", shared_ptr<CameraBossMovieState_Fourth>(new CameraBossMovieState_Fourth(obj)));
			//�{�X��|�����Ƃ��̃��[�r�[�p�̃J�����X�e�[�g
			AddState(L"BossDieMovie_First", shared_ptr<CameraBossDieMovieState_First>(new CameraBossDieMovieState_First(obj)));

			ChangeState(L"Normal");
		}
	};

}