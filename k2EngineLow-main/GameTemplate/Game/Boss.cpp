#include "stdafx.h"
#include "Boss.h"
#include "Game.h"
#include "IBossState.h"
#include "BossIdleState.h"
#include "BossHitReactionState.h"
#include "BossConstants.h"
#include "BossUI.h"

/// <summary>
/// ボスの名前空間
/// </summary>
namespace nsBoss
{
	Boss::Boss()
	{
		//アニメーションの初期化
		animationClips[enAnimClip_Idle].Load("Assets/animData/boss/boss_Idle.tka");
		animationClips[enAnimClip_Idle].SetLoopFlag(true);
		animationClips[enAnimClip_Dead].Load("Assets/animData/boss/boss_Dead.tka");
		animationClips[enAnimClip_Dead].SetLoopFlag(false);
		animationClips[enAnimClip_Reaction].Load("Assets/animData/boss/boss_Reaction.tka");
		animationClips[enAnimClip_Reaction].SetLoopFlag(false);
		animationClips[enAnimClip_Jump].Load("Assets/animData/boss/boss_Jump.tka");
		animationClips[enAnimClip_Jump].SetLoopFlag(false);
		animationClips[enAnimClip_Magic].Load("Assets/animData/boss/boss_Magic.tka");
		animationClips[enAnimClip_Magic].SetLoopFlag(false);
		animationClips[enAnimClip_Punch].Load("Assets/animData/boss/boss_Punch.tka");
		animationClips[enAnimClip_Punch].SetLoopFlag(false);
		animationClips[enAnimClip_Swipe].Load("Assets/animData/boss/boss_Swipe.tka");
		animationClips[enAnimClip_Swipe].SetLoopFlag(false);
		animationClips[enAnimClip_Walk].Load("Assets/animData/boss/boss_Walk.tka");
		animationClips[enAnimClip_Walk].SetLoopFlag(true);
	}

	Boss::~Boss()
	{
		DeleteGO(m_collisionObject);
		DeleteGO(m_ui);
	}

	void Boss::InitModel()
	{
		m_model.Init("Assets/modelData/Boss/Boss.tkm", animationClips, enAnimClip_Num, enModelUpAxisZ);
		m_model.SetPosition(m_position);
		m_model.SetRotation(m_rotation);
		m_model.SetScale(m_scale);
		m_model.Update();

		//アニメーションイベント用の関数を設定する
		m_model.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
			OnAnimationEvent(clipName, eventName);
			});

		// キャラクターコントローラーを初期化
		m_charaCon.Init(
			110.0f,			//半径
			170.0f,			//高さ
			m_position		//座標
		);

		// HPの設定
		m_hp = MAX_HP;

		// 初期ステートを設定
		m_bossState = new BossIdleState(this);
		m_bossState->Enter();

		//UIを作成
		m_ui = NewGO<BossUI>(0, "bossui");
	}

	void Boss::Update()
	{
		// ステートを変更するか
		IBossState* bossState = m_bossState->StateChange();

		// 変更先のステートが設定されているならば
		if (bossState != nullptr)
		{
			// ステートを変更する。
			delete m_bossState;
			m_bossState = bossState;
			m_bossState->Enter();
		}

		// 各ステートの更新処理を実行。
		m_bossState->Update();

		// アニメーションを再生する。
		PlayAnimation(m_currentAnimationClip);

		// モデルを更新する。
		m_model.Update();
	}

	void Boss::FollowPlayer()
	{
		//移動速度の初期化
		m_moveSpeed.x = 0.0f;
		m_moveSpeed.z = 0.0f;

		//プレイヤーの方向を求める
		Vector3 toPlayerDir = m_player->GetPosition() - m_position;
		toPlayerDir.Normalize();

		//最終的な移動速度の計算
		m_moveSpeed += toPlayerDir * MOVE_SPEED;
		m_moveSpeed.y = 0.0f;

		//重力の設定
		m_moveSpeed.y -= 980.0f * g_gameTime->GetFrameDeltaTime();

		//キャラクターコントローラーを使用して座標を更新
		m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

		//座標を設定
		m_model.SetPosition(m_position);
	}

	void Boss::Turn()
	{
		//移動速度から回転を求める
		m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * 2.0f, m_rotMove, m_moveSpeed);

		//前方向の設定
		m_forward = m_rotMove;
		m_forward.Normalize();

		//回転を設定する
		m_rotation.SetRotationYFromDirectionXZ(m_rotMove);
		m_model.SetRotation(m_rotation);
	}

	void Boss::OnDestroy()
	{
		//リストから削除
		m_game->RemoveEnemyFromList(this);

		//削除
		DeleteGO(this);
	}

	void Boss::PlayAnimation(EnAnimationClip currentAnimationClip)
	{
		// アニメーションを再生
		m_model.PlayAnimation(currentAnimationClip, m_complementTime);
	}

	void Boss::SetCollision()
	{
		//コリジョンオブジェクトを作成する。
		m_collisionObject = NewGO<CollisionObject>(0);
		Quaternion rot;
		rot.SetRotationDegX(90.0f);

		//カプセルのコリジョンを作成する。
		m_collisionObject->CreateCapsule(
			m_position,
			rot,
			30.0f,
			180.0f
		);

		//コリジョンに名前を付ける
		m_collisionObject->SetName("boss");

		//コリジョンオブジェクトが自動で削除されないようにする
		m_collisionObject->SetIsEnableAutoDelete(false);
	}

	void Boss::MoveCollision()
	{
		//コリジョンを横に倒す
		Quaternion rot = m_rotation;
		rot.AddRotationDegX(180.0f);

		//コリジョンの回転,移動を設定する
		m_collisionObject->SetRotation(rot);
		m_collisionObject->SetPosition(m_position);
	}

	void Boss::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
	{
		//キーの名前が「attack_start」の時。
		if (wcscmp(eventName, L"attack_start") == 0)
		{
			//コリジョンオブジェクトを作成する。
			auto collisionObject = NewGO<CollisionObject>(0);
			Vector3 collisionPosition = m_position;
			//座標をプレイヤーの少し前,上に設定する。
			collisionPosition += m_forward * 100.0f;
			collisionPosition.y += 100.0f;
			//球状のコリジョンを作成する。
			collisionObject->CreateSphere(
				collisionPosition,		//座標。
				Quaternion::Identity,	//回転。
				50.0f					//半径。
			);
			collisionObject->SetName("human_attack");
		}
		//キーの名前が「attack_end」の時。
		else if (wcscmp(eventName, L"attack_end") == 0)
		{

		}
	}

	void Boss::GameClear()
	{
		//ゲームクリアを呼び出す
		m_game->SetGameClear();
	}

	void Boss::SetReactionState()
	{
		m_bossState = new BossHitReactionState(this);
		m_bossState->Enter();
	}

	void Boss::Render(RenderContext& rc)
	{
		m_model.Draw(rc);
	}
}