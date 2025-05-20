/*!
@file ZakoState.cpp
@brief ŽG‹›‚ĚXe[g
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	/*

	//-------------------------------------------------
	// ’ĘŹíŽG‹›“G‚ĚXe[gć“Ş
	//-------------------------------------------------

	//‰˝‚ŕ‚Č‚˘‚Ć‚«‚ĚXe[g
	void EnemyZakoStandState::Enter()
	{
		m_enemyZako->ChangeAnim(L"Stand");//—§‚ÂAj[V‡“‚É•ĎŤX
	}
	void EnemyZakoStandState::Update(float deltaTime)
	{
		auto stage = m_enemyZako->GetStage();
		auto attackType = m_enemyZako->GetAttackType();

		auto isLand = m_enemyZako->GetLand();//’…’n‚µ‚Ä‚˘‚é‚©‚Ět‰O

		//ŽG‹›“G‚Ě^Cv‚É‚ć‚Á‚ÄŤUŚ‚‚Ě•ű–@‚Ş•Ď‚í‚é
		//‰“‹——Ł
		m_timeOfShot += deltaTime;
		if (attackType == m_enemyZako->Zako_Long && isLand)
		{
			m_enemyZako->ChangeState(L"Alignment");//Ž˛Ť‡‚í‚ą‚©‚çŽn‚Ü‚é
		}
		//‹ß‹——Ł
		if (attackType == m_enemyZako->Zako_Melee && isLand)
		{
			m_enemyZako->ChangeState(L"PreparationforMelee");//Ú‹ß‚µ‚ÄŤUŚ‚‚·‚é
		}
	}
	void EnemyZakoStandState::Exit()
	{
		//‘Ĺ‚ÂJE“g_E“ŠZbg
		m_timeOfShot = 0.0f;
	}

	//Ú‹ßí‚đ‚·‚é‚Ć‚«‚ĚXe[g
	void EnemyZakoMeleeState::Enter()
	{
		auto LandFlag = m_enemyZako->GetLand();
		auto testVector = m_enemyZako->GetVelocity();

		//ŤUŚ‚‚Á‚Ű‚˘Aj[V‡“‚É‚µ‚Ä‚Ý‚é
		m_enemyZako->ChangeAnim(L"Down");
		//m_enemyZako->ChangeAnim(L"Walk");//•ŕ‚­Aj[V‡“‚É•ĎŤX

		//ŤUŚ‚‚µ‚Ä‚˘‚é^O’Ç‰Á
		m_enemyZako->AddTag(L"AttackNow");
	}
	void EnemyZakoMeleeState::Update(float deltaTime)
	{
		auto stage = m_enemyZako->GetStage();

		m_timeOfAttack += deltaTime;

		//Aj[V‡“ŤXVŽžŠÔÝ’č
		m_enemyZako->SetAddTimeAnimation(deltaTime * 2.5f);

		//ŤUŚ‚”»’č‚Ě¶¬
		if (m_timeOfAttack >= m_timeOfAttackAdd && m_Attack)
		{
			auto tmp = m_enemyZako->GetAttackPtr()->GetHitInfo();
			tmp.HitOnce = true;
			tmp.Damage = 5;
			tmp.HitVel_Stand = Vec3(-3, 5, 0);
			tmp.HitTime_Stand = .8f;
			tmp.Type = AttackType::Enemy;
			//tmp.ForceRecover = false;//mbNobN‚·‚é
			m_enemyZako->DefAttack(.5f, tmp);
			m_enemyZako->GetAttackPtr()->SetPos(Vec3(3, 1, 0));
			auto AttackPtr = m_enemyZako->GetAttackPtr();
			AttackPtr->GetComponent<Transform>()->SetScale(Vec3(3.7f, 3.0f, 3.0f));
			AttackPtr->SetCollScale(1.0f);

			m_enemyZako->SetAttackFlag(false);//ŤUŚ‚”»’č‚Ş•ˇ””­¶‚ł‚ą‚Č‚˘‚ć‚¤‚É‚·‚é
			m_Attack = false;//ŤUŚ‚”»’č‚Ş•ˇ””­¶‚ł‚ą‚Č‚˘‚ć‚¤‚É‚·‚é
		}

		//ę’čŽžŠÔ‚˝‚Á‚˝‚çŤUŚ‚Xe[g‚đ‚â‚ß‚é
		if (m_timeOfAttack >= m_timeMaxOfAttack)
		{
			m_enemyZako->ChangeState(L"Stand");
		}
	}
	void EnemyZakoMeleeState::Exit()
	{
		m_Attack = true;
		m_timeOfAttack = 0.0f;

		//ŤUŚ‚‚µ‚Ä‚˘‚é^OŤíŹś
		m_enemyZako->RemoveTag(L"AttackNow");
	}

	//Ú‹ßí‚đ‚·‚é‚Ć‚«‚ĚŹ€”őXe[g(ŤUŚ‚‚Ĺ‚«‚é‹——Ł‚É‚Č‚é‚Ü‚Ĺ‹ß‚Ă‚­)
	void EnemyZakoPreparationforMeleeState::Enter()
	{
		m_enemyZako->ChangeAnim(L"Walk");//•ŕ‚­Aj[V‡“‚É•ĎŤX

		//Ź‰Šú‚ĚvŚC„[‚Ć‚Ě‹——Ł‚É‚ć‚Á‚Ä‘«‚Ě‘¬‚ł‚đ•Ď‚¦‚é
		auto playerdist = m_enemyZako->GetPlayerDist();
		SppedChange();

		//fobN—p
		auto test = m_speed;
		auto a = 0;
	}
	void EnemyZakoPreparationforMeleeState::Update(float deltaTime)
	{
		//vŚC„‚Ć‚Ě‹——Ł‚É‚ć‚Á‚ÄXs[h‚đ•Ď‚¦‚é
		SppedChange();

		auto stage = m_enemyZako->GetStage();
		auto meleeRange = 8.0f;//Ú‹ßŤUŚ‚—LŚř”ÍÍ

		//Player‚Ě•űŚü‚É‰ń“]‚·‚é
		auto PushAngle = XM_PIDIV4 / 4;//‰ń“]‚Ě‚¸‚ę
		m_enemyZako->RotateToPlayer(1.0f, PushAngle);

		auto attackFlag = m_enemyZako->GetAttackFlag();//ŤUŚ‚t‰O‚đŽó‚ŻŽć‚é

		//ŤUŚ‚‚ĚN[‹^C€‚đ‰ß‚¬‚Ä‚˘‚ę‚ÎÚ‹ß‚»‚¤‚Ĺ‚Č‚Ż‚ę‚Î—Ł‚ę‚é
		if (attackFlag)
		{
			//—LŚř”ÍÍ‚Ü‚Ĺ‹ß‚Ă‚Ż‚˝‚ç‹ßÚŤUŚ‚‚đ‚·‚é‚»‚¤‚Ĺ‚Č‚Ż‚ę‚ÎA‚»‚±‚Ü‚ĹÚ“®
			if (m_enemyZako->GetPlayerDist() < meleeRange)
			{
				//ŤUŚ‚‚Ě‚˝‚ß‚É—§‚żŽ~‚Ü‚é‚Ě‚Ĺ—§‚ÂAj[V‡“‚É•ĎŤX
				m_enemyZako->ChangeAnim(L"Stand");

				//ŤUŚ‚t‰O‚ŞI“‚Č‚çŤUŚ‚‚Ĺ‚«‚é
				if (!attackFlag) return;
				m_enemyZako->ChangeState(L"Melee");
			}
			else if (m_enemyZako->GetPlayerDist() >= meleeRange)
			{
				//Ú“®’†‚Č‚Ě‚Ĺ‚»‚ę‚ÉŤ‡‚í‚ą‚˝Aj[V‡“
				m_enemyZako->ChangeAnim(L"Walk");

				//i‚Ţ‹——Ł‚đŚ‚ß‚é
				auto move = m_enemyZako->GetForward() * m_speed;

				auto LandFlag = m_enemyZako->GetLand();
				if (LandFlag)
				{
					move.y = 0.0f;
				}

				m_enemyZako->SetVelocity(move);
				//Aj[V‡“ŤXVŽžŠÔÝ’č
				m_enemyZako->SetAddTimeAnimation(deltaTime * 2.5f);
			}
		}
		else if (!attackFlag)//vŚC„[‚©‚ç‹——Ł‚đŽć‚é
		{
			if (m_enemyZako->GetPlayerDist() < meleeRange * 3.0f)
			{
				//Ú“®’†‚Č‚Ě‚Ĺ‚»‚ę‚ÉŤ‡‚í‚ą‚˝Aj[V‡“
				m_enemyZako->ChangeAnim(L"Walk");

				//i‚Ţ‹——Ł‚đŚ‚ß‚é
				auto move = m_enemyZako->GetForward() * -(m_speed * 0.8);

				auto LandFlag = m_enemyZako->GetLand();
				if (LandFlag)
				{
					move.y = 0.0f;
				}

				m_enemyZako->SetVelocity(move);
				//Aj[V‡“ŤXVŽžŠÔÝ’č
				m_enemyZako->SetAddTimeAnimation(deltaTime * 2.5f);
			}

		}


	}
	void EnemyZakoPreparationforMeleeState::Exit()
	{

	}

	//vŚC„[‚Ć‚Ě‹——Ł‚É‚ć‚Á‚Ä‹r‚ĚXs[h‚đ•Ď‚¦‚éŹ—ť
	void EnemyZakoPreparationforMeleeState::SppedChange()
	{
		//‹——Ł‚É‚ć‚Á‚ÄXs[h‚đ•Ď‚¦‚é
		auto playerdist = m_enemyZako->GetPlayerDist();
		if (playerdist > 50.0f)//‰“
		{
			m_speed = 15.0f;
		}
		else if (playerdist > 30.0f)//’†
		{
			m_speed = 7.0f;
		}
		else//‹ß‚˘
		{
			m_speed = 10.0f;
		}
	}


	//‹…‚đ‘Ĺ‚Â’Ľ‘O‚ĚŽ˛Ť‡‚í‚ą‚ĚXe[g
	void EnemyZakoAlignmentState::Enter()
	{

	}
	void EnemyZakoAlignmentState::Update(float deltaTime)
	{
		auto stage = m_enemyZako->GetStage();

		//Player‚Ě•űŚü‚É‰ń“]‚·‚é
		auto PushAngle = XM_PIDIV4 / 4;//‰ń“]‚Ě‚¸‚ę
		m_enemyZako->RotateToPlayer(1.0f, PushAngle);

		//—LŚř”ÍÍ‚Ü‚ĹÚ‹ß‚·‚éAvŚC„[‚É‚ ‚é’ö“x‹ß‚Ă‚©‚ę‚˝‚ç‹——Ł‚đŽć‚é
		auto Range = 25.0f;
		if (m_enemyZako->GetPlayerDist() >= Range)//‹ß‚Ă‚­
		{
			//Ú“®’†‚Č‚Ě‚Ĺ‚»‚ę‚ÉŤ‡‚í‚ą‚˝Aj[V‡“
			m_enemyZako->ChangeAnim(L"Walk");

			//i‚Ţ‹——Ł‚đŚ‚ß‚é
			m_speed = 10.0f;
			auto move = m_enemyZako->GetForward() * m_speed;

			auto LandFlag = m_enemyZako->GetLand();
			if (LandFlag)
			{
				move.y = 0.0f;
			}

			m_enemyZako->SetVelocity(move);
			//Aj[V‡“ŤXVŽžŠÔÝ’č
			m_enemyZako->SetAddTimeAnimation(deltaTime * 2.5f);
		}
		if (m_enemyZako->GetPlayerDist() <= Range - 5.0f)//—Ł‚ę‚é
		{
			//Ú“®’†‚Č‚Ě‚Ĺ‚»‚ę‚ÉŤ‡‚í‚ą‚˝Aj[V‡“
			m_enemyZako->ChangeAnim(L"Walk");

			//i‚Ţ‹——Ł‚đŚ‚ß‚é
			m_speed = -7.0f;
			auto move = m_enemyZako->GetForward() * m_speed;

			auto LandFlag = m_enemyZako->GetLand();
			if (LandFlag)
			{
				move.y = 0.0f;
			}

			m_enemyZako->SetVelocity(move);
			//Aj[V‡“ŤXVŽžŠÔÝ’č(Śă‚ë‚Éi‚Ţ‚Ě‚Ĺ‹tŤÄ¶‚É‚µ‚˝‚˘)
			m_enemyZako->SetAddTimeAnimation(deltaTime * 2.5f);
		}

		//ę’čŽžŠÔ‚˝‚Á‚˝‚çŤUŚ‚‚·‚é
		m_timeOfShot += deltaTime;
		if (m_timeOfShot >= m_timeMaxOfShot)
		{
			m_timeOfShot = 0.0f;//ŠZbg
			m_enemyZako->ChangeState(L"Shot");//‘Ĺ‚ÂXe[g‚Ş‚Č‚˘‚Ě‚ĹR“gAEg
		}
	}
	void EnemyZakoAlignmentState::Exit()
	{
		//‘Ĺ‚ÂJE“g_E“ŠZbg
		m_timeOfShot = 0.0f;
	}

	//ŤUŚ‚‚đ‚·‚éXe[g(‰“‹——Ł)
	void EnemyZakoShotState::Enter()
	{
		auto stage = m_enemyZako->GetStage();
		auto posEnemy = m_enemyZako->GetPosition();
		//’e¶¬
		auto bullet = stage->AddGameObject<Bullet>(posEnemy, Vec3(0.0f), Vec3(0.4f), 30.0f,
			dynamic_pointer_cast<Actor>(m_enemyZako), 30.0f, ActorName_Enemy);

		m_enemyZako->ChangeAnim(L"Shot");//Ś‚‚ÂAj[V‡“‚É•ĎŤX
	}
	void EnemyZakoShotState::Update(float deltaTime)
	{
		auto stage = m_enemyZako->GetStage();

		////–Ú•W‚Ć‚Č‚éŠp“xŽć“ľ
		auto angleTarget = m_enemyZako->GetPlayerSubDirection();

		//Aj[V‡“ŤXVŽžŠÔÝ’č
		m_enemyZako->SetAddTimeAnimation(deltaTime);

		//ę’čŽžŠÔ‚˝‚Á‚˝‚çStandXe[g‚É–ß‚é
		m_timeOfAttack += deltaTime;
		if (m_timeOfAttack >= m_timeMaxOfAttack)
		{
			m_enemyZako->ChangeState(L"Stand");
		}
	}
	void EnemyZakoShotState::Exit()
	{
		//‘Ĺ‚ÂJE“g_E“ŠZbg
		m_timeOfAttack = 0.0f;
	}


	//_[W‚đŽó‚Ż‚˝Xe[g
	void EnemyZakoHitState::Enter()
	{
		auto hitInfo = m_enemyZako->GetHitInfo();
		auto HPNow = m_enemyZako->GetHPCurrent();
		//ŤUŚ‚‚đŽó‚Ż‚˝‚Ě‚ĹqbgobN‚·‚é
		m_enemyZako->HitBack();
		//_[WŹ—ť
		m_enemyZako->SetHPCurrent(HPNow - hitInfo.Damage);

		m_enemyZako->ChangeAnim(L"Stand");//_[W‚đŽó‚Ż‚˝Aj[V‡“‚É•ĎŤX
	}
	void EnemyZakoHitState::Update(float deltaTime)
	{
		//ę’čŽžŠÔ‚˝‚Á‚˝‚çStandXe[g‚É–ß‚é
		m_enemyZako->HitBackStandBehavior();

		//Aj[V‡“ŤXVŽžŠÔÝ’č
		m_enemyZako->SetAddTimeAnimation(deltaTime);
	}
	void EnemyZakoHitState::Exit()
	{

	}
	//-------------------------------------------------
	// ’ĘŹíŽG‹›“G‚ĚXe[gŹI’[
	//-------------------------------------------------


	//-------------------------------------------------
	// ”ň‚ÔUR‚ĚXe[gć“Ş
	// ------------------------------------------------

	// ‰˝‚ŕ‚Č‚˘‚Ć‚«‚ĚXe[g----------------------
	void EnemyZakoFlyingStandState::Enter()
	{
		m_enemyZako->ChangeAnim(L"Stand");//‘Ň‹@Aj[V‡“‚É•ĎŤX
	}

	void EnemyZakoFlyingStandState::Update(float deltatime)
	{
		auto stage = m_enemyZako->GetStage();
		auto attackType = m_enemyZako->GetAttackType();

		auto isLand = m_enemyZako->GetLand();//’…’n(‘Ř‹óH)‚µ‚Ä‚˘‚é‚©‚Ět‰O

	}

	void EnemyZakoFlyingStandState::Exit()
	{

	}
	// END-----------------------------------------

	//Ú‹ßí‚đ‚·‚é‚Ć‚«‚ĚŹ€”őXe[g-----------------
	void EnemyZakoFlyingPreparationforMeleeState::Enter()
	{
		m_enemyZako->ChangeAnim(L"Walk");//•ŕ‚­Aj[V‡“‚É•ĎŤX
	}

	void EnemyZakoFlyingPreparationforMeleeState::Update(float deltaTime)
	{
		auto stage = m_enemyZako->GetStage();


		auto meleeRange = 10.0f;//Ú‹ßŤUŚ‚—LŚř”ÍÍ

		//Player‚Ě•űŚü‚É‰ń“]‚·‚é
		auto PushAngle = XM_PIDIV4 / 4;//‰ń“]‚Ě‚¸‚ę
		m_enemyZako->RotateToPlayer(1.0f, PushAngle);

		auto attackFlag = m_enemyZako->GetAttackFlag();
		//—LŚř”ÍÍ‚Ü‚Ĺ‹ß‚Ă‚Ż‚˝‚ç‹ßÚŤUŚ‚‚đ‚·‚é‚»‚¤‚Ĺ‚Č‚Ż‚ę‚ÎA‚»‚±‚Ü‚ĹÚ“®
		if (m_enemyZako->GetPlayerDist() < meleeRange)
		{
			//ŤUŚ‚‚Ě‚˝‚ß‚É—§‚żŽ~‚Ü‚é‚Ě‚Ĺ—§‚ÂAj[V‡“‚É•ĎŤX
			m_enemyZako->ChangeAnim(L"Stand");

			//ŤUŚ‚t‰O‚ŞI“‚Č‚çŤUŚ‚‚Ĺ‚«‚é
			if (!attackFlag) return;
			m_enemyZako->ChangeState(L"Melee");
		}
		else if (m_enemyZako->GetPlayerDist() >= meleeRange)
		{
			//Ú“®’†‚Č‚Ě‚Ĺ‚»‚ę‚ÉŤ‡‚í‚ą‚˝Aj[V‡“
			m_enemyZako->ChangeAnim(L"Walk");

			//i‚Ţ‹——Ł‚đŚ‚ß‚é
			auto move = m_enemyZako->GetForward() * 10.0f;

			auto LandFlag = m_enemyZako->GetLand();
			if (LandFlag)
			{
				move.y = 0.0f;
			}

			m_enemyZako->SetVelocity(move);
			//Aj[V‡“ŤXVŽžŠÔÝ’č
			m_enemyZako->SetAddTimeAnimation(deltaTime * 2.5f);
		}
	}

	void EnemyZakoFlyingPreparationforMeleeState::Exit()
	{

	}
	// END--------------------------------------------

	// ŤUŚ‚‚đ‚·‚é‚Ć‚«‚ĚXe[g(‹ß‹——Ł)-----
	void EnemyZakoFlyingMeleeState::Enter()
	{
		auto LandFlag = m_enemyZako->GetLand();
		auto testVector = m_enemyZako->GetVelocity();

		//ŤUŚ‚‚Á‚Ű‚˘Aj[V‡“‚É‚µ‚Ä‚Ý‚é
		m_enemyZako->ChangeAnim(L"Down");
		//m_enemyZako->ChangeAnim(L"Walk");//•ŕ‚­Aj[V‡“‚É•ĎŤX

	}

	void EnemyZakoFlyingMeleeState::Update(float deltaTime)
	{
		auto stage = m_enemyZako->GetStage();

		m_timeOfAttack += deltaTime;

		//Aj[V‡“ŤXVŽžŠÔÝ’č
		m_enemyZako->SetAddTimeAnimation(deltaTime * 2.5f);

		//ŤUŚ‚”»’č‚Ě¶¬
		if (m_timeOfAttack >= m_timeOfAttackAdd && m_Attack)
		{
			auto tmp = m_enemyZako->GetAttackPtr()->GetHitInfo();
			tmp.HitOnce = true;
			tmp.Damage = 5;
			tmp.HitVel_Stand = Vec3(-3, 5, 0);
			tmp.HitTime_Stand = .8f;
			tmp.Type = AttackType::Enemy;
			//tmp.ForceRecover = false;//mbNobN‚·‚é
			m_enemyZako->DefAttack(.5f, tmp);
			m_enemyZako->GetAttackPtr()->SetPos(Vec3(3, 1, 0));
			auto AttackPtr = m_enemyZako->GetAttackPtr();
			AttackPtr->GetComponent<Transform>()->SetScale(Vec3(3.7f, 3.0f, 3.0f));
			AttackPtr->SetCollScale(1.0f);

			m_enemyZako->SetAttackFlag(false);//ŤUŚ‚”»’č‚Ş•ˇ””­¶‚ł‚ą‚Č‚˘‚ć‚¤‚É‚·‚é
			m_Attack = false;//ŤUŚ‚”»’č‚Ş•ˇ””­¶‚ł‚ą‚Č‚˘‚ć‚¤‚É‚·‚é
		}

		//ę’čŽžŠÔ‚˝‚Á‚˝‚çŤUŚ‚Xe[g‚đ‚â‚ß‚é
		if (m_timeOfAttack >= m_timeMaxOfAttack)
		{
			m_enemyZako->ChangeState(L"Stand");
		}

	}

	void EnemyZakoFlyingMeleeState::Exit()
	{

	}
	// END-----------------------------------

	// ‰“‹——Ł‚Ě’Ľ‘O‚ĚŽ˛Ť‡‚í‚ą‚ĚŽž‚ĚXe[g-----
	void EnemyZakoFlyingAlignmentState::Enter()
	{

	}

	void EnemyZakoFlyingAlignmentState::Update(float deltaTime)
	{
		auto stage = m_enemyZako->GetStage();

		//–Ú•W‚Ć‚Č‚éŠp“xŽć“ľ
		//auto angleTarget = m_enemyZako->GetPlayerSubDirection();
		//auto angleTarget = atan2f(targetVec.z, targetVec.x);


		//Player‚Ě‚˘‚é•űŚü‚đŚvŽZ
		auto playerPos = stage->GetSharedGameObject<Player>(L"Player")->GetPosition();
		auto EnemyPos = m_enemyZako->GetPosition();
		auto targetVec = playerPos - EnemyPos;
		auto angleTarget = (atan2f(targetVec.z, -targetVec.x) / 2);
		angleTarget += XMConvertToRadians(90.0f);

		//‘ĺ‘Ě‚»‚Ě•űŚü‚ÉŚü‚˘‚˝‚çPlayer‚Ě•űŚü‚ÉŚü‚˘‚Ä‚˘‚é‚Ć‚Ý‚Č‚·
		if (abs(angleTarget) < XMConvertToRadians(3.0f))
		{
			angleTarget = 0.0f;
		}
		//‰ń“]Ź—ť
		if (angleTarget != 0.0f)
		{
			auto qt = m_enemyZako->GetComponent<Transform>()->GetBeforeQuaternion();
			qt.y = 0.0f;
			////qt.y += angleTarget;
			qt = qt * Quat(0.0f, sin(angleTarget) / 2.0f, 0.0f, cos(angleTarget) / 2.0f);
			m_enemyZako->GetComponent<Transform>()->SetQuaternion(qt);
		}


		//ę’čŽžŠÔ‚˝‚Á‚˝‚çŤUŚ‚‚·‚é
		m_timeOfShot += deltaTime;
		if (m_timeOfShot >= m_timeMaxOfShot)
		{
			m_timeOfShot = 0.0f;//ŠZbg
			m_enemyZako->ChangeState(L"Shot");//‘Ĺ‚ÂXe[g‚Ş‚Č‚˘‚Ě‚ĹR“gAEg
		}

	}

	void EnemyZakoFlyingAlignmentState::Exit()
	{
		//‘Ĺ‚ÂJE“g_E“ŠZbg
		m_timeOfShot = 0.0f;

	}
	// END--------------------------------------

	// ŤUŚ‚‚đ‚·‚é‚Ć‚«‚ĚXe[g(‰“‹——Ł)-----
	void EnemyZakoFlyingShotState::Enter()
	{
		auto stage = m_enemyZako->GetStage();
		auto posEnemy = m_enemyZako->GetPosition();
		//’e¶¬
		auto bullet = stage->AddGameObject<Bullet>(posEnemy, Vec3(0.0f), Vec3(0.4f), 10.0f,
			dynamic_pointer_cast<Actor>(m_enemyZako), 10.0f, ActorName_Enemy);

		m_enemyZako->ChangeAnim(L"Shot");//Ś‚‚ÂAj[V‡“‚É•ĎŤX

	}

	void EnemyZakoFlyingShotState::Update(float deltaTime)
	{
		auto stage = m_enemyZako->GetStage();

		////–Ú•W‚Ć‚Č‚éŠp“xŽć“ľ
		auto angleTarget = m_enemyZako->GetPlayerSubDirection();

		//Aj[V‡“ŤXVŽžŠÔÝ’č
		m_enemyZako->SetAddTimeAnimation(deltaTime);

		//ę’čŽžŠÔ‚˝‚Á‚˝‚çStandXe[g‚É–ß‚é
		m_timeOfAttack += deltaTime;
		if (m_timeOfAttack >= m_timeMaxOfAttack)
		{
			m_enemyZako->ChangeState(L"Stand");
		}

	}

	void EnemyZakoFlyingShotState::Exit()
	{
		//‘Ĺ‚ÂJE“g_E“ŠZbg
		m_timeOfAttack = 0.0f;

	}
	// END-----------------------------------

	//_[W‚đŽó‚Ż‚˝‚Ć‚«-------------------
	void EnemyZakoFlyingHitState::Enter()
	{
		auto hitInfo = m_enemyZako->GetHitInfo();
		auto HPNow = m_enemyZako->GetHPCurrent();

		// _[WŹ—ť
		m_enemyZako->SetHPCurrent(HPNow - hitInfo.Damage);

		// _[W‚đŽó‚Ż‚˝Aj[V‡“‚É•ĎŤX
		m_enemyZako->ChangeAnim(L"Stand");
	}

	void EnemyZakoFlyingHitState::Update(float deltaTime)
	{
		//ę’čŽžŠÔ‚˝‚Á‚˝‚çStandXe[g‚É–ß‚é
		m_enemyZako->HitBackStandBehavior();

		//Aj[V‡“ŤXVŽžŠÔÝ’č
		m_enemyZako->SetAddTimeAnimation(deltaTime);

	}

	void EnemyZakoFlyingHitState::Exit()
	{

	}
	// END------------------------------------

	//-------------------------------------------------------
	// ”ň‚ÔUR‚ĚXe[gŹI’[
	//-------------------------------------------------------

	*/

}