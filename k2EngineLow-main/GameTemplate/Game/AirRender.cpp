#include "stdafx.h"
#include "AirRender.h"

bool AirRender::Start()
{
	//�C���X�^���V���O�`��p���f���̏�����
	m_modelRender.InitInstancing(
		"Assets/modelData/object/air.tkm",
		nullptr,
		0,
		enModelUpAxisZ,
		false,
		m_maxAir
	);
	return true;
}

void AirRender::Update()
{

}

void AirRender::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}