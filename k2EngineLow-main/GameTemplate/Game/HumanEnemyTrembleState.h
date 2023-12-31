#pragma once
#include "IHumanEnemyState.h"

/// <summary>
/// ヒューマンエネミーの名前空間
/// </summary>
namespace nsHumanEnemy
{
	//前方宣言
	class HumanEnemy;

	/// <summary>
	/// エネミーの四つん這い待機ステートクラス
	/// </summary>
	class HumanEnemyTrembleState : public IHumanEnemyState
	{
	public:
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		/// <param name="player">エネミーのインスタンス。</param>
		HumanEnemyTrembleState(HumanEnemy* enemy) : IHumanEnemyState(enemy) {}
		/// <summary>
		/// デストラクタ。
		/// </summary>
		~HumanEnemyTrembleState() override;
		/// <summary>
		/// ステート開始時の処理。
		/// </summary>
		void Enter() override;
		/// <summary>
		/// ステートの遷移処理。
		/// </summary>
		/// <returns>遷移するステート</returns>
		IHumanEnemyState* StateChange() override;
		/// <summary>
		/// ステートにおける更新処理。
		/// </summary>
		void Update() override;
	};
}