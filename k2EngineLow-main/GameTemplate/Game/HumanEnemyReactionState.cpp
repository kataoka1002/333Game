#include "stdafx.h"
#include "HumanEnemyReactionState.h"
#include "HumanEnemyIdleState.h"

/// <summary>
/// �q���[�}���G�l�~�[�̖��O���
/// </summary>
namespace nsHumanEnemy
{
	HumanEnemyReactionState::~HumanEnemyReactionState()
	{

	}

	void HumanEnemyReactionState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_enemy->SetAnimation(HumanEnemy::enAnimClip_Reaction, 0.3f);
	}

	IHumanEnemyState* HumanEnemyReactionState::StateChange()
	{
		// �A�j���[�V�����̍Đ����I�������
		if (m_enemy->GetIsPlayingAnimation() == false)
		{
			return new HumanEnemyIdleState(m_enemy);
		}

		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void HumanEnemyReactionState::Update()
	{
		
	}
}
