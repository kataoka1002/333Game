#include "stdafx.h"
#include "BenchiSmallRender.h"

bool BenchiSmallRender::Start()
{
	//�C���X�^���V���O�`��p���f���̏�����
	m_modelRender.InitInstancing(
		"Assets/modelData/object/bench_small.tkm",
		nullptr,
		0,
		enModelUpAxisZ,
		false,
		m_maxModel
	);
	return true;
}

void BenchiSmallRender::Update()
{

}

void BenchiSmallRender::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}