#include "stdafx.h"
#include "HumanEnemyWalkState.h"
#include "HumanEnemyDeadState.h"
#include "HumanEnemyIdleState.h"
#include "HumanEnemyAttackState.h"

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
		if (m_enemy->GetHP() <= KNEELING_HP)
		{
			return new HumanEnemyDeadState(m_enemy);
		}

		//��苗���ȏ�ɂȂ�ƒǂ������Ă��Ȃ��Ȃ�
		if (m_enemy->CheckDistanceToPlayer() >= CHASE_RANGE)
		{
			return new HumanEnemyIdleState(m_enemy);
		}

		//��苗���ȉ��ōU��
		if (m_enemy->CheckDistanceToPlayer() <= ATTACK_RANGE)
		{
			return new HumanEnemyAttackState(m_enemy);
		}

		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void HumanEnemyWalkState::Update()
	{
		//�v���C���[��ǂ�������
		m_enemy->FollowPlayer();

		//��]����
		m_enemy->Turn();
	}
}
