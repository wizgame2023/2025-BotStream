/*!
@file MyGameObject.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	class MyGameObject : public GameObject
	{
	private:
		bool m_pose;//�|�[�Y�t���O
	public:
		MyGameObject(const shared_ptr<Stage>& stagePtr);
		~MyGameObject();

		Vec3 GetPosition() {
			return GetComponent<Transform>()->GetPosition();
		}
		void SetPosition(Vec3 pos) {
			GetComponent<Transform>()->SetPosition(pos);
		}

		Quat GetQuaternion() {
			return GetComponent<Transform>()->GetQuaternion();
		}
		void SetQuaternion(Quat quat) {
			GetComponent<Transform>()->SetQuaternion(quat);
		}

		Vec3 GetScale() {
			return GetComponent<Transform>()->GetScale();
		}
		void SetScale(Vec3 scale) {
			GetComponent<Transform>()->SetScale(scale);
		}

		//�N�H�[�^�j�I����ł̉�]���� 1q:�ŏ��̉�] v:��]�� rad:��]��
		Quat RotateQuat(const Quat q, const Vec3 v, const float rad) {
			Quat r = Quat(cos(rad / 2), v.x * sin(rad / 2), v.y * sin(rad / 2), v.z * sin(rad / 2));
			Quat r2 = Quat(cos(rad / 2), -v.x * sin(rad / 2), -v.y * sin(rad / 2), -v.z * sin(rad / 2));
			return q * r * r2;
		}

	protected:

		//void OnCreate()override;
		//void OnUpdate()override;
	};

}
//end basecross