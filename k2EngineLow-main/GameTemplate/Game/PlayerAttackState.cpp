#include "stdafx.h"
#include "PlayerAttackState.h"
#include "PlayerIdleState.h"

/// <summary>
/// �v���C���[�̖��O���
/// </summary>
namespace nsPlayer
{
	PlayerAttackState::~PlayerAttackState()
	{

	}

	void PlayerAttackState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_player->SetAnimation(Player::enAnimClip_Attack, 0.2f);
	}

	IPlayerState* PlayerAttackState::StateChange()
	{
		//�A�j���[�V�����̍Đ����I�������B
		if (m_player->GetIsPlayingAnimation() == false)
		{
			//�v���C���[�̏������Z�b�g
			m_player->ResetAttack();

			//�X�^���o�C�ɂ���
			return new PlayerIdleState(m_player);
		}

		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void PlayerAttackState::Update()
	{

	}
}