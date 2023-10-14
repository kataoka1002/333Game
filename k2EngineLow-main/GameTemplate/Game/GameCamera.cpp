#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"

namespace
{
	//�����_���王�_�܂ł̃x�N�g��
	const Vector3 TO_CAMERA_POSITION = { 0.0f, 200.0f, 400.0f };

	//�J�����̍ő�ړ����x
	const float MAX_CAMERA_SPEED = 1000.0f;

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

	//�����_���王�_�܂ł̃x�N�g����ݒ�
	m_toCameraPos.Set(TO_CAMERA_POSITION);

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
	//�����_���v�Z����B
	Vector3 target = m_player->GetPosition();

	//�v���C���̑������炿����Ə�𒍎��_�Ƃ���B
	target.y += 80.0f;
	target += g_camera3D->GetForward() * 20.0f;

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

void GameCamera::Update()
{
	//�J�����̎��_�ƒ����_�̐ݒ�
	UpdatePositionAndTarget();

	//�J�����̍X�V�B
	m_springCamera.Update();
}