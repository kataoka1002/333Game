#pragma once
#include "IBossState.h"

/// <summary>
/// ボスの名前空間
/// </summary>
namespace nsBoss
{
	//前方宣言
	class Boss;

	/// <summary>
	/// ボスの歩きステートクラス
	/// </summary>
	class BossWalkState : public IBossState
	{
	public:
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		/// <param name="player">エネミーのインスタンス。</param>
		BossWalkState(Boss* boss) : IBossState(boss) {}
		/// <summary>
		/// デストラクタ。
		/// </summary>
		~BossWalkState() override;
		/// <summary>
		/// ステート開始時の処理。
		/// </summary>
		void Enter() override;
		/// <summary>
		/// ステートの遷移処理。
		/// </summary>
		/// <returns>遷移するステート</returns>
		IBossState* StateChange() override;
		/// <summary>
		/// ステートにおける更新処理。
		/// </summary>
		void Update() override;
	};
}