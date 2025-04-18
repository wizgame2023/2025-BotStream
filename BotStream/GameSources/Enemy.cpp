/*!
@file Enemy.cpp
@brief �G�̏���
*/

#include "stdafx.h"
#include "Project.h"
#include "Actor.h"
#include "Enemy.h"

namespace basecross {
	EnemyBase::EnemyBase(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale) :
		Actor(stagePtr, pos, rot, scale),
		m_used(false)
	{

	}

	EnemyBase::EnemyBase(const shared_ptr<Stage>&stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, bool use) :
		Actor(stagePtr, pos, rot, scale),
		m_used(use)
	{

	}

	void EnemyBase::HitBackStandBehavior() {
		m_hitbacktime -= _delta;
		if (m_hitbacktime <= 0) {
			m_state->ChangeState(L"Walk");
		}
	}

	void EnemyBase::OnCreate() {
		Actor::OnCreate();
		//Transform�ݒ�
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//�h���[���b�V���̐ݒ�
		auto ptrDraw = GetComponent<PNTBoneModelDraw>();
		ptrDraw->SetMeshResource(L"Boss1");
		ptrDraw->SetDiffuse(Col4(0));
		ptrDraw->SetSamplerState(SamplerState::LinearWrap);
		ptrDraw->SetMeshToTransformMatrix(spanMat);
		ptrDraw->SetTextureResource(L"Tx_Boss1");
		
		//�R���W�����쐬
		auto ptrColl = AddComponent<CollisionSphere>();//�R���W�����X�t�B�A�̕����ǂɂԂ��锻��Ɉ�a�����Ȃ�
		ptrColl->SetAfterCollision(AfterCollision::Auto);
		AddTag(L"Enemy");

		m_state = shared_ptr<EnemyStateMachine>(new EnemyStateMachine(GetThis<GameObject>()));
	}

	void EnemyBase::OnUpdate() {
		Actor::OnUpdate();
		//m_used=false�Ȃ�\����������Update��return
		if (GetDrawActive() != m_used) {
			SetDrawActive(m_used);
		}
		if (m_used == false) {
			m_LandDetect->SetDrawActive(false);
			m_LandDetect->SetUpdateActive(false);
			SetUpdateActive(m_used);
			return;
		}

		//���n����(���������Ԓ��Ȃ炻������Z����)
		OnLanding();

		//����
		if (!m_isLand) {
			Gravity();
		}
		else {
			Friction();
		}

		//�Ȃ�₩���
		m_state->Update();

	}

	//--------------------------------------------------------------------------

	void BossFirst::OnCreate() {
		Actor::OnCreate();
		//Transform�ݒ�
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f),
			Vec3(0.0f, -3.25f, 0.0f)
		);

		//�h���[���b�V���̐ݒ�
		auto ptrDraw = GetComponent<PNTBoneModelDraw>();
		ptrDraw->SetMeshResource(L"Boss1");
		ptrDraw->SetDiffuse(Col4(0));
		ptrDraw->SetSamplerState(SamplerState::LinearWrap);
		ptrDraw->SetMeshToTransformMatrix(spanMat);
		ptrDraw->SetTextureResource(L"Tx_Boss1");

		RegisterAnim();
		ChangeAnim(L"Walk");

		//�R���W�����쐬
		auto ptrColl = AddComponent<CollisionSphere>();//�R���W�����X�t�B�A�̕����ǂɂԂ��锻��Ɉ�a�����Ȃ�
		ptrColl->SetAfterCollision(AfterCollision::Auto);

		ptrColl->SetMakedRadius(3);
		ptrColl->SetDrawActive(true);//debug

		m_LandDetect->SetBindPos(Vec3(0, -3.0f, 0));

		AddTag(L"Enemy");
		//�{�X�p�̂��̂ɍ����ւ���H
		m_state = shared_ptr<EnemyStateMachine>(new EnemyStateMachine(GetThis<GameObject>()));
	}

	void BossFirst::OnUpdate() {
		EnemyBase::OnUpdate();
		//�A�j���[�V�����Đ�
		GetComponent<PNTBoneModelDraw>()->UpdateAnimation(_delta);

		GetComponent<Transform>()->SetPosition((m_velocity * _delta) + GetComponent<Transform>()->GetPosition());

		if (!m_isLand) bindPos -= _delta;
		//m_LandDetect->SetBindPos(Vec3(0, -2.4f, 0));

		////�f�o�b�N�p
		wstringstream wss(L"");
		auto scene = App::GetApp()->GetScene<Scene>();
		auto quat = GetComponent<Transform>()->GetQuaternion();
		wss /* << L"�f�o�b�O�p������ "*/
			<< L"\n Pos.x " << m_pos.x << " Pos.z " << m_pos.z
			<< L" Vel.x " << m_velocity.x << L"\ Vel.y " << m_velocity.y << L" Vel.z " << m_velocity.z
			<< endl << "onLand: " << m_isLand << " LandDetect: " << m_LandDetect->GetLand()
			<< L"\nQuat : (" << L"\n" << quat.x << L"\n" << quat.y << L"\n" << quat.z << L"\n" << quat.w
			<< L"\nAngle : " << bindPos << endl;

		scene->SetDebugString(wss.str());

	}

	void BossFirst::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		DetectBeingAttacked(Other);
	}

	void BossFirst::OnDamaged() {
		m_state->ChangeState(L"Hit");
	}

	void BossFirst::RegisterAnim() {
		auto ptrDraw = GetComponent<PNTBoneModelDraw>();
		//��
		ptrDraw->AddAnimation(L"Idle", 0, 25, true, 30.0f);
		//��]
		ptrDraw->AddAnimation(L"Rotate", 26, 154, true, 30.0f);
		//�̂�����
		ptrDraw->AddAnimation(L"Walk", 181, 169, false, 60.0f);
		//�̂�����
		ptrDraw->AddAnimation(L"HitBack", 488, 52, false, 60.0f);
	}
}
//end basecross
