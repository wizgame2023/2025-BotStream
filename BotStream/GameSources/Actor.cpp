/*!
@file Actor.cpp
@brief Player��Enemy�Ȃǂ̃L�����N�^�[�p�̃N���X
*/

#include "stdafx.h"
#include "Project.h"
#include "Actor.h"

namespace basecross {
	Actor::Actor(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale) :
		ObjectMove(stagePtr),
		m_pos(pos),
		m_rot(rot),
		m_scale(scale)
	{

	}
	Actor::~Actor()
	{

	}

	void Actor::OnCreate() {
		auto stage = GetStage();

		//Actor�O���[�v�ɓo�^����
		auto group = stage->GetSharedObjectGroup(L"Actor");
		if (group) {
			group->IntoGroup(GetThis<GameObject>());
		}

		//�`��R���|�[�l���g�̒ǉ�
		AddComponent<PNTBoneModelDraw>();

		//���n����̐����A�q�I�u�W�F�N�g�ɂ���
		m_LandDetect = stage->AddGameObject<LandDetect>();
		m_LandDetect->GetComponent<Transform>()->SetParent(dynamic_pointer_cast<GameObject>(GetThis<Actor>()));

		//�U������̐���
		m_AttackCol = GetStage()->AddGameObject<AttackCollision>();
		m_AttackCol->GetComponent<Transform>()->SetParent(dynamic_pointer_cast<GameObject>(GetThis<Actor>()));


	}

	void Actor::OnUpdate() {
		_delta = App::GetApp()->GetElapsedTime();
	}

	//�ō����x
	void Actor::SpeedLimit(float multiply) {
		float limit = m_speedMax * multiply;
		auto angle = m_velocity;
		angle.y = 0;
		if (angle.length() > 0) {
			angle.normalize();
			if (angle.x > 0) {
				if (m_velocity.x > angle.x * limit) m_velocity.x = angle.x * limit;
			}
			else {
				if (m_velocity.x < angle.x * limit) m_velocity.x = angle.x * limit;
			}
			if (angle.z > 0) {
				if (m_velocity.z > angle.z * limit) m_velocity.z = angle.z * limit;
			}
			else {
				if (m_velocity.z < angle.z * limit) m_velocity.z = angle.z * limit;
			}
		}
		//�����̏I�[���x
		if (m_velocity.y < m_fallTerminal) m_velocity.y = m_fallTerminal;
	}

	//���C(�n��̂�)
	void Actor::Friction() {
		//�Ö��C
		if (m_accel == Vec3(0)) {
			m_velocity.x -= m_velocity.x * m_friction * (1000.0f / 60.0f) * _delta;
			m_velocity.z -= m_velocity.z * m_friction * (1000.0f / 60.0f) * _delta;
			if (m_velocity.length() < m_frictionThreshold) m_velocity.x = 0;
			return;
		}
		//�����C
		if (m_accel != Vec3(0)) {
			m_velocity.x -= m_velocity.x * m_frictionDynamic * (1000.0f / 60.0f) * _delta;
			m_velocity.z -= m_velocity.z * m_frictionDynamic * (1000.0f / 60.0f) * _delta;
		}
	}

	//�d��
	void Actor::Gravity() {
		m_velocity.y += m_gravity * _delta;
	}

	//OnCollisionEnter�ɒu��
	void Actor::DetectBeingAttacked(shared_ptr<GameObject>& other) {
		auto atk = dynamic_pointer_cast<AttackCollision>(other);
		if (!atk) return;

		m_hitDirection = other->GetComponent<Transform>()->GetWorldPosition();
		m_hitDirection -= GetComponent<Transform>()->GetWorldPosition();

		bool isAttacked = false;
		HitInfo info = atk->GetHitInfo();

		isAttacked = isAttacked || (FindTag(L"Enemy") && info.Type == AttackType::Player);
		isAttacked = isAttacked || (FindTag(L"Player") && info.Type == AttackType::Enemy);
		//�U�����󂯂���
		if (isAttacked) {
			//�U�����肩��U���̃f�[�^���擾
			m_GetHitInfo = info;

			if (info.HitOnce == true) {
				//�U�����������
				atk->ActivateCollision(0);
			}

			//��e������
			OnDamaged();
		}
	}

	//�����Ă�������̃Q�b�^�[
	float Actor::GetAngle()
	{
		return -m_angle;
	}

	//�����Ă�������̃Z�b�^�[
	void Actor::SetAngle(float angle)
	{
		m_angle = angle;
	}

	void Actor::OnLanding()
	{
		if (m_disableLandDetect > 0) {
			m_disableLandDetect -= _delta;
		}
		else {
			if (m_LandDetect->GetLand() != m_isLand) {
				//���n��������
				if (!m_isLand)
				{
					m_velocity.y = 0;
					EfkPlaying(L"Landing", GetAngle(), Vec3(0, 1, 0));

				}

				m_isLand = !m_isLand;
			}
		}
	}

	//�G�t�F�N�g���o������
	void Actor::AddEffect(int addEffect)
	{
		switch (addEffect)
		{
		case PlayerEffect_Attack1:
			EfkPlaying(L"Sword", GetAngle() + XM_PI, Vec3(0, 1, 0));
			break;
		case PlayerEffect_Attack2:
			EfkPlaying(L"Sword", GetAngle() + XM_PI, Vec3(0, 1, 0), Col4(0.22f, 1.0f, 0.48f, 1.0f));
			break;
		case PlayerEffect_Attack3:
			EfkPlaying(L"Sword", GetAngle() + XM_PI, Vec3(0, 1, 0), Col4(1.0f, 0.94f, 0.45f, 1.0f));
			break;
		case PlayerEffect_AttackEx:
			EfkPlaying(L"Sword", GetAngle() + XM_PI, Vec3(0, 1, 0), Col4(0.22f, 1.0f, 0.48f, 1.0f));
			EfkPlaying(L"Sword", GetAngle(), Vec3(0, 1, 0));
			break;
		case PlayerEffect_Beam:
			EfkPlaying(L"Laser", GetAngle() + XM_PIDIV2, Vec3(0, 1, 0));
			break;
		default:
			break;
		}
	}

	// �G�t�F�N�g�̃v���C
	void Actor::EfkPlaying(wstring EfkKey, float rad, Vec3 rotate)
	{
		rotate.normalize();
		auto trans = GetComponent<Transform>();
		auto plPos = trans->GetPosition();

		auto efkHandler = EffectManager::Instance().PlayEffect(EfkKey, plPos);
		EffectManager::Instance().SetRotation(efkHandler, Vec3(rotate.x, rotate.y, rotate.z), rad);
	}

	void Actor::EfkPlaying(wstring EfkKey, float rad, Vec3 rotate, Col4 changeColor)
	{
		rotate.normalize();
		auto trans = GetComponent<Transform>();
		auto plPos = trans->GetPosition();

		auto efkHandler = EffectManager::Instance().PlayEffect(EfkKey, plPos);
		EffectManager::Instance().SetAllColor(efkHandler, changeColor);//�G�t�F�N�g�̐F��ς���
		EffectManager::Instance().SetRotation(efkHandler, Vec3(rotate.x, rotate.y, rotate.z), rad);
	}
}
//end basecross
