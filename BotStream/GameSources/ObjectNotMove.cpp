/*!
@file ObjectNotMove.cpp
@brief �����Ȃ��I�u�W�F�N�g
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	ObjectNotMove::ObjectNotMove(const shared_ptr<Stage>& stagePtr) :
		MyGameObject(stagePtr)
	{

	}
	ObjectNotMove::~ObjectNotMove()
	{

	}


	//�Ó]�p�̃I�u�W�F�N�g
	void DarkObj::OnCreate()
	{
		//Transform�ݒ�
		auto m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(m_scale);

		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 0.21f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XMConvertToRadians(-0.0f), 0.0f),
			Vec3(0.0f, 0.0f, +0.1f)
		);

		//�h���[���b�V���̐ݒ�
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetTextureResource(L"Fadeout");
		ptrDraw->SetDiffuse(m_color);
		ptrDraw->SetOwnShadowActive(false);//�e�͏���
		ptrDraw->SetDrawActive(true);
		ptrDraw->SetEmissive(m_color); // ���Ȕ����J���[�i���C�e�B���O�ɂ��A�e���������ʂ�����j
		ptrDraw->SetOwnShadowActive(false); // �e�̉f�荞�݂𔽉f������
		ptrDraw->SetMeshToTransformMatrix(spanMat);

		//�R���W�����쐬
		auto ptrColl = AddComponent<CollisionObb>(); // �R���W�����X�t�B�A�̕����ǂɂԂ��锻��Ɉ�a�����Ȃ�
		ptrColl->SetAfterCollision(AfterCollision::None);
		ptrColl->SetDrawActive(false);
	}

	void DarkObj::OnUpdate()
	{

	}

	void DarkObj::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		if (!m_activateCollsion) return;

		auto player = dynamic_pointer_cast<Player>(Other);

		auto stage = GetWaveStage(true);
		auto firstStage = dynamic_pointer_cast<StageFirst>(stage);

		

		if (player)
		{
			//�X�e�[�W�Ó]�J�n
			firstStage->FadeOutStart();
		}


		//auto test = (StageFirst)stage;
	}

}
//end basecross
