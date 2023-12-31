#include "stdafx.h"
#include "BossHitReactionState.h"
#include "BossIdleState.h"

/// <summary>
/// ボスの名前空間
/// </summary>
namespace nsBoss
{
	BossHitReactionState::~BossHitReactionState()
	{

	}

	void BossHitReactionState::Enter()
	{
		// 再生するアニメーションを設定。
		m_boss->SetAnimation(Boss::enAnimClip_Reaction, 0.3f);
	}

	IBossState* BossHitReactionState::StateChange()
	{
		//アニメーションの再生が終わったら
		if (m_boss->GetIsPlayingAnimation() == false)
		{
			return new BossIdleState(m_boss);
		}

		// ここまで来たらステートを遷移しない。
		return nullptr;
	}

	void BossHitReactionState::Update()
	{
		
	}
}
