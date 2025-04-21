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
		//PtrMultiLight->
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

		CreateSharedObjectGroup(L"Actor");

		auto grond = AddGameObject<Ground>();
		grond->AddTag(L"CameraObstacles");//�J�����̏�Q�ɂȂ肦�镨�ɕt������^�O
		grond->GetComponent<Transform>()->SetScale(Vec3(500.0f, 6.0f, 500.0f));
		grond->GetComponent<Transform>()->SetPosition(Vec3(0.0f, -5.0f, 0.0f));

		
		//Player�쐬
		auto player = AddGameObject<Player>(Vec3(0.0f,0.0f,0.0f),Vec3(0.0f,0.0f,0.0f),Vec3(1.0f,1.0f,1.0f));
		SetSharedGameObject(L"Player", player);

		//�e�X�g�p�̃L���[�u
		AddGameObject<Cube>(Vec3(10.0f, 0.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		AddGameObject<Cube>(Vec3(10.0f, 0.0f, -10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Col4(1.0f, 0.0f, 0.0f, 1.0f));
		AddGameObject<Cube>(Vec3(10.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Col4(1.0f, 1.0f, 0.0f, 1.0f));
		AddGameObject<Cube>(Vec3(-10.0f, 0.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Col4(0.0f, 1.0f, 0.0f, 1.0f));
		AddGameObject<Cube>(Vec3(-10.0f, 0.0f, -10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Col4(0.0f, 0.0f, 1.0f, 1.0f));
		AddGameObject<Cube>(Vec3(-10.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Col4(1.0f, 0.0f, 1.0f, 1.0f));
		AddGameObject<Cube>(Vec3(0.0f, 0.0f, -8.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Col4(1.0f, 0.0f, 1.0f, 1.0f));
		AddGameObject<Cube>(Vec3(0.0f, 0.0f, +10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Col4(1.0f, 0.5f, 0.5f, 1.0f));

		//�J�������ז����邽�߂̃L���[�u
		//auto zyama = AddGameObject<Cube>(Vec3(0.0f, 0.0f, +15.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(30.0f, 50.0f, 1.0f), Col4(1.0f, 0.5f, 0.5f, 1.0f));
		//zyama->AddTag(L"CameraObstacles");
		//zyama =AddGameObject<Cube>(Vec3(0.0f, 0.0f, -15.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(30.0f, 50.0f, 1.0f), Col4(1.0f, 0.5f, 0.5f, 1.0f));
		//zyama->AddTag(L"CameraObstacles");
		//zyama =AddGameObject<Cube>(Vec3(15.0f, 0.0f, 0.0f), Vec3(0.0f, XMConvertToRadians(90.0f), 0.0f), Vec3(30.0f, 50.0f, 1.0f), Col4(1.0f, 0.5f, 0.5f, 1.0f));
		//zyama->AddTag(L"CameraObstacles");
		//zyama =AddGameObject<Cube>(Vec3(-15.0f, 0.0f, 0.0f), Vec3(0.0f, XMConvertToRadians(90.0f), 0.0f), Vec3(30.0f, 50.0f, 1.0f), Col4(1.0f, 0.5f, 0.5f, 1.0f));
		//zyama->AddTag(L"CameraObstacles");
		//zyama =AddGameObject<Cube>(Vec3(0.0f, 0.0f, +20.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(50.0f, 50.0f, 1.0f), Col4(1.0f, 0.5f, 0.5f, 1.0f));
		//zyama->AddTag(L"CameraObstacles");
		//zyama =AddGameObject<Cube>(Vec3(0.0f, 0.0f, -20.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(50.0f, 50.0f, 1.0f), Col4(1.0f, 0.5f, 0.5f, 1.0f));
		//zyama->AddTag(L"CameraObstacles");
		//zyama =AddGameObject<Cube>(Vec3(20.0f, 0.0f, 0.0f), Vec3(0.0f, XMConvertToRadians(90.0f), 0.0f), Vec3(50.0f, 50.0f, 1.0f), Col4(1.0f, 0.5f, 0.5f, 1.0f));
		//zyama->AddTag(L"CameraObstacles");
		//zyama =AddGameObject<Cube>(Vec3(-20.0f, 0.0f, 0.0f), Vec3(0.0f, XMConvertToRadians(90.0f), 0.0f), Vec3(50.0f, 50.0f, 1.0f), Col4(1.0f, 0.5f, 0.5f, 1.0f));
		//zyama->AddTag(L"CameraObstacles");


		//AddGameObject<Cube>(Vec3(0.0f, -1.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(30.0f, 1.0f, 30.0f), Col4(1.0f, 1.0f, 1.0f, 1.0f));

		//AddGameObject<Enemy>(Vec3(10.0f, 0.0f, -10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		//�G�l�~�[�}�l�[�W���[
		auto enemyMgr = AddGameObject<EnemyManager>();
		SetSharedGameObject(L"EnemyManager", enemyMgr);

		//�{�X����
		auto boss = AddGameObject<BossFirst>(Vec3(10, 10, 1), Vec3(0), Vec3(1));
		SetSharedGameObject(L"Boss", boss);
		enemyMgr->InstBoss(dynamic_pointer_cast<EnemyBase>(boss));

		//enemyMgr->InstEnemy(Vec3(10.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		//enemyMgr->InstEnemy(Vec3(10.0f, 0.0f, -10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		//enemyMgr->InstEnemy(Vec3(10.0f, 0.0f, +10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		//enemyMgr->InstEnemy(Vec3(-10.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		//enemyMgr->InstEnemy(Vec3(-10.0f, 0.0f, -10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		//enemyMgr->InstEnemy(Vec3(-10.0f, 0.0f, +10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		//enemyMgr->InstEnemy(Vec3(0.0f, 0.0f, +10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		//enemyMgr->InstEnemy(Vec3(0.0f, 0.0f, -10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));

		//�G�̍U������
		auto enemyAttackObj = AddGameObject<EnemyCube>(Vec3(-20.0f, -1.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(3.0f, 1.0f, 3.0f), Col4(1.0f, 0.0f, 1.0f, 1.0f));

		//Player�֌W��UI����
		auto playerUI = AddGameObject<PlayerGaugeUI>(100);
		SetSharedGameObject(L"PlayerUI", playerUI);
		//playerUI->SetPLMaxHPSprite(player->GetHPMax());//

		//�J�����}�l�[�W���쐬
		auto cameraManager = AddGameObject<CameraManager>();
		SetSharedGameObject(L"CameraManager", cameraManager);

		//�R���W�����}�l�[�W���[�쐬
		AddGameObject<StageCollisionController>();
	}

	void StageSanpei::OnUpdate()
	{
		//�G�t�F�N�g�֌W
		EffectManager::Instance().InterfaceUpdate();
	}

	void StageSanpei::OnDraw()
	{
		EffectManager::Instance().InterfaceDraw();
	}

}
//end basecross
