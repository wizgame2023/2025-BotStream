#pragma once
/*!
@file PartsManager.h
@brief �p�[�c���Ǘ�����I�u�W�F�N�g
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	class PartsManager : public MyGameObject
	{

	public:
		PartsManager(const shared_ptr<Stage>& stagePtr) :
			MyGameObject(stagePtr)
		{
		}
		~PartsManager(){}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void PartsDrop(Vec3 dropPos);

	};

	class EquippedParts : public MyGameObject
	{
	private:
		vector<PartsStatus> m_partspoach;
		vector<PartsStatus> m_EquippedParts;

	public:
		EquippedParts(const shared_ptr<Stage>& stagePtr) :
			MyGameObject(stagePtr)
		{
		}
		~EquippedParts() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		//�|�[�`�Ƀp�[�c�����鏈��
		void AddParts(PartsStatus addParts)
		{
			auto player = GetStage()->GetSharedGameObject<Player>(L"Player");

			//�������Ă���S�Ẵp�[�c�̃X�e�[�^�X
			PartsStatus allParts;
			allParts.addAttack = 0;
			allParts.addAttribute = 0;
			allParts.addHP = 0;
			allParts.addSpeed = 0.0f;

			//�p�[�c�|�[�`�ɓ����
			m_partspoach.push_back(addParts);

			//�p�[�c���R�ȏ�t���Ă��Ȃ��Ƃ��͎����I�ɑ��������
			if (m_EquippedParts.size() < 3)
			{
				m_EquippedParts.push_back(addParts);
			}
			//�����łȂ���΁A�p�[�c���Â�����̂ĂĐV������ɕς���
			else if (m_EquippedParts.size() >= 3)
			{
				//��ԍŏ��Ɏ�������̂��폜���ĐV������ɓ��ꂽ���̂�����
				m_EquippedParts.erase(m_EquippedParts.begin());
				m_EquippedParts.push_back(addParts);
			}

			int roop = 0;
			//�������Ă��鍇�v�̃X�e�[�^�X���v�Z����
			for (auto parts : m_EquippedParts)
			{
				allParts.id = 0;
				allParts.addAttack += m_EquippedParts[roop].addAttack;
				allParts.addAttribute += m_EquippedParts[roop].addAttribute;
				allParts.addHP += m_EquippedParts[roop].addHP;
				allParts.addSpeed += m_EquippedParts[roop].addSpeed;
				allParts.partsImagePass = L"�g��Ȃ��f�[�^";
				allParts.partsName = L"�g��Ȃ��f�[�^";

				roop++;
			}

			//�p�[�c����SE�Đ�
			auto m_SEManager = App::GetApp()->GetXAudio2Manager();
			m_SEManager->Start(L"GetPartsSE", 0, 0.4f * m_SEVol);

			//�v���C���[�ɑ����������v�̃X�e�[�^�X�l��n��
			player->SetEquippedParts(allParts);
		}

		//�����p�[�c���󂯎�鏈��
		vector<PartsStatus> GetEquippedParts()
		{
			return m_EquippedParts;
		}

		//�|�[�`���󂯎�鏈��
		vector<PartsStatus> GetParts()
		{
			return m_partspoach;
		}

	};
}