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
		// ������
		auto trans = GetComponent<Transform>();
		trans->SetPosition(m_pos);
		trans->SetRotation(m_rot);
		trans->SetScale(m_scale);

		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(0.5f, 0.5f, 0.5f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XMConvertToRadians(0.0f), 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetDiffuse(Col4(1, 1, 1, 1));
		ptrDraw->SetMeshToTransformMatrix(spanMat);

		// �p�[�c�^�C�v�ɂ���Č����ڂ��ς��
		switch (m_partsStatus.partsType)
		{
		case parts:
			ptrDraw->SetMeshResource(L"PartsMesh");
			ptrDraw->SetTextureResource(L"PartsTex");
			break;
		case PatchParts:
			ptrDraw->SetMultiMeshResource(L"PatchPartsMesh");
			ptrDraw->SetTextureResource(L"PatchPartsTex");
			break;
		case MoterParts:
			ptrDraw->SetMeshResource(L"MoterPartsMesh");
			ptrDraw->SetTextureResource(L"MoterPartsTex");
			break;
		default:
			break;
		}

		// OBB�Փ˔����t����
		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetAfterCollision(AfterCollision::None);

		auto stage = GetStage();

		// �p�[�c�|�[�`�擾
		m_partspoach = stage->GetSharedGameObject<EquippedParts>(L"PartsPoach");
		
		// �r���{�[�h����
		m_billBoard = stage->AddGameObject<BillBoard>(GetThis<GameObject>(),
			m_partsStatus.partsImagePass
			);
		m_billBoard->SetPushY(5.0f);
		
		
	}

	void Parts::OnUpdate()
	{
		// �|�[�Y��Ԃ��I���Ȃ�A�b�v�f�[�g���Ȃ�
		if (m_pose) return;

		auto stage = GetStage();
		auto player = stage->GetSharedGameObject<Player>(L"Player");
		auto playerPos = player->GetPosition();
		auto deltaTime = App::GetApp()->GetElapsedTime();
		// �v���C���[�ƃp�[�c�̋��������
		auto distPos = playerPos - m_pos;

		// �v���C���[�Ƃ̋��������ȉ��Ȃ�p�[�c�����r���{�[�h���o��
		if (distPos.length() <= 30.0f)
		{
			m_billBoard->SetScale(Vec3(7.0f, 7.0f, 1.0f));
		}
		else
		{
			m_billBoard->SetScale(Vec3(0.0f, 0.0f, 0.0f));
		}

		// �J�����̏����擾����
		auto stageCamera = OnGetDrawCamera();
		auto cameraPos = stageCamera->GetEye();

		// �J�����ƃp�[�c�̋��������
		distPos = cameraPos - m_pos;

		// �J�������p�[�c�����r���{�[�h�ɋ߂������瓧���ɂ���
		if (distPos.length() <= 15.0f)
		{
			m_billBoard->SetScale(Vec3(0.0f, 0.0f, 0.0f));
		}


		// ��]����
		auto partsQt = GetComponent<Transform>()->GetQuaternion();
		partsQt = partsQt * (Quat(0.0f, 1.0f * (sin(XMConvertToRadians(-2.0f) / 2.0f)), 0.0f, cos(XMConvertToRadians(-2.0f) / 2.0f)) * deltaTime);

		GetComponent<Transform>()->SetQuaternion(partsQt);
	}

	void Parts::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		//�p�[�c�̃f�[�^��n��
		if (Other->FindTag(L"Player"))
		{
			//SetDrawActive(false);
			GetStage()->RemoveGameObject<Parts>(GetThis<Parts>());
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
		Parts::OnUpdate();
	}

}