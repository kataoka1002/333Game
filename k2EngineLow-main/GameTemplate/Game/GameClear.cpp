#include "stdafx.h"
#include "GameClear.h"
#include "ResultCamera.h"

GameClear::GameClear()
{

}

GameClear::~GameClear()
{
	DeleteGO(m_resultCamera);
}

void GameClear::InitCamera()
{
	//�J�����̐ݒ�
	m_resultCamera = NewGO<ResultCamera>(0, "resultcamera");
	m_resultCamera->SetTarget(m_playerPosition);
}

void GameClear::Update()
{
	//�^�C�g���֖߂�
	GoTitle();

	//�Đ�����A�j���[�V������ݒ�
	SetAnimation();

	// �A�j���[�V�������Đ�
	m_playerModel.PlayAnimation(m_currentAnimationClip, 0.5f);
	m_playerModel.Update();
}

void GameClear::OnDestroy()
{
	//���g�̍폜
	DeleteGO(this);
}

void GameClear::Render(RenderContext& rc)
{
	m_playerModel.Draw(rc);
}