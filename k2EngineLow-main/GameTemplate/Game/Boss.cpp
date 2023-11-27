#include "stdafx.h"
#include "Boss.h"
#include "Game.h"
#include "IBossState.h"
#include "BossIdleState.h"
#include "BossHitReactionState.h"
#include "BossDeadState.h"
#include "BossConstants.h"
#include "BossUI.h"

/// <summary>
/// �{�X�̖��O���
/// </summary>
namespace nsBoss
{
	Boss::Boss()
	{
		//�A�j���[�V�����̏�����
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
		animationClips[enAnimClip_JumpAttack].Load("Assets/animData/boss/boss_JumpAttack.tka");
		animationClips[enAnimClip_JumpAttack].SetLoopFlag(false);
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

		//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��
		m_model.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
			OnAnimationEvent(clipName, eventName);
			});

		// �L�����N�^�[�R���g���[���[��������
		m_charaCon.Init(
			110.0f,			//���a
			170.0f,			//����
			m_position		//���W
		);

		// HP�̐ݒ�
		m_hp = MAX_HP;

		//�N�[���_�E���^�C���̐ݒ�
		m_magicCoolDowmTime = MAGIC_COOLDOWN_TIME;
		m_hitCoolDowmTime = HIT_COOLDOWN_TIME;

		// �����X�e�[�g��ݒ�
		m_bossState = new BossIdleState(this);
		m_bossState->Enter();

		//UI���쐬
		m_ui = NewGO<BossUI>(0, "bossui");
	}

	void Boss::Update()
	{
		// �X�e�[�g��ύX���邩
		IBossState* bossState = m_bossState->StateChange();

		// �ύX��̃X�e�[�g���ݒ肳��Ă���Ȃ��
		if (bossState != nullptr)
		{
			// �X�e�[�g��ύX����B
			delete m_bossState;
			m_bossState = bossState;
			m_bossState->Enter();
		}

		//�v���C���[�̍U���������������𔻒肷��(��������Object�N���X�Ŕ���)
		DidAttackHit();

		//�̗͂̃`�F�b�N�����Đ����Ă��邩���m�F����
		CheckHP();

		// �e�X�e�[�g�̍X�V���������s�B
		m_bossState->Update();

		// �A�j���[�V�������Đ�����B
		PlayAnimation(m_currentAnimationClip);

		// ���f�����X�V����B
		m_model.Update();
	}

	void Boss::FollowPlayer()
	{
		//�ړ����x�̏�����
		m_moveSpeed.x = 0.0f;
		m_moveSpeed.z = 0.0f;

		//�v���C���[�̕��������߂�
		Vector3 toPlayerDir = m_player->GetPosition() - m_position;
		toPlayerDir.Normalize();

		//�ŏI�I�Ȉړ����x�̌v�Z
		m_moveSpeed += toPlayerDir * MOVE_SPEED;
		m_moveSpeed.y = 0.0f;

		//�d�͂̐ݒ�
		m_moveSpeed.y -= 980.0f * g_gameTime->GetFrameDeltaTime();

		//�L�����N�^�[�R���g���[���[���g�p���č��W���X�V
		m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
		
		//���W��ݒ�
		m_model.SetPosition(m_position);
	}

	void Boss::Turn()
	{
		//�ړ����x�����]�����߂�
		m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * 2.0f, m_rotMove, m_moveSpeed);

		//�O�����̐ݒ�
		m_forward = m_rotMove;
		m_forward.Normalize();

		//��]��ݒ肷��
		m_rotation.SetRotationYFromDirectionXZ(m_rotMove);
		m_model.SetRotation(m_rotation);
	}

	void Boss::OnDestroy()
	{
		//���X�g����폜
		m_game->RemoveEnemyFromList(this);

		//�폜
		DeleteGO(this);
	}

	void Boss::PlayAnimation(EnAnimationClip currentAnimationClip)
	{
		// �A�j���[�V�������Đ�
		m_model.PlayAnimation(currentAnimationClip, m_complementTime);
	}

	void Boss::SetCollision()
	{
		//�R���W�����I�u�W�F�N�g���쐬����B
		m_collisionObject = NewGO<CollisionObject>(0);
		Quaternion rot;
		rot.SetRotationDegX(90.0f);

		//�J�v�Z���̃R���W�������쐬����B
		m_collisionObject->CreateCapsule(
			m_position,
			rot,
			30.0f,
			180.0f
		);

		//�R���W�����ɖ��O��t����
		m_collisionObject->SetName("boss");

		//�R���W�����I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���
		m_collisionObject->SetIsEnableAutoDelete(false);
	}

	void Boss::MoveCollision()
	{
		//�R���W���������ɓ|��
		Quaternion rot = m_rotation;
		rot.AddRotationDegX(180.0f);

		//�R���W�����̉�],�ړ���ݒ肷��
		m_collisionObject->SetRotation(rot);
		m_collisionObject->SetPosition(m_position);
	}

	void Boss::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
	{
		//�L�[�̖��O���uattack_start�v�̎��B
		if (wcscmp(eventName, L"attack_start") == 0)
		{
			//�R���W�����I�u�W�F�N�g���쐬����B
			auto collisionObject = NewGO<CollisionObject>(0);
			Vector3 collisionPosition = m_position;
			//���W���v���C���[�̏����O,��ɐݒ肷��B
			collisionPosition += m_forward * 100.0f;
			collisionPosition.y += 100.0f;
			//����̃R���W�������쐬����B
			collisionObject->CreateSphere(
				collisionPosition,		//���W�B
				Quaternion::Identity,	//��]�B
				50.0f					//���a�B
			);
			collisionObject->SetName("human_attack");
		}
		//�L�[�̖��O���uattack_end�v�̎��B
		else if (wcscmp(eventName, L"attack_end") == 0)
		{

		}
		//�L�[�̖��O���ujump_start�v�̎��B
		else if (wcscmp(eventName, L"jump_start") == 0)
		{
			m_isJumping = true;
		}
		//�L�[�̖��O���ujump_end�v�̎��B
		else if (wcscmp(eventName, L"jump_end") == 0)
		{
			m_isJumping = false;
		}
		//�L�[�̖��O���ujumpAttack_start�v�̎��B
		else if (wcscmp(eventName, L"jumpAttack_start") == 0)
		{
			//�R���W�����I�u�W�F�N�g���쐬����B
			auto collisionObject = NewGO<CollisionObject>(0);
			Vector3 collisionPosition = m_position;
			//���W���v���C���[�̏����O�ɐݒ肷��B
			collisionPosition += m_forward * 50.0f;
			//����̃R���W�������쐬����B
			collisionObject->CreateSphere(
				collisionPosition,		//���W�B
				Quaternion::Identity,	//��]�B
				250.0f					//���a�B
			);
			collisionObject->SetName("boss_jumpAttack");
		}
	}

	void Boss::GameClear()
	{
		//�Q�[���N���A���Ăяo��
		m_game->SetGameClear();
	}

	void Boss::SetReactionState()
	{
		//���A�N�V�����X�e�[�g���Z�b�g����
		m_bossState = new BossHitReactionState(this);
		m_bossState->Enter();
	}

	void Boss::PlayReaction()
	{
		//���ɂȂ�
	}

	void Boss::CheckHP()
	{
		// �̗͂�0�ȉ��Ŏ��S
		if (m_hp <= 0.0f)
		{
			m_bossState = new BossDeadState(this);
			m_bossState->Enter();
		}
	}

	void Boss::CalcMagicCoolDown()
	{
		m_magicCoolDowmTime -= g_gameTime->GetFrameDeltaTime();

		if (m_magicCoolDowmTime <= 0.0f)
		{
			m_magicCoolDowmTime = MAGIC_COOLDOWN_TIME;
			m_isMagicCoolDowm = false;
		}
	}

	void Boss::CalcHitCoolDown()
	{
		m_hitCoolDowmTime -= g_gameTime->GetFrameDeltaTime();

		if (m_hitCoolDowmTime <= 0.0f)
		{
			m_hitCoolDowmTime = HIT_COOLDOWN_TIME;
			m_isHitCoolDowm = false;
		}
	}

	void Boss::JumpAttack()
	{
		//�W�����v������Ȃ��Ȃ�
		if (m_isJumping != true)
		{
			return;
		}

		//��ԂŃv���C���[�̏ꏊ�܂Ŕ��ł���
		m_position = Math::Lerp(g_gameTime->GetFrameDeltaTime() * JUMP_ATTACK_SPEED, m_position, m_jumpTargetPos);

		m_model.SetPosition(m_position);
		m_charaCon.SetPosition(m_position);
	}

	void Boss::Render(RenderContext& rc)
	{
		m_model.Draw(rc);
	}
}