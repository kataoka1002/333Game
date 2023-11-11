#include "stdafx.h"
#include "House.h"

namespace
{
	const Vector3 BG_POSITION = { 0.0f,0.0f,-1000.0f };
}

House::House()
{

}

House::~House()
{

}

bool House::Start()
{
	//�w�i���f���̏�����
	m_bgModel.Init("Assets/modelData/bg/terrain.tkm", nullptr, 0, enModelUpAxisZ, true);
	m_bgModel.SetPosition(m_position);
	m_bgModel.SetScale(m_scale);
	m_bgModel.SetRotation(m_rotation);
	m_bgModel.Update();

	// �ÓI�����I�u�W�F�N�g���쐬�B
	m_phyStaticObject.CreateFromModel(m_bgModel.GetModel(), m_bgModel.GetModel().GetWorldMatrix());

	//�Օ����Ƃ���������t�^����
	m_phyStaticObject.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Wall);

	return true;
}

void House::Update()
{
	if (g_pad[0]->IsPress(enButtonRight))
	{
		m_position.x += 10.0f;
	}
	if (g_pad[0]->IsPress(enButtonLeft))
	{
		m_position.x -= 10.0f;
	}
	if (g_pad[0]->IsPress(enButtonUp))
	{
		m_position.y += 10.0f;
	}
	if (g_pad[0]->IsPress(enButtonDown))
	{
		m_position.y -= 10.0f;
	}


	m_bgModel.SetPosition(m_position);
	m_bgModel.Update();
}

void House::Render(RenderContext& rc)
{
	m_bgModel.Draw(rc);
}