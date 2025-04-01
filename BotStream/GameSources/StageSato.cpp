/*!
@file StageSato.cpp
@brief �Q�[���X�e�[�W����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------
	void StageSato::CreateViewLight() {
		const Vec3 eye(0.0f, 5.0f, -10.0f);
		const Vec3 at(0.0f);
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(eye);
		PtrCamera->SetAt(at);
		//�}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		PtrMultiLight->SetDefaultLighting();
	}

#pragma comment(lib, "Effekseer.lib")
#pragma comment(lib, "EffekseerRendererDX11.lib")


	void StageSato::OnCreate() {
		try {
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();

			//Player�쐬
			auto player = AddGameObject<Player>(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
			SetSharedGameObject(L"Player", player);

			// �C���^�[�t�F�C�X�̍쐬
			m_ShEfkInterface = ObjectFactory::Create<EfkInterface>();

			// �p�X�擾
			wstring DataDir;
			App::GetApp()->GetDataDirectory(DataDir);
			wstring LaserDir = DataDir + L"Effect/Laser01.efk";
			wstring SwordDir = DataDir + L"Effect/Sword.efk";
			//wstring LandingDir = DataDir + L"LandingSample.efk";

			// �G�t�F�N�g�쐬
			m_EfkLaser = ObjectFactory::Create<EfkEffect>(m_ShEfkInterface, LaserDir);
			m_EfkSword = ObjectFactory::Create<EfkEffect>(m_ShEfkInterface, SwordDir);
			//m_EfkLanding = ObjectFactory::Create<EfkEffect>(m_ShEfkInterface, LandingDir);

			// ���W
			Vec3 sample = Vec3(0.0f, 0.0f, 0.0f);

			// �G�t�F�N�g�̃N���G�C�g
			//m_EfkPlay = ObjectFactory::Create<EfkPlay>(m_EfkLaser, sample, 30.0f);
		}
		catch (...) {
			throw;
		}

	}

	void StageSato::OnUpdate()
	{
		//�V�F�A�z��Ƀv���C���[��ǉ�
		m_Player = GetSharedGameObject<Player>(L"Player");
		auto plyrTrans = m_Player->GetTransform();
		auto plyrFwrd = m_Player->GetAngle();
		//bool plyrLanding = m_Player->GetPlyrGround();

		m_ShEfkInterface->OnUpdate();
		auto camera = GetView()->GetTargetCamera();
		m_ShEfkInterface->SetViewProj(camera->GetViewMatrix(), camera->GetProjMatrix());

		auto cntl = App::GetApp()->GetInputDevice().GetControlerVec();
		auto plPos = plyrTrans->GetPosition();

		if (cntl[0].wPressedButtons & XINPUT_GAMEPAD_B)
		{
			m_EfkPlay = ObjectFactory::Create<EfkPlay>(m_EfkLaser, plPos, 30.0f);
			m_EfkPlay->SetRotation(Vec3(0, 1, 0), plyrFwrd + XM_PIDIV2);
			m_EfkPlay->SetScale(Vec3(0.3f));
		}

		if (cntl[0].wPressedButtons & XINPUT_GAMEPAD_X)
		{
			m_EfkPlay = ObjectFactory::Create<EfkPlay>(m_EfkSword, plPos, 30.0f);
			m_EfkPlay->SetRotation(Vec3(0, 1, 0), plyrFwrd + XM_PI);
			m_EfkPlay->SetScale(Vec3(0.5f));
		}

	}

	void StageSato::OnDraw()
	{
		m_ShEfkInterface->OnDraw();
	}
}
//end basecross
