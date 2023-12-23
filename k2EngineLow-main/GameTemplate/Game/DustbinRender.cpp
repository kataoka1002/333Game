#include "stdafx.h"
#include "DustbinRender.h"

bool DustbinRender::Start()
{
	//�C���X�^���V���O�`��p���f���̏�����
	m_modelRender.InitInstancing(
		"Assets/modelData/object/dustbin.tkm",
		nullptr,
		0,
		enModelUpAxisZ,
		false,
		m_maxModel
	);
	return true;
}

void DustbinRender::Update()
{

}

void DustbinRender::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}