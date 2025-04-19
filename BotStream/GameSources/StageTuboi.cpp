/*!
@file GameTuboi.cpp
@brief �Q�[���X�e�[�W����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------
	void StageTuboi::CreateViewLight() {
		const Vec3 eye(0.0f, 5.0f, -5.0f);
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



	void StageTuboi::OnCreate() {
		try {
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
		}
		catch (...) {
			throw;
		}

		CreateSprite();
		AddGameObject<Ground>();

		CreateSharedObjectGroup(L"Actor");

		auto player = AddGameObject<Player>(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		SetSharedGameObject(L"Player", player);

		auto enemyMgr = AddGameObject<EnemyManager>();
		SetSharedGameObject(L"EnemyManager", enemyMgr);

		auto boss = AddGameObject<BossFirst>(Vec3(1, 10 ,1), Vec3(0), Vec3(1));
		SetSharedGameObject(L"Boss", boss);
		enemyMgr->InstBoss(dynamic_pointer_cast<EnemyBase>(boss));

		//test
		enemyMgr->InstEnemy(Vec3(1), Vec3(0), Vec3(1));

		//�J�����}�l�[�W���쐬
		auto cameraMgr = AddGameObject<CameraManager>();
		SetSharedGameObject(L"CameraManager", cameraMgr);
	}

	//�X�v���C�g�֌W�A�X�e�[�W�ł���Ă����̂��킩��񂩂��ŕ���
	void StageTuboi::CreateSprite()
	{
		//�R���g���[���[�֌W--------------------------------------------------
		const float buttonPosX = 500, buttonPosY = -200;
		const Vec2 AUV1(0.0f, 0.0f), AUV2(0.333f, 0.25f), XUVX1(0.333f, 0.0f), XUVX2(0.666f, 0.25f),
			RBUV1(0.0f, 0.25f), RBUV2(0.333f, 0.5f), RSUV1(0.333f, 0.25f), RSUV2(0.666f, 0.5f),
			LSUV1(0.666f, 0.25f), LSUV2(1.0f, 0.5f), HOLDAUV1(0.0f, 0.5f), HOLDAUV2(0.666f, 1.0f),
			buttonSize(50.0f, 50.0f);
		//A
		auto sprite = AddGameObject<Sprite>(
			L"Buttons",         // �e�N�X�`����
			buttonSize,      // �T�C�Y
			Vec3(buttonPosX, buttonPosY, 0));        // �\���ʒu
		sprite->SetUVRect(AUV1, AUV2);

		//X
		sprite = AddGameObject<Sprite>(
			L"Buttons",         // �e�N�X�`����
			buttonSize,      // �T�C�Y
			Vec3(buttonPosX - 75, buttonPosY, 0));        // �\���ʒu
		sprite->SetUVRect(XUVX1, XUVX2);

		//RB
		sprite = AddGameObject<Sprite>(
			L"Buttons",         // �e�N�X�`����
			buttonSize,      // �T�C�Y
			Vec3(buttonPosX + 75, buttonPosY, 0));        // �\���ʒu
		sprite->SetUVRect(RBUV1, RBUV2);

		//RS
		sprite = AddGameObject<Sprite>(
			L"Buttons",         // �e�N�X�`����
			buttonSize,      // �T�C�Y
			Vec3(buttonPosX, buttonPosY + 75, 0));        // �\���ʒu
		sprite->SetUVRect(RSUV1, RSUV2);

		//LS
		sprite = AddGameObject<Sprite>(
			L"Buttons",         // �e�N�X�`����
			buttonSize,      // �T�C�Y
			Vec3(buttonPosX + 75, buttonPosY + 75, 0));        // �\���ʒu
		sprite->SetUVRect(LSUV1, LSUV2);

		//HOLD + A
		sprite = AddGameObject<Sprite>(
			L"Buttons",         // �e�N�X�`����
			buttonSize,      // �T�C�Y
			Vec3(buttonPosX - 75, buttonPosY + 75, 0));        // �\���ʒu
		sprite->SetUVRect(HOLDAUV1, HOLDAUV2);

		//--------------------------------------------------------------------

		//����̃e�L�X�g�֌W--------------------------------------------------
		const Vec2 ATKUV1(0.0f, 0.0f), ATKUV2(0.5f, 0.333f),
			AVOIDUV1(0.5f, 0.0f), AVOIDUV2(1.0f, 0.333f),
			CAMERAUV1(0.0f, 0.333f), CAMERAUV2(0.5f, 0.666f),
			DASHUV1(0.5f, 0.333f), DASHUV2(1.0f, 0.666f),
			LOCKONUV1(0.0f, 0.666f), LOCKONUV2(0.5f, 1.0f),
			MOVEUV1(0.5f, 0.666f), MOVEUV2(1.0f, 1.0f),
			textSize(100, 50);

		const float textPosX = -500, textPosY = -200;

		//�U��
		sprite = AddGameObject<Sprite>(
			L"Texts",			//�e�N�X�`����
			textSize,      // �T�C�Y
			Vec3(textPosX, buttonPosY, 0));        // �\���ʒu
		sprite->SetUVRect(ATKUV1, ATKUV2);

		//���
		sprite = AddGameObject<Sprite>(
			L"Texts",			//�e�N�X�`����
			textSize,      // �T�C�Y
			Vec3(textPosX + 75, buttonPosY, 0));        // �\���ʒu
		sprite->SetUVRect(AVOIDUV1, AVOIDUV2);

		// �J����
		sprite = AddGameObject<Sprite>(
			L"Texts", 			//�e�N�X�`����
			textSize,      // �T�C�Y
			Vec3(textPosX, textPosY + 75, 0));		//�\���ʒu
		sprite->SetUVRect(CAMERAUV1, CAMERAUV2);

		// �_�b�V��
		sprite = AddGameObject<Sprite>(
			L"Texts",  			//�e�N�X�`����
			textSize,       // �T�C�Y
			Vec3(textPosX + 75, textPosY + 75, 0));		//�\���ʒu
		sprite->SetUVRect(DASHUV1, DASHUV2);

		// ���b�N�I��
		sprite = AddGameObject<Sprite>(
			L"Texts",  			//�e�N�X�`����
			textSize,       // �T�C�Y
			Vec3(textPosX, textPosY + 150, 0));		//�\���ʒu
		sprite->SetUVRect(LOCKONUV1, LOCKONUV2);

		// �ړ�
		sprite = AddGameObject<Sprite>(
			L"Texts",  			//�e�N�X�`����
			textSize,       // �T�C�Y
			Vec3(textPosX + 75, textPosY + 150, 0));	//�\���ʒu
		sprite->SetUVRect(MOVEUV1, MOVEUV2);

		//--------------------------------------------------------------------

		//�v���C���[�̃Q�[�W�֌W----------------------------------------------
		const Vec2 gaugeSize(300, 75), hpGaugeSize(gaugeSize.x * 0.8f, gaugeSize.y * 0.3f),
			spGaugeSize(gaugeSize.x * 0.47f, gaugeSize.y * 0.09f);

		//hp�̏����ʒu
		const float gaugePosX = 0.0f, gaugePosY = -250;

		m_gaugeFrameSprite = AddGameObject<Sprite>(
			L"PLGauge",  			//�e�N�X�`����
			gaugeSize,       // �T�C�Y
			Vec3(gaugePosX, gaugePosY, 0));	//�\���ʒu
		m_gaugeFrameSprite->SetDrawLayer(1);

		m_plHPSprite = AddGameObject<Sprite>(
			L"PLHP",  			//�e�N�X�`����
			hpGaugeSize,       // �T�C�Y
			Vec3(gaugePosX + (gaugeSize.x * 0.066f), gaugePosY - 0.7f, 0));	//�\���ʒu
		m_plHPSprite->SetDrawLayer(2);

		m_plSPSprite = AddGameObject<Sprite>(
			L"PLSP",  			//�e�N�X�`����
			spGaugeSize,       // �T�C�Y
			Vec3(gaugePosX - (gaugeSize.x * 0.098f), gaugePosY - 19.8f, 0));	//�\���ʒu
		m_plSPSprite->SetDrawLayer(2);

		//--------------------------------------------------------------------

		//���݂̍U�����@�֌W--------------------------------------------------
		Vec2 weaponSize(40, 40);
		// �ߐ�
		m_katanaSprite = AddGameObject<Sprite>(
			L"KatanaTex",  			//�e�N�X�`����
			weaponSize,       // �T�C�Y
			Vec3(200, 150, 0));	//�\���ʒu

		m_gunSprite = AddGameObject<Sprite>(
			L"GunTex",  			//�e�N�X�`����
			weaponSize,       // �T�C�Y
			Vec3(200, 150, 0));	//�\���ʒu
		m_gunSprite->OnClear(true);
		//--------------------------------------------------------------------
	}

}
//end basecross
