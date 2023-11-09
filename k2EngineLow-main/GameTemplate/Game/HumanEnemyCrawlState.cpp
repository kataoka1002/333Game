#include "stdafx.h"
#include "HumanEnemyCrawlState.h"
#include "HumanEnemyDieState.h"
#include "HumanEnemyTrembleState.h"

/// <summary>
/// �q���[�}���G�l�~�[�̖��O���
/// </summary>
namespace nsHumanEnemy
{
	HumanEnemyCrawlState::~HumanEnemyCrawlState()
	{

	}

	void HumanEnemyCrawlState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_enemy->SetAnimation(HumanEnemy::enAnimClip_Crawl, 0.4f);
	}

	IHumanEnemyState* HumanEnemyCrawlState::StateChange()
	{
		// �̗͂�0�ȉ��Ŏ��S
		if (m_enemy->GetHP() <= DEAD_LINE)
		{
			return new HumanEnemyDieState(m_enemy);
		}

		//��苗���ȏ�ɂȂ�ƒǂ������Ă��Ȃ��Ȃ�
		if (m_enemy->CheckDistanceToPlayer() >= CHASE_RANGE_KNEELING)
		{
			return new HumanEnemyTrembleState(m_enemy);
		}

		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void HumanEnemyCrawlState::Update()
	{
		//�v���C���[��ǂ�������
		m_enemy->FollowPlayer();

		//��]����
		m_enemy->Turn();

		//�R���W�����̏���
		m_enemy->MoveCollision();
	}
}
