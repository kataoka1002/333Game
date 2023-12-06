#include "stdafx.h"
#include "ObjectBase.h"
#include "Player.h"
#include "Game.h"
#include <math.h>
#include "collision/CollisionObject.h"
#include "Boss.h"

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

	//�������x
	const float FALL_SPEED = 100.0f;

	//�����񂹂�鋗��
	const float ATTRACT_LIMIT = 800.0f;

	//�΂߂̊p�x�̍ő�l
	const float MAX_DIAGONAL_ROTATION = 20.0f;

	//�΂߂ɂȂ鑬��
	const float DIAGONAL_ROTATION_SPEED = 0.5f;
}

bool ObjectBase::Start()
{
	//�v���C���[��T��
	m_player = FindGO<nsPlayer::Player>("player");

	//�Q�[����T��
	m_game = FindGO<Game>("game");

	//���f���̏�����
	InitModel();

	m_sphereCollider.Create(1.0f);

	return true;
}

void ObjectBase::Move()
{
	switch (m_objectState)
	{
		//�Î~��
	case enObjectState_Quiescence:
		//�����񂹂�邩�ǂ����𔻒�
		CalcAimingDirection();
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
		//�Փ˂������ǂ����̏���
		CalcCollision();
		break;

		//������
	case enObjectState_Falling:
		//�������̏���
		FallingObject();
		//���n����
		CheckToLand();
		break;

	default:
		break;
	}

	//�I�u�W�F�N�g���Î~��Ԃ���Ȃ��Ȃ�����
	if (m_objectState != enObjectState_Quiescence && m_targetUI != nullptr)
	{
		//UI�̍폜
		DeleteGO(m_targetUI);
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
	m_degree += 1.0f;

	//360�x��-1�`1���������̂�0�x�ɖ߂�
	if (m_degree >= 360.0f)
	{
		m_degree = 0.0f;
	}

	//�p�x�����W�A���ɕϊ�
	float rad = m_degree * PI / 180.0f;

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

	//�΂߂ɂ���
	m_diagonalRot += DIAGONAL_ROTATION_SPEED;
	if (m_diagonalRot >= MAX_DIAGONAL_ROTATION)
	{
		m_diagonalRot = MAX_DIAGONAL_ROTATION;
	}
	m_rotation.SetRotationDegZ(m_diagonalRot);

	m_model.SetRotation(m_rotation);
}

void ObjectBase::InitBlowAway()
{
	//�G�t�F�N�g����
	EffectEmitter* efe = NewGO<EffectEmitter>(0);
	efe->Init(enEffectName_ObjectPush);
	efe->SetScale(Vector3::One);
	efe->SetRotation(m_player->GetRotation());
	efe->SetPosition(m_position);
	efe->Play();

	//�R���W�����̏�����
	InitCollision();

	//���ł��������̌���(���e�B�N���̕���)
	m_flightSpeed = g_camera3D->GetForward() * BLOW_AWAY_SPEED;
	m_flightSpeed += g_camera3D->GetRight() * -90.0f;

	//������уX�e�[�g�ɕύX
	m_objectState = enObjectState_Blow;

	m_canAttract = false;
}

void ObjectBase::BlowAway()
{	
	//������ђ��̃L�����R���̍X�V
	m_position = m_charaCon.Execute(m_flightSpeed, g_gameTime->GetFrameDeltaTime());
	
	//�R���W�������L�����R���Ɠ����ʒu�ɏo��悤�ɃZ�b�g
	m_collisionPosition = { m_position.x,m_position.y + m_collisionAddPos,m_position.z };

	//�R���W�����̃|�W�V�����̃Z�b�g
	m_collisionObject->SetPosition(m_collisionPosition);

	m_smokeEfeInterval++;
	if (m_smokeEfeInterval >= 2)
	{
		//�G�t�F�N�g����
		EffectEmitter* efe = NewGO<EffectEmitter>(0);
		efe->Init(enEffectName_ObjectSmoke);
		efe->SetScale(Vector3::One);
		//efe->SetRotation();
		efe->SetPosition(m_position);
		efe->Play();
		m_smokeEfeInterval = 0;
	}
}

void ObjectBase::FallingObject()
{
	//�������x�̐ݒ�
	Vector3 fallSpeed = { 0.0f,0.0f,0.0f };

	//�����n�߂Ă���̎��Ԃ��v������
	m_fallingTime += g_gameTime->GetFrameDeltaTime();

	//���Ԃɂ�鎩�R�����̑������v�Z
	fallSpeed.y = -9.8f * m_fallingTime * FALL_SPEED;
	
	//�������̃L�����R���̍X�V
	m_position = m_charaCon.Execute(fallSpeed, g_gameTime->GetFrameDeltaTime());
}

void ObjectBase::CheckToLand()
{
	//�n�ʂɒ�������
	if (m_charaCon.IsOnGround())
	{
		//������������Ƃ��̏���
		OnDestroy();
	}
}

void ObjectBase::CalcCollision()
{	
	//�G�l�~�[�ƂԂ��������𔻒�
	for (auto enemy : m_game->GetEnemyList())
	{
		if (m_collisionObject->IsHit(enemy->GetCharaCon()))
		{
			//�G�l�~�[�̓_���[�W���󂯂����̏������s��
			enemy->HandleDamageEvent(m_damage);

			enemy->PlayReaction();

			//������������Ƃ��̏���
			OnDestroy();

			//UI�̍폜
			if (m_targetUI != nullptr)
			{
				DeleteGO(m_targetUI);
			}

			return;
		}

		if (enemy->GetCollision() != nullptr)
		{
			if (m_collisionObject->IsHit(enemy->GetCollision()))
			{
				//�G�l�~�[�̓_���[�W���󂯂����̏������s��
				enemy->HandleDamageEvent(m_damage);

				//������������Ƃ��̏���
				OnDestroy();

				//UI�̍폜
				if (m_targetUI != nullptr)
				{
					DeleteGO(m_targetUI);
				}

				return;
			}
		}	
	}

	//�{�X�ƂԂ��������𔻒�
	nsBoss::Boss* boss = FindGO<nsBoss::Boss>("boss");
	if (m_collisionObject->IsHit(boss->GetCharaCon()))
	{
		//�{�X�̓_���[�W���󂯂����̏������s��
		boss->HandleDamageEvent(m_damage);

		//�{�X�̃X�e�[�g�̕ύX���s��
		boss->SetReactionState();

		//������������Ƃ��̏���
		OnDestroy();

		//UI�̍폜
		if (m_targetUI != nullptr)
		{
			DeleteGO(m_targetUI);
		}

		return;
	}
}

//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�Ǘp)
struct SweepResultWall : public btCollisionWorld::ConvexResultCallback
{
	//�Փ˃t���O�B
	bool isHit = false;			

	virtual	btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//�ǂƂԂ����ĂȂ�������B
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall) {
			//�Փ˂����͕̂ǂł͂Ȃ��B
			return 0.0f;
		}

		//�ǂƂԂ�������t���O��true�ɁB
		isHit = true;
		return 0.0f;
	}
};

