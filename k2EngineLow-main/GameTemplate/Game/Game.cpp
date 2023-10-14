#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	
}

Game::~Game()
{
	//�폜
	DeleteGO(m_skyCube);
	DeleteGO(m_player);
	DeleteGO(m_bg);
	DeleteGO(m_camera);
	DeleteGO(m_light);
}

bool Game::Start()
{
	//�X�J�C�L���[�u�̍쐬
	SetSkyCube();

	//�v���C���[�̍쐬
	m_player = NewGO<Player>(0, "player");

	//�w�i�X�e�[�W�̍쐬
	m_bg = NewGO<BackGround>(0, "background");

	//�Q�[���J�����̍쐬
	m_camera = NewGO<GameCamera>(0, "gamecamera");

	//���C�g�̍쐬
	m_light = NewGO<Lighting>(0, "lighting");

	return true;
}

void Game::Update()
{

}

void Game::SetSkyCube()
{
	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_skyCube->SetLuminance(1.0f);
	m_skyCube->SetScale(1000.0f);
	m_skyCube->SetPosition({ 0.0f,100.0f,0.0f });
	m_skyCube->SetType((EnSkyCubeType)enSkyCubeType_Day);
}

void Game::Render(RenderContext& rc)
{

}