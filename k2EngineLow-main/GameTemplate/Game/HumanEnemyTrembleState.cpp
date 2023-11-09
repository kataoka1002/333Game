#include "stdafx.h"
#include "HumanEnemyTrembleState.h"
#include "HumanEnemyDieState.h"
#include "HumanEnemyCrawlState.h"

/// <summary>
/// �q���[�}���G�l�~�[�̖��O���
/// </summary>
namespace nsHumanEnemy
{
	HumanEnemyTrembleState::~HumanEnemyTrembleState()
	{

	}

	void HumanEnemyTrembleState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_enemy->SetAnimation(HumanEnemy::enAnimClip_Tremble, 0.5f);
	}

	IHumanEnemyState* HumanEnemyTrembleState::StateChange()
	{
		// �̗͂�0�ȉ��Ŏ��S
		if (m_enemy->GetHP() <= DEAD_LINE)
		{
			return new HumanEnemyDieState(m_enemy);
		}

		//��苗���ȉ��ɂȂ�ƒǂ������Ă���
		if (m_enemy->CheckDistanceToPlayer() <= CHASE_RANGE_KNEELING)
		{
			return new HumanEnemyCrawlState(m_enemy);
		}

		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void HumanEnemyTrembleState::Update()
	{
		//�R���W�����̏���
		m_enemy->MoveCollision();
	}
}
