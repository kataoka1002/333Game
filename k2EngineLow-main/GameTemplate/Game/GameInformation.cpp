#include "stdafx.h"
#include "GameInformation.h"

namespace
{
	const int ENABLE_BOSSBATTLE_ENEMY_NUM = 1;
}

GameInformation::~GameInformation()
{

}

bool GameInformation::Start()
{
	return true;
}

void GameInformation::Update()
{
	//�{�X��Ȃ�
	if (m_isInBossBattle)
	{
		//�{�X���j���Ԃ̌v��
		m_bossKillTime += g_gameTime->GetFrameDeltaTime();
	}
}