#include "stdafx.h"
#include "SolarPanelRender.h"

bool SolarPanelRender::Start()
{
	//�C���X�^���V���O�`��p���f���̏�����
	m_modelRender.InitInstancing(
		"Assets/modelData/object/solarPanel.tkm",
		nullptr,
		0,
		enModelUpAxisZ,
		false,
		m_maxModel
	);
	return true;
}

void SolarPanelRender::Update()
{

}

void SolarPanelRender::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}