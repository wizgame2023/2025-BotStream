/*!
@file Parts.cpp
@brief �p�[�c�I�u�W�F�N�g
*/

#include "stdafx.h"
#include "Project.h"


namespace basecross {

	//�p�[�c�N���X
	Parts::Parts(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale) :
		ObjectMove(stagePtr),
		m_pos(pos),
		m_rot(rot),
		m_scale(scale)
	{

	}
	Parts::~Parts()
	{

	}

	void Parts::OnCreate()
	{
		//������
		auto trans = GetComponent<Transform>();
		trans->SetPosition(m_pos);
		trans->SetRotation(m_rot);
		trans->SetScale(m_scale);
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetDiffuse(Col4(1, 0, 0, 1));

		//OBB�Փ˔����t����
		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetAfterCollision(AfterCollision::None);

		auto stage = GetStage();

		//�p�[�c�|�[�`�擾
		m_partspoach = stage->GetSharedGameObject<EquippedParts>(L"PartsPoach");
		
		//�r���{�[�h����
		m_billBoard = stage->AddGameObject<BillBoard>(GetThis<GameObject>(),
			m_partsStatus.partsImagePass
			);
		m_billBoard->SetPushY(10.0f);
		
		
	}

	void Parts::OnUpdate()
	{
		auto stage = GetStage();
		auto player = stage->GetSharedGameObject<Player>(L"Player");
		auto playerPos = player->GetPosition();

		//�v���C���[�ƃp�[�c�̋��������
		auto distPos = playerPos - m_pos;

		//�v���C���[�Ƃ̋��������ȉ��Ȃ�p�[�c�����r���{�[�h���o��
		if (distPos.length() <= 30.0f)
		{
			m_billBoard->SetScale(Vec3(10.0f, 1.0f, 10.0f));
		}
		else
		{
			m_billBoard->SetScale(Vec3(0.0f, 0.0f, 0.0f));
		}
	}

	void Parts::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		//�p�[�c�̃f�[�^��n��
		if (Other->FindTag(L"Player"))
		{
			SetDrawActive(false);
			m_partspoach->AddParts(m_partsStatus);
		}

	}



	//���p�[�c�p
    void HeadParts::OnCreate()
    {
		Parts::OnCreate();
    }

    void HeadParts::OnUpdate()
    {

	}

}