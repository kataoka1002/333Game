#include "stdafx.h"
#include "Player.h"

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
		100.0f,			//����
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

	//�A�j���[�V�����̍Đ�
	PlayAnimation();

	//�X�e�[�g�Ǘ�
	ManageState();

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

	//atan2��tan�Ƃ̒l���p�x(���W�A���P��)�ɕϊ����Ă����֐�
	//��]�p�x�����߂Ă���
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	m_rotation.SetRotationY(-angle);

	//��]��ݒ肷��
	m_playerModel.SetRotation(m_rotation);
}

void Player::PlayAnimation()
{
	switch (m_playerState) 
	{
	//�v���C���[�X�e�[�g��0(�ҋ@)��������
	case enPlayerState_Idle:
		//�ҋ@�A�j���[�V�������Đ�
		m_playerModel.PlayAnimation(enAnimClip_Idle, 0.15f);
		break;

	//�v���C���[�X�e�[�g����������������
	case enPlayerState_Walk:
		//�����A�j���[�V�������Đ�
		m_playerModel.PlayAnimation(enAnimClip_Walk, 0.15f);
		break;

	default:
		break;
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

	//�n�ʂɕt���Ă�����
	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)
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

void Player::Render(RenderContext& rc)
{
	m_playerModel.Draw(rc);
}