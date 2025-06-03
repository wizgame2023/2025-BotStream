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

	//�p�[�c�̃X�e�[�^�X�ꗗ
	struct PartsStatus
	{
		//ID
		int id;
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

		void SetPose(bool onOff)
		{
			m_pose = onOff;
		}

		void OnCollisionEnter(shared_ptr<GameObject>& Other)override;
	};

	//���p�[�c�p
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
			m_partsStatus.addSpeed = 0.05f;
			m_partsStatus.addAttack = 0;
			m_partsStatus.addAttribute = 10;

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
			m_partsStatus.addSpeed = 0.3f;
			m_partsStatus.addAttack = 0;
			m_partsStatus.addAttribute = 10;

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

			//���O
			m_partsStatus.partsName = L"�퓬�p�p�b�`";
			//�Ƃ肠�����K���ȃe�N�X�`���ɂ���
			m_partsStatus.partsImagePass = L"PartsBattlePatchTEX";
		}
		~PartsBattlePatch()
		{
		}

	};
}