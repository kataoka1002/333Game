#pragma once
#include "SkyCube.h"
class GameInformation;

class ResultUI : public IGameObject
{
public:
	enum enResultUIState
	{
		enResultUIState_Fade,
		enResultUIState_You,
		enResultUIState_Enemy,
		enResultUIState_EnemyNum,
		enResultUIState_Boss,
		enResultUIState_BossTime,
		enResultUIState_Score,
		enResultUIState_ScoreValue,
		enResultUIState_GoTitle,
	};

public:
	~ResultUI();
	bool Start();
	void Update();
	float CalcSinValue(float speed);
	void FadeProcess();
	void YouProcess();
	void EnemyProcess();
	void EnemyNumProcess();
	void BossProcess();
	void BossTimeProcess();
	void ScoreProcess();
	void ScoreValueProcess();
	void GoTitleProcess();
	void CalcScaleAndAlpha(float& scale, float& alpa);
	void CalcFontScale(float& scale, enResultUIState next);
	void Render(RenderContext& rc);

	void SetPlayerDead(bool which)
	{
		m_isPlayerDead = which;
	}

	void SetKillEnemyNum(const int num)
	{
		m_killEnemyNum = num;
	}

	void SetBossKillTime(const float time)
	{
		m_bossKillTime = time;
	}

	void SetFontAlpha()
	{
		m_fontColor -= 0.1f;
		m_enemyNumFont.SetColor({ m_fontColor ,m_fontColor ,m_fontColor ,m_fontColor });
		m_bossTimeFont.SetColor({ m_fontColor ,m_fontColor ,m_fontColor ,m_fontColor });
		m_scoreFont.SetColor({ m_fontColor ,m_fontColor ,m_fontColor ,m_fontColor });
	}

private:
	GameInformation* m_gameInfo = nullptr;
	SkyCube* m_skyCube = nullptr;

	enResultUIState m_resultUIState = enResultUIState_Fade;

	SpriteRender m_fadeSprite;
	SpriteRender m_youSprite;
	SpriteRender m_resultSprite;
	SpriteRender m_scoreSprite;
	SpriteRender m_goTitleSprite;
	SpriteRender m_enemySprite;
	SpriteRender m_bossSprite;

	float m_step = 0.1f;				//�傫�����ς�鑬��
	FontRender m_enemyNumFont;			//�|�����G�̐��̃t�H���g
	int m_killEnemyNum = 0;				//�|�����G�̐�
	int m_drawNum = 0;					//���\�����Ă���,�|�����G�̐�
	float m_enemyFontScale = 1.0f;		//�|�����G�̐��̃t�H���g�̑傫��

	FontRender m_bossTimeFont;			//�{�X���j�ɂ����������Ԃ̃t�H���g
	float m_bossKillTime = 0.0f;		//�{�X���j����
	float m_bossFontScale = 1.0f;		//�{�X���j���Ԃ̑傫��
	float m_drawTime = 0.0f;			//���\�����Ă���,�{�X���j����

	FontRender m_scoreFont;
	float m_score = 0.0f;				//�X�R�A
	float m_scoreFontScale = 1.0f;		//�X�R�A�t�H���g�̑傫��
	int m_drawScore = 0.0f;				//���\�����Ă���,�X�R�A

	bool m_isPlayerDead = false;		//�v���C���[�����񂾂��ǂ���

	float m_degree = 0.0f;				//�����x�̕ύX�p�p�x

	float m_youAlpha = 0.0f;
	float m_youScale = 3.0f;
	float m_enemyAlpha = 0.0f;
	float m_enemyScale = 3.0f;
	float m_bossAlpha = 0.0f;
	float m_bossScale = 3.0f;
	float m_scoreAlpha = 0.0f;
	float m_scoreScale = 3.0f;
	float m_fadeAlpha = 1.0f;

	float m_time = 0.5f;				//�ŏ��������n�܂�܂ł̎���

	float m_fontColor = 1.0f;			//�t�H���g�̐F�A�����x
};

