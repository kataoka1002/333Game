#include "stdafx.h"
#include "ObjectBase.h"
#include "Player.h"
#include <math.h>

namespace
{
	//��
	const float PI = 3.141592;

	//�����񂹂��鑬��
	const float OBJECT_SPPED = 20.0f;
	const float OBJECT_VELOCITY = 10.0f;

	//�J�[�u�̊p�x
	const float CURVATURE_RADIUS = 10.0f;
	const float DAMPING = 0.1f;

	//�㉺�Ƀt���t�����鑬��
	const float UP_DOWN_SPEED = 1.5f;

	//�I�u�W�F�N�g�̃��[�J���|�W�V����
	const Vector3 OBJECT_LOCAL_POSITION = { 100.0f,150.0f,0.0f };

	//������ԑ���
	const float BLOW_AWAY_SPEED = 3000.0f;
}

bool ObjectBase::Start()
{
	//�v���C���[��T��
	m_player = FindGO<Player>("player");

	//���f���̏�����
	InitModel();

	return true;
}

void ObjectBase::Move()
{
	switch (m_objectState)
	{
	//�Î~��
	case enObjectState_Quiescence:
		break;

	//�ҋ@��
	case enObjectState_Idle:
		//�t���t��
		IdleMove();

		//��]
		Turn(g_camera3D->GetForward());

		break;

	//�����񂹒�
	case enObjectState_Attract:
		//�^�[�Q�b�g�̐ݒ�
		CalcTargetPosition();

		//�����񂹂�
		AttractedToPlayer();

		//��]
		Turn(g_camera3D->GetForward());

		break;

	//������ђ�
	case enObjectState_Blow:
		//������ԏ���
		BlowAway();

		break;

	default:
		break;
	}

	//�X�V
	m_charaCon.SetPosition(m_position);
	m_model.SetPosition(m_position);
}

void ObjectBase::IdleMove()
{
	//�v���C���[��Ǐ]����
	FollowPlayer();

	//�㉺�ɓ���
	UpDown();
}

void ObjectBase::FollowPlayer()
{
	//�v���C���[�̉�]��n��
	m_rotation = m_player->GetRotation();

	//�v���C���[�̉�]�ɍ��킹�����[�J���|�W�V�������v�Z
	Vector3 localPos = OBJECT_LOCAL_POSITION;
	m_rotation.Multiply(localPos);

	//�v���C���[�̃|�W�V�����Ƀ��[�J���|�W�V�����𑫂����ꏊ���^�[�Q�b�g�ɂ���
	Vector3 target = m_player->GetPosition() + localPos;

	//���炩�Ɉړ�������
	m_position = Math::Lerp(g_gameTime->GetFrameDeltaTime() * 5.0f, m_position, target);
}

void ObjectBase::UpDown()
{
	//sin�g���g���Ċ��炩�ɏ㉺������
	//�p�x��傫�����Ă���
	degree += 1.0f;

	//360�x��-1�`1���������̂�0�x�ɖ߂�
	if (degree >= 360.0f)
	{
		degree = 0.0f;
	}

	//�p�x�����W�A���ɕϊ�
	float rad = degree * PI / 180.0f;

	//sin�̒l�����߂�
	float sinValue = sin(rad);

	//���߂��l�𑬂��ɂ����čŏI�I�ȑ��������߂�
	float UpDownSpeed = UP_DOWN_SPEED * sinValue;
	m_position.y += UpDownSpeed;
}

void ObjectBase::AttractedToPlayer()
{
	//�^�[�Q�b�g�܂ł̕����A�������v�Z
	Vector3 toTarget = m_targetPosition - m_position;
	Vector3 toTargetDir = toTarget;
	toTargetDir.Normalize();

	//�^�[�Q�b�g�ɏ����߂Â�����
	if (toTarget.Length() <= 200.0f)
	{
		//�ړ��ʂ��v�Z����
		Vector3 speed = toTargetDir * 10.0f;
		m_position += speed;
	}
	else
	{
		//�ړ��ʂ��v�Z����
		CalcVelocity(OBJECT_SPPED, CURVATURE_RADIUS, DAMPING);
		m_position += m_velocity;
	}

	//�^�[�Q�b�g�ɋ߂Â�����
	if (toTarget.Length() <= 5.0f)
	{
		//�ҋ@���̃X�e�[�g�ɕύX
		m_objectState = enObjectState_Idle;
	}
}

