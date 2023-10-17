#include "stdafx.h"
#include "Player.h"
#include "Teapot.h"

namespace
{
	
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
}

Player::~Player()
{

}

bool Player::Start()
{
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
	//�ړ����x�̏�����
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	//���X�e�B�b�N�̓��͗ʂ��󂯎��
	float LStick_x = g_pad[0]->GetLStickXF();
	float LStick_y = g_pad[0]->GetLStickYF();

	//�J�����̑O�������ƉE�������擾
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();

	//XZ���ʂł̑O�������A�E�����ɕϊ�����
	cameraForward.y = 0.0f;
	cameraForward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();

	//XZ�����̈ړ����x���N���A
	m_moveSpeed += cameraForward * LStick_y * 500.0f;	//�������ւ̈ړ����x�����Z
	m_moveSpeed += cameraRight * LStick_x * 500.0f;		//�E�����ւ̈ړ����x�����Z

	//�L�����N�^�[�R���g���[���[���g�p���č��W���X�V
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	//���W��ݒ�
	m_playerModel.SetPosition(m_position);
}

void Player::Turn()
{
	//���̃t���[���ł̓L�����͈ړ����Ă��Ȃ��̂Ő��񂷂�K�v�͂Ȃ�
	if (fabsf(m_moveSpeed.x) < 0.001f && fabsf(m_moveSpeed.z) < 0.001f) 
	{
		return;
	}

	m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * 8.0f, m_rotMove, m_moveSpeed);
	m_rotation.SetRotationYFromDirectionXZ(m_rotMove);


	//atan2��tan�Ƃ̒l���p�x(���W�A���P��)�ɕϊ����Ă����֐�
	//��]�p�x�����߂Ă���
	///float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//m_rotation.SetRotationY(-angle);

	//��]��ݒ肷��
	m_playerModel.SetRotation(m_rotation);
}

void Player::Action()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_teapot = FindGO<Teapot>("teapot");

		m_teapot->InitAttract(m_position);

		m_playerState = enPlayerState_Attract;
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

	//������,�X�^���o�C,�X�e�[�g�Ȃ�
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

	//�I�u�W�F�N�g�������Ă��Ȃ��Ƃ�
	if (m_isHoldingObject == false)
	{
		//�n�ʂɒ�����x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)
		if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
		{
			//�X�e�[�g�𑖂�ɂ���B
			m_playerState = enPlayerState_Walk;
		}
		//x��z�̈ړ����x������������(�X�e�B�b�N�̓��͂�����������)
		else
		{
			//�X�e�[�g��ҋ@�ɂ���B
			m_playerState = enPlayerState_Idle;
		}
	}
	//�����Ă���Ƃ�
	else
	{
		//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)
		if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
		{
			//�X�e�[�g���\�������ɂ���B
			m_playerState = enPlayerState_Standwalk;
		}
		else
		{
			//�X�^���o�C�ɂ���
			m_playerState = enPlayerState_Standby;
		}
	}
}

void Player::PlayAnimation()
{
	switch (m_playerState) 
	{
	//�v���C���[�X�e�[�g���ҋ@��������
	case enPlayerState_Idle:
		//�ҋ@�A�j���[�V�������Đ�
		m_playerModel.PlayAnimation(enAnimClip_Idle, 0.2f);
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

	default:
		break;
	}
}

void Player::Render(RenderContext& rc)
{
	m_playerModel.Draw(rc);
}