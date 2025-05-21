/*!
@file EnemyState.cpp
@brief ìGÇÃÉXÉeÅ[Ég
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	void EnemyWalkState::Enter() {

	}
	void EnemyWalkState::Update(float deltatime) {
		auto p = dynamic_pointer_cast<EnemyBase>(_obj.lock());
		p->ChangeAnim(L"Idle");

	}
	void EnemyWalkState::Exit() {

	}

	void EnemyAttackState::Enter() {

	}
	void EnemyAttackState::Update(float deltatime) {

	}
	void EnemyAttackState::Exit() {

	}

	void EnemyHitState::Enter() {
		auto p = dynamic_pointer_cast<EnemyBase>(_obj.lock());
		p->ChangeAnim(L"HitBack", true);
		p->HitBack();
	}
	void EnemyHitState::Update(float deltatime) {
		auto p = dynamic_pointer_cast<EnemyBase>(_obj.lock());
		p->HitBackStandBehavior();
	}
	void EnemyHitState::Exit() {

	}

	//------------------------------------------------------------------------------------------
	// BOSS1
	//------------------------------------------------------------------------------------------

	void BossFirstStandState::Enter() {
		m_time = 0;
		auto boss = dynamic_pointer_cast<BossFirst>(_obj.lock());
		boss->ChangeAnim(L"Idle");
	}
	void BossFirstStandState::Update(float deltatime) {
		m_time += deltatime;

		auto boss = dynamic_pointer_cast<BossFirst>(_obj.lock());
		boss->RotateToPlayer(1.0f);

		if (m_time >= m_startAttack * (boss->GetHPCurrent() / boss->GetHPMax()) && 
			rnd() % 1000 <= m_startAttackRand) {
			const float dist = boss->GetPlayerDist();

			if (dist < m_farDist) {
				//ãﬂÇ¢ÅI
				boss->ChangeState(L"Attack");
			}
			else {
				//âìÇ¢ÅI
				boss->ChangeState(L"Chase");
			}

			//ímÇÈÇ©ÅIâÙÇÈÅI
			if (rnd() % 1000 <= m_spinRand) {
				boss->ChangeState(L"SpinStart");
			}

			//Ç‚Ç¡Çœí@Ç≠Ç©Ç‡Åc
			if (rnd() % 1000 <= m_slamRand) {
				boss->ChangeState(L"SlamStart");
			}

			//éûë„ÇÕîÚÇ—ìπãÔÇ≈ÇµÇÂ
			if (rnd() % 1000 <= m_sphereRand) {
				boss->ChangeState(L"SphereStart");
			}

			//Ç†Å[Ç‡Ç§ÉLÉåÇΩÇÌÅ@èIÇÌÇËÇæÇÊÇ®ëO
			if (boss->IsRecoveredFromArmorBreak()) {
				boss->ChangeState(L"BeamStart");
			}
		}
	}
	void BossFirstStandState::Exit() {

	}


	void BossFirstChaseState::Enter() {

		auto boss = dynamic_pointer_cast<EnemyBase>(_obj.lock());
		boss->ChangeAnim(L"Walk", true);
		m_time = 0;
	}
	void BossFirstChaseState::Update(float deltatime) {
		m_time += deltatime;
		auto boss = dynamic_pointer_cast<EnemyBase>(_obj.lock());
		boss->RotateToPlayer(1.0f, m_rotateThreshold);

		boss->SetVelocity(boss->GetForward() * m_chaseSpeed);

		if (m_time >= m_sphereSwitchTime && rnd() % 1000 <= m_sphereRand) {
			boss->ChangeState(L"SphereStart");
		}

		if (boss->GetPlayerDist() < m_closeDist) {
			//ãﬂÇ¢ÅI
			if (rnd() % 1000 <= m_slamRand) {
				//í@Ç≠ÅI
				boss->ChangeState(L"SlamStart");
			}
			else {
				//éhÇ∑ÅI
				boss->ChangeState(L"Attack");
			}
			
		}
	}
	void BossFirstChaseState::Exit() {

	}


	void BossFirstAttackState::Enter() {
		auto boss = dynamic_pointer_cast<EnemyBase>(_obj.lock());
		boss->ChangeAnim(L"AttackClose1", true);
		m_time = 0;
		m_attacked = false;
	}
	void BossFirstAttackState::Update(float deltatime) {
		m_time += deltatime;

		auto boss = dynamic_pointer_cast<EnemyBase>(_obj.lock());
		//çUåÇîªíËÇÃíËã`
		if (m_time >= m_startup && !m_attacked) {
			m_attacked = !m_attacked;
			boss->PlaySnd(L"Enemy_Slash", 1.0f, 0);

			auto tmp = boss->GetAttackPtr()->GetHitInfo();
			tmp.HitOnce = true;
			tmp.Type = AttackType::Enemy;
			tmp.Damage = 10;
			tmp.HitVel_Stand = Vec3(-5, 0, 0);
			tmp.HitTime_Stand = 1.2f;
			boss->DefAttack(.3f, tmp);
			boss->GetAttackPtr()->SetCollScale(6.0f);
			boss->GetAttackPtr()->SetPos(Vec3(3.0f, 1, 0));
		}

		if (m_time >= m_end) {
			boss->ChangeState(L"Attack2");
		}
	}
	void BossFirstAttackState::Exit() {

	}

	void BossFirstAttack2State::Enter() {
		auto boss = dynamic_pointer_cast<EnemyBase>(_obj.lock());
		boss->ChangeAnim(L"AttackClose2", true);
		m_time = 0;
		m_attacked = false;
	}
	void BossFirstAttack2State::Update(float deltatime) {
		m_time += deltatime;

		auto boss = dynamic_pointer_cast<EnemyBase>(_obj.lock());
		//çUåÇîªíËÇÃíËã`
		if (m_time >= m_startup && !m_attacked) {
			m_attacked = !m_attacked;
			boss->PlaySnd(L"Enemy_Slash", 1.0f, 0);

			auto tmp = boss->GetAttackPtr()->GetHitInfo();
			tmp.HitOnce = true;
			tmp.Type = AttackType::Enemy;
			tmp.Damage = 10;
			tmp.HitVel_Stand = Vec3(-10, 20, 0);
			tmp.HitTime_Stand = 1.2f;
			boss->DefAttack(.3f, tmp);
			boss->GetAttackPtr()->SetCollScale(6.0f);
			boss->GetAttackPtr()->SetPos(Vec3(3.0f, 1, 0));
		}

		if (m_time >= m_end) {
			boss->ChangeState(L"Stand");
		}
	}
	void BossFirstAttack2State::Exit() {

	}

	void BossFirstSpinStartState::Enter() {
		auto boss = dynamic_pointer_cast<EnemyBase>(_obj.lock());
		boss->ChangeAnim(L"AttackSpin1", true);
		m_time = 0;
	}
	void BossFirstSpinStartState::Update(float deltatime) {
		m_time += deltatime;

		auto boss = dynamic_pointer_cast<EnemyBase>(_obj.lock());
		//çUåÇ
		if (m_time >= m_startup) {
			boss->ChangeState(L"Spin");
		}
	}
	void BossFirstSpinStartState::Exit() {

	}

	void BossFirstSpinState::Enter() {
		auto boss = dynamic_pointer_cast<EnemyBase>(_obj.lock());
		boss->ChangeAnim(L"AttackSpin2", true);
		boss->PlaySnd(L"Enemy_Spin", 1.0f, 0);
		m_end = m_allFrame + m_allFramePlus * (1.0f - (boss->GetHPCurrent() / boss->GetHPMax()));

		//èâä˙âª
		m_speed = 0;
		m_spinSpeed = 0;
		m_time = 0;
		m_totalTime = 0;
		m_attacked = false;
	}
	void BossFirstSpinState::Update(float deltatime) {
		m_time += deltatime;
		m_totalTime += deltatime;
		auto boss = dynamic_pointer_cast<EnemyBase>(_obj.lock());

		//âÒì]ë¨ìx
		if (m_spinSpeed <= m_spinSpeedMax) {
			m_spinSpeed += m_spinAccel * deltatime;
		}
		else if (m_spinSpeed > m_spinSpeedMax) {
			m_spinSpeed = m_spinSpeedMax;
		}
		//é¿ç€Ç…âÒì]Ç≥ÇπÇÈ
		Quat q = boss->GetQuaternion();
		q = boss->RotateQuat(q, Vec3(0, 1, 0), m_spinSpeed * deltatime);
		boss->SetQuaternion(q);

		//à⁄ìÆë¨ìx
		if (m_speed <= m_speedMax) {
			m_speed += m_accel * deltatime;
		}
		else if (m_speed > m_speedMax) {
			m_speed = m_speedMax;
		}
		//ÉvÉåÉCÉÑÅ[ñ⁄ä|ÇØÇƒà⁄ìÆ
		Vec3 vel = boss->GetPlayerDistInVec3().normalize();
		vel.y = 0;

		boss->SetVelocity(vel * m_speed);

		//çUåÇîªíËÇÃíËã`
		if (m_spinSpeed >= m_spinSpeedMax / 2 && !m_attacked) {
			m_attacked = !m_attacked;
			m_time = 0;

			auto tmp = boss->GetAttackPtr()->GetHitInfo();
			tmp.HitOnce = true;
			tmp.Type = AttackType::Enemy;
			tmp.Damage = 1;
			tmp.HitVel_Stand = Vec3(-8, 3, 0);
			tmp.HitTime_Stand = 1.2f;
			boss->DefAttack(.1f, tmp);
			boss->GetAttackPtr()->SetCollScale(10.0f);
			boss->GetAttackPtr()->SetPos(Vec3(0, 3, 0));
		}

		if (m_time > m_attackTime) {
			m_attacked = false;
		}

		if (m_totalTime >= m_end) {
			boss->ChangeState(L"SpinOver");
		}
	}
	void BossFirstSpinState::Exit() {

	}

	void BossFirstSpinOverState::Enter() {
		auto boss = dynamic_pointer_cast<EnemyBase>(_obj.lock());
		boss->ChangeAnim(L"AttackSpin3", true);

		boss->StopSnd();
		boss->PlaySnd(L"Enemy_SpinFinish", 1.0f, 0);
		//ÇøÇÂÇ¡Ç∆Ç∏Ç¬å∏ë¨Ç∑ÇÈä¥Ç∂
		m_spinSpeed = m_spinSpeedMax;
		m_time = 0;

		//çUåÇîªíËÇÃíËã`
		auto tmp = boss->GetAttackPtr()->GetHitInfo();
		tmp.HitOnce = true;
		tmp.Type = AttackType::Enemy;
		tmp.Damage = 10;
		tmp.HitVel_Stand = Vec3(-30, 30, 0);
		tmp.HitTime_Stand = 1.5f;
		boss->DefAttack(.1f, tmp);
		boss->GetAttackPtr()->SetCollScale(10.0f);
		boss->GetAttackPtr()->SetPos(Vec3(0, 3, 0));
	}
	void BossFirstSpinOverState::Update(float deltatime) {
		m_time += deltatime;
		m_spinSpeed -= m_spinSpeed * (1000.0f / 60.0f) * deltatime;

		auto boss = dynamic_pointer_cast<EnemyBase>(_obj.lock());
		//âÒì]Ç≥ÇπÇÈ
		Quat q = boss->GetQuaternion();
		q = boss->RotateQuat(q, Vec3(0, 1, 0), m_spinSpeed * deltatime);
		boss->SetQuaternion(q);

		if (m_time >= m_end) {
			boss->ChangeState(L"Bonus");
		}
	}
	void BossFirstSpinOverState::Exit() {

	}

	void BossFirstSphereStartState::Enter() {
		auto boss = dynamic_pointer_cast<BossFirst>(_obj.lock());
		boss->ChangeAnim(L"Rotate", true);
		m_time = 0;
		m_finishRotate = false;
	}
	void BossFirstSphereStartState::Update(float deltatime) {
		auto boss = dynamic_pointer_cast<BossFirst>(_obj.lock());
		boss->RotateToPlayer(m_rotateSpeed, m_rotateThreshold);

		if (boss->GetPlayerSubDirection() <= m_rotateThreshold && !m_finishRotate) {
			m_finishRotate = true;
			boss->ChangeAnim(L"AttackSphereStart", false);
		}

		if (m_finishRotate) {
			m_time += deltatime;
		}

		if (m_time >= m_startup)
		{
			boss->ChangeState(L"Sphere1");
		}
	}
	void BossFirstSphereStartState::Exit() {

	}

	void BossFirstSphere1State::Enter() {
		auto p = dynamic_pointer_cast<BossFirst>(_obj.lock());
		p->ChangeAnim(L"AttackSphere1", true);

		m_time = 0;
		m_attacked[0] = false;
		m_attacked[1] = false;
	}
	void BossFirstSphere1State::Update(float deltatime) {
		m_time += deltatime;
		auto boss = dynamic_pointer_cast<BossFirst>(_obj.lock());

		//îÚÇ—ìπãÔ
		if (m_time >= m_fireTime[0] && !m_attacked[0]) {
			m_attacked[0] = !m_attacked[0];
			boss->PlaySnd(L"Beam", 1.0f, 0);

			Vec3 pos = boss->GetPosition() + FirePosAdjust(boss->GetForward(), m_firePos[0]) + m_fireHeight;
			boss->GetStage()->AddGameObject<BossFirstSphere>(pos, boss->GetQuaternion(), Vec3(1.0f), m_towardPlayerTime[0]);

			pos = boss->GetPosition() + FirePosAdjust(boss->GetForward(), m_firePos[1]) + m_fireHeight;
			boss->GetStage()->AddGameObject<BossFirstSphere>(pos, boss->GetQuaternion(), Vec3(1.0f), m_towardPlayerTime[1]);
		}

		//îÚÇ—ìπãÔáA
		if (m_time >= m_fireTime[1] && !m_attacked[1]) {
			m_attacked[1] = !m_attacked[1];
			boss->PlaySnd(L"Beam", 1.0f, 0);

			Vec3 pos = boss->GetPosition() + FirePosAdjust(boss->GetForward(), m_firePos[2]) + m_fireHeight;
			boss->GetStage()->AddGameObject<BossFirstSphere>(pos, boss->GetQuaternion(), Vec3(1.0f), m_towardPlayerTime[2]);

			pos = boss->GetPosition() + FirePosAdjust(boss->GetForward(), m_firePos[3]) + m_fireHeight;
			boss->GetStage()->AddGameObject<BossFirstSphere>(pos, boss->GetQuaternion(), Vec3(1.0f), m_towardPlayerTime[3]);
		}

		if (m_time >= m_endTime) {
			boss->ChangeState(L"Stand");
		}
	}
	void BossFirstSphere1State::Exit() {

	}

	void BossFirstSphereEndState::Enter() {
		auto boss = dynamic_pointer_cast<BossFirst>(_obj.lock());
		boss->ChangeAnim(L"AttackSphereEnd", true);
		m_time = 0;
	}
	void BossFirstSphereEndState::Update(float deltatime) {
		m_time += deltatime;

		if (m_time >= m_recoveryTime)
		{
			auto boss = dynamic_pointer_cast<BossFirst>(_obj.lock());
			boss->ChangeState(L"Stand");
		}
	}
	void BossFirstSphereEndState::Exit() {

	}

	void BossFirstBeamStartState::Enter() {
		auto boss = dynamic_pointer_cast<BossFirst>(_obj.lock());
		boss->ChangeAnim(L"AttackSPBeam1", true);
		m_time = 0;

	}
	void BossFirstBeamStartState::Update(float deltatime) {
		m_time += deltatime;

		auto boss = dynamic_pointer_cast<BossFirst>(_obj.lock());
		boss->RotateToPlayer(2.0f, m_rotateThreshold);

		if (m_time >= m_startup) {
			boss->ChangeState(L"BeamFire");
			boss->AddEffect(EnemyEffect_Beam);

			boss->PlaySnd(L"Beam", 1.0f, 0);
		}
	}
	void BossFirstBeamStartState::Exit() {

	}

	void BossFirstBeamFireState::Enter() {
		auto p = dynamic_pointer_cast<BossFirst>(_obj.lock());
		p->ChangeAnim(L"AttackSPBeam2", true);

		m_time = 0;
		m_attackTime = 0;
		m_beamCnt = 0;
		m_isFinalBlow = false;
	}
	void BossFirstBeamFireState::Update(float deltatime) {
		m_time += deltatime;
		m_attackTime += deltatime;
		auto boss = dynamic_pointer_cast<BossFirst>(_obj.lock());

		if (m_attackTime >= m_attackTimeMax && !m_isFinalBlow) {
			if (m_time > m_attackTimeEnd) {
				m_isFinalBlow = true;
			}
			Vec3 pos = boss->GetPosition() + Vec3(0, -1.5, 0);
			float vel = m_beamHitVelBase + (m_beamHitVelAdd * m_beamCnt);
			boss->GetStage()->AddGameObject<BossFirstBeam>(pos, Vec3(0.0f), Vec3(1.0f), boss, vel, m_isFinalBlow);
			
			m_attackTime = 0;
			m_beamCnt++;
		}

		if (m_time >= m_activeTime) {
			boss->ChangeState(L"BeamEnd");
		}
	}
	void BossFirstBeamFireState::Exit() {

	}

	void BossFirstBeamEndState::Enter() {
		auto p = dynamic_pointer_cast<BossFirst>(_obj.lock());
		p->ChangeAnim(L"AttackSPBeam3", true);
		m_time = 0;
	}
	void BossFirstBeamEndState::Update(float deltatime) {
		m_time += deltatime;

		if (m_time >= m_recovery) {
			auto p = dynamic_pointer_cast<BossFirst>(_obj.lock());
			p->ChangeState(L"Stand");
		}
	}
	void BossFirstBeamEndState::Exit() {

	}

	void BossFirstSlamStartState::Enter() {
		auto boss = dynamic_pointer_cast<BossFirst>(_obj.lock());
		boss->ChangeAnim(L"AttackSlamStart", true);
		m_time = 0;

	}
	void BossFirstSlamStartState::Update(float deltatime) {
		m_time += deltatime;

		auto boss = dynamic_pointer_cast<BossFirst>(_obj.lock());
		boss->RotateToPlayer(2.0f, m_rotateThreshold);

		if (m_time >= m_endTime) {
			boss->ChangeState(L"Slam1");
		}
	}
	void BossFirstSlamStartState::Exit() {

	}

	void BossFirstSlam1State::Enter() {
		auto boss = dynamic_pointer_cast<BossFirst>(_obj.lock());
		boss->ChangeAnim(L"AttackSlam1", true);
		m_time = 0;
		m_attacked = false;
	}
	void BossFirstSlam1State::Update(float deltatime) {
		m_time += deltatime;

		auto boss = dynamic_pointer_cast<BossFirst>(_obj.lock());

		//çUåÇîªíËÇÃíËã`
		if (m_time >= m_attackTime && !m_attacked) {
			m_attacked = !m_attacked;
			boss->PlaySnd(L"Enemy_Slam", 1.0f, 0);

			auto tmp = boss->GetAttackPtr()->GetHitInfo();
			tmp.HitOnce = true;
			tmp.Type = AttackType::Enemy;
			tmp.Damage = 12;
			tmp.HitVel_Stand = Vec3(-5, 0, 0);
			tmp.HitTime_Stand = 1.2f;
			boss->DefAttack(.3f, tmp);
			boss->GetAttackPtr()->SetCollScale(10.0f);
			boss->GetAttackPtr()->SetPos(Vec3(3.0f, 1, 0));
		}

		if (m_time >= m_endTime) {
			boss->ChangeState(L"Slam2");
		}
	}
	void BossFirstSlam1State::Exit() {

	}

	void BossFirstSlam2State::Enter() {
		auto boss = dynamic_pointer_cast<BossFirst>(_obj.lock());
		boss->ChangeAnim(L"AttackSlam2", true);
		m_time = 0;
		m_attacked = false;
	}
	void BossFirstSlam2State::Update(float deltatime) {
		m_time += deltatime;

		auto boss = dynamic_pointer_cast<BossFirst>(_obj.lock());

		//çUåÇîªíËÇÃíËã`
		if (m_time >= m_attackTime && !m_attacked) {
			m_attacked = !m_attacked;
			boss->PlaySnd(L"Enemy_Slam", 1.0f, 0);
			
			//10ÇÕÇ∏ÇÁÇ∑íºê¸ãóó£
			Vec3 pos = boss->GetPosition() + boss->GetForward() * 10 + Vec3(0, 2, 0);
			boss->GetStage()->AddGameObject<BossFirstShockwave>(pos, Vec3(0.0f), Vec3(1.0f), boss);
		}


		if (m_time >= m_endTime) {
			boss->ChangeState(L"SlamEnd");
		}
	}
	void BossFirstSlam2State::Exit() {

	}

	void BossFirstSlamEndState::Enter() {
		auto boss = dynamic_pointer_cast<BossFirst>(_obj.lock());
		boss->ChangeAnim(L"AttackSlamEnd", true);
		m_time = 0;
	}
	void BossFirstSlamEndState::Update(float deltatime) {
		m_time += deltatime;

		if (m_time >= m_endTime) {
			auto boss = dynamic_pointer_cast<BossFirst>(_obj.lock());
			boss->ChangeState(L"Stand");
		}
	}
	void BossFirstSlamEndState::Exit() {

	}

	void BossFirstHitState::Enter() {
		auto p = dynamic_pointer_cast<BossFirst>(_obj.lock());
		p->ChangeAnim(L"HitBack", true);
		p->HitBack();
	}
	void BossFirstHitState::Update(float deltatime) {
		auto p = dynamic_pointer_cast<BossFirst>(_obj.lock());
		p->HitBackStandBehavior();
	}
	void BossFirstHitState::Exit() {

	}

	void BossFirstStunState::Enter() {
		auto p = dynamic_pointer_cast<BossFirst>(_obj.lock());
		p->ChangeAnim(L"KnockedDown", true);
		p->HitBack();
		m_time = 0;
	}
	void BossFirstStunState::Update(float deltatime) {
		m_time += deltatime;

		if (m_time > m_downTime) {
			auto p = dynamic_pointer_cast<BossFirst>(_obj.lock());
			p->ChangeState(L"StunRecover");
		}
	}
	void BossFirstStunState::Exit() {

	}

	void BossFirstStunRecoverState::Enter() {
		auto p = dynamic_pointer_cast<BossFirst>(_obj.lock());
		p->ChangeAnim(L"WakeUp", true);
		p->HitBack();
		m_time = 0;
	}
	void BossFirstStunRecoverState::Update(float deltatime) {
		m_time += deltatime;

		if (m_time > m_recoverTime) {
			auto p = dynamic_pointer_cast<BossFirst>(_obj.lock());
			p->ChangeState(L"Stand");
		}

	}
	void BossFirstStunRecoverState::Exit() {

	}

	void BossFirstDeadState::Enter() {
		auto boss = dynamic_pointer_cast<BossFirst>(_obj.lock());
		boss->ChangeAnim(L"KnockedDown", true);
		boss->HitBack();

		boss->PlaySnd(L"Enemy_Defeat", 1.0f, 0);
	}
	void BossFirstDeadState::Update(float deltatime) {

	}
	void BossFirstDeadState::Exit() {

	}

	void BossFirstBonusState::Enter() {
		auto boss = dynamic_pointer_cast<BossFirst>(_obj.lock());
		boss->ChangeAnim(L"Bonus", false);

		m_time = 0;
		m_sndPlayed = false;
	}
	void BossFirstBonusState::Update(float deltatime) {
		m_time += deltatime;

		auto boss = dynamic_pointer_cast<BossFirst>(_obj.lock());

		if (m_time >= m_sndTime && !m_sndPlayed) {
			boss->PlaySnd(L"Enemy_Slash", 1.0f, 0);
			m_sndPlayed = true;
		}

		if (m_time >= m_end) {
			boss->ChangeState(L"Stand");
		}
	}
	void BossFirstBonusState::Exit() {

	}
}
//end basecross