void ObjectBase::InitAttract()
{
	//�^�[�Q�b�g�̐ݒ�
	CalcTargetPosition();

	//�^�[�Q�b�g�܂ł̋���,�������v�Z
	Vector3 toTarget = m_targetPosition - m_position;
	Vector3 toTargetDir = toTarget;
	toTargetDir.Normalize();

	//��яo�������������_���Ō��߂�
	Quaternion rotation;
	rotation.SetRotationDeg(Vector3{0.0f,0.0f,1.0f}, rand() % 180);

	//�E�����̐ݒ�
	Vector3 m_right = Vector3::AxisX;

	//�N�H�[�^�j�I���ɂ���]���K�p���ꂽ�V�����x�N�g����������
	rotation.Apply(m_right);

	//����
	SetVelocity(m_right * OBJECT_VELOCITY);

	//�����񂹃X�e�[�g�ɕύX
	m_objectState = enObjectState_Attract;
}

void ObjectBase::CalcVelocity(const float speed, const float curvatureRadius, const float damping)
{
	//���S�͂����߂�
	float maxCentripetalAccel = speed * speed / curvatureRadius;

	//�X�s�[�h�Ɍ������������ċt�����ւ̗͂����߂�
	float propulsion = speed * damping;

	//�ڕW�̒n�_
	Vector3 targetPosition = m_targetPosition;

	//�ڕW�܂ł̃x�N�g��
	Vector3 toTarget = targetPosition - m_position;

	//���x��ݒ肵�A���K�����đł��o���ꂽ�������擾
	Vector3 vn = m_velocity;
	vn.Normalize();

	//�ڕW�܂ł̃x�N�g���Ƒł��o���ꂽ�����̓��ς����߂�(�G�֌������x�N�g����vn�Ɏˉe�� T'�̑傫�������܂�)
	float dot = toTarget.Dot(vn);

	//T'����ڕW�܂ł̃x�N�g��(�Ȃ��邽�߂ɉ���������x)�����߂�
	Vector3 centripetalAccel = toTarget - (vn * dot);

	//���̒��������߂�
	float centripetalAccelMagnitude = centripetalAccel.Length();

	//1�ȏ�Ȃ璷����1�ɐ��K����,1�����Ȃ炻�̂܂�
	if (centripetalAccelMagnitude > 1.0f)
	{
		//�x�N�g���̐��K�����s��
		centripetalAccel /= centripetalAccelMagnitude;
	}

	//������maxCentripetalAccel�ȉ��Ȃ炻�̂܂܁A�ȏ�Ȃ璷����maxCentripetalAccel�ɂ���
	Vector3 force = centripetalAccel * maxCentripetalAccel;

	//���i��
	force += vn * propulsion;

	//��C��R
	force -= m_velocity * damping;

	//���x�ϕ�
	m_velocity += force * g_gameTime->GetFrameDeltaTime();
}

void ObjectBase::CalcTargetPosition()
{
	//�v���C���[�̉�]��n��
	Quaternion rot = m_player->GetRotation();

	//�v���C���[�̉�]�ɍ��킹�����[�J���|�W�V�������v�Z
	Vector3 localPos = OBJECT_LOCAL_POSITION;
	rot.Multiply(localPos);

	//�v���C���[�̃|�W�V������n��	
	m_targetPosition = m_player->GetPosition();

	//�v���C���[�̃|�W�V�����Ɍv�Z�������[�J���|�W�V�����𑫂�
	m_targetPosition += localPos;
}

void ObjectBase::Turn(Vector3 speed)
{
	//���炩�ɉ�]������
	m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * 2.0f, m_rotMove, speed);
	m_rotation.SetRotationYFromDirectionXZ(m_rotMove);

	m_model.SetRotation(m_rotation);
}

void ObjectBase::InitBlowAway()
{
	//���ł��������̌���(�J�����������Ă�������ɔ��ł���)
	m_flightSpeed = g_camera3D->GetForward() * BLOW_AWAY_SPEED;

	//������уX�e�[�g�ɕύX
	m_objectState = enObjectState_Blow;
}

void ObjectBase::BlowAway()
{	
	//������ђ��̃L�����R���̍X�V
	m_position = m_charaCon.Execute(m_flightSpeed, g_gameTime->GetFrameDeltaTime());
}