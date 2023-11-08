#include "stdafx.h"
#include "Player.h"
#include "Teapot.h"
#include "GameCamera.h"
#include "Game.h"
#include "ObjectBase.h"
#include "IPlayerState.h"
#include "PlayerIdleState.h"
#include "PlayerJumpState.h"

namespace
{
	const float MOVE_SPEED = 250.0f;

	const float MAX_HP = 100.0f;
}

/// <summary>
/// �v���C���[�̖��O���
/// </summary>
namespace nsPlayer
{
	Player::Player()
	{
		//�A�j���[�V�����̏�����
		animationClips[enAnimClip_Idle].Load("Assets/animData/player/player_idle.tka");
		animationClips[enAnimClip_Idle].SetLoopFlag(true);
		animationClips[enAnimClip_Walk].Load("Assets/animData/player/player_walk.tka");
		animationClips[enAnimClip_Walk].SetLoopFlag(true);
		animationClips[enAnimClip_Jump].Load("Assets/animData/player/player_jump.tka");
		animationClips[enAnimClip_Jump].SetLoopFlag(false);
		animationClips[enAnimClip_Attract].Load("Assets/animData/player/player_attract.tka");
		animationClips[enAnimClip_Attract].SetLoopFlag(false);
		animationClips[enAnimClip_Standby].Load("Assets/animData/player/player_standby.tka");
		animationClips[enAnimClip_Standby].SetLoopFlag(true);
		animationClips[enAnimClip_Standwalk].Load("Assets/animData/player/player_standwalk.tka");
		animationClips[enAnimClip_Standwalk].SetLoopFlag(true);
		animationClips[enAnimClip_Attack].Load("Assets/animData/player/player_attack.tka");
		animationClips[enAnimClip_Attack].SetLoopFlag(false);
		animationClips[enAnimClip_WalkRight].Load("Assets/animData/player/player_walk_right.tka");
		animationClips[enAnimClip_WalkRight].SetLoopFlag(true);
		animationClips[enAnimClip_WalkLeft].Load("Assets/animData/player/player_walk_left.tka");
		animationClips[enAnimClip_WalkLeft].SetLoopFlag(true);
		animationClips[enAnimClip_WalkBack].Load("Assets/animData/player/player_walk_back.tka");
		animationClips[enAnimClip_WalkBack].SetLoopFlag(true);
		animationClips[enAnimClip_WalkJump].Load("Assets/animData/player/player_walk_jump.tka");
		animationClips[enAnimClip_WalkJump].SetLoopFlag(false);
		animationClips[enAnimClip_Dead].Load("Assets/animData/player/player_dead.tka");
		animationClips[enAnimClip_Dead].SetLoopFlag(false);
		animationClips[enAnimClip_Reaction].Load("Assets/animData/player/player_reaction.tka");
		animationClips[enAnimClip_Reaction].SetLoopFlag(false);
	}

	Player::~Player()
	{

	}

	bool Player::Start()
	{
		m_camera = FindGO<GameCamera>("gamecamera");

		m_game = FindGO<Game>("game");

		//�v���C���[���f���̏�����
		m_playerModel.Init("Assets/modelData/player/player.tkm", animationClips, enAnimClip_Num, enModelUpAxisZ);
		m_playerModel.SetPosition(m_position);
		m_playerModel.SetRotation(m_rotation);
		m_playerModel.SetScale(m_scale);
		m_playerModel.Update();

		//�L�����N�^�[�R���g���[���[��������
		m_charaCon.Init(
			20.0f,			//���a
			120.0f,			//����
			m_position		//���W
		);

		//�̗͂̐ݒ�
		m_hp = MAX_HP;

		// �����X�e�[�g��ݒ�
		m_playerState = new PlayerIdleState(this);
		m_playerState->Enter();

		return true;
	}

	void Player::Update()
	{
		// �X�e�[�g��ύX���邩
		IPlayerState* playerState = m_playerState->StateChange();

		// �ύX��̃X�e�[�g���ݒ肳��Ă���Ȃ��
		if (playerState != nullptr)
		{
			// �X�e�[�g��ύX����B
			delete m_playerState;
			m_playerState = playerState;
			m_playerState->Enter();
		}

		// �e�X�e�[�g�̍X�V���������s�B
		m_playerState->Update();

		// �A�j���[�V�������Đ�����B
		PlayAnimation(m_currentAnimationClip);

		// ���f�����X�V����B
		m_playerModel.Update();
	}

	void Player::PlayAnimation(EnAnimationClip currentAnimationClip)
	{
		// �A�j���[�V�������Đ�
		m_playerModel.PlayAnimation(currentAnimationClip, m_complementTime);
	}

