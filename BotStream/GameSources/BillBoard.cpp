/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	BillBoard::BillBoard(
		const shared_ptr<Stage>& StagePtr,
		shared_ptr<GameObject>& actorPtr, 
		wstring spriteName, 
		int layer, 
		float pushY, 
		Vec3 scale, 
		Col4 color, 
		float pushX) :
		MyGameObject(StagePtr),
		m_actor(actorPtr),
		m_textureName(spriteName),
		m_pushY(pushY),
		m_scale(scale),
		m_color(color),
		m_layer(layer),
		m_pushX(pushX)
	{}
	BillBoard::~BillBoard() {}

	//������
	void BillBoard::OnCreate() {

		auto PtrTransform = GetComponent<Transform>();
		if (!m_actor.expired()) {
			auto SeekPtr = m_actor.lock();
			auto SeekTransPtr = SeekPtr->GetComponent<Transform>();
			auto Pos = SeekTransPtr->GetPosition();
			Pos.y += m_pushY;
			Pos.x += m_pushX;
			PtrTransform->SetPosition(Pos);
			PtrTransform->SetScale(m_scale);
			PtrTransform->SetQuaternion(SeekTransPtr->GetQuaternion());
			//�ύX�ł���X�N�G�A���\�[�X���쐬

			//�C���f�b�N�X���쐬���邽�߂̔z��
			//vector<uint16_t> indices;
			//Square�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateSquare(1.0f, m_vertices, m_indices);
			//UV�l�̕ύX
			//���㒸�_
			m_vertices[0].textureCoordinate = Vec2(0, 0);
			//�E�㒸�_
			m_vertices[1].textureCoordinate = Vec2(1, 0);
			//�������_
			m_vertices[2].textureCoordinate = Vec2(0, 1.0f);
			//�E�����_
			m_vertices[3].textureCoordinate = Vec2(1, 1.0f);

			//���_�̌^��ς����V�������_���쐬(�������Ȃ��ƐF�����Ȃ�)
			vector<VertexPositionColorTexture> new_vertices;
			for (auto& v : m_vertices) {
				VertexPositionColorTexture nv;
				nv.position = v.position;
				nv.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
				nv.textureCoordinate = v.textureCoordinate;
				new_vertices.push_back(nv);
			}
			//�V�������_���g���ă��b�V�����\�[�X�̍쐬
			m_SquareMeshResource = MeshResource::CreateMeshResource<VertexPositionColorTexture>(new_vertices, m_indices, true);

			auto DrawComp = AddComponent<PCTStaticDraw>();
			DrawComp->SetMeshResource(m_SquareMeshResource);
			DrawComp->SetTextureResource(m_textureName);
			// �G�t�F�N�g�̏����錻�ۂ̉���
			DrawComp->SetDepthStencilState(DepthStencilState::Read);
			SetAlphaActive(true);
			SetDrawLayer(m_layer);
		}

	}
	void BillBoard::OnUpdate() {
		if (m_actor.expired())
		{
			GetStage()->RemoveGameObject<BillBoard>(GetThis<BillBoard>());
		}
		if (!m_actor.expired()) {
			auto SeekPtr = m_actor.lock();
			auto SeekTransPtr = SeekPtr->GetComponent<Transform>();

			//�g�����X�t�H�[���擾
			auto PtrTransform = GetComponent<Transform>();
			auto Pos = SeekTransPtr->GetPosition();
			Pos.y += m_pushY;
			Pos.x += m_pushX;
			PtrTransform->SetPosition(Pos);
			PtrTransform->SetScale(m_scale);

			auto DrawComp = GetComponent<PCTStaticDraw>();
			DrawComp->SetTextureResource(m_textureName);//�e�N�X�`���X�V

			auto PtrCamera = GetStage()->GetView()->GetTargetCamera();

			Quat Qt;
			//�������J�����ڐ��ɂ���
			Qt = Billboard(PtrCamera->GetAt() - PtrCamera->GetEye());

			PtrTransform->SetQuaternion(Qt);

		}

	}

	//�r���{�[�h�̃e�N�X�`���ύX
	void BillBoard::ChangeTexture(wstring textureName)
	{
		m_textureName = textureName;
	}

	//�T�C�Y�̃Z�b�^�[
	void BillBoard::SetScale(Vec3 scale)
	{
		m_scale = scale;
	}

	//�o�����鍂���̃Z�b�^�[
	void BillBoard::SetPushY(float pushY)
	{
		m_pushY = pushY;
	}

	void BillBoard::SetBillUV(Vec2 topLeft, Vec2 botRight)
	{
		//UV�l�̕ύX
		//���㒸�_
		m_vertices[0].textureCoordinate = Vec2(topLeft);
		//�E�㒸�_
		m_vertices[1].textureCoordinate = Vec2(botRight.x, topLeft.y);
		//�������_
		m_vertices[2].textureCoordinate = Vec2(topLeft.x, botRight.y);
		//�E�����_
		m_vertices[3].textureCoordinate = Vec2(botRight);

		//�V�������_�ɍX�V
		vector<VertexPositionColorTexture> new_vertices;
		for (auto& v : m_vertices) {
			VertexPositionColorTexture nv;
			nv.position = v.position;
			nv.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
			nv.textureCoordinate = v.textureCoordinate;
			new_vertices.push_back(nv);
		}

		//�V�������_���g���ă��b�V�����\�[�X�̍쐬
		m_SquareMeshResource = MeshResource::CreateMeshResource<VertexPositionColorTexture>(new_vertices, m_indices, true);

		//���b�V���̍X�V
		auto DrawComp = GetComponent<PCTStaticDraw>();
		DrawComp->SetMeshResource(m_SquareMeshResource);
		DrawComp->SetTextureResource(m_textureName);


	}
	
	void BillBoard::RemoveBill()
	{
		// �폜
		GetStage()->RemoveGameObject<BillBoard>(GetThis<BillBoard>());
	}

	//------------------------------------------------
	// �_���[�W�r���{�[�h
	//------------------------------------------------
	DamageBill::DamageBill(
		const shared_ptr<Stage>& stagePtr,
		shared_ptr<GameObject>& actorPtr,
		wstring spriteName,
		int layer,
		float pushY,
		Vec3 scale,
		Col4 color,
		float pushX,
		float displayTime)
		: BillBoard(stagePtr, actorPtr, spriteName, layer, pushY, scale, color, pushX),
		m_displayTime(displayTime)
	{
	}

	// ==============================================
	// �r���{�[�h�̃N���X���p�����Ă��邩��A������������������������������������OnCreate�͏����ȁI�I�I�I�I�I�I�I�I�I�I
	// ==============================================

	void DamageBill::OnUpdate()
	{
		// �A�N�^�����݂��Ȃ����A�\�����Ԃ𒴂����ꍇ�͂��̃r���{�[�h���폜
		if (m_actor.expired() || m_time >= m_displayTime)
		{
			GetStage()->RemoveGameObject<DamageBill>(GetThis<DamageBill>());
			return;
		}

		m_time += App::GetApp()->GetElapsedTime();

		auto actorPtr = m_actor.lock();
		// �󂯎�����A�N�^��Transform���擾
		auto actorTrans = actorPtr->GetComponent<Transform>();
		// �J�����̈ʒu���擾
		auto cam = GetStage()->GetView()->GetTargetCamera();

		// �J�����̕������擾���ăr���{�[�h�̌������v�Z
		Quat qt = Billboard(cam->GetAt() - cam->GetEye());

		// �r���{�[�h�̈ʒu����
		Vec3 localOffset(m_pushX, 0.0f, 0.0f);
		// ��]�̒���
		Vec3 rotatedOffset = rotate(qt, localOffset);

		// �A�N�^�̈ʒu���擾���āA�r���{�[�h�̈ʒu���v�Z
		Vec3 pos = actorTrans->GetPosition();
		pos += Vec3(0.0f, m_pushY, 0.0f);
		pos += rotatedOffset;

		auto trans = GetComponent<Transform>();
		trans->SetPosition(pos);
		trans->SetScale(m_scale);
		trans->SetQuaternion(qt);

		auto draw = GetComponent<PCTStaticDraw>();
		draw->SetTextureResource(m_textureName);
	}

	//------------------------------------------------
	// �_���[�W�r���{�[�h�̖{�̍��W�݂�����
	//------------------------------------------------
	DamageBillRoot::DamageBillRoot(const shared_ptr<Stage>& stagePtr, shared_ptr<GameObject>& actorPtr, float pushY)
		: MyGameObject(stagePtr), m_actor(actorPtr), m_pushY(pushY) {
	}

	Quat DamageBillRoot::Billboard(const Vec3& line)
	{
		Vec3 temp = line;
		Mat4x4 rot;
		Vec3 up(0, 1.0f, 0);
		Vec2 tempXZ(temp.x, temp.z);
		if (tempXZ.length() < 0.1f) {
			up = Vec3(0, 0, 1.0f);
		}
		temp.normalize();
		rot = XMMatrixLookAtLH(Vec3(0, 0, 0), temp, up);
		rot.inverse();
		Quat qt = rot.quatInMatrix();
		qt.normalize();
		return qt;
	}

	void DamageBillRoot::OnCreate()
	{
		SetDrawActive(false); // ����
	}

	void DamageBillRoot::OnUpdate()
	{
		// �A�N�^�����݂��Ȃ��ꍇ�͂��̃r���{�[�h���폜
		if (m_actor.expired()) {
			GetStage()->RemoveGameObject<DamageBillRoot>(GetThis<DamageBillRoot>());
			return;
		}

		auto actorLock = m_actor.lock();
		auto actorTrans = actorLock->GetComponent<Transform>();
		auto trans = GetComponent<Transform>();

		Vec3 pos = actorTrans->GetPosition();
		pos.y += m_pushY;
		trans->SetPosition(pos);

		auto cam = GetStage()->GetView()->GetTargetCamera();
		Quat qt = Billboard(cam->GetAt() - cam->GetEye());
		trans->SetQuaternion(qt);
	}

}

//end basecross
