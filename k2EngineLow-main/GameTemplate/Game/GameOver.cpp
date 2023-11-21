#include "stdafx.h"
#include "GameOver.h"
#include "ResultCamera.h"

GameOver::GameOver()
{

}

GameOver::~GameOver()
{
	DeleteGO(m_resultCamera);
}

void GameOver::InitCamera()
{
	//�J�����̐ݒ�
	m_resultCamera = NewGO<ResultCamera>(0, "resultcamera");
	m_resultCamera->SetTarget(m_playerPosition);

	//�^�C�g���֖߂�
	GoTitle();
}

void GameOver::Update()
{
	// �A�j���[�V�������Đ�
	m_playerModel.PlayAnimation(m_currentAnimationClip, 0.3f);
	m_playerModel.Update();
}

void GameOver::OnDestroy()
{
	//���g�̍폜
	DeleteGO(this);
}

void GameOver::Render(RenderContext& rc)
{
	m_playerModel.Draw(rc);
}