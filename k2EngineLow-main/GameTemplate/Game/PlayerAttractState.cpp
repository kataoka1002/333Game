#include "stdafx.h"
#include "PlayerAttractState.h"
#include "PlayerStandbyState.h"

/// <summary>
/// �v���C���[�̖��O���
/// </summary>
namespace nsPlayer
{
	PlayerAttractState::~PlayerAttractState()
	{

	}

	void PlayerAttractState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_player->SetAnimation(Player::enAnimClip_Attract, 0.5f);

		// �I�u�W�F�N�g�ێ���Ԃɂ���
		m_player->SetIsHoldingObject(true);
	}

	IPlayerState* PlayerAttractState::StateChange()
	{
		//�A�j���[�V�����̍Đ����I�������B
		if (m_player->GetIsPlayingAnimation() == false)
		{
			//�X�^���o�C�ɂ���
			return new PlayerStandbyState(m_player);
		}

		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void PlayerAttractState::Update()
	{
		// ��]�����B
		m_player->Turn();
	}
}