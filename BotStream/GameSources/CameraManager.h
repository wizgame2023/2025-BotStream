/*!
@file CameraManager.h
@brief �J�����̊Ǘ�
*/

#pragma once
#include "stdafx.h"
#include "Project.h"
#include "Player.h"

namespace basecross{
	//�^�[�Q�b�g���̃f�[�^
	class targetsDeta
	{
	public:
		float lockOnAngle;//�v���C���[���猩�ă^�[�Q�b�g�̂������
		int leftOrRight;//�E�����ǂ���ɂ���̂��@false����true���E

		targetsDeta(float lockOnAngle, int leftOrRight) :
			lockOnAngle(lockOnAngle),
			leftOrRight(leftOrRight)
		{
		}
		~targetsDeta()
		{
		}
	};


	class EnemyBase;
	class CameraRayCast;
	class CameraManager : public MyGameObject
	{
	private:
		weak_ptr<Camera> m_stageCamera;//�X�e�[�W��̃J����
		shared_ptr<Camera> m_lockStageCamera;//���b�N�ł����X�e�[�W��̃J����
		shared_ptr<PNTStaticDraw> m_ptrDraw;
		shared_ptr<Stage> m_stage;
		shared_ptr<Sprite> m_spriteAttack = nullptr;
		float m_delta;

		CONTROLER_STATE m_controler;//�R���g���[���[
		Vec2 m_contrloerVec;//�R���g���[���[�̉E�X�e�B�b�N����

		float m_cameraAngleY;  //Player���猩�ăJ�����̂���p�xY��
		float m_cameraAngleX;  //Player���猩�ăJ�����̂���p�xX��
		float m_range;//Player����ǂ̂��炢����邩
		float m_targetRange;//���b�N�I���͈̔�
		float m_speedXAxis;//x���̉�]�X�s�[�h
		float m_speedYAxis;//y���̉�]�X�s�[�h
		float m_addAngleXAxis;//X���̒ǉ���]�x
		float m_addAngleYAxis;//Y���̒ǉ���]�x


		shared_ptr<CameraRayCast> m_cameraRayCast;
		Vec3 m_playerPos;//�v���C���[�|�W�V����
		Vec3 m_cameraPos;//�J�����|�W�V����

		bool m_movePlayerAngleFlag;//�v���C���[�̌����Ă�������ɉ�]���邩�̃t���O
		float m_targetAngleY;//�^�[�Q�b�g�����邽�߂Ɍ����p�x(Y��)
		float m_targetDis;//LockOnTarget�̋����f�o�b�N�p

		//SE�p
		shared_ptr<SoundItem> m_SE = nullptr;//�Đ����Ă���SE
		shared_ptr<XAudio2Manager> m_SEManager = nullptr;//SE�Ȃǂ��Đ����邽�߂̃}�l�[�W��

		//���b�N�I���̏���////////////////////////////////////////////////////////////
		vector<shared_ptr<EnemyBase>> m_targets;//�^�[�Q�b�g���
		vector<Vec3> m_targetsPos;//LockOnCan��Pos��ۑ�����z��
		shared_ptr<EnemyBase> m_targetObj;//�^�[�Q�b�g�Ώ�
		bool m_lockOnFlag;//���b�N�I���ł��邩�ł��Ȃ����̕ϐ�
		bool m_lockOnUse;//���b�N�I�����邩���Ȃ����̕ϐ�
		bool m_lockOnChangeFlag;//���b�N�I����ς������̃t���O
		int m_lockOnNum;//LockOnTarget�����߂邽�߂̕ϐ�

		bool m_stickFlag;//�X�e�B�b�N���X������͂��󂯎�邩�̃t���O

		vector<targetsDeta> m_targesDeta;//LockOnTarget�̃f�[�^���������z��
		vector<float> m_lockOnAngle;//LockOnCan��Player�ɂƂ��Ăǂ̕����ɂ���̂��̕ϐ�
		/////////////////////////////////////////////////////////////////////////////

		float m_meleeRange;//�ߐڐ퓬�͈̔�
		bool m_meleeFlag;//�ߐڐ퓬���Ă������̃t���O	

		bool m_poseFlag;//�|�[�Y�̃t���O
		
		//�E����������Ƃ��^�񒆂�
		enum LeftOrRight
		{
			Middle,
			Left,
			Right
		};

	public:
		CameraManager(const shared_ptr<Stage>& stagePtr,float range = 15.0f,float targetRange = 25.0f,float melleRange = 5.0f,
			float speedXAxis = 1.0f,float speedYAxis = 3.0f);
		~CameraManager();

		void OnCreate()override;//�쐬
		void OnUpdate()override;//�X�V

		void LockOn(shared_ptr<GameObject> lockOnObj, shared_ptr<Player> originObj);//���b�N�I���@�\
		void MovePlayerAngle(float playerAngle);//Player�̔w����������p�x�ɂ���
		void MoveLockAt(Vec3 targetPos);//�����_�̈ړ�����//��������Ƃ���
		bool MoveAngle(float targetAngle,int XorY);//��]�x�̈ړ�����
		void AdjustmentAngle();//�p�x�̒���
		
		void UpdateTargesDeta(Vec3 playerPos);//LockOnCan�̃f�[�^���X�V����֐�
		void ChangeLockOn(int leftOrRight,float targetAngle);//LockOnTarget��ύX���鏈��

		//LockOnCan�����߂�֐�
		void LockOnCandidate(vector<shared_ptr<EnemyBase>> enemyVec, Vec3 playerPos);
		//���b�N�I���̉���
		void LockOff(vector<shared_ptr<EnemyBase>> enemyVec);

		void CameraAngleXLimit(float maxRad= XMConvertToRadians(140.0f), float minRad = XMConvertToRadians(10.0f));//�J������X����]�̐���
		void CameraPosUpdate();//�J�����̃|�W�V�����̍X�V
		void InertialRotation();//�����t���̉�]����
		
		//�ߋ����U�������邩�̏����̃Q�b�^�[
		bool GetMeleeFlag();
		//void SetMeleeFlag(bool onOff);

		//�^�[�Q�b�g�ΏۂƂ̋�����n��
		float GetTargetDis();

		//�|�[�Y�����̃I���I�t
		void PoseSwitch(bool onOff);

		//�߉��ǂ���̍U�������邩�̏���
		void MeleeFlagUpdate();

		//���b�N�I������
		void LockOn(shared_ptr<Player> player);
		//���b�N�I������G�����߂鏈��
		void SelectTargetObj(vector<shared_ptr<EnemyBase>> enemyVec,float playerAngle);
		//���b�N�I���������������
		void ConditionsLockOff(vector<shared_ptr<EnemyBase>> enemyVec);

		//�p�x�̃Q�b�^�Z�b�^
		//�������@X����Y���ǂ���̎��̊p�x����邩
		float GetAngle(wstring XorY)
		{
			if (XorY == L"X")
			{
				return m_cameraAngleX;
			}
			if (XorY == L"Y")
			{
				return m_cameraAngleY;
			}

			return 0;
		}

		//�^�[�Q�b�g�Ώۂ�n���֐�
		shared_ptr<Actor> GetTargetObj();
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
		void OnUpdate()override;
	};

	//�f�o�b�N�p�̂����̎l�p�����
	class EnemyCube : public Actor
	{
	private:
		Col4 m_color;//�F
	public:
		EnemyCube(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, Col4 color = Col4(1.0f, 1.0f, 1.0f, 1.0f));
		~EnemyCube();
		void OnCreate()override;//�쐬
		void OnUpdate()override;//�X�V
	};


}
//end basecross

