/*!
@file AttackCollision.h
@brief �U������
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {

	enum class AttackType {
		Player,
		Enemy
	};

	//�U�����肩�瑊��ɓ`������ꗗ
	struct HitInfo {
	public:
		AttackType Type;
		//�_���[�W
		int Damage;
		//�q�b�g�X�g�b�v����
		float PauseTime;
		//�n�㑊��̂̂����莞��
		float HitTime_Stand;
		//�n�㑊��̂̂�����ړ�����
		Vec3 HitVel_Stand;
		//�󒆑���̂̂����莞��
		float HitTime_Air;
		//�󒆑���̐�����΂�����
		Vec3 HitVel_Air;
		//�_�E������
		bool DoFall;
		//�_�E������(�󒆑���̂�)
		bool DoFall_Air;
		//�_�E�������Ȃ�
		bool ForceRecover;
		//��x�����q�b�g���Ȃ�
		bool HitOnce;

		//�G�t�F�N�g�E���̃L�[
		wstring HitEffect;
		wstring GuardEffect;
		wstring HitSound;
		wstring GuardSound;
		
		//�R���X�g���N�^
		HitInfo::HitInfo(AttackType attacktype, int damage, float pausetime,
			float hittime_stand, Vec3 hitvel_stand,
			float hittime_air, Vec3 hitvel_air,
			bool dofall, bool dofall_air,
			bool forcerecover, bool hitonce,
			wstring hiteffect, wstring guardeffect,
			wstring hitsound, wstring guardsound
		)
		{
			this->Type = attacktype;
			this->Damage = damage;
			this->PauseTime = pausetime;
			this->HitTime_Stand = hittime_stand;
			this->HitVel_Stand = hitvel_stand;
			this->HitTime_Air = hittime_air;
			this->HitVel_Air = hitvel_air;
			this->DoFall = dofall;
			this->DoFall_Air = dofall_air;
			this->ForceRecover = forcerecover;
			this->HitOnce = hitonce;
			this->HitEffect = hiteffect;
			this->GuardEffect = guardeffect;
			this->HitSound = hitsound;
			this->GuardSound = guardeffect;
		}
		HitInfo::HitInfo() {
			this->Type = AttackType::Player;
			this->Damage = 0;
			this->PauseTime = 0;
			this->HitTime_Stand = 0;
			this->HitVel_Stand = Vec3(0);
			this->HitTime_Air = 0;
			this->HitVel_Air = Vec3(0);
			this->DoFall = false;
			this->DoFall_Air = false;
			this->ForceRecover = false;
			this->HitOnce = false;
			this->HitEffect = L"";
			this->GuardEffect = L"";
			this->HitSound = L"";
			this->GuardSound = L"";
		}
	};

	class AttackCollision : public MyGameObject
	{
		shared_ptr<CollisionSphere> m_collision;

		//�U���̃X�y�b�N
		HitInfo m_info;
		//��������
		float m_ActiveTime = 0.0f;
	public:
		AttackCollision(const shared_ptr<Stage>& stagePtr);
		~AttackCollision();

		void OnCreate() override;
		void OnUpdate() override;

		shared_ptr<CollisionSphere> GetCollisionPtr() {
			return m_collision;
		}

		//Position�Q�b�^�E�Z�b�^
		void SetPos(Vec3 pos) {
			GetComponent<Transform>()->SetPosition(pos);
		}
		Vec3 GetPos() {
			return GetComponent<Transform>()->GetPosition();
		}

		//����̔��a��ς���
		void SetCollScale(float scale) {
			m_collision->SetMakedRadius(scale);
		}
		//����̎������Ԃ�ݒ�
		void ActiveCollision(float activetime) {
			m_ActiveTime = activetime;
		}

		//�U������𑊎�ɓ��Ă����ɌĂ΂��֐�
		HitInfo GetHitInfo() {
			return m_info;
		}
		//�U��������X�V����
		void SetHitInfo(HitInfo info) {
			this->m_info = info;
		}
	};
}
//end basecross

