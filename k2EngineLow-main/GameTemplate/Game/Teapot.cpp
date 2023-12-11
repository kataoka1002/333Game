#include "stdafx.h"
#include "Teapot.h"
#include "Game.h"

namespace
{
	//�R���W�����̑傫��
	const float COLLISION_SCALE = 30.0f;

	//�^����_���[�W
	const float DAMAGE_AMOUNT = 50.0f;

	//�L�����R���̔��a
	const float CHARACON_RADIUS = 40.0f;

	//�L�����R���̍���
	const float CHARACON_HEIGHT = 10.0f;
}

Teapot::Teapot()
{

}

Teapot::~Teapot()
{
	DeleteGO(m_collisionObject);
}

void Teapot::Update()
{
	//����
	Move();

	m_model.Update();
}

void Teapot::InitModel()
{
	m_model.Init("Assets/modelData/object/teapot.tkm");
	m_model.SetPosition(m_position);
	m_model.SetRotation(m_rotation);
	m_model.SetScale(m_scale);
	m_model.Update();

	//�L�����N�^�[�R���g���[���[��������
	m_charaCon.Init(
		20.0f,			//���a
		10.0f,			//����
		m_position		//���W
	);

	//�^����_���[�W�̐ݒ�
	m_damage = DAMAGE_AMOUNT;
}

void Teapot::InitCollision()
{
	//�R���W�����I�u�W�F�N�g���쐬����B
	m_collisionObject = NewGO<CollisionObject>(0);

	//����̃R���W�������쐬����B
	m_collisionObject->CreateSphere(
		m_position,				//���W
		Quaternion::Identity,	//��]
		COLLISION_SCALE			//���a
	);

	//�R���W�����ɖ��O��t����
	m_collisionObject->SetName("teapot");

	//�R���W�����I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���
	m_collisionObject->SetIsEnableAutoDelete(false);
}

void Teapot::OnDestroy()
{
	//���X�g����폜
	m_game->RemoveObjectFromList(this);

	//�������g�̍폜
	DeleteGO(this);
}

void Teapot::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}