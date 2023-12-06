#include "stdafx.h"
#include "PlayerStandbyState.h"
#include "PlayerAttackState.h"

/// <summary>
/// �v���C���[�̖��O���
/// </summary>
namespace nsPlayer
{
	PlayerStandbyState::~PlayerStandbyState()
	{

	}

	void PlayerStandbyState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_player->SetAnimation(Player::enAnimClip_Standby, 0.5f);
	}

	IPlayerState* PlayerStandbyState::StateChange()
	{
		if (m_player->GetCanAttack())
		{
			//�U�����ł͂Ȃ�����
			m_player->SetCanAttack(false);

			//�U���X�e�[�g�ɑJ��
			return new PlayerAttackState(m_player);
		}

		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void PlayerStandbyState::Update()
	{
		// �ړ������B
		m_player->Move();

		// ��]�����B
		m_player->Turn();

		if (g_pad[0]->IsTrigger(enButtonB))
		{
			// �U������
			m_player->Attack();
		}

		// �������̕ύX����
		m_player->ChangeWalkingStyle();
	}
}