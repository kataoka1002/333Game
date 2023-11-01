#include "stdafx.h"
#include "PlayerWalkJumpState.h"
#include "PlayerWalkState.h"

/// <summary>
/// �v���C���[�̖��O���
/// </summary>
namespace nsPlayer
{
	PlayerWalkJumpState::~PlayerWalkJumpState()
	{

	}

	void PlayerWalkJumpState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_player->SetAnimation(Player::enAnimClip_WalkJump, 0.2f);
	}

	IPlayerState* PlayerWalkJumpState::StateChange()
	{
		//�A�j���[�V�����̍Đ����I�������B
		if (m_player->GetOnGround() == true)
		{
			return new PlayerWalkState(m_player);
		}

		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void PlayerWalkJumpState::Update()
	{
		// �ړ������B
		m_player->Move();

		// ��]�����B
		m_player->Turn();
	}
}