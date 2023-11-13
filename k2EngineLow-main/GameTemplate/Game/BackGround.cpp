#include "stdafx.h"
#include "BackGround.h"

namespace
{
	const Vector3 BG_POSITION = { 0.0f,0.0f,-1000.0f };
}

BackGround::BackGround()
{

}

BackGround::~BackGround()
{

}

bool BackGround::Start()
{
	//�w�i���f���̏�����
	m_bgModel.Init("Assets/modelData/map/map.tkm", nullptr, 0, enModelUpAxisZ, true);
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

void BackGround::Update()
{
	m_bgModel.Update();
}

void BackGround::Render(RenderContext& rc)
{
	m_bgModel.Draw(rc);
}