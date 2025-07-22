#pragma once
/*!
@file Parts.h
@brief �p�[�c�I�u�W�F�N�g
*/

#include "stdafx.h"
#include "Project.h"
#include "ObjectMove.h"

namespace basecross {
	class EquippedParts;
	class BillBoard;

	//�p�[�c�^�C�v
	enum partsType
	{
		parts,
		MoterParts,
		PatchParts
	};

	//�p�[�c�̃X�e�[�^�X�ꗗ
	struct PartsStatus
	{
		//ID
		int id;
		//�p�[�c�^�C�v
		int partsType;
		//�U����
		int addAttack;
		//���x
		float addSpeed;
		//�P����
		int addAttribute;
		//�ǉ�HP
		int addHP;
		//�p�[�c�̖��O
		wstring partsName;
		//�p�[�c�̎ʐ^�̃p�X��
		wstring partsImagePass;
	};

	//�p�[�c�N���X
	class Parts : public ObjectMove
	{
	protected:
		shared_ptr<Transform> m_trans;
		Vec3 m_pos;//�ʒu
		Vec3 m_rot;//��]
		Vec3 m_scale;//�傫��

		//�p�[�c�̃X�e�[�^�X
		PartsStatus m_partsStatus;
		//�p�[�c������|�[�`
		shared_ptr<EquippedParts> m_partspoach;

		//�r���{�[�h
		shared_ptr<BillBoard> m_billBoard;

		bool m_pose = false;

	public:
		Parts(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale);
		~Parts();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void SetPause(bool onOff)
		{
			m_pose = onOff;
		}

		void OnCollisionEnter(shared_ptr<GameObject>& Other)override;
	};

	//�p�[�c(����p�[�c)
	class HeadParts : public Parts
	{
		//shared_ptr<Stage> m_stage;


	public:
		HeadParts(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale) :
			Parts(stagePtr,pos,rot,scale)
		{
			//�ǂꂮ�炢�X�e�[�^�X���グ�邩���߂�
			m_partsStatus.id = 1;
			m_partsStatus.addHP = 10;
			m_partsStatus.addSpeed = 0.1f;
			m_partsStatus.addAttack = 0;
			m_partsStatus.addAttribute = 10;

			// �ǂ̃^�C�v�̃p�[�c�����߂�
			m_partsStatus.partsType = parts;

			//���O
			m_partsStatus.partsName = L"�e�X�g�p�p�[�c";
			//�Ƃ肠�����K���ȃe�N�X�`���ɂ���
			m_partsStatus.partsImagePass = L"PartsTestTEX";
		}
		~HeadParts()
		{
		}
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};


	//�p�[�c(�����\���[�^�[)
	class PartsHiMoter :public Parts
	{
	private:

	public:
		PartsHiMoter(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale) :
			Parts(stagePtr, pos, rot, scale)
		{
			//�ǂꂮ�炢�X�e�[�^�X���グ�邩���߂�
			m_partsStatus.id = 2;
			m_partsStatus.addHP = 0;
			m_partsStatus.addSpeed = 0.4f;
			m_partsStatus.addAttack = 0;
			m_partsStatus.addAttribute = 10;

			// �ǂ̃^�C�v�̃p�[�c�����߂�
			m_partsStatus.partsType = MoterParts;

			//���O
			m_partsStatus.partsName = L"�����\���[�^�[";
			//�Ƃ肠�����K���ȃe�N�X�`���ɂ���
			m_partsStatus.partsImagePass = L"PartsHiMoterTEX";
		}
		~PartsHiMoter()
		{
		}
	};


	//�p�[�c(�퓬�p�p�b�`)
	class PartsBattlePatch :public Parts
	{
	private:

	public:
		PartsBattlePatch(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale) :
			Parts(stagePtr, pos, rot, scale)
		{
			//�ǂꂮ�炢�X�e�[�^�X���グ�邩���߂�
			m_partsStatus.id = 3;
			m_partsStatus.addHP = 20;
			m_partsStatus.addSpeed = 0.0f;
			m_partsStatus.addAttack = 3;
			m_partsStatus.addAttribute = -20;

			// �ǂ̃^�C�v�̃p�[�c�����߂�
			m_partsStatus.partsType = PatchParts;

			//���O
			m_partsStatus.partsName = L"�퓬�p�p�b�`";
			//�Ƃ肠�����K���ȃe�N�X�`���ɂ���
			m_partsStatus.partsImagePass = L"PartsBattlePatchTEX";
		}
		~PartsBattlePatch()
		{
		}

	};


	//�p�[�c(����p�[�c(������))
	class PartsPrototype2 : public Parts
	{

	public:
		PartsPrototype2(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale) :
			Parts(stagePtr, pos, rot, scale)
		{
			//�ǂꂮ�炢�X�e�[�^�X���グ�邩���߂�
			m_partsStatus.id = 4;
			m_partsStatus.addHP = 15;
			m_partsStatus.addSpeed = 0.15;
			m_partsStatus.addAttack = 0;
			m_partsStatus.addAttribute = 15;

			// �ǂ̃^�C�v�̃p�[�c�����߂�
			m_partsStatus.partsType = parts;

			//���O
			m_partsStatus.partsName = L"����p�[�c(������)";
			//�Ƃ肠�����K���ȃe�N�X�`���ɂ���
			m_partsStatus.partsImagePass = L"Stage1-1Tex";
		}
		~PartsPrototype2()
		{
		}
	};


