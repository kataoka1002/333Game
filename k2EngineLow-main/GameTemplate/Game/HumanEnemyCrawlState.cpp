#include "stdafx.h"
#include "HumanEnemyCrawlState.h"

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
		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void HumanEnemyCrawlState::Update()
	{

	}
}
