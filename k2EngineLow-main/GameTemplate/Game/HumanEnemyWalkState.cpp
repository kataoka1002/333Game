#include "stdafx.h"
#include "HumanEnemyWalkState.h"
#include "HumanEnemyDeadState.h"

/// <summary>
/// �q���[�}���G�l�~�[�̖��O���
/// </summary>
namespace nsHumanEnemy
{
	HumanEnemyWalkState::~HumanEnemyWalkState()
	{

	}

	void HumanEnemyWalkState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_enemy->SetAnimation(HumanEnemy::enAnimClip_Walk, 0.5f);
	}

	IHumanEnemyState* HumanEnemyWalkState::StateChange()
	{
		// �̗͂�50�ȉ��Ŏl�񔇂�
		if (m_enemy->GetHP() <= 50.0f)
		{
			return new HumanEnemyDeadState(m_enemy);
		}

		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void HumanEnemyWalkState::Update()
	{
		m_enemy->FollowPlayer();
		m_enemy->Turn();
	}
}
