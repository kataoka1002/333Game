#include "stdafx.h"
#include "GameClear.h"
#include "ResultCamera.h"
#include "ResultUI.h"
#include "BlackFade.h"

GameClear::GameClear()
{

}

GameClear::~GameClear()
{
	DeleteGO(m_resultCamera);
	DeleteGO(m_resultUI);
	DeleteGO(m_blackFade);
}

void GameClear::InitCamera()
{
	//カメラの設定
	m_resultCamera = NewGO<ResultCamera>(0, "resultcamera");
	m_resultCamera->SetTarget(m_playerPosition);
}

void GameClear::InitUI()
{
	//UIの設定
	m_resultUI = NewGO<ResultUI>(0, "resultui");
	m_resultUI->SetPlayerDead(false);
}

void GameClear::Update()
{
	//タイトルへ戻る
	GoTitle();

	//再生するアニメーションを設定
	SetAnimation();

	if (m_pressAButton)
	{
		m_resultUI->SetFontAlpha();
	}

	// アニメーションを再生
	m_playerModel.PlayAnimation(m_currentAnimationClip, 0.5f);
	m_playerModel.Update();
}

void GameClear::OnDestroy()
{
	//自身の削除
	DeleteGO(this);
}

void GameClear::Render(RenderContext& rc)
{
	m_playerModel.Draw(rc);
	m_bgModel.Draw(rc);
}