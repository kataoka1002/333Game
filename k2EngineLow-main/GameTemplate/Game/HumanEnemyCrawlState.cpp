#include "stdafx.h"
#include "HumanEnemyCrawlState.h"
#include "HumanEnemyDieState.h"

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
		m_enemy->SetAnimation(HumanEnemy::enAnimClip_Crawl, 0.5f);
	}

	IHumanEnemyState* HumanEnemyCrawlState::StateChange()
	{
		// �̗͂�0�ȉ��Ŏ��S
		if (m_enemy->GetHP() <= 0.0f)
		{
			return new HumanEnemyDieState(m_enemy);
		}

		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void HumanEnemyCrawlState::Update()
	{
		m_enemy->FollowPlayer();
		m_enemy->Turn();
		m_enemy->MoveCollision();
	}
}
