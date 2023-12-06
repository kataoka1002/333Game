#include "stdafx.h"
#include "PlayerReactionState.h"
#include "PlayerIdleState.h"

/// <summary>
/// �v���C���[�̖��O���
/// </summary>
namespace nsPlayer
{
	PlayerReactionState::~PlayerReactionState()
	{

	}

	void PlayerReactionState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_player->SetAnimation(Player::enAnimClip_Reaction, 0.1f);
	}

	IPlayerState* PlayerReactionState::StateChange()
	{
		//�A�j���[�V�����̍Đ����I�������
		if (m_player->GetIsPlayingAnimation() == false)
		{
			//�U�����ł͂Ȃ�����
			m_player->SetCanAttack(false);

			//�v���C���[�̏������Z�b�g
			m_player->ResetAttack();

			return new PlayerIdleState(m_player);
		}

		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void PlayerReactionState::Update()
	{
		
	}
}