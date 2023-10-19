#include "stdafx.h"
#include "Teapot.h"

Teapot::Teapot()
{

}

Teapot::~Teapot()
{

}

void Teapot::Update()
{
	//����
	Move();

	m_model.Update();
}

void Teapot::InitModel()
{
	m_position = { 0.0f,50.0f,240.0f };

	m_model.Init("Assets/modelData/object/teapot.tkm");
	m_model.SetPosition(m_position);
	m_model.SetScale(0.6f);
	m_model.Update();

	//�L�����N�^�[�R���g���[���[��������
	m_charaCon.Init(
		20.0f,			//���a
		10.0f,			//����
		m_position		//���W
	);
}

void Teapot::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}