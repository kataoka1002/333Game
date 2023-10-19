#include "stdafx.h"
#include "Player.h"
#include "Teapot.h"
#include "GameCamera.h"

namespace
{
	const float MOVE_SPEED = 250.0f;
}

Player::Player()
{
	//�A�j���[�V�����̏�����
	animationClips[enAnimClip_Idle].Load("Assets/animData/player/player_idle.tka");
	animationClips[enAnimClip_Idle].SetLoopFlag(true);
	animationClips[enAnimClip_Walk].Load("Assets/animData/player/player_walk.tka");
	animationClips[enAnimClip_Walk].SetLoopFlag(true);
	animationClips[enAnimClip_Attract].Load("Assets/animData/player/player_attract.tka");
	animationClips[enAnimClip_Attract].SetLoopFlag(false);
	animationClips[enPlayerState_Standby].Load("Assets/animData/player/player_standby.tka");
	animationClips[enPlayerState_Standby].SetLoopFlag(true); 
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

}

Player::~Player()
{

}

bool Player::Start()
{
	m_camera = FindGO<GameCamera>("gamecamera");

	//�v���C���[���f���̏�����
	m_playerModel.Init("Assets/modelData/player/player.tkm", animationClips, enAnimClip_Num, enModelUpAxisZ);
	m_playerModel.SetScale(3.0f);
	m_playerModel.SetPosition(Vector3{ 0.0f,50.0f,100.0f });
	m_playerModel.Update();

	//�L�����N�^�[�R���g���[���[��������
	m_charaCon.Init(
		20.0f,			//���a
		120.0f,			//����
		m_position		//���W
	);

	return true;
}

void Player::Update()
{
	//�ړ��̏���
	Move();

	//��]����
	Turn();

	//�A�N�V�����̏���
	Action();

	//�X�e�[�g�Ǘ�
	ManageState();

	//�A�j���[�V�����̍Đ�
	PlayAnimation();

	//�X�V
	m_playerModel.Update();
}

void Player::Move()
{
	//�����񂹒�,�U�����͓����Ȃ�
	if (m_playerState == enPlayerState_Attract || m_playerState == enPlayerState_Attack)
	{
		return;
	}

	//�ړ����x�̏�����
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

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

	//�L�����N�^�[�R���g���[���[���g�p���č��W���X�V
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	//���W��ݒ�
	m_playerModel.SetPosition(m_position);
}

void Player::Turn()
{
	//�U�����͉��Ȃ�
	if (m_playerState == enPlayerState_Attack)
	{
		return;
	}

	//���炩�ɉ��悤�ɂ���
	//�I�u�W�F�N�g�������Ă��鎞
	if (m_playerState == enPlayerState_Standby || m_playerState == enPlayerState_Standwalk
		|| m_playerState == enPlayerState_Attract || m_playerState == enPlayerState_WalkLeft 
		|| m_playerState == enPlayerState_WalkRight || m_playerState == enPlayerState_WalkBack)
	{	
		//�J�����̌��������]�����߂�
		Vector3 rotSpeed = g_camera3D->GetForward();
		m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * 5.0f, m_rotMove, rotSpeed);
	}
	else
	{
		//�ړ����x�����]�����߂�
		m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * 8.0f, m_rotMove, m_moveSpeed);
	}

	//��]��ݒ肷��
	m_rotation.SetRotationYFromDirectionXZ(m_rotMove);
	m_playerModel.SetRotation(m_rotation);
}

void Player::Action()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_teapot = FindGO<Teapot>("teapot");

		m_teapot->InitAttract();

		m_playerState = enPlayerState_Attract;

		//�J�������ߋ����ɂ���
		m_camera->SetNearCamera(true);
	}

	if (g_pad[0]->IsTrigger(enButtonB))
	{
		m_playerState = enPlayerState_Attack;

		m_teapot = FindGO<Teapot>("teapot");

		m_teapot->InitBlowAway();
	}
}

