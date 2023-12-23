#include "stdafx.h"
#include "BenchiBigRender.h"

bool BenchiBigRender::Start()
{
	//�C���X�^���V���O�`��p���f���̏�����
	m_modelRender.InitInstancing(
		"Assets/modelData/object/bench_big.tkm",
		nullptr,
		0,
		enModelUpAxisZ,
		false,
		m_maxModel
	);
	return true;
}

void BenchiBigRender::Update()
{

}

void BenchiBigRender::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}