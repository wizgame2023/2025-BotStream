
/*!
@file Scene.cpp
@brief �V�[������
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	�Q�[���V�[��
	//--------------------------------------------------------------------------------------
	void Scene::OnCreate(){
		try {
			//�N���A����F��ݒ�
			Col4 Col;
			Col.set(31.0f / 255.0f, 30.0f / 255.0f, 71.0f / 255.0f, 255.0f / 255.0f);
			SetClearColor(Col);
			//�������g�ɃC�x���g�𑗂�
			//����ɂ��e�X�e�[�W��I�u�W�F�N�g��Create���ɃV�[���ɃA�N�Z�X�ł���
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToGameStage");
			GameResourses();
		}
		catch (...) {
			throw;
		}
	}

	Scene::~Scene() {
	}

	void Scene::OnEvent(const shared_ptr<Event>& event) {
		if (event->m_MsgStr == L"ToGameStage") {
			//�ŏ��̃A�N�e�B�u�X�e�[�W�̐ݒ�
			ResetActiveStage<StageTuboi>();
		}
	}

	void Scene::GameResourses()
	{
		auto& app = App::GetApp();

		auto path = app->GetDataDirWString();
		auto texPath = path + L"Textures/";
		auto modPath = path + L"Models/";
		auto SoundPath = path + L"Sounds/";

		//�e�N�X�`��
		wstring strTexture = texPath + L"StoneBrick.png";
		app->RegisterTexture(L"StoneBrick", strTexture);

		//���f��

		//�{�[���}���`���b�V��
		auto boneMultiModelMesh = MultiMeshResource::CreateBoneModelMultiMesh(modPath, L"Spearmen_Animation.bmf");//���̃v���C���[���b�V��(����)
		app->RegisterResource(L"Spearmen", boneMultiModelMesh);
		//�{�[���}���`���b�V���p�e�N�X�`��
		auto boneMultiModelTexture = modPath + L"Spearmen_T.png";
		app->RegisterTexture(L"SpearmenTexture", boneMultiModelTexture);

		//BGM
		app->RegisterWav(L"Title", SoundPath + L"Title.wav");
		app->RegisterWav(L"Diagnosis", SoundPath + L"Diagnosis.wav");
		app->RegisterWav(L"SelectStage", SoundPath + L"SelectStage.wav");
		app->RegisterWav(L"EnemyWave", SoundPath + L"EnemyWave.wav");
		app->RegisterWav(L"BossWave", SoundPath + L"BossWave.wav");
		app->RegisterWav(L"Result", SoundPath + L"Result.wav");


		//SE
		app->RegisterWav(L"Decision", SoundPath + L"Decision.wav");
		app->RegisterWav(L"Decision2", SoundPath + L"Decision2.wav");
		app->RegisterWav(L"Landing", SoundPath + L"Landing.wav");
		app->RegisterWav(L"Dash", SoundPath + L"Dash.wav");
		app->RegisterWav(L"ArmorBreak", SoundPath + L"ArmorBreak.wav");
		app->RegisterWav(L"Attack1", SoundPath + L"Attack1.wav");
		app->RegisterWav(L"Attack2", SoundPath + L"Attack2.wav");
		app->RegisterWav(L"Attack3", SoundPath + L"Attack3.wav");
		app->RegisterWav(L"HandGun", SoundPath + L"HandGun.wav");
		app->RegisterWav(L"Reload", SoundPath + L"Reload.wav");
		app->RegisterWav(L"AssaultRifle", SoundPath + L"AssaultRifle.wav");

	}

}
//end basecross
