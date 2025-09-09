/*!
@file PartsManager.cpp
@brief �p�[�c���Ǘ�����I�u�W�F�N�g
*/

#include "stdafx.h"
#include "Project.h"


namespace basecross {

	void PartsManager::OnCreate()
	{

	}

	void PartsManager::OnUpdate()
	{

	}

	//�p�[�c�h���b�v����
	void PartsManager::PartsDrop(Vec3 dropPos)
	{
		// �󒆂ɕ�����ł���G����h���b�v���Ă���ɓ���悤�ɂ���
		dropPos.y = 1.0f;

		float par = rand() % 100;
		int drop = rand() % 9;
		auto stage = GetStage();

		// �f�o�b�O�p�̃h���b�v�p�[�c�̔ԍ�(���ނ�����)
		static int debug = 0;
		if (debug > 8) debug = 0;

		par = 10;//�e�X�g�p�ɐ�ΐ�������
		//drop = 0;
		//35%���炢�̊m���Ńp�[�c�h���b�v
		if (par > 0 && par <= 15)
		{
			switch(debug)
			//switch (drop)
			{
			case 0:
				//�퓬�p�p�b�`
				stage->AddGameObject<PartsBattlePatch>(dropPos,Vec3(0.0f,0.0f,0.0f),Vec3(2.0f,2.0f,2.0f));
				break;
			case 1:
				//����p�[�c
				stage->AddGameObject<HeadParts>(dropPos, Vec3(0.0f, 0.0f, 0.0f), Vec3(2.0f, 2.0f, 2.0f));
				break;
			case 2:
				//�����\���[�^�[
				stage->AddGameObject<PartsHiMoter>(dropPos, Vec3(0.0f, 0.0f, 0.0f), Vec3(2.0f, 2.0f, 2.0f));
				break;
			case 3:
				//�����p�[�c
				stage->AddGameObject<PartsPrototype2>(dropPos, Vec3(0.0f, 0.0f, 0.0f), Vec3(2.0f, 2.0f, 2.0f));
				break;
			case 4:
				//���S�Ńp�[�c
				stage->AddGameObject<PartsCompleteVersion>(dropPos, Vec3(0.0f, 0.0f, 0.0f), Vec3(2.0f, 2.0f, 2.0f));
				break;
			case 5:
				//�������[�^�[
				stage->AddGameObject<PartsModMoter>(dropPos, Vec3(0.0f, 0.0f, 0.0f), Vec3(2.0f, 2.0f, 2.0f));
				break;
			case 6:
				//�������[�^�[
				stage->AddGameObject<PartsOldMoter>(dropPos, Vec3(0.0f, 0.0f, 0.0f), Vec3(2.0f, 2.0f, 2.0f));
				break;
			case 7:
				//�U�������p�b�`
				stage->AddGameObject<PartsPowerPatch>(dropPos, Vec3(0.0f, 0.0f, 0.0f), Vec3(2.0f, 2.0f, 2.0f));
				break;
			case 8:
				//�ėp�p�b�`
				stage->AddGameObject<PartsBalancePatch>(dropPos, Vec3(0.0f, 0.0f, 0.0f), Vec3(2.0f, 2.0f, 2.0f));
				break;
			default:
				break;
			}
			debug += 1;
		}
	}



	void EquippedParts::OnCreate()
	{
		
	}

	void EquippedParts::OnUpdate()
	{
		auto test = m_EquippedParts;
		auto a = 0;
	}
}