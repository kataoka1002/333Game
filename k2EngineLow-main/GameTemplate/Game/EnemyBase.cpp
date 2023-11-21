#include "stdafx.h"
#include "EnemyBase.h"
#include "Game.h"

bool EnemyBase::Start()
{
	m_game = FindGO<Game>("game");
	m_player = FindGO<nsPlayer::Player>("player");

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

bool EnemyBase::DidAttackHit()
{
	//�U���p�̃R���W�����̔z����擾����B
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_punch");
	//�z���for���ŉ񂷁B
	for (auto collision : collisions)
	{
		//�R���W�����ƃL�����R�����Փ˂�����B
		if (collision->IsHit(m_charaCon))
		{
			//�_���[�W��^����
			HandleDamageEvent(10.0f);

			return true;
		}
	}

	return false;
}