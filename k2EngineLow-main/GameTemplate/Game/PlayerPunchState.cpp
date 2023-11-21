#include "stdafx.h"
#include "PlayerPunchState.h"
#include "PlayerIdleState.h"

/// <summary>
/// �v���C���[�̖��O���
/// </summary>
namespace nsPlayer
{
	PlayerPunchState::~PlayerPunchState()
	{

	}

	void PlayerPunchState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_player->SetAnimation(Player::enAnimClip_Punch, 0.2f);
	}

	IPlayerState* PlayerPunchState::StateChange()
	{
		//�A�j���[�V�����̍Đ����I�������B
		if (m_player->GetIsPlayingAnimation() == false)
		{
			//�A�C�h���X�e�[�g�ɑJ��
			return new PlayerIdleState(m_player);
		}

		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void PlayerPunchState::Update()
	{

	}
}