void Player::ManageState()
{
	//�n�ʂɕt���Ă��Ȃ�������
	if (m_charaCon.IsOnGround() == false)
	{
		//�X�e�[�g���W�����v���ɂ���
		m_playerState = enPlayerState_Jump;

		return;
	}

	//�����񂹃X�e�[�g�Ȃ�
	if (m_playerState == enPlayerState_Attract)
	{
		//�A�j���[�V�����̍Đ����I�������B
		if (m_playerModel.IsPlayingAnimation() == false)
		{
			//�X�^���o�C�ɂ���
			m_playerState = enPlayerState_Standby;
			m_isHoldingObject = true;
		}

		return;
	}

	//�U���X�e�[�g�Ȃ�
	if (m_playerState == enPlayerState_Attack)
	{
		//�A�j���[�V�����̍Đ����I�������B
		if (m_playerModel.IsPlayingAnimation() == false)
		{
			//�A�C�h���ɂ���
			m_playerState = enPlayerState_Idle;
			m_isHoldingObject = false;

			//�J�������������ɖ߂�
			m_camera->SetNearCamera(false);
		}

		return;
	}

	//�I�u�W�F�N�g�������Ă���Ƃ�
	if (m_isHoldingObject) 
	{
		//�X�e�B�b�N�̓��͗ʂɂ���ăX�e�[�g��ύX
		if (LStick_x <= -0.5f)
		{
			//�������X�e�[�g�ɂ���
			m_playerState = enPlayerState_WalkLeft;
			return;
		}
		else if (LStick_x >= 0.5f)
		{
			//�E�����X�e�[�g�ɂ���
			m_playerState = enPlayerState_WalkRight;
			return;
		}

		if (LStick_y <= -0.5f)
		{
			//�������X�e�[�g�ɂ���
			m_playerState = enPlayerState_WalkBack;
			return;
		}
		
		m_playerState = (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f) ? enPlayerState_Standwalk : enPlayerState_Standby;
	}
	//�����Ă��Ȃ��Ƃ�
	else 
	{
		//�X�e�B�b�N�̓��͗ʂɂ���ăX�e�[�g��ύX
		m_playerState = (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f) ? enPlayerState_Walk : enPlayerState_Idle;
	}
}

void Player::PlayAnimation()
{
	switch (m_playerState) 
	{
	//�v���C���[�X�e�[�g���ҋ@��������
	case enPlayerState_Idle:
		//�ҋ@�A�j���[�V�������Đ�
		m_playerModel.PlayAnimation(enAnimClip_Idle, 0.5f);
		break;

	//�v���C���[�X�e�[�g����������������
	case enPlayerState_Walk:
		//�����A�j���[�V�������Đ�
		m_playerModel.PlayAnimation(enAnimClip_Walk, 0.2f);
		break;
		
	//�v���C���[�X�e�[�g�������񂹂�������
	case enAnimClip_Attract:
		//�����񂹃A�j���[�V�������Đ�
		m_playerModel.PlayAnimation(enPlayerState_Attract, 0.5f);
		break;
		
	//�v���C���[�X�e�[�g���X�^���o�C��������
	case enPlayerState_Standby:
		//�����񂹃A�j���[�V�������Đ�
		m_playerModel.PlayAnimation(enAnimClip_Standby, 0.5f);
		break; 
			
	//�v���C���[�X�e�[�g���\��������������
	case enPlayerState_Standwalk:
		//�\�������A�j���[�V�������Đ�
		m_playerModel.PlayAnimation(enAnimClip_Standwalk, 0.2f);
		break;

	//�v���C���[�X�e�[�g���\��������������
	case enPlayerState_Attack:
		//�\�������A�j���[�V�������Đ�
		m_playerModel.PlayAnimation(enAnimClip_Attack, 0.2f);
		break;

	//�v���C���[�X�e�[�g���E������������
	case enPlayerState_WalkRight:
		//�E�����A�j���[�V�������Đ�
		m_playerModel.PlayAnimation(enAnimClip_WalkRight, 0.3f);
		break;

	//�v���C���[�X�e�[�g����������������
	case enPlayerState_WalkLeft:
		//�������A�j���[�V�������Đ�
		m_playerModel.PlayAnimation(enAnimClip_WalkLeft, 0.3f);
		break;

	//�v���C���[�X�e�[�g����������������
	case enPlayerState_WalkBack:
		//�������A�j���[�V�������Đ�
		m_playerModel.PlayAnimation(enAnimClip_WalkBack, 0.3f);
		break;

	default:
		break;
	}
}

void Player::Render(RenderContext& rc)
{
	m_playerModel.Draw(rc);
}

