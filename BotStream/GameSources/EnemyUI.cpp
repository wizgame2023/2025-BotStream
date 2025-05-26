#pragma once
/*!
@file EnemyUI.cpp
@brief �G�p��UI����
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	BillBoardGauge::BillBoardGauge(const shared_ptr<Stage>& StagePtr,
		shared_ptr<GameObject>& actorPtr, wstring spriteName, int layer, float pushY, Vec3 scale) :
		BillBoard(StagePtr, actorPtr, spriteName, layer, pushY, scale),
		m_indices(vector<uint16_t>())
	{

	}

	BillBoardGauge::~BillBoardGauge()
	{

	}

	void BillBoardGauge::OnCreate()
	{
		m_parsecond = 0;//�S�̂̉��p�[�Z���g�o�������߂�
		auto PtrTransform = GetComponent<Transform>();
		if (!m_actor.expired()) {
			auto SeekPtr = m_actor.lock();
			auto SeekTransPtr = SeekPtr->GetComponent<Transform>();
			auto Pos = SeekTransPtr->GetPosition();
			Pos.y += m_pushY;
			PtrTransform->SetPosition(Pos);
			PtrTransform->SetScale(m_scale);
			PtrTransform->SetQuaternion(SeekTransPtr->GetQuaternion());
			//�ύX�ł���X�N�G�A���\�[�X���쐬

			//Square�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateSquare(1.0f, m_vertices, m_indices);

			//UV�l�̕ύX
			//���㒸�_
			m_vertices[0].textureCoordinate = Vec2(0.0f, 0.0f);
			//�E�㒸�_
			m_vertices[1].textureCoordinate = Vec2(1.0f * m_parsecond, 0.0f);
			//�������_
			m_vertices[2].textureCoordinate = Vec2(0.0f, 1.0f);
			//�E�����_
			m_vertices[3].textureCoordinate = Vec2(1.0f * m_parsecond, 1.0f);

			//���_�̌^��ς����V�������_���쐬
			vector<VertexPositionColorTexture> new_vertices;
			for (auto& v : m_vertices) {
				VertexPositionColorTexture nv;
				nv.position = v.position;
				nv.color = Col4(1.0f, 0.0f, 0.0f, 1.0f);//��
				nv.textureCoordinate = v.textureCoordinate;
				new_vertices.push_back(nv);
			}
			//�V�������_���g���ă��b�V�����\�[�X�̍쐬
			m_SquareMeshResource = MeshResource::CreateMeshResource<VertexPositionColorTexture>(new_vertices, m_indices, true);

			auto DrawComp = AddComponent<PCTStaticDraw>();
			DrawComp->SetMeshResource(m_SquareMeshResource);
			DrawComp->SetTextureResource(m_textureName);
			SetAlphaActive(true);
			SetDrawLayer(m_layer);
		}

	}

	void BillBoardGauge::OnUpdate()
	{
		if (m_actor.expired())
		{
			GetStage()->RemoveGameObject<BillBoard>(GetThis<BillBoard>());
		}
		if (!m_actor.expired()) {
			auto SeekPtr = m_actor.lock();
			auto SeekTransPtr = SeekPtr->GetComponent<Transform>();

			//�A�j���[�V��������/////////////////////////////////////////
			m_vertices[1].position.x = -0.5f + (1.0f * m_parsecond);
			m_vertices[3].position.x = -0.5f + (1.0f * m_parsecond);
			m_vertices[1].position.y = 0.5f;
			m_vertices[3].position.y = -0.5f;
			//UV���W�̐ݒ�		
			auto move = (1.0f * m_parsecond);
			m_vertices[1].textureCoordinate.x = move;
			m_vertices[3].textureCoordinate.x = move;
			////////////////////////////////////////////////////////////

			//�V�������_�ɍX�V
			vector<VertexPositionColorTexture> new_vertices;
			for (auto& v : m_vertices) {
				VertexPositionColorTexture nv;
				nv.position = v.position;
				nv.color = Col4(1.0f, 0.0f, 0.0f, 1.0f);//��
				nv.textureCoordinate = v.textureCoordinate;
				new_vertices.push_back(nv);
			}
			//�V�������_���g���ă��b�V�����\�[�X�̍쐬
			m_SquareMeshResource = MeshResource::CreateMeshResource<VertexPositionColorTexture>(new_vertices, m_indices, true);

			auto PtrTransform = GetComponent<Transform>();
			auto Pos = SeekTransPtr->GetPosition();
			Pos.y += m_pushY;
			PtrTransform->SetPosition(Pos);
			PtrTransform->SetScale(m_scale);

			//���b�V���̍X�V
			auto DrawComp = GetComponent<PCTStaticDraw>();
			DrawComp->SetMeshResource(m_SquareMeshResource);
			DrawComp->SetTextureResource(m_textureName);

			auto PtrCamera = GetStage()->GetView()->GetTargetCamera();

			Quat Qt;
			//�������J�����ڐ��ɂ���
			Qt = Billboard(PtrCamera->GetAt() - PtrCamera->GetEye());

			PtrTransform->SetQuaternion(Qt);

		}
		//auto verticesVec = m_SquareMeshResource->GetBackupVerteces<VertexPositionColorTexture>();
		////verticesVec[1].position
		//
		////���[���h�}�g���b�N�X�擾
		//auto world = GetComponent<Transform>()->GetWorldMatrix();
		//world.transpose();//���[���h���W�ł���悤�ɕϊ�

	}

	//���p�[�Z���g�e�N�X�`�����o�������߂�
	void BillBoardGauge::SetPercent(float parcent)
	{
		m_parsecond = parcent;
	}


	//--------------------------------------------------
	//  �{�X��HP�Q�[�W
	//--------------------------------------------------

	void BossGaugeUI::OnCreate()
	{
		m_stage = GetStage();
		//m_hitPointMax = m
		CreateSprite();
		ClearBossGaugeUI(true);
	}

	void BossGaugeUI::OnUpdate()
	{
		Vec3 framePos = m_gaugeFrameSp->GetComponent<Transform>()->GetPosition();
		auto cntl = App::GetApp()->GetInputDevice().GetControlerVec();
		auto keybord = App::GetApp()->GetInputDevice().GetKeyState();

		// �䗦�ŃX�P�[�����O(HP)
		float hpRatio = m_hitPoint / m_hitPointMax;
		hpRatio = clamp(hpRatio, 0.0f, 1.0f);

		auto hpTrans = m_hitPointSp->GetComponent<Transform>();
		hpTrans->SetScale(Vec3(hpRatio, 1.0f, 1.0f));

		// ���[�Œ�
		const float gaugeWidth = 300.0f * 0.8f;
		float hpOffsetX = (hpRatio - 1.0f) * (gaugeWidth * 0.965f);
		hpTrans->SetPosition(Vec3(20.0f + hpOffsetX, -200.0f, 0.0f));

		// �g�̈ʒu����̑��΍��W�i�v���}�C�␳�j
		Vec3 hpOffset(0.0f * 0.066f, 0.7f, 0.0f);
		hpTrans->SetPosition(framePos + hpOffset + Vec3(hpOffsetX, 0.0f, 0.0f));

	}

	void BossGaugeUI::CreateSprite()
	{
		float frameSizeX = 500, frameSizeY = 75;
		m_gaugeFrameSp = m_stage->AddGameObject<Sprite>(
			L"BossGaugeFrame",
			Vec2(frameSizeX, frameSizeY),
			Vec3(0.0f, 350.0f, 0.0f)
		);

		auto framePos = m_gaugeFrameSp->GetPosition();

		m_hitPointSp = m_stage->AddGameObject<Sprite>(
			L"BossHPMater",
			Vec2(frameSizeX * 0.966f, frameSizeY * 0.826f),
			Vec3(framePos.x, framePos.y + 0.7f, framePos.z)
		);
	}
	// END ---------------------------------------------------

	//--------------------------------------------------
	//  �G�Ƀ_���[�W��^��������UI�݂����Ȃ��
	//--------------------------------------------------

	void EnemyDamageBill::OnCreate()
	{
		auto PtrTransform = GetComponent<Transform>();
		if (!m_actor.expired()) {
			auto SeekPtr = m_actor.lock();
			auto SeekTransPtr = SeekPtr->GetComponent<Transform>();
			auto Pos = SeekTransPtr->GetPosition();
			Pos.y += m_pushY;
			PtrTransform->SetPosition(Pos);
			PtrTransform->SetScale(m_scale);
			PtrTransform->SetQuaternion(SeekTransPtr->GetQuaternion());
			//�ύX�ł���X�N�G�A���\�[�X���쐬

			//Square�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateSquare(1.0f, m_vertices, m_indices);

			//SetBillUV(Vec2(0.0f, 0.0f), Vec2(0.1f, 1.0f));

			//���_�̌^��ς����V�������_���쐬
			vector<VertexPositionColorTexture> new_vertices;
			for (auto& v : m_vertices) {
				VertexPositionColorTexture nv;
				nv.position = v.position;
				nv.color = Col4(1.0f, 0.0f, 0.0f, 1.0f);//��
				nv.textureCoordinate = v.textureCoordinate;
				new_vertices.push_back(nv);
			}
			//�V�������_���g���ă��b�V�����\�[�X�̍쐬
			m_SquareMeshResource = MeshResource::CreateMeshResource<VertexPositionColorTexture>(new_vertices, m_indices, true);

			auto DrawComp = AddComponent<PCTStaticDraw>();
			DrawComp->SetMeshResource(m_SquareMeshResource);
			DrawComp->SetTextureResource(m_textureName);
			SetAlphaActive(true);
			SetDrawLayer(m_layer);
		}

	}

	void EnemyDamageBill::OnUpdate()
	{
		auto cntl = App::GetApp()->GetInputDevice().GetControlerVec();

		auto PtrCamera = GetStage()->GetView()->GetTargetCamera();
		auto PtrTransform = GetComponent<Transform>();

		Quat Qt;
		//�������J�����ڐ��ɂ���
		Qt = Billboard(PtrCamera->GetAt() - PtrCamera->GetEye());

		PtrTransform->SetQuaternion(Qt);
	}


	// �_���[�W�\��
	void EnemyDamageBill::ShowDamage(int value, float digitSize/*, BillBoard bill*/)
	{
		// ���l�𕶎���ɕϊ�
		std::string str = std::to_string(value);
		// UV�̕�
		float uvWidth = 1.0f / 10.0f;

		for (size_t i = 0; i < str.size(); ++i)
		{
			char setValue = str[i];

			if (setValue < '0' || setValue > '9') continue;

			int digit = setValue - '0';
			float u1 = digit * uvWidth;
			float u2 = u1 + uvWidth;

			SetBillUV(Vec2(u1, 0.0f), Vec2(u2, 1.0f));
		}
	}
	// END ---------------------------------------------------
}