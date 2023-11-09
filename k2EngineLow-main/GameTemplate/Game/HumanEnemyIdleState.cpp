#include "stdafx.h"
#include "HumanEnemyIdleState.h"
#include "HumanEnemyDeadState.h"
#include "HumanEnemyWalkState.h"
#include "HumanEnemyAttackState.h"

/// <summary>
/// ヒューマンエネミーの名前空間
/// </summary>
namespace nsHumanEnemy
{
	HumanEnemyIdleState::~HumanEnemyIdleState()
	{

	}

	void HumanEnemyIdleState::Enter()
	{
		// 再生するアニメーションを設定。
		m_enemy->SetAnimation(HumanEnemy::enAnimClip_Idle, 0.5f);
	}

	IHumanEnemyState* HumanEnemyIdleState::StateChange()
	{
		// 体力が50以下で四つん這い
		if (m_enemy->GetHP() <= 50.0f)
		{
			return new HumanEnemyDeadState(m_enemy);
		}

		if (g_pad[0]->IsTrigger(enButtonLB1))
		{
			return new HumanEnemyWalkState(m_enemy);
		}

		if (g_pad[0]->IsTrigger(enButtonLB2))
		{
			return new HumanEnemyAttackState(m_enemy);
		}
			
		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void HumanEnemyIdleState::Update()
	{
		
	}
}
