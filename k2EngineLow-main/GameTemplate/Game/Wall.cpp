#include "stdafx.h"
#include "Wall.h"

namespace
{
	const Vector3 WALL_SIZE = { 900.0f,200.0f,10.0f };
}

Wall::~Wall()
{

}

bool Wall::Start()
{
	//�R���W�����I�u�W�F�N�g���쐬����B
	m_collisionObject = NewGO<CollisionObject>(0);

	//�{�b�N�X�̃R���W�������쐬����B
	m_collisionObject->CreateBox(
		m_position,
		m_rotation,
		WALL_SIZE
	);

	//�R���W�����ɖ��O��t����
	m_collisionObject->SetName("BossFloorWall");

	//�R���W�����I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���
	m_collisionObject->SetIsEnableAutoDelete(false);

	return true;
}

void Wall::Update()
{
	m_collisionObject->SetRotation(m_rotation);
	m_collisionObject->SetPosition(m_position);
}