	void Player::Move()
	{
		//�ړ����x�̏�����
		m_moveSpeed.x = 0.0f;
		m_moveSpeed.z = 0.0f;
	
		//���̏�W�����v����Ȃ��Ȃ�
		if (m_isJumping != true)
		{
			//���X�e�B�b�N�̓��͗ʂ��󂯎��
			LStick_x = g_pad[0]->GetLStickXF();
			LStick_y = g_pad[0]->GetLStickYF();

			//�J�����̑O�������ƉE�������擾
			Vector3 cameraForward = g_camera3D->GetForward();
			Vector3 cameraRight = g_camera3D->GetRight();

			//XZ���ʂł̑O�������A�E�����ɕϊ�����
			cameraForward.y = 0.0f;
			cameraForward.Normalize();
			cameraRight.y = 0.0f;
			cameraRight.Normalize();

			//XZ�����̈ړ����x���N���A
			m_moveSpeed += cameraForward * LStick_y * MOVE_SPEED;	//�������ւ̈ړ����x�����Z
			m_moveSpeed += cameraRight * LStick_x * MOVE_SPEED;		//�E�����ւ̈ړ����x�����Z
		}

		//�d�͂̐ݒ�
		m_moveSpeed.y -= 980.0f * g_gameTime->GetFrameDeltaTime();

		//�L�����N�^�[�R���g���[���[���g�p���č��W���X�V
		m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

		if (m_charaCon.IsOnGround()) 
		{
			//�n�ʂɂ���
			m_moveSpeed.y = 0.0f;

			//���̏�W�����v������Ȃ�
			SetIsJumping(false);
		}
	
		//���W��ݒ�
		m_playerModel.SetPosition(m_position);
	}

	void Player::Turn()
	{	
		//���炩�ɉ��悤�ɂ���
		//�I�u�W�F�N�g�������Ă��鎞
		if (m_isHoldingObject)
		{	
			//�J�����̌��������]�����߂�
			Vector3 rotSpeed = g_camera3D->GetForward();
			m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * 5.0f, m_rotMove, rotSpeed);
		}
		else
		{
			//�ړ����x�����]�����߂�
			m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * 3.5f, m_rotMove, m_moveSpeed);
		}
	
		//��]��ݒ肷��
		m_rotation.SetRotationYFromDirectionXZ(m_rotMove);
		m_playerModel.SetRotation(m_rotation);
	}

	void Player::Attract()
	{
		if (m_holdingObject != nullptr)
		{
			return;
		}

		for (auto object : m_game->GetObjectList())
		{
			if (object->GetCanAttract() == true)
			{
				//�����񂹂̏�����
				object->InitAttract();

				//�����񂹉\��ԂɕύX
				m_canAttract = true;

				//�J�������ߋ����ɂ���
				m_camera->SetNearCamera(true);

				//�����Ă���I�u�W�F�N�g�̐ݒ�
				m_holdingObject = object;
				break;
			}
		}
	}

	void Player::Attack()
	{
		if (m_holdingObject == nullptr)
		{
			return;
		}

		//�X�e�[�g���U�����ɕύX
		m_canAttack = true;

		//�Ԃ���΂�����
		m_holdingObject->InitBlowAway();

		//�����Ă���I�u�W�F�N�g�͖����ɂ���
		m_holdingObject = nullptr;
	}

	void Player::ResetAttack()
	{
		//�J�������������ɖ߂�
		m_camera->SetNearCamera(false);

		//�I�u�W�F�N�g�ێ���Ԃ�����
		m_isHoldingObject = false;
	}

	void Player::ChangeWalkingStyle()
	{
		//�X�e�B�b�N�̓��͗ʂɂ���ĕ�������ς���
		if (LStick_x <= -0.5f)
		{
			//�������A�j���[�V����
			SetAnimation(enAnimClip_WalkLeft, 0.3f);
			return;
		}
		else if (LStick_x >= 0.5f)
		{
			//�E�����A�j���[�V����
			SetAnimation(enAnimClip_WalkRight, 0.3f);
			return;
		}

		if (LStick_y <= -0.5f)
		{
			//�������A�j���[�V����
			SetAnimation(enAnimClip_WalkBack, 0.3f);
			return;
		}

		if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
		{
			//�X�^���h�����A�j���[�V����
			SetAnimation(enAnimClip_Standwalk, 0.2f);
		}
		else
		{
			//�X�^���o�C�A�j���[�V����
			SetAnimation(enAnimClip_Standby, 0.2f);
		}
	}

	void Player::Render(RenderContext& rc)
	{
		m_playerModel.Draw(rc);
	}
}