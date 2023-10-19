#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"

namespace
{
	//�����_���王�_�܂ł̃x�N�g��
	const Vector3 TO_CAMERA_POSITION = { 0.0f, 150.0f, -300.0f };

	//�����_���王�_�܂ł̃x�N�g��(�ߋ�����)
	const Vector3 TO_CAMERA_POSITION_NEAR = { 0.0f,100.0f,-270.0f };

	//�����_�̃��[�J���|�W�V����
	const Vector3 TARGET_LOCAL_POSITION = { 0.0f,180.0f,0.0f };

	//�����_�̃��[�J���|�W�V����(�ߋ�����)
	const Vector3 TARGET_LOCAL_POSITION_NEAR = { 60.0f,150.0f,0.0f };

	//�J�����̍ő�ړ����x
	const float MAX_CAMERA_SPEED = 800.0f;

	//�J�����̓����蔻��̑傫��
	const float CAMERA_COLLISION_SCALE = 5.0f;

	//�J�����̈ړ����x
	const float CAMERA_MOVE_POWER_X = 1.0f;
	const float CAMERA_MOVE_POWER_Y = 1.0f;

	//������
	const float FAR_PLANE = 15000.0f;
}

GameCamera::GameCamera()
{
	
}

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	//�v���C���[�̃C���X�^���X��T���B
	m_player = FindGO<Player>("player");

	//�����_���王�_�܂ł̃x�N�g�������߂�
	SetNearCamera(false);

	//�����ʂ̐ݒ�
	g_camera3D->SetFar(FAR_PLANE);

	//�΂˃J�����̏������B
	m_springCamera.Init(
		*g_camera3D,			//�΂˃J�����̏������s���J�������w�肷��B
		MAX_CAMERA_SPEED,		//�J�����̈ړ����x�̍ő�l�B
		true,					//�J�����ƒn�`�Ƃ̂����蔻�����邩�ǂ����̃t���O�Btrue���Ƃ����蔻����s���B
		CAMERA_COLLISION_SCALE	//�J�����ɐݒ肳��鋅�̃R���W�����̔��a�B��R������true�̎��ɗL���ɂȂ�B
	);

	return true;
}

void GameCamera::UpdatePositionAndTarget()
{
	//�����_�����߂�
	Vector3 target = CalcTargetPosition();

	Vector3 toCameraPosOld = m_toCameraPos;

	//�p�b�h�̓��͂��g���ăJ�������񂷁B
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	//Y������̉�]
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, x * CAMERA_MOVE_POWER_X);
	qRot.Apply(m_toCameraPos);

	//X������̉�]�B
	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, y * CAMERA_MOVE_POWER_Y);
	qRot.Apply(m_toCameraPos);

	//�J�����̉�]�̏�����`�F�b�N����B
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();

	//�J���������������
	if (toPosDir.y < -0.5f)
	{
		m_toCameraPos = toCameraPosOld;
	}
	//�J����������������
	else if (toPosDir.y > 0.8f)
	{
		m_toCameraPos = toCameraPosOld;
	}

	//���_���v�Z����B
	Vector3 pos = target + m_toCameraPos;

	//�o�l�J�����ɒ����_�Ǝ��_��ݒ肷��B
	m_springCamera.SetPosition(pos);
	m_springCamera.SetTarget(target);
}

void GameCamera::SetNearCamera(bool isNear)
{
	//�J�����̉�]��n��
	Matrix rot = m_springCamera.GetCameraRotation();

	Vector3 toCameraPos;

	//�v���C���[���X�^���o�C���Ȃ�
	if (isNear == true)
	{
		//�ߋ����J����ON
		m_isNearCamera = true;

		//�����_���王�_�܂ł̋�����ݒ�
		toCameraPos = TO_CAMERA_POSITION_NEAR;

		//�J�����̉�]�s��̏C��
		rot.m[0][1] = 1.0f;
		rot.m[1][1] = 0.0f;
		rot.m[2][1] = 0.0f;
		rot.m[3][1] = 0.0f;
	}
	else
	{
		//�ߋ����J����OFF
		m_isNearCamera = false;

		//�����_���王�_�܂ł̋�����ݒ�
		toCameraPos = TO_CAMERA_POSITION;
	}

	//�J�����̉�]�ɍ��킹�����[�J���|�W�V�������v�Z
	Vector3 localPos = toCameraPos;
	rot.Apply3x3(localPos);

	//�J�����܂ł̋����ɃZ�b�g
	m_toCameraPos.Set(localPos);
}

Vector3 GameCamera::CalcTargetPosition()
{
	//�����_���v�Z����B
	Vector3 target = m_player->GetPosition();

	//�ߋ����J�����̂Ƃ��Ȃ�
	if (m_isNearCamera)
	{
		//�v���C���[�̉�]��n��
		Quaternion rot = m_player->GetRotation();

		//�v���C���[�̉�]�ɍ��킹�����[�J���|�W�V�������v�Z
		Vector3 localPos = TARGET_LOCAL_POSITION_NEAR;
		rot.Multiply(localPos);

		//�v���C���[�̃|�W�V�����Ɍv�Z�������[�J���|�W�V�����𑫂�
		target += localPos;
	}
	else
	{
		//�v���C���[�̑������炿����Ə�𒍎��_�Ƃ���B
		target += TARGET_LOCAL_POSITION;
	}

	//�����O�𒍎��_�ɂ���
	target += g_camera3D->GetForward() * 20.0f;	

	return target;
}

void GameCamera::Update()
{
	//�J�����̎��_�ƒ����_�̐ݒ�
	UpdatePositionAndTarget();

	//�J�����̍X�V�B
	m_springCamera.Update();
}

