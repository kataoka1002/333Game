#include "stdafx.h"
#include "PlantLongRender.h"

bool PlantLongRender::Start()
{
	//�C���X�^���V���O�`��p���f���̏�����
	m_modelRender.InitInstancing(
		"Assets/modelData/object/plant_long.tkm",
		nullptr,
		0,
		enModelUpAxisZ,
		false,
		m_maxModel
	);
	return true;
}

void PlantLongRender::Update()
{

}

void PlantLongRender::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}