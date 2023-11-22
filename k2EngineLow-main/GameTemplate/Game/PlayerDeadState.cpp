#include "stdafx.h"
#include "PlayerDeadState.h"

/// <summary>
/// �v���C���[�̖��O���
/// </summary>
namespace nsPlayer
{
	PlayerDeadState::~PlayerDeadState()
	{

	}

	void PlayerDeadState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_player->SetAnimation(Player::enAnimClip_Dead, 0.5f);
	}

	IPlayerState* PlayerDeadState::StateChange()
	{
		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void PlayerDeadState::Update()
	{
		//�A�j���[�V�����̍Đ����I�������
		if (m_player->GetIsPlayingAnimation() == false)
		{
			//�Q�[���I�[�o�[��ʂ�
			m_player->GameOver();
		}
	}
}