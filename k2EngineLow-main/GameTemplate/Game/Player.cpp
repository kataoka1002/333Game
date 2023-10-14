#include "stdafx.h"
#include "Player.h"

namespace
{
	
}

Player::Player()
{
	//�A�j���[�V�����̏�����
	animationClips[enAnimClip_Idle].Load("Assets/animData/player/player_idle.tka");
	animationClips[enAnimClip_Idle].SetLoopFlag(true);
}

Player::~Player()
{

}

bool Player::Start()
{
	//�v���C���[���f���̏�����
	m_playerModel.Init("Assets/modelData/player/player.tkm", animationClips, enAnimClip_Num, enModelUpAxisZ);
	m_playerModel.SetScale(3.0f);
	m_playerModel.SetPosition(Vector3{ 0.0f,50.0f,100.0f });
	m_playerModel.Update();

	return true;
}

void Player::Update()
{
	m_position = Vector3{ 0.0f,50.0f,100.0f };
	m_playerModel.SetPosition(m_position);
	m_playerModel.Update();
}

void Player::Render(RenderContext& rc)
{
	m_playerModel.Draw(rc);
}