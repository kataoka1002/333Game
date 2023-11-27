#include "stdafx.h"
#include "HumanEnemyAttackState.h"
#include "HumanEnemyIdleState.h"
#include "HumanEnemyDeadState.h"
#include "HumanEnemyReactionState.h"

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

		////�U�����󂯂���
		//if (m_enemy->DidAttackHit())
		//{
		//	// �̗͂����ȉ��Ŏl�񔇂�
		//	if (m_enemy->GetHP() <= KNEELING_HP)
		//	{
		//		//�̗�0�ȉ��Ŏ��S
		//		return new HumanEnemyDeadState(m_enemy);
		//	}
		//	else
		//	{
		//		//�̗͂���Ȃ烊�A�N�V�����̂�
		//		return new HumanEnemyReactionState(m_enemy);
		//	}
		//}

		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void HumanEnemyAttackState::Update()
	{
		
	}
}
