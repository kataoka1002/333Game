#include "stdafx.h"
#include "RockBigRender.h"

bool RockBigRender::Start()
{
	//�C���X�^���V���O�`��p���f���̏�����
	m_modelRender.InitInstancing(
		"Assets/modelData/object/rock_big.tkm",
		nullptr,
		0,
		enModelUpAxisZ,
		false,
		m_maxModel
	);
	return true;
}

void RockBigRender::Update()
{

}

void RockBigRender::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}