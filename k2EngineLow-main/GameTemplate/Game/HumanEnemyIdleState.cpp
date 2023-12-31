#include "stdafx.h"
#include "HumanEnemyIdleState.h"
#include "HumanEnemyDeadState.h"
#include "HumanEnemyWalkState.h"
#include "HumanEnemyAttackState.h"
#include "HumanEnemyReactionState.h"

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
		//一定距離以下になると追いかけてくる
		if (m_enemy->CheckDistanceToPlayer() <= CHASE_RANGE)
		{
			return new HumanEnemyWalkState(m_enemy);
		}

		//一定距離以下になると攻撃してくる
		if (m_enemy->CheckDistanceToPlayer() <= ATTACK_RANGE)
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
