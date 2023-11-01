#include "stdafx.h"
#include "PlayerJumpState.h"
#include "PlayerIdleState.h"

/// <summary>
/// �v���C���[�̖��O���
/// </summary>
namespace nsPlayer
{
	PlayerJumpState::~PlayerJumpState()
	{

	}

	void PlayerJumpState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_player->SetAnimation(Player::enAnimClip_Jump, 0.1f);

		//���̏�W�����v���ɂ���
		m_player->SetIsJumping(true);
	}

	IPlayerState* PlayerJumpState::StateChange()
	{
		//�A�j���[�V�����̍Đ����I�������B
		if (m_player->GetOnGround() == true)
		{
			return new PlayerIdleState(m_player);
		}

		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void PlayerJumpState::Update()
	{
		// �ړ������B
		m_player->Move();
	}
}