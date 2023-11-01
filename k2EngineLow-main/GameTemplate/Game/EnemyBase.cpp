#include "stdafx.h"
#include "EnemyBase.h"
#include "Game.h"

bool EnemyBase::Start()
{
	m_game = FindGO<Game>("game");

	//���f���̏�����
	InitModel();

	return true;
}

void EnemyBase::HandleDamageEvent(float damage)
{
	//�_���[�W��^����
	m_hp -= damage;
	m_hp = max(0.0f, m_hp);
}