	//�p�[�c(���S�Ńp�[�c)
	class PartsCompleteVersion : public Parts
	{

	public:
		PartsCompleteVersion(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale) :
			Parts(stagePtr, pos, rot, scale)
		{
			//�ǂꂮ�炢�X�e�[�^�X���グ�邩���߂�
			m_partsStatus.id = 5;
			m_partsStatus.addHP = 20;
			m_partsStatus.addSpeed = 0.2;
			m_partsStatus.addAttack = 0;
			m_partsStatus.addAttribute = 20;

			// �ǂ̃^�C�v�̃p�[�c�����߂�
			m_partsStatus.partsType = parts;

			//���O
			m_partsStatus.partsName = L"���S�Ńp�[�c";
			//�Ƃ肠�����K���ȃe�N�X�`���ɂ���
			m_partsStatus.partsImagePass = L"Stage1-2Tex";
		}
		~PartsCompleteVersion()
		{
		}
	};


	//�p�[�c(�������[�^�[)
	class PartsModMoter :public Parts
	{
	private:

	public:
		PartsModMoter(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale) :
			Parts(stagePtr, pos, rot, scale)
		{
			//�ǂꂮ�炢�X�e�[�^�X���グ�邩���߂�
			m_partsStatus.id = 6;
			m_partsStatus.addHP = 0;
			m_partsStatus.addSpeed = 0.25f;
			m_partsStatus.addAttack = 2;
			m_partsStatus.addAttribute = -10;

			// �ǂ̃^�C�v�̃p�[�c�����߂�
			m_partsStatus.partsType = MoterParts;

			//���O
			m_partsStatus.partsName = L"�������[�^�[";
			//�Ƃ肠�����K���ȃe�N�X�`���ɂ���
			m_partsStatus.partsImagePass = L"Stage2-1Tex";
		}
		~PartsModMoter()
		{
		}
	};


	//�p�[�c(�Âт����[�^�[)
	class PartsOldMoter :public Parts
	{
	private:

	public:
		PartsOldMoter(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale) :
			Parts(stagePtr, pos, rot, scale)
		{
			//�ǂꂮ�炢�X�e�[�^�X���グ�邩���߂�
			m_partsStatus.id = 7;
			m_partsStatus.addHP = 0;
			m_partsStatus.addSpeed = 0.15f;
			m_partsStatus.addAttack = 1;
			m_partsStatus.addAttribute = -20;

			// �ǂ̃^�C�v�̃p�[�c�����߂�
			m_partsStatus.partsType = MoterParts;

			//���O
			m_partsStatus.partsName = L"�Âт����[�^�[";
			//�Ƃ肠�����K���ȃe�N�X�`���ɂ���
			m_partsStatus.partsImagePass = L"Stage2-2Tex";
		}
		~PartsOldMoter()
		{
		}
	};


	//�p�[�c(�p���[�^�p�b�`)
	class PartsPowerPatch :public Parts
	{
	private:

	public:
		PartsPowerPatch(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale) :
			Parts(stagePtr, pos, rot, scale)
		{
			//�ǂꂮ�炢�X�e�[�^�X���グ�邩���߂�
			m_partsStatus.id = 8;
			m_partsStatus.addHP = 0;
			m_partsStatus.addSpeed = 0;
			m_partsStatus.addAttack = 7;
			m_partsStatus.addAttribute = 5;

			// �ǂ̃^�C�v�̃p�[�c�����߂�
			m_partsStatus.partsType = PatchParts;

			//���O
			m_partsStatus.partsName = L"�p���[�^�p�b�`";
			//�Ƃ肠�����K���ȃe�N�X�`���ɂ���
			m_partsStatus.partsImagePass = L"Stage3-1Tex";
		}
		~PartsPowerPatch()
		{
		}

	};


	//�p�[�c(�o�����X�^�p�b�`)
	class PartsBalancePatch :public Parts
	{
	private:

	public:
		PartsBalancePatch(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale) :
			Parts(stagePtr, pos, rot, scale)
		{
			//�ǂꂮ�炢�X�e�[�^�X���グ�邩���߂�
			m_partsStatus.id = 9;
			m_partsStatus.addHP = 10;
			m_partsStatus.addSpeed = 0.2f;
			m_partsStatus.addAttack = 2;
			m_partsStatus.addAttribute = 20;

			// �ǂ̃^�C�v�̃p�[�c�����߂�
			m_partsStatus.partsType = PatchParts;

			//���O
			m_partsStatus.partsName = L"�o�����X�^�p�b�`";
			//�Ƃ肠�����K���ȃe�N�X�`���ɂ���
			m_partsStatus.partsImagePass = L"Stage3-2Tex";
		}
		~PartsBalancePatch()
		{
		}

	};

}