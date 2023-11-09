#include "stdafx.h"
#include "HumanEnemyAttackState.h"
#include "HumanEnemyIdleState.h"

/// <summary>
/// �q���[�}���G�l�~�[�̖��O���
/// </summary>
namespace nsHumanEnemy
{
	HumanEnemyAttackState::~HumanEnemyAttackState()
	{

	}

	void HumanEnemyAttackState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_enemy->SetAnimation(HumanEnemy::enAnimClip_Attack, 0.2f);
	}

	IHumanEnemyState* HumanEnemyAttackState::StateChange()
	{
		//�A�j���[�V�����̍Đ����I�������
		if (m_enemy->GetIsPlayingAnimation() == false)
		{
			return new HumanEnemyIdleState(m_enemy);
		}

		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void HumanEnemyAttackState::Update()
	{
		
	}
}
