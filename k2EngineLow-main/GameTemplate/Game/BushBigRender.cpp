#include "stdafx.h"
#include "BushBigRender.h"

bool BushBigRender::Start()
{
	//�C���X�^���V���O�`��p���f���̏�����
	m_modelRender.InitInstancing(
		"Assets/modelData/object/bush_big.tkm",
		nullptr,
		0,
		enModelUpAxisZ,
		false,
		m_maxModel
	);
	return true;
}

void BushBigRender::Update()
{

}

void BushBigRender::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}