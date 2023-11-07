#include "stdafx.h"
#include "HumanEnemyIdleState.h"
#include "HumanEnemyDeadState.h"

/// <summary>
/// �q���[�}���G�l�~�[�̖��O���
/// </summary>
namespace nsHumanEnemy
{
	HumanEnemyIdleState::~HumanEnemyIdleState()
	{

	}

	void HumanEnemyIdleState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_enemy->SetAnimation(HumanEnemy::enAnimClip_Idle, 0.5f);
	}

	IHumanEnemyState* HumanEnemyIdleState::StateChange()
	{
		// �̗͂�0�ȉ��Ŏ��S
		if (m_enemy->GetHP() <= 0.0f)
		{
			return new HumanEnemyDeadState(m_enemy);
		}
			
		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void HumanEnemyIdleState::Update()
	{
		
	}
}
