/*!
@file EnemyZako.h
@brief �G���G
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void EnemyZako::OnCreate()
	{
		Actor::OnCreate();

		//�X�e�[�^�X������
		m_HPMax = 130.0f;
		m_HPCurrent = m_HPMax;
		m_armorMax = 30.0f;
		m_armor = m_armorMax;
		m_armorRecoverTime = 10.0f;
		m_armorRecover = 0.0f;
		m_stunMax = 1;

		//Transform�ݒ�
		m_trans = GetComponent<Transform>();
		SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(1.0f / 5, 1.0f / 5, 1.0f / 5),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		//�h���[���b�V���̐ݒ�
		auto ptrDraw = GetComponent<PNTBoneModelDraw>();

		//�U���^�C�v�ɂ���Č����ڂ��ς��
		if (m_AttackType == Zako_Long)
		{
			ptrDraw->SetMeshResource(L"Enemy_A");
		}
		if (m_AttackType == Zako_Melee)
		{
			ptrDraw->SetMeshResource(L"Enemy_C");
		}
		ptrDraw->SetDiffuse(Col4(1.0f));
		ptrDraw->SetSamplerState(SamplerState::LinearWrap);
		ptrDraw->SetMeshToTransformMatrix(spanMat);

		//�A�j���[�V�����ǉ�(�U���^�C�v�ɂ���Ēǉ��A�j���[�V�������ς��)
		ptrDraw->AddAnimation(L"Stand", 0, 1, 24.0f);
		ptrDraw->AddAnimation(L"Walk", 0, 224, 24.0f);
		ptrDraw->AddAnimation(L"Shot", 225, 136, false, 24.0f);
		ptrDraw->AddAnimation(L"Down", 362, 62, false, 24.0f);
		ptrDraw->AddAnimation(L"Hit", 350, 11, false, 24.0f);
		ptrDraw->AddAnimation(L"Stan", 320, 19, false, 24.0f);
		if (m_AttackType == Zako_Melee)
		{
			ptrDraw->AddAnimation(L"Melee_Jamp", 625, 74, false, 24.0f);
			ptrDraw->AddAnimation(L"Melee_Scratch", 700, 22, false, 24.0f);
		}

		//�e������i�V���h�E�}�b�v��`�悷��j
		auto shadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		shadowPtr->SetMeshResource(L"Enemy_A");
		shadowPtr->SetMeshToTransformMatrix(spanMat);


		//�R���W�����쐬
		auto ptrColl = AddComponent<CollisionSphere>();//�R���W�����X�t�B�A�̕����ǂɂԂ��锻��Ɉ�a�����Ȃ�
		ptrColl->SetAfterCollision(AfterCollision::Auto);
		ptrColl->SetDrawActive(false);//�f�o�b�N�p

		AddTag(L"Enemy");
		AddTag(L"EnemyZako");

		m_player = GetStage()->GetSharedGameObject<Player>(L"Player");

		//�ڒn����̐ݒ�
		m_LandDetect->SetBindPos(Vec3(0, -2.0f, 0));
		m_LandDetect->GetComponent<CollisionSphere>()->SetMakedRadius(1.0f);
		m_LandDetect->GetComponent<Transform>()->SetScale(Vec3(1.0f, 1.0f, 1.0f));
		//m_LandDetect->SetCollScale(3.0f);

		//�X�e�[�g�}�V������
		m_state = shared_ptr<EnemyZakoStateMachine>(new EnemyZakoStateMachine(GetThis<GameObject>()));

		//�����HP�o�[��\��������		
		m_HPBer = GetStage()->AddGameObject<BillBoardGauge>(GetThis<GameObject>(), L"ZakoHPMater", 3, 5.0f, Vec3(2.0f, 0.5f, 5.0f));
		m_HPFrame = GetStage()->AddGameObject<BillBoard>(GetThis<GameObject>(), L"ZakoGaugeFrame", 3, 5.0f, Vec3(2.0f, 0.5f, 5.0f));
		m_HPBer->SetPercent(1.0f);

		SetAlphaActive(true);
	}

	void EnemyZako::OnUpdate()
	{
		//�����|�[�Y�t���O���I���ł���΃A�b�v�f�[�g�����͏o���Ȃ��Ȃ�
		if (m_PauseFlag)
		{
			return;
		}
		//�����A�g��Ȃ���Ԃ���g����ԂɕύX���ꂽ�������o�ϐ��̏�����������	
		if (!m_beforUsed)
		{
			if (m_used)
			{
				// ������
				Initialize();	
				GetComponent<PNTBoneModelDraw>()->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 1.0f));
				GetComponent<Shadowmap>()->SetDrawActive(true);
			}
		}
		if (m_beforUsed)
		{
			if (!m_used)
			{
				auto stage = GetStage();
				auto pos = GetComponent<Transform>()->GetPosition();
				stage->GetSharedGameObject<PartsManager>(L"PartsManager")->PartsDrop(pos);
			}
		}
		//���݂̎g�p�󋵂ƌ���ׂĕς���Ă��Ȃ�������
		m_beforUsed = m_used;

		////�A�[�}�[��
		//if (m_armorMax != 0 && m_armor <= 0) 
		//{
		//	m_armorRecoverCountTime += _delta;
		//	if (m_armorRecoverTime <= m_armorRecoverCountTime) 
		//	{
		//		m_armor = m_armorMax;
		//		m_armorRecoverCountTime = 0;
		//	}
		//}

		EnemyBase::OnUpdate();

		//���n����(���������Ԓ��Ȃ炻������Z����)
		OnLanding();
		//HP�o�[�̏���
		UpdateHPBer();
		//�U���̃N�[���^�C��
		TimeOfAttackCool();

		//HP���[���ɂȂ���������邽�߂̏���������
		if (m_HPCurrent <= 0)
		{
			RemoveTag(L"LockOnCan");
			RemoveTag(L"LockOnTarget");

			//m_used = false;
		}

		//�A�j���[�V�����X�V
		GetComponent<PNTBoneModelDraw>()->UpdateAnimation(m_addTimeAnimation);
		//�ʒu�X�V
		//SpeedLimit(3.0f); //�X�s�[�h���~�b�g
		UpdatePosition();
	}

	//����������
	void EnemyZako::Initialize()
	{
		m_HPCurrent = m_HPMax;
		m_attackFlag = false;
		m_timeCountOfAttackCool = 3.0f;
		//�����X�e�[�g�ɖ߂�
		ChangeState(L"Stand");
	}

	void EnemyZako::CreateDamageBill(shared_ptr<GameObject> actorPtr, int damage, float pushY, float scale, float displayTime)
	{
		string dmgStr = to_string(damage);                    // �󂯎�����_���[�W�̕�����(30�ł���΁u3�v�Ɓu0�v��2����)
		int len = (int)dmgStr.size();                        // �_���[�W�̌���
		float charWidth = scale;                             // �e�X�v���C�g��
		float totalW = charWidth * len;                        // �S�̕�
		float startX = -totalW / 2.0f + charWidth / 2.0f;   // ���[�
		for (int i = 0; i < len; i++)
		{
			float offsetX = startX + i * charWidth;

			auto damageBill = GetStage()->AddGameObject<DamageBill>(
				actorPtr,                    // �e�r���{�[�h�͓G�ɂ�������
				L"DamageNumbers",                  // �e�N�X�`����
				3,                           // ���C���[
				pushY,                       // Y���I�t�Z�b�g
				Vec3(scale),				 // �T�C�Y
				Col4(1, 1, 1, 1),            // �F (���F�Ȃ�)
				offsetX,                     // ���E�I�t�Z�b�g
				displayTime					 // �\������
			);

			// �_���[�W�̕�����ݒ�
			damageBill->SetBillUV(
				Vec2((dmgStr[i] - '0') * 0.1f, 0.0f), // UV�̍���
				Vec2((dmgStr[i] - '0' + 1) * 0.1f, 1.0f) // UV�̉E��
			);
		}
	}

	//HP�o�[�̏���
	void EnemyZako::UpdateHPBer()
	{
		//�r���{�[�h�̏��� �������g�p����Ă��邩�ǂ����Ńr���{�[�h���o�邩�łȂ������߂�
		if (!m_used)
		{
			m_HPFrame->SetScale(Vec3(0.0f));
			m_HPBer->SetScale(Vec3(0.0f));
		}
		if (m_used)
		{
			m_HPFrame->SetScale(Vec3(2.0f, 0.5f, 5.0f));
			m_HPBer->SetScale(Vec3(2.0f, 0.5f, 5.0f));

			// �B�e�p
			//m_HPFrame->SetScale(Vec3(0.0f));
			//m_HPBer->SetScale(Vec3(0.0f));

			//HP�̊����ɂ���ăQ�[�W������
			float HPPercent = (float)m_HPCurrent / (float)m_HPMax;
			m_HPBer->SetPercent(HPPercent);
		}
	}

	//�U���̃N�[���^�C��
	void EnemyZako::TimeOfAttackCool()
	{
		//�U���̃N�[���^�C��
		if (!m_attackFlag)
		{
			m_timeCountOfAttackCool += _delta;
			//�N�[���^�C���߂�����U���ł���悤�ɂȂ�
			if (m_timeCountOfAttackCool >= m_timeOfAttackCool)
			{
				m_timeCountOfAttackCool = 0.0f;//���Z�b�g
				m_attackFlag = true;
			}
		}
	}

	//�_���[�W���󂯂��ۂ̏���
	void EnemyZako::HitBackStandBehavior()
	{
		//�_���[�W���󂯂����HP�ɂ���ăX�e�[�g�̑J�ڂ�ς���
		m_hitbacktime -= _delta;

		//�U�����󂯂��Ƃ��̏���
		if (m_hitbacktime <= 0) 
		{
			ChangeState(L"Stand");
		}
	}

	//�R���W��������
	void EnemyZako::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		DetectBeingAttacked(Other);
	}

	//�폜������
	void EnemyZako::OnDestroy()
	{
		Actor::OnDestroy();

		auto stage = GetStage();
		stage->GetSharedGameObject<PartsManager>(L"PartsManager")->PartsDrop(m_pos);
	}

	//�_���[�W���󂯂��ۂ̏���
	void EnemyZako::OnDamaged()
	{
		float damage = m_getHitInfo.Damage;
		float pushY = 2.5f;
		float scale = 0.7f;
		float displayTime = 0.5f;

		bool isArmorBreak = m_armor > 0;

		//�X�^���l�ւ̃_���[�W
		m_stun += m_getHitInfo.StunDamage;
		bool isStun = m_stun >= m_stunMax;

		m_armor -= CalculateDamage(m_getHitInfo.Damage);

		//hp������Ȃ�_���[�W��������
		if (m_HPCurrent > 0)
		{
			//�A�[�}�[�����邩�Ȃ����Ń_���[�W���������ς��
			if (m_armor <= 0)
			{
				//�A�[�}�[�ϋv�l���Ȃ��Ȃ�����
				if (isArmorBreak)
				{
					AddEffect(EnemyZakoEffect_ArmorBreak);
					//PlaySnd(L"ZakoArmorBreak", 1.0f, 0);
					m_SEManager->Start(L"ZakoArmorBreak", false, 1.0f * m_SEVol);
				}

				CreateDamageBill(GetThis<GameObject>(), damage, pushY, scale, displayTime);
				m_state->ChangeState(L"Hit");
			}
			else
			{
				CreateDamageBill(GetThis<GameObject>(), damage, pushY, scale, displayTime);
				m_HPCurrent -= CalculateDamage(m_getHitInfo.Damage) / 5.0f;
			}

			//�X�^�����̉��o
			if (isStun) {
				AddEffect(EnemyEffect_Stun);
				App::GetApp()->GetXAudio2Manager()->Start(L"ArmorBreak", 0, 0.9f);
				m_stun = 0;

				m_state->ChangeState(L"Stun");
			}

			//���ꏈ���ڍs
			if (m_HPCurrent <= 0)
			{
				m_state->ChangeState(L"Die");
			}

		}
	}

	//�������̎G���G
	void EnemyZakoLong::OnCreate()
	{
		Actor::OnCreate();

		//��������G���G��HP��50�Ƃ���
		m_HPMax = 50.0f;
		m_HPCurrent = m_HPMax;

		//Transform�ݒ�
		m_trans = GetComponent<Transform>();
		SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(1.0f / 5, 1.0f / 5, 1.0f / 5),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		//�h���[���b�V���̐ݒ�
		auto ptrDraw = GetComponent<PNTBoneModelDraw>();
		ptrDraw->SetMeshResource(L"Enemy_A");
		ptrDraw->SetDiffuse(Col4(1.0f));
		ptrDraw->SetSamplerState(SamplerState::LinearWrap);
		ptrDraw->SetMeshToTransformMatrix(spanMat);

		//�e������i�V���h�E�}�b�v��`�悷��j
		auto shadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		shadowPtr->SetMeshResource(L"Enemy_A");
		shadowPtr->SetMeshToTransformMatrix(spanMat);

		//�A�j���[�V�����ǉ�(�U���^�C�v�ɂ���Ēǉ��A�j���[�V�������ς��)
		ptrDraw->AddAnimation(L"Stand", 0, 1, 24.0f);
		ptrDraw->AddAnimation(L"Walk", 0, 224, 24.0f);
		ptrDraw->AddAnimation(L"Shot", 225, 136, 24.0f);
		ptrDraw->AddAnimation(L"Down", 362, 424, 24.0f);
		ptrDraw->AddAnimation(L"Hit", 543, 29, false, 24.0f);
		ptrDraw->AddAnimation(L"Stan", 463, 78, false, 24.0f);

		//�R���W�����쐬
		auto ptrColl = AddComponent<CollisionSphere>();//�R���W�����X�t�B�A�̕����ǂɂԂ��锻��Ɉ�a�����Ȃ�
		ptrColl->SetAfterCollision(AfterCollision::Auto);
		ptrColl->SetDrawActive(false);//�f�o�b�N�p

		AddTag(L"Enemy");
		AddTag(L"EnemyZako");

		m_player = GetStage()->GetSharedGameObject<Player>(L"Player");

		//�ڒn����̐ݒ�
		m_LandDetect->SetBindPos(Vec3(0, -2.0f, 0));
		m_LandDetect->GetComponent<CollisionSphere>()->SetMakedRadius(1.0f);
		m_LandDetect->GetComponent<Transform>()->SetScale(Vec3(1.0f, 1.0f, 1.0f));

		//�X�e�[�g�}�V������
		m_state = shared_ptr<EnemyZakoStateMachine>(new EnemyZakoStateMachine(GetThis<GameObject>()));

		//�����HP�o�[��\��������
		m_HPFrame = GetStage()->AddGameObject<BillBoard>(GetThis<GameObject>(), L"ZakoGaugeFrame", 4, 5.0f, Vec3(2.0f, 0.5f, 5.0f));
		m_HPBer = GetStage()->AddGameObject<BillBoardGauge>(GetThis<GameObject>(), L"BossHPMater", 3, 5.0f, Vec3(2.0f, 0.5f, 5.0f));
		m_HPBer->SetPercent(1.0f);
	}


	void EnemyZakoFlying::OnCreate()
	{
		Actor::OnCreate();

		//��������G���G��HP��50�Ƃ���
		m_HPMax = 50.0f;
		m_HPCurrent = m_HPMax;

		//Transform�ݒ�
		m_trans = GetComponent<Transform>();
		SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

		// �d��OFF
		m_doPhysics = false;


		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(1.0f / 5, 1.0f / 5, 1.0f / 5),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		//�h���[���b�V���̐ݒ�
		auto ptrDraw = GetComponent<PNTBoneModelDraw>();
		//�U���^�C�v�ɂ���Č����ڂ��ς��
		if (m_AttackType == Zako_Long)
		{
			ptrDraw->SetMeshResource(L"Enemy_A");
		}
		if (m_AttackType == Zako_Melee)
		{
			ptrDraw->SetMultiMeshResource(L"Enemy_B");
		}
		ptrDraw->SetDiffuse(Col4(0.5f));
		//ptrDraw->SetEmissive(Col4(1));
		ptrDraw->SetSamplerState(SamplerState::LinearWrap);
		ptrDraw->SetMeshToTransformMatrix(spanMat);
		//ptrDraw->SetTextureResource(L"Tx_Boss1");

		//�e������i�V���h�E�}�b�v��`�悷��j
		auto shadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		shadowPtr->SetMeshResource(L"Enemy_C");
		shadowPtr->SetMeshToTransformMatrix(spanMat);


		//�A�j���[�V�����ǉ�(�U���^�C�v�ɂ���Ēǉ��A�j���[�V�������ς��)
		ptrDraw->AddAnimation(L"Stand", 0, 1, 60.0f);
		ptrDraw->AddAnimation(L"Walk", 126, 49, 60.0f);
		ptrDraw->AddAnimation(L"Charge", 0, 125, 60.0f);
		ptrDraw->AddAnimation(L"Down", 637, 88, false, 60.0f);
		ptrDraw->AddAnimation(L"Hit", 637, 25, false, 60.0f);
		ptrDraw->AddAnimation(L"Stan", 637, 88, false, 60.0f);
	
		//�R���W�����쐬
		auto ptrColl = AddComponent<CollisionSphere>();//�R���W�����X�t�B�A�̕����ǂɂԂ��锻��Ɉ�a�����Ȃ�
		ptrColl->SetAfterCollision(AfterCollision::Auto);
		ptrColl->SetDrawActive(false);//�f�o�b�N�p

		AddTag(L"Enemy");
		AddTag(L"EnemyZako");

		m_player = GetStage()->GetSharedGameObject<Player>(L"Player");

		//�ڒn����̐ݒ�
		m_LandDetect->SetBindPos(Vec3(0, -1.0f, 0));
		m_LandDetect->GetComponent<Transform>()->SetScale(Vec3(2.0f, 2.0f, 2.0f));
		//m_LandDetect->SetCollScale(3.0f);

		//�X�e�[�g�}�V������
		m_state = shared_ptr<EnemyZakoFlyingStateMachine>(new EnemyZakoFlyingStateMachine(GetThis<GameObject>()));

		//�����HP�o�[��\��������
		m_HPFrame = GetStage()->AddGameObject<BillBoard>(GetThis<GameObject>(), L"ZakoGaugeFrame", 4, 5.0f, Vec3(2.0f, 0.5f, 5.0f));
		m_HPBer = GetStage()->AddGameObject<BillBoardGauge>(GetThis<GameObject>(), L"BossHPMater", 3, 5.0f, Vec3(2.0f, 0.5f, 5.0f));
		m_HPBer->SetPercent(1.0f);

		//m_damageBill = GetStage()->AddGameObject<EnemyDamageBill>(GetThis<GameObject>(), L"Numbers", 2, 7.0f, Vec3(0.5f, 2.0f, 1.0f));

		//auto m_billBoard2 = GetStage()->AddGameObject<BillBoard>(GetThis<GameObject>(), L"BossHPMater", 3, 5.0f, Vec3(2.0f, 0.5f, 5.0f));
	}

	void EnemyZakoFlying::OnUpdate()
	{
		//�����|�[�Y�t���O���I���ł���΃A�b�v�f�[�g�����͏o���Ȃ��Ȃ�
		if (m_PauseFlag)
		{
			return;
		}
		if (!m_beforUsed)
		{
			if (m_used)
			{
				m_HPCurrent = m_HPMax;
				m_attackFlag = false;
				m_timeCountOfAttackCool = 3.0f;
				//�����X�e�[�g�ɖ߂�
				ChangeState(L"Stand");
				GetComponent<PNTBoneModelDraw>()->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
		if (m_beforUsed)
		{
			if (!m_used)
			{
				auto stage = GetStage();
				auto pos = GetComponent<Transform>()->GetPosition();
				stage->GetSharedGameObject<PartsManager>(L"PartsManager")->PartsDrop(pos);
			}
		}
		//���݂̎g�p�󋵂ƌ���ׂĕς���Ă��Ȃ�������
		m_beforUsed = m_used;

		EnemyBase::OnUpdate();

		//���n����(���������Ԓ��Ȃ炻������Z����)
		OnLanding();

		//�����I�ȏ���
		if (m_doPhysics) {
			if (!m_isLand) {
				Gravity();
			}
			else {
				//Friction();
			}
		}


		//HP�o�[�̏���
		UpdateHPBer();
		//�U���̃N�[���^�C��
		TimeOfAttackCool();

		//HP���[���ɂȂ����������
		if (m_HPCurrent <= 0)
		{
			RemoveTag(L"LockOnCan");
			RemoveTag(L"LockOnTarget");

			m_used = false;

			//GetStage()->RemoveGameObject<EnemyZako>(GetThis<EnemyZako>());
			//GetStage()->RemoveGameObject<LandDetect>(m_LandDetect);
			//GetStage()->RemoveGameObject<AttackCollision>(m_AttackCol);
		}

		//�A�j���[�V�����X�V
		GetComponent<PNTBoneModelDraw>()->UpdateAnimation(m_addTimeAnimation);

		GetComponent<Transform>()->SetPosition((m_velocity * _delta) + GetComponent<Transform>()->GetPosition());
	}

}