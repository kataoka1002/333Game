#include "stdafx.h"
#include "FenceRender.h"

bool FenceRender::Start()
{
	//�C���X�^���V���O�`��p���f���̏�����
	m_modelRender.InitInstancing(
		"Assets/modelData/object/fence.tkm",
		nullptr,
		0,
		enModelUpAxisZ,
		false,
		m_maxModel
	);
	return true;
}

void FenceRender::Update()
{

}

void FenceRender::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}