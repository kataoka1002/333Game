#include "stdafx.h"
#include "BarrierFence.h"
#include "Game.h"

namespace
{
	//�R���W�����̑傫��
	const float COLLISION_SCALE = 30.0f;

	//�^����_���[�W
	const float DAMAGE_AMOUNT = 50.0f;
}

BarrierFence::BarrierFence()
{

}

BarrierFence::~BarrierFence()
{
	DeleteGO(m_collisionObject);
}

void BarrierFence::Update()
{
	//����
	Move();


	m_model.Update();
}

void BarrierFence::InitModel()
{
	m_model.Init("Assets/modelData/object/barrierFence.tkm");
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

void BarrierFence::InitCollision()
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
	m_collisionObject->SetName("barrierFence");

	//�R���W�����I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���
	m_collisionObject->SetIsEnableAutoDelete(false);
}

void BarrierFence::OnDestroy()
{
	//���X�g����폜
	m_game->RemoveObjectFromList(this);

	//�������g�̍폜
	DeleteGO(this);
}

void BarrierFence::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}