#include "stdafx.h"
#include "BarrierFenceRender.h"

bool BarrierFenceRender::Start()
{
	//�C���X�^���V���O�`��p���f���̏�����
	m_modelRender.InitInstancing(
		"Assets/modelData/object/barrierFence.tkm",
		nullptr,
		0,
		enModelUpAxisZ,
		false,
		m_maxModel
	);
	return true;
}

void BarrierFenceRender::Update()
{

}

void BarrierFenceRender::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}