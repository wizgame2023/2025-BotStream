/*!
@file StageSanpei.cpp
@brief �Q�[���X�e�[�W����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------
	void StageSanpei::CreateViewLight() {
		const Vec3 eye(0.0f, 15.0f, -15.0f);
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



	void StageSanpei::OnCreate() {
		try {
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
		}
		catch (...) {
			throw;
		}	
		
		//Player�쐬
		auto player = AddGameObject<Player>(Vec3(0.0f,0.0f,0.0f),Vec3(0.0f,0.0f,0.0f),Vec3(1.0f,1.0f,1.0f));
		SetSharedGameObject(L"Player", player);

		AddGameObject<Cube>(Vec3(10.0f, 0.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		AddGameObject<Cube>(Vec3(10.0f, 0.0f, -10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f),Col4(1.0f,0.0f,0.0f,1.0f));
		AddGameObject<Cube>(Vec3(-10.0f, 0.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Col4(0.0f, 1.0f, 0.0f, 1.0f));
		AddGameObject<Cube>(Vec3(-10.0f, 0.0f, -10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Col4(0.0f, 0.0f, 1.0f, 1.0f));

		//�J�����}�l�[�W���쐬
		AddGameObject<CameraManager>();
	}

}
//end basecross
