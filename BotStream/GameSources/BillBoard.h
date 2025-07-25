/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class BillBoard :public MyGameObject
	{
	protected:
		Quat Billboard(const Vec3& Line)
		{
			Vec3 Temp = Line;
			Mat4x4 RotMatrix;
			Vec3 DefUp(0, 1.0f, 0);
			Vec2 TempVec2(Temp.x, Temp.z);
			if (TempVec2.length() < 0.1f)
			{
				DefUp = Vec3(0, 0, 1.0f);
			}
			Temp.normalize();
			RotMatrix = XMMatrixLookAtLH(Vec3(0, 0, 0), Temp, DefUp);
			RotMatrix.inverse();
			Quat Qt;
			Qt = RotMatrix.quatInMatrix();
			Qt.normalize();
			return Qt;
		}

		weak_ptr<GameObject> m_actor;

		shared_ptr<MeshResource> m_SquareMeshResource;

		vector<uint16_t> m_indices;//�C���f�b�N�X���

		vector<VertexPositionNormalTexture> m_vertices;

		wstring m_textureName;

		Vec3 m_scale;

		Col4 m_color;

		float m_pushY;

		float m_pushX;

		int m_layer;

		// �r���{�[�h�̍폜�܂ł̎���
		float m_removeTime = 0.0f;
	public:
		//�\�z�Ɣj��
		BillBoard(const shared_ptr<Stage>& StagePtr,
			const shared_ptr<GameObject>& actorPtr,
			const wstring& spriteName,
			int layer = 2,
			float pushY = 18.0f,
			const Vec3& scale = Vec3(3.0f, 3.0f, 3.0f),
			const Col4& color = Col4(1.0f, 1.0f, 1.0f, 1.0f),
			float pushX = 0.0f);

		virtual ~BillBoard();
		//������
		virtual void OnCreate() override;
		//�ω�
		virtual void OnUpdate() override;

		//�X�v���C�g�ύX
		virtual void ChangeTexture(wstring spriteName);

		//�T�C�Y�ύX
		virtual void SetScale(Vec3 scale);

		//�o�����鍂���ύX
		virtual void SetPushY(float pushY);

		// UV�l�̕ύX
		void SetBillUV(Vec2 topLeft, Vec2 botRight);

		// �r���{�[�h�̍폜
		void RemoveBill();
	};

	// �_���[�W�r���{�[�h
	class DamageBill : public BillBoard
	{
		//�r���{�[�h��\�����Ă�������
		float m_displayTime = 0.0f;

		// ���݂̎���
		float m_time = 0.0f;
	public:
		DamageBill(
			const shared_ptr<Stage>& stagePtr,
			const shared_ptr<GameObject>& actorPtr,
			const wstring& spriteName,
			int layer = 2,
			float pushY = 18.0f,
			const Vec3& scale = Vec3(3.0f, 3.0f, 3.0f),
			const Col4& color = Col4(1.0f, 1.0f, 1.0f, 1.0f),
			float pushX = 0.0f,

			float displayTime = 1.0f // �r���{�[�h�̕\������
		);

		virtual ~DamageBill() {}

		virtual void OnUpdate() override;
	};

}
//end basecross