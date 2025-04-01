/*!
@file CameraManager.h
@brief �J�����̊Ǘ�
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
#include "Player.h"

namespace basecross{
	class Enemy;
	class CameraManager : public MyGameObject
	{
	private:
		weak_ptr<Camera> m_stageCamera;//�X�e�[�W��̃J����
		shared_ptr<Camera> m_lockStageCamera;//���b�N�ł����X�e�[�W��̃J����
		shared_ptr<PNTStaticDraw> m_ptrDraw;

		float m_cameraAngle;  //Player���猩�ăJ�����̂���p�x
		float m_range;//Player����ǂ̂��炢����邩
		float m_targetRange;//���b�N�I���͈̔�

		shared_ptr<Enemy> m_targetObj;//�^�[�Q�b�g�Ώ�
		bool m_lockOn;//���b�N�I�����邩���Ȃ����̕ϐ�
		int m_lockOnNum;//���b�N�I���Ώۂ����߂邽�߂̕ϐ�

	public:
		CameraManager(const shared_ptr<Stage>& stagePtr,float range = 20.0f,float targetRange = 15.0f);
		~CameraManager();

		void OnCreate()override;//�쐬
		void OnUpdate()override;//�X�V

		void LockOn(shared_ptr<GameObject> lockOnObj, shared_ptr<Player> originObj);//���b�N�I���@�\
		void MovePlayerAngle(float playerAngle);//Player�̔w����������p�x�ɂ���
		void AdjustmentAngle();//�p�x�̒���

	};

	//�J�����̃��C�L���X�g
	class CameraRayCast :public ObjectMove
	{
	private:
		Vec3 m_pos;//�ʒu
		Vec3 m_rot;//��]
		Vec3 m_scale;//�傫��

	public:
		CameraRayCast(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale);
		~CameraRayCast();

		void OnCreate()override;
		void OnUpdate()override;
	};

	//�J�����̃��b�N�I���͈�
	class LockOnRange : public ObjectMove
	{
	private:
		Vec3 m_pos;//�ʒu
		Vec3 m_rot;//��]
		Vec3 m_scale;//�傫��
		float m_range;//���b�N�I���˒�

		shared_ptr<PNTStaticDraw> m_ptrDraw;
		shared_ptr<Transform> m_trans;
		weak_ptr<Player> m_player;//Player
	public:
		LockOnRange(const shared_ptr<Stage>& stagePtr,float range,shared_ptr<Player> player);
		~LockOnRange();

		void OnCreate()override;
		void OnUpdate()override;
	};

	class LockOnLook : public ObjectMove
	{
	private:
		//Vec3 m_pos;//�ʒu
		Vec3 m_rot;//��]
		Vec3 m_scale;//�傫��
		Vec3 m_pushPos;//�ǉ����W
		Col4 m_color;//�F

		weak_ptr<Actor> m_parentObj;//�ǐՑΏ�
		shared_ptr<Transform> m_trans;
	public:
		LockOnLook(const shared_ptr<Stage>& stagePtr, Vec3 rot, Vec3 scale,weak_ptr<Actor> parenatObj,Vec3 pushPos = Vec3(0.0f,0.0f,0.0f));
		~LockOnLook();
		void OnCreate()override;//�쐬
		void OnUpdate()override;//�X�V
	};


	//�f�o�b�N�p�̂����̎l�p�����
	class Cube : public ObjectNotMove
	{
	private:
		Vec3 m_pos;//�ʒu
		Vec3 m_rot;//��]
		Vec3 m_scale;//�傫��
		Col4 m_color;//�F
	public:
		Cube(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, Col4 color = Col4(1.0f, 1.0f, 1.0f, 1.0f));
		~Cube();
		void OnCreate()override;//�쐬
		//void OnUpdate()override;
	};

}
//end basecross

