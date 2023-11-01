#include "stdafx.h"
#include "Teapot.h"
#include "Game.h"

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