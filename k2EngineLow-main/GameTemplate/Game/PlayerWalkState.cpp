#include "stdafx.h"
#include "PlayerWalkState.h"
#include "PlayerAttractState.h"
#include "PlayerIdleState.h"
#include "PlayerWalkJumpState.h"
#include "PlayerPunchState.h"

/// <summary>
/// �v���C���[�̖��O���
/// </summary>
namespace nsPlayer
{
	PlayerWalkState::~PlayerWalkState()
	{

	}

	void PlayerWalkState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_player->SetAnimation(Player::enAnimClip_Walk, 0.2f);
	}

	IPlayerState* PlayerWalkState::StateChange()
	{
		if (m_player->GetCanAttract())
		{
			// �����񂹕s�̏�Ԃɖ߂�
			m_player->SetCanAttract(false);

			// �����񂹃X�e�[�g�ɑJ�ڂ���B
			return new PlayerAttractState(m_player);
		}

		if (g_pad[0]->GetLStickXF() == 0.000f || g_pad[0]->GetLStickYF() == 0.000f)
		{
			// �A�C�h���X�e�[�g�ɑJ�ڂ���
			return new PlayerIdleState(m_player);
		}

		if (g_pad[0]->IsTrigger(enButtonX))
		{
			//�W�����v����
			m_player->SetJump();

			// �����W�����v�X�e�[�g�ɑJ�ڂ���
			return new PlayerWalkJumpState(m_player);
		}

		if (g_pad[0]->IsTrigger(enButtonY))
		{
			// �p���`�X�e�[�g�ɑJ�ڂ���
			return new PlayerPunchState(m_player);
		}

		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void PlayerWalkState::Update()
	{
		// �ړ������B
		m_player->Move();

		// ��]�����B
		m_player->Turn();
	}
}