void ObjectBase::CalcAimingDirection()
{
	//�v���C���[�Ƃ̋��������������瑁�����^�[��
	Vector3 distance = m_player->GetPosition() - m_position;
	if (distance.Length() > ATTRACT_LIMIT)
	{
		//UI���쐬����Ă���Ȃ�
		if (m_makeTargetUI == true && m_targetUI != nullptr)
		{
			//�폜
			DeleteGO(m_targetUI);

			//UI���쐬��ԂɕύX
			m_makeTargetUI = false;
		}

		return;
	}

	//�f�t�H���g�͈����񂹂�Ȃ�
	m_canAttract = false;

	//�J�����܂ł̕��������߂�
	Vector3 toCameraDir = g_camera3D->GetPosition() - m_position;
	toCameraDir.Normalize();

	//�x�N�g�����t�����ɂ���
	toCameraDir *= -1.0f;

	//���ς����߂�
	float innerProduct = toCameraDir.Dot(g_camera3D->GetForward());

	//���ς��P��菬�����Ȃ�(2�̃x�N�g�����ʂ̕����������Ă���)�Ȃ�
	if (innerProduct < 0.9995f)
	{
		//UI���쐬����Ă���Ȃ�
		if (m_makeTargetUI == true && m_targetUI != nullptr)
		{
			//�폜
			DeleteGO(m_targetUI);

			//UI���쐬��ԂɕύX
			m_makeTargetUI = false;
		}

		return;
	}

	////�R���C�_�[�̎n�_�ƏI�_
	//btTransform start, end;
	//start.setIdentity();
	//end.setIdentity();

	////�n�_�̓I�u�W�F�N�g�̍��W
	//start.setOrigin(btVector3(m_position.x, m_position.y, m_position.z));
	////�I�_�̓J�����̍��W
	//end.setOrigin(btVector3(g_camera3D->GetPosition().x, g_camera3D->GetPosition().y, g_camera3D->GetPosition().z));

	////�R���C�_�[���n�_����I�_�܂œ�������,�Փ˂��邩�ǂ����𒲂ׂ�
	//SweepResultWall callback;
	//PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);

	////�ǂƏՓ˂�����
	//if (callback.isHit == true)
	//{
	//	return;
	//}

	//�����񂹉\�ɂ���
	m_canAttract = true;

	//�����܂ŗ���UI���܂��Ȃ��Ȃ�
	if (m_makeTargetUI == false)
	{
		//UI�̍쐬
		m_targetUI = NewGO<TargetUI>(0, "targetui");
		m_targetUI->SetObjectPosition(m_position);

		//UI�쐬�ς݂ɕύX
		m_makeTargetUI = true;
	}
}