/*!
@file ZakoState.cpp
@brief �G���̃X�e�[�g
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	/*

	//-------------------------------------------------
	// �ʏ�G���G�̃X�e�[�g�擪
	//-------------------------------------------------

	//�����Ȃ��Ƃ��̃X�e�[�g
	void EnemyZakoStandState::Enter()
	{
		m_enemyZako->ChangeAnim(L"Stand");//���A�j���[�V�����ɕύX
	}
	void EnemyZakoStandState::Update(float deltaTime)
	{
		auto stage = m_enemyZako->GetStage();
		auto attackType = m_enemyZako->GetAttackType();

		auto isLand = m_enemyZako->GetLand();//���n���Ă��邩�̃t���O

		//�G���G�̃^�C�v�ɂ���čU���̕��@���ς��
		//������
		m_timeOfShot += deltaTime;
		if (attackType == m_enemyZako->Zako_Long && isLand)
		{
			m_enemyZako->ChangeState(L"Alignment");//�����킹����n�܂�
		}
		//�ߋ���
		if (attackType == m_enemyZako->Zako_Melee && isLand)
		{
			m_enemyZako->ChangeState(L"PreparationforMelee");//�ڋ߂��čU������
		}
	}
	void EnemyZakoStandState::Exit()
	{
		//�łJ�E���g�_�E�����Z�b�g
		m_timeOfShot = 0.0f;
	}

	//�ڋߐ������Ƃ��̃X�e�[�g
	void EnemyZakoMeleeState::Enter()
	{
		auto LandFlag = m_enemyZako->GetLand();
		auto testVector = m_enemyZako->GetVelocity();

		//�U�����ۂ��A�j���[�V�����ɂ��Ă݂�
		m_enemyZako->ChangeAnim(L"Down");
		//m_enemyZako->ChangeAnim(L"Walk");//�����A�j���[�V�����ɕύX

		//�U�����Ă���^�O�ǉ�
		m_enemyZako->AddTag(L"AttackNow");
	}
	void EnemyZakoMeleeState::Update(float deltaTime)
	{
		auto stage = m_enemyZako->GetStage();

		m_timeOfAttack += deltaTime;

		//�A�j���[�V�����X�V���Ԑݒ�
		m_enemyZako->SetAddTimeAnimation(deltaTime * 2.5f);

		//�U������̐���
		if (m_timeOfAttack >= m_timeOfAttackAdd && m_Attack)
		{
			auto tmp = m_enemyZako->GetAttackPtr()->GetHitInfo();
			tmp.HitOnce = true;
			tmp.Damage = 5;
			tmp.HitVel_Stand = Vec3(-3, 5, 0);
			tmp.HitTime_Stand = .8f;
			tmp.Type = AttackType::Enemy;
			//tmp.ForceRecover = false;//�m�b�N�o�b�N����
			m_enemyZako->DefAttack(.5f, tmp);
			m_enemyZako->GetAttackPtr()->SetPos(Vec3(3, 1, 0));
			auto AttackPtr = m_enemyZako->GetAttackPtr();
			AttackPtr->GetComponent<Transform>()->SetScale(Vec3(3.7f, 3.0f, 3.0f));
			AttackPtr->SetCollScale(1.0f);

			m_enemyZako->SetAttackFlag(false);//�U�����肪�������������Ȃ��悤�ɂ���
			m_Attack = false;//�U�����肪�������������Ȃ��悤�ɂ���
		}

		//��莞�Ԃ�������U���X�e�[�g����߂�
		if (m_timeOfAttack >= m_timeMaxOfAttack)
		{
			m_enemyZako->ChangeState(L"Stand");
		}
	}
	void EnemyZakoMeleeState::Exit()
	{
		m_Attack = true;
		m_timeOfAttack = 0.0f;

		//�U�����Ă���^�O�폜
		m_enemyZako->RemoveTag(L"AttackNow");
	}

	//�ڋߐ������Ƃ��̏����X�e�[�g(�U���ł��鋗���ɂȂ�܂ŋ߂Â�)
	void EnemyZakoPreparationforMeleeState::Enter()
	{
		m_enemyZako->ChangeAnim(L"Walk");//�����A�j���[�V�����ɕύX

		//�����̃v���C���[�Ƃ̋����ɂ���đ��̑�����ς���
		auto playerdist = m_enemyZako->GetPlayerDist();
		SppedChange();

		//�f�o�b�N�p
		auto test = m_speed;
		auto a = 0;
	}
	void EnemyZakoPreparationforMeleeState::Update(float deltaTime)
	{
		//�v���C���Ƃ̋����ɂ���ăX�s�[�h��ς���
		SppedChange();

		auto stage = m_enemyZako->GetStage();
		auto meleeRange = 8.0f;//�ڋߍU���L���͈�

		//Player�̕����ɉ�]����
		auto PushAngle = XM_PIDIV4 / 4;//��]�̂���
		m_enemyZako->RotateToPlayer(1.0f, PushAngle);

		auto attackFlag = m_enemyZako->GetAttackFlag();//�U���t���O���󂯎��

		//�U���̃N�[���^�C�����߂��Ă���ΐڋ߂����łȂ���Η����
		if (attackFlag)
		{
			//�L���͈͂܂ŋ߂Â�����ߐڍU�������邻���łȂ���΁A�����܂ňړ�
			if (m_enemyZako->GetPlayerDist() < meleeRange)
			{
				//�U���̂��߂ɗ����~�܂�̂ŗ��A�j���[�V�����ɕύX
				m_enemyZako->ChangeAnim(L"Stand");

				//�U���t���O���I���Ȃ�U���ł���
				if (!attackFlag) return;
				m_enemyZako->ChangeState(L"Melee");
			}
			else if (m_enemyZako->GetPlayerDist() >= meleeRange)
			{
				//�ړ����Ȃ̂ł���ɍ��킹���A�j���[�V����
				m_enemyZako->ChangeAnim(L"Walk");

				//�i�ދ��������߂�
				auto move = m_enemyZako->GetForward() * m_speed;

				auto LandFlag = m_enemyZako->GetLand();
				if (LandFlag)
				{
					move.y = 0.0f;
				}

				m_enemyZako->SetVelocity(move);
				//�A�j���[�V�����X�V���Ԑݒ�
				m_enemyZako->SetAddTimeAnimation(deltaTime * 2.5f);
			}
		}
		else if (!attackFlag)//�v���C���[���狗�������
		{
			if (m_enemyZako->GetPlayerDist() < meleeRange * 3.0f)
			{
				//�ړ����Ȃ̂ł���ɍ��킹���A�j���[�V����
				m_enemyZako->ChangeAnim(L"Walk");

				//�i�ދ��������߂�
				auto move = m_enemyZako->GetForward() * -(m_speed * 0.8);

				auto LandFlag = m_enemyZako->GetLand();
				if (LandFlag)
				{
					move.y = 0.0f;
				}

				m_enemyZako->SetVelocity(move);
				//�A�j���[�V�����X�V���Ԑݒ�
				m_enemyZako->SetAddTimeAnimation(deltaTime * 2.5f);
			}

		}


	}
	void EnemyZakoPreparationforMeleeState::Exit()
	{

	}

	//�v���C���[�Ƃ̋����ɂ���ċr�̃X�s�[�h��ς��鏈��
	void EnemyZakoPreparationforMeleeState::SppedChange()
	{
		//�����ɂ���ăX�s�[�h��ς���
		auto playerdist = m_enemyZako->GetPlayerDist();
		if (playerdist > 50.0f)//��
		{
			m_speed = 15.0f;
		}
		else if (playerdist > 30.0f)//��
		{
			m_speed = 7.0f;
		}
		else//�߂�
		{
			m_speed = 10.0f;
		}
	}


	//����ł��O�̎����킹�̃X�e�[�g
	void EnemyZakoAlignmentState::Enter()
	{

	}
	void EnemyZakoAlignmentState::Update(float deltaTime)
	{
		auto stage = m_enemyZako->GetStage();

		//Player�̕����ɉ�]����
		auto PushAngle = XM_PIDIV4 / 4;//��]�̂���
		m_enemyZako->RotateToPlayer(1.0f, PushAngle);

		//�L���͈͂܂Őڋ߂���A�v���C���[�ɂ�����x�߂Â��ꂽ�狗�������
		auto Range = 25.0f;
		if (m_enemyZako->GetPlayerDist() >= Range)//�߂Â�
		{
			//�ړ����Ȃ̂ł���ɍ��킹���A�j���[�V����
			m_enemyZako->ChangeAnim(L"Walk");

			//�i�ދ��������߂�
			m_speed = 10.0f;
			auto move = m_enemyZako->GetForward() * m_speed;

			auto LandFlag = m_enemyZako->GetLand();
			if (LandFlag)
			{
				move.y = 0.0f;
			}

			m_enemyZako->SetVelocity(move);
			//�A�j���[�V�����X�V���Ԑݒ�
			m_enemyZako->SetAddTimeAnimation(deltaTime * 2.5f);
		}
		if (m_enemyZako->GetPlayerDist() <= Range - 5.0f)//�����
		{
			//�ړ����Ȃ̂ł���ɍ��킹���A�j���[�V����
			m_enemyZako->ChangeAnim(L"Walk");

			//�i�ދ��������߂�
			m_speed = -7.0f;
			auto move = m_enemyZako->GetForward() * m_speed;

			auto LandFlag = m_enemyZako->GetLand();
			if (LandFlag)
			{
				move.y = 0.0f;
			}

			m_enemyZako->SetVelocity(move);
			//�A�j���[�V�����X�V���Ԑݒ�(���ɐi�ނ̂ŋt�Đ��ɂ�����)
			m_enemyZako->SetAddTimeAnimation(deltaTime * 2.5f);
		}

		//��莞�Ԃ�������U������
		m_timeOfShot += deltaTime;
		if (m_timeOfShot >= m_timeMaxOfShot)
		{
			m_timeOfShot = 0.0f;//���Z�b�g
			m_enemyZako->ChangeState(L"Shot");//�łX�e�[�g���Ȃ��̂ŃR�����g�A�E�g
		}
	}
	void EnemyZakoAlignmentState::Exit()
	{
		//�łJ�E���g�_�E�����Z�b�g
		m_timeOfShot = 0.0f;
	}

	//�U��������X�e�[�g(������)
	void EnemyZakoShotState::Enter()
	{
		auto stage = m_enemyZako->GetStage();
		auto posEnemy = m_enemyZako->GetPosition();
		//�e����
		auto bullet = stage->AddGameObject<Bullet>(posEnemy, Vec3(0.0f), Vec3(0.4f), 30.0f,
			dynamic_pointer_cast<Actor>(m_enemyZako), 30.0f, ActorName_Enemy);

		m_enemyZako->ChangeAnim(L"Shot");//���A�j���[�V�����ɕύX
	}
	void EnemyZakoShotState::Update(float deltaTime)
	{
		auto stage = m_enemyZako->GetStage();

		////�ڕW�ƂȂ�p�x�擾
		auto angleTarget = m_enemyZako->GetPlayerSubDirection();

		//�A�j���[�V�����X�V���Ԑݒ�
		m_enemyZako->SetAddTimeAnimation(deltaTime);

		//��莞�Ԃ�������Stand�X�e�[�g�ɖ߂�
		m_timeOfAttack += deltaTime;
		if (m_timeOfAttack >= m_timeMaxOfAttack)
		{
			m_enemyZako->ChangeState(L"Stand");
		}
	}
	void EnemyZakoShotState::Exit()
	{
		//�łJ�E���g�_�E�����Z�b�g
		m_timeOfAttack = 0.0f;
	}


	//�_���[�W���󂯂��X�e�[�g
	void EnemyZakoHitState::Enter()
	{
		auto hitInfo = m_enemyZako->GetHitInfo();
		auto HPNow = m_enemyZako->GetHPCurrent();
		//�U�����󂯂��̂Ńq�b�g�o�b�N����
		m_enemyZako->HitBack();
		//�_���[�W����
		m_enemyZako->SetHPCurrent(HPNow - hitInfo.Damage);

		m_enemyZako->ChangeAnim(L"Stand");//�_���[�W���󂯂��A�j���[�V�����ɕύX
	}
	void EnemyZakoHitState::Update(float deltaTime)
	{
		//��莞�Ԃ�������Stand�X�e�[�g�ɖ߂�
		m_enemyZako->HitBackStandBehavior();

		//�A�j���[�V�����X�V���Ԑݒ�
		m_enemyZako->SetAddTimeAnimation(deltaTime);
	}
	void EnemyZakoHitState::Exit()
	{

	}
	//-------------------------------------------------
	// �ʏ�G���G�̃X�e�[�g�I�[
	//-------------------------------------------------


	//-------------------------------------------------
	// ��ԃU�R�̃X�e�[�g�擪
	// ------------------------------------------------

	// �����Ȃ��Ƃ��̃X�e�[�g----------------------
	void EnemyZakoFlyingStandState::Enter()
	{
		m_enemyZako->ChangeAnim(L"Stand");//�ҋ@�A�j���[�V�����ɕύX
	}

	void EnemyZakoFlyingStandState::Update(float deltatime)
	{
		auto stage = m_enemyZako->GetStage();
		auto attackType = m_enemyZako->GetAttackType();

		auto isLand = m_enemyZako->GetLand();//���n(�؋�H)���Ă��邩�̃t���O

	}

	void EnemyZakoFlyingStandState::Exit()
	{

	}
	// END-----------------------------------------

	//�ڋߐ������Ƃ��̏����X�e�[�g-----------------
	void EnemyZakoFlyingPreparationforMeleeState::Enter()
	{
		m_enemyZako->ChangeAnim(L"Walk");//�����A�j���[�V�����ɕύX
	}

	void EnemyZakoFlyingPreparationforMeleeState::Update(float deltaTime)
	{
		auto stage = m_enemyZako->GetStage();


		auto meleeRange = 10.0f;//�ڋߍU���L���͈�

		//Player�̕����ɉ�]����
		auto PushAngle = XM_PIDIV4 / 4;//��]�̂���
		m_enemyZako->RotateToPlayer(1.0f, PushAngle);

		auto attackFlag = m_enemyZako->GetAttackFlag();
		//�L���͈͂܂ŋ߂Â�����ߐڍU�������邻���łȂ���΁A�����܂ňړ�
		if (m_enemyZako->GetPlayerDist() < meleeRange)
		{
			//�U���̂��߂ɗ����~�܂�̂ŗ��A�j���[�V�����ɕύX
			m_enemyZako->ChangeAnim(L"Stand");

			//�U���t���O���I���Ȃ�U���ł���
			if (!attackFlag) return;
			m_enemyZako->ChangeState(L"Melee");
		}
		else if (m_enemyZako->GetPlayerDist() >= meleeRange)
		{
			//�ړ����Ȃ̂ł���ɍ��킹���A�j���[�V����
			m_enemyZako->ChangeAnim(L"Walk");

			//�i�ދ��������߂�
			auto move = m_enemyZako->GetForward() * 10.0f;

			auto LandFlag = m_enemyZako->GetLand();
			if (LandFlag)
			{
				move.y = 0.0f;
			}

			m_enemyZako->SetVelocity(move);
			//�A�j���[�V�����X�V���Ԑݒ�
			m_enemyZako->SetAddTimeAnimation(deltaTime * 2.5f);
		}
	}

	void EnemyZakoFlyingPreparationforMeleeState::Exit()
	{

	}
	// END--------------------------------------------

	// �U��������Ƃ��̃X�e�[�g(�ߋ���)-----
	void EnemyZakoFlyingMeleeState::Enter()
	{
		auto LandFlag = m_enemyZako->GetLand();
		auto testVector = m_enemyZako->GetVelocity();

		//�U�����ۂ��A�j���[�V�����ɂ��Ă݂�
		m_enemyZako->ChangeAnim(L"Down");
		//m_enemyZako->ChangeAnim(L"Walk");//�����A�j���[�V�����ɕύX

	}

	void EnemyZakoFlyingMeleeState::Update(float deltaTime)
	{
		auto stage = m_enemyZako->GetStage();

		m_timeOfAttack += deltaTime;

		//�A�j���[�V�����X�V���Ԑݒ�
		m_enemyZako->SetAddTimeAnimation(deltaTime * 2.5f);

		//�U������̐���
		if (m_timeOfAttack >= m_timeOfAttackAdd && m_Attack)
		{
			auto tmp = m_enemyZako->GetAttackPtr()->GetHitInfo();
			tmp.HitOnce = true;
			tmp.Damage = 5;
			tmp.HitVel_Stand = Vec3(-3, 5, 0);
			tmp.HitTime_Stand = .8f;
			tmp.Type = AttackType::Enemy;
			//tmp.ForceRecover = false;//�m�b�N�o�b�N����
			m_enemyZako->DefAttack(.5f, tmp);
			m_enemyZako->GetAttackPtr()->SetPos(Vec3(3, 1, 0));
			auto AttackPtr = m_enemyZako->GetAttackPtr();
			AttackPtr->GetComponent<Transform>()->SetScale(Vec3(3.7f, 3.0f, 3.0f));
			AttackPtr->SetCollScale(1.0f);

			m_enemyZako->SetAttackFlag(false);//�U�����肪�������������Ȃ��悤�ɂ���
			m_Attack = false;//�U�����肪�������������Ȃ��悤�ɂ���
		}

		//��莞�Ԃ�������U���X�e�[�g����߂�
		if (m_timeOfAttack >= m_timeMaxOfAttack)
		{
			m_enemyZako->ChangeState(L"Stand");
		}

	}

	void EnemyZakoFlyingMeleeState::Exit()
	{

	}
	// END-----------------------------------

	// �������̒��O�̎����킹�̎��̃X�e�[�g-----
	void EnemyZakoFlyingAlignmentState::Enter()
	{

	}

	void EnemyZakoFlyingAlignmentState::Update(float deltaTime)
	{
		auto stage = m_enemyZako->GetStage();

		//�ڕW�ƂȂ�p�x�擾
		//auto angleTarget = m_enemyZako->GetPlayerSubDirection();
		//auto angleTarget = atan2f(targetVec.z, targetVec.x);


		//Player�̂���������v�Z
		auto playerPos = stage->GetSharedGameObject<Player>(L"Player")->GetPosition();
		auto EnemyPos = m_enemyZako->GetPosition();
		auto targetVec = playerPos - EnemyPos;
		auto angleTarget = (atan2f(targetVec.z, -targetVec.x) / 2);
		angleTarget += XMConvertToRadians(90.0f);

		//��̂��̕����Ɍ�������Player�̕����Ɍ����Ă���Ƃ݂Ȃ�
		if (abs(angleTarget) < XMConvertToRadians(3.0f))
		{
			angleTarget = 0.0f;
		}
		//��]����
		if (angleTarget != 0.0f)
		{
			auto qt = m_enemyZako->GetComponent<Transform>()->GetBeforeQuaternion();
			qt.y = 0.0f;
			////qt.y += angleTarget;
			qt = qt * Quat(0.0f, sin(angleTarget) / 2.0f, 0.0f, cos(angleTarget) / 2.0f);
			m_enemyZako->GetComponent<Transform>()->SetQuaternion(qt);
		}


		//��莞�Ԃ�������U������
		m_timeOfShot += deltaTime;
		if (m_timeOfShot >= m_timeMaxOfShot)
		{
			m_timeOfShot = 0.0f;//���Z�b�g
			m_enemyZako->ChangeState(L"Shot");//�łX�e�[�g���Ȃ��̂ŃR�����g�A�E�g
		}

	}

	void EnemyZakoFlyingAlignmentState::Exit()
	{
		//�łJ�E���g�_�E�����Z�b�g
		m_timeOfShot = 0.0f;

	}
	// END--------------------------------------

	// �U��������Ƃ��̃X�e�[�g(������)-----
	void EnemyZakoFlyingShotState::Enter()
	{
		auto stage = m_enemyZako->GetStage();
		auto posEnemy = m_enemyZako->GetPosition();
		//�e����
		auto bullet = stage->AddGameObject<Bullet>(posEnemy, Vec3(0.0f), Vec3(0.4f), 10.0f,
			dynamic_pointer_cast<Actor>(m_enemyZako), 10.0f, ActorName_Enemy);

		m_enemyZako->ChangeAnim(L"Shot");//���A�j���[�V�����ɕύX

	}

	void EnemyZakoFlyingShotState::Update(float deltaTime)
	{
		auto stage = m_enemyZako->GetStage();

		////�ڕW�ƂȂ�p�x�擾
		auto angleTarget = m_enemyZako->GetPlayerSubDirection();

		//�A�j���[�V�����X�V���Ԑݒ�
		m_enemyZako->SetAddTimeAnimation(deltaTime);

		//��莞�Ԃ�������Stand�X�e�[�g�ɖ߂�
		m_timeOfAttack += deltaTime;
		if (m_timeOfAttack >= m_timeMaxOfAttack)
		{
			m_enemyZako->ChangeState(L"Stand");
		}

	}

	void EnemyZakoFlyingShotState::Exit()
	{
		//�łJ�E���g�_�E�����Z�b�g
		m_timeOfAttack = 0.0f;

	}
	// END-----------------------------------

	//�_���[�W���󂯂��Ƃ�-------------------
	void EnemyZakoFlyingHitState::Enter()
	{
		auto hitInfo = m_enemyZako->GetHitInfo();
		auto HPNow = m_enemyZako->GetHPCurrent();

		// �_���[�W����
		m_enemyZako->SetHPCurrent(HPNow - hitInfo.Damage);

		// �_���[�W���󂯂��A�j���[�V�����ɕύX
		m_enemyZako->ChangeAnim(L"Stand");
	}

	void EnemyZakoFlyingHitState::Update(float deltaTime)
	{
		//��莞�Ԃ�������Stand�X�e�[�g�ɖ߂�
		m_enemyZako->HitBackStandBehavior();

		//�A�j���[�V�����X�V���Ԑݒ�
		m_enemyZako->SetAddTimeAnimation(deltaTime);

	}

	void EnemyZakoFlyingHitState::Exit()
	{

	}
	// END------------------------------------

	//-------------------------------------------------------
	// ��ԃU�R�̃X�e�[�g�I�[
	//-------------------------------------------------------

	*/
}