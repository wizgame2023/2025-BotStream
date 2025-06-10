/*!
@file EffectManger.cpp
@brief ポーズを管理するManager
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross 
{
	//--------------------------------------------------------------------------------------
	//	class EffectManager
	//--------------------------------------------------------------------------------------
	EffectManager& EffectManager::Instance() 
	{
		static EffectManager instance;
		return instance;
	}

	void EffectManager::CreateEfkInterface()
	{
		if (m_isCreateEffectManager) return;
		// デバイスの取得
		auto dev = App::GetApp()->GetDeviceResources();
		auto pDx11Device = dev->GetD3DDevice();
		auto pID3D11DeviceContext = dev->GetD3DDeviceContext();

		// 描画用インスタンスの生成
		m_renderer = Renderer::Create(pDx11Device, pID3D11DeviceContext, 2000);
		
		// エフェクト管理用インスタンスの生成
		m_manager = Manager::Create(2000);

		// 描画用インスタンスから描画機能を設定
		m_manager->SetSpriteRenderer(m_renderer->CreateSpriteRenderer());
		m_manager->SetRibbonRenderer(m_renderer->CreateRibbonRenderer());
		m_manager->SetRingRenderer(m_renderer->CreateRingRenderer());
		m_manager->SetTrackRenderer(m_renderer->CreateTrackRenderer());
		m_manager->SetModelRenderer(m_renderer->CreateModelRenderer());

		// 描画用インスタンスからテクスチャの読込機能を設定
		// 独自拡張可能、現在はファイルから読み込んでいる。
		m_manager->SetTextureLoader(m_renderer->CreateTextureLoader());
		m_manager->SetModelLoader(m_renderer->CreateModelLoader());
		
		m_isCreateEffectManager = true;
	}

	void EffectManager::InterfaceUpdate()
	{
		m_manager->Update();

		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		auto camera = stage->GetView()->GetTargetCamera();
		SetViewProj(camera->GetViewMatrix(), camera->GetProjMatrix());
	}

	void EffectManager::InterfaceDraw()
	{
		m_renderer->BeginRendering();

		m_manager->Draw();

		m_renderer->EndRendering();
	}
	
	void Mat4x4ToMatrix44(const Mat4x4& src, Matrix44& dest)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				dest.Values[i][j] = src(i, j);
			}
		}
	}

	void EffectManager::SetViewProj(const Mat4x4& view, const Mat4x4& proj)
	{
		Matrix44 v, p;
		Mat4x4ToMatrix44(view, v);
		Mat4x4ToMatrix44(proj, p);
		m_renderer->SetCameraMatrix(v);
		m_renderer->SetProjectionMatrix(p);
	}

	void EffectManager::RegisterEffect(const wstring EfkKey, const wstring EfkFilePash)
	{
		// ファイルパスのnullチェック
		try 
		{
			if (EfkFilePash == L"") 
			{
				throw BaseException
				(
					L"ファイルパスが空白です",
					L"if (m_FileName == L\"\")",
					L"EfkEffect::EfkEffect()"
				);
			}
			// エフェクトを作成
			auto effect = Effect::Create(m_manager, (const char16_t*)EfkFilePash.c_str());
			// 重複キー登録を検査
			map<wstring, EffectRef>::iterator it;
			for (it = m_effectList.begin(); it != m_effectList.end(); it++)
			{
				if (it->first == EfkKey)
				{
					throw BaseException
					(
						L"重複したキーネームがあります",
						L"if (it->first == EfkKey)",
						L"EfkEffect::EfkEffect()"
					);
				}
			}
			// リストに登録
			m_effectList[EfkKey] = effect;
		}
		catch (...) 
		{
			throw;
		}
	}

	Handle EffectManager::PlayEffect(const wstring EfkKey, const Vec3 Emitter, const float StartFreme)
	{
		return m_manager->Play(m_effectList[EfkKey], Vector3D(Emitter.x, Emitter.y, Emitter.z), StartFreme);
	}

	void EffectManager::AddPosition(const Handle& EfkHandle, const Vec3 Position)
	{
		m_manager->AddLocation(EfkHandle, Vector3D(Position.x, Position.y, Position.z));
	}

	void EffectManager::SetPosition(const Handle& EfkHandle, const Vec3 Position)
	{
		m_manager->SetLocation(EfkHandle, Vector3D(Position.x, Position.y, Position.z));
	}

	void EffectManager::SetRotationFromEulerAngles(const Handle& EfkHandle, const Vec3 EulerAngles)
	{
		float pitch = XMConvertToRadians(EulerAngles.x);
		float yaw = XMConvertToRadians(EulerAngles.y);
		float roll = XMConvertToRadians(EulerAngles.z);

		XMVECTOR quat = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);

		// クォータニオンの成分を抽出
		float x = XMVectorGetX(quat);
		float y = XMVectorGetY(quat);
		float z = XMVectorGetZ(quat);
		float w = XMVectorGetW(quat);

		// 軸、角度変換
		float angle = 2.0f * acosf(w);
		float s = sqrtf(1.0f - w * w);
		Vec3 axis;

		if (s < 0.001f)
		{
			// 角度が非常に小さい場合、回転軸は任意に設定。ここではX軸を使う
			axis = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
		}
		else
		{
			axis = XMVectorSet(x / s, y / s, z / s, 0.0f);
		}

		m_manager->SetRotation(EfkHandle, Vector3D(axis.x, axis.y, axis.z), angle);
	}

	void EffectManager::SetRotationFromQuaternion(const Handle& EfkHandle, const Quat Quaternion)
	{
		// クォータニオンの成分を抽出
		float x = XMVectorGetX(Quaternion);
		float y = XMVectorGetY(Quaternion);
		float z = XMVectorGetZ(Quaternion);
		float w = XMVectorGetW(Quaternion);

		// 軸、角度変換
		float angle = 2.0f * acosf(w);
		float s = sqrtf(1.0f - w * w);
		Vec3 axis;

		if (s < 0.001f)
		{
			// 角度が非常に小さい場合、回転軸は任意に設定。ここではX軸を使う
			axis = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
		}
		else
		{
			axis = XMVectorSet(x / s, y / s, z / s, 0.0f);
		}

		m_manager->SetRotation(EfkHandle, Vector3D(axis.x, axis.y, axis.z), angle);
	}

	void EffectManager::SetRotationFromAxisAngle(const Handle& EfkHandle, const Vec3 Axis, const float Angle)
	{
		m_manager->SetRotation(EfkHandle, Vector3D(Axis.x, Axis.y, Axis.z), Angle);
	}

	void EffectManager::SetScale(const Handle& EfkHandle, const Vec3 Scale)
	{
		m_manager->SetScale(EfkHandle, Scale.x, Scale.y, Scale.z);
	}
	
	void EffectManager::SetAllColor(const Handle& EfkHandle, const Col4 Col)
	{
		auto color = Col4(Col) * 255;
		m_manager->SetAllColor(EfkHandle, Color(color.x, color.y, color.z, color.w));
	}

	void EffectManager::StopEffect(const Handle& EfkHandle)
	{
		if (EfkHandle != 1)
		{
			m_manager->StopEffect(EfkHandle);
		}
	}

	void EffectManager::SetSpeed(const Handle& EfkHandle, const float Speed)
	{
		m_manager->SetSpeed(EfkHandle, Speed);
	}

	Handle EffectManager::GetEffNull()
	{
		return Handle(-10000);
	}

}