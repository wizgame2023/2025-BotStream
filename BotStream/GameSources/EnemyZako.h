/*!
@file EnemyZako.h
@brief �G���G
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class EnemyZako : public EnemyBase
	{
	protected:
		void OnDamaged() override;

		//HP�o�[�p�̃r���{�[�h
		shared_ptr<BillBoard> m_HPFrame = nullptr;
		shared_ptr<BillBoardGauge> m_HPBer = nullptr;

		//�U���̃^�C�v�@�e�X�g�p�ɋߋ����ɂ������̂ł�������
		int m_AttackType = Zako_Melee;

		//�A�j���[�V�����̍X�V����
		float m_addTimeAnimation = 0.0f;

		//�U���̃N�[���_�E���֌W
		bool m_attackFlag = false;
		float m_timeOfAttackCool = 5.0f;
		float m_timeCountOfAttackCool = 3.0f;

		bool m_beforUsed = false;

	public:
		// �G���G�̎�ނ�ǉ�����Ƃ��͂����ɏ���
		enum EnemyZakoAttackType
		{
			Zako_Melee,//�ߋ����^
			Zako_Long,//�������^
			Zako_Flying,//�؋�^
		};

		EnemyZako(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, bool used = false, int attackType = Zako_Melee) :
			EnemyBase(stagePtr, pos, rot, scale, used),
			m_AttackType(attackType)
		{

		}
		~EnemyZako() {}

		//�󂯂��U���̏���n���Q�b�^�[
		HitInfo EnemyZako::GetHitInfo()
		{
			return m_getHitInfo;
		}

		void OnCreate() override;
		void OnUpdate() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
		void OnDestroy()override;

		//�_���[�W�r���{�[�h
		void CreateDamageBill(
			shared_ptr<GameObject> actorPtr,
			int damage,
			float pushY = 2.0f,
			float scale = 1.0f,
			float displayTime = 0.5f
		);

		//�r���{�[�h�̏���
		virtual void UpdateHPBer();

		//�U���̃N�[���^�C��
		virtual void TimeOfAttackCool();

		//�_���[�W���󂯂��Ƃ���Stand�X�e�[�g�ɖ߂鏈��
		virtual void HitBackStandBehavior()override;

		//�U���̃^�C�v�̃Q�b�^
		virtual int GetAttackType()
		{
			return m_AttackType;
		}

		//�U���t���O�̃Q�b�^�Z�b�^
		bool GetAttackFlag()
		{
			return m_attackFlag;
		}
		void SetAttackFlag(bool attackFlag)
		{
			m_attackFlag = attackFlag;
		}

		//�A�j���[�V�����̒ǉ����Ԃ̃Q�b�^�Z�b�^
		float GetAddTimeAnimation()
		{
			return m_addTimeAnimation;
		}
		void SetAddTimeAnimation(float addTimeAnimation)
		{
			m_addTimeAnimation = addTimeAnimation;
		}
	};

	//�������̎G��
	class EnemyZakoLong :public EnemyZako
	{
	private:

	public:
		EnemyZakoLong(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, bool used = false) :
			EnemyZako(stagePtr, pos, rot, scale, used, Zako_Long)
		{

		}
		~EnemyZakoLong() {};

		void OnCreate()override;

	};

	class EnemyZakoFlying : public EnemyZako
	{
	public:
		enum EnemyZakoFlyingAttackType
		{
			Zako_Melee,//�ߋ����^
			Zako_Long//�������^
		};

		EnemyZakoFlying(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, bool used = false) :
			EnemyZako(stagePtr, pos, rot, scale, used)
		{

		}
		~EnemyZakoFlying() {}

		void OnCreate() override;
		void OnUpdate() override;

	};

}