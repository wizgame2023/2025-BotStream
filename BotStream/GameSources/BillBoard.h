/*!
@file Character.h
@brief キャラクターなど
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

		vector<uint16_t> m_indices;//インデックス情報

		vector<VertexPositionNormalTexture> m_vertices;

		wstring m_textureName;

		Vec3 m_scale;

		Col4 m_color;

		float m_pushY;

		float m_pushX;

		int m_layer;

		// ビルボードの削除までの時間
		float m_removeTime = 0.0f; 
	public:
		//構築と破棄
		BillBoard(const shared_ptr<Stage>& StagePtr,
			shared_ptr<GameObject>& actorPtr,
			wstring spriteName, 
			int layer = 2, 
			float pushY = 18.0f,
			Vec3 scale = Vec3(3.0f, 3.0f, 3.0f), 
			Col4 color = Col4(1.0f, 1.0f, 1.0f, 1.0f), 
			float pushX = 0.0f);

		virtual ~BillBoard();
		//初期化
		virtual void OnCreate() override;
		//変化
		virtual void OnUpdate() override;

		//スプライト変更
		virtual void ChangeTexture(wstring spriteName);

		//サイズ変更
		virtual void SetScale(Vec3 scale);

		//出現する高さ変更
		virtual void SetPushY(float pushY);

		// UV値の変更
		void SetBillUV(Vec2 topLeft, Vec2 botRight);

		// ビルボードの削除
		void RemoveBill();
	};

	// ダメージビルボード
	class DamageBill : public BillBoard 
	{
	public:
		DamageBill(
			const shared_ptr<Stage>& stagePtr,
			shared_ptr<GameObject>& actorPtr,
			wstring spriteName,
			int layer = 2,
			float pushY = 18.0f,
			Vec3 scale = Vec3(3.0f, 3.0f, 3.0f),
			Col4 color = Col4(1.0f, 1.0f, 1.0f, 1.0f),
			float pushX = 0.0f
		);

		virtual ~DamageBill() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		// ビルボード削除
		void RemoveBill();
	};

	// ダメージビルボードの本体座標みたいな
	class DamageBillRoot : public MyGameObject 
	{
		weak_ptr<GameObject> m_actor;
		float m_pushY;
		Quat Billboard(const Vec3& line);
	public:
		DamageBillRoot(const shared_ptr<Stage>& stagePtr, shared_ptr<GameObject>& actorPtr, float pushY = 18.0f);
		virtual ~DamageBillRoot() {}
		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		// ビルボード削除
		void RemoveBill();
	};
}
//end basecross