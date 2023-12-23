#include "stdafx.h"
#include "ConeRender.h"

bool ConeRender::Start()
{
	//�C���X�^���V���O�`��p���f���̏�����
	m_modelRender.InitInstancing(
		"Assets/modelData/object/cone.tkm",
		nullptr,
		0,
		enModelUpAxisZ,
		false,
		m_maxModel
	);
	return true;
}

void ConeRender::Update()
{

}

void ConeRender::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}