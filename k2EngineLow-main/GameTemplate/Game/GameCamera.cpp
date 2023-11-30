#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"
#include "GameInformation.h"
#include "Boss.h"
#include "BlackFade.h"

namespace
{
	//�����_���王�_�܂ł̃x�N�g��
	const Vector3 TO_CAMERA_POSITION = { 0.0f, 150.0f, -300.0f };
	const Vector3 TO_CAMERA_START_POSITION = { 0.0f, 150.0f, 300.0f };

	//�����_���王�_�܂ł̃x�N�g��(�ߋ�����)
	const Vector3 TO_CAMERA_POSITION_NEAR = { 0.0f,100.0f,-270.0f };

	//�����_�̃��[�J���|�W�V����
	const Vector3 TARGET_LOCAL_POSITION = { 0.0f,180.0f,0.0f };

	//�����_�̃��[�J���|�W�V����(�ߋ�����)
	const Vector3 TARGET_LOCAL_POSITION_NEAR = { 60.0f,150.0f,0.0f };

	//�J�����̍ő�ړ����x
	const float MAX_CAMERA_SPEED = 5000.0f;

	//�J�����̓����蔻��̑傫��
	const float CAMERA_COLLISION_SCALE = 5.0f;

	//�J�����̈ړ����x
	const float CAMERA_MOVE_POWER_X = 1.3f;
	const float CAMERA_MOVE_POWER_Y = 1.0f;

	//������
	const float FAR_PLANE = 35000.0f;

	//�{�X�܂ł̃x�N�g��
	const Vector3 TO_CAMERA_POSITION_BOSS = { 380.0f, -30.0f, 140.0f };

	//�I�[�v�j���O���̃x�N�g��
	const Vector3 TO_CAMERA_POSITION_OP = { 0.0f, 0.0f, 100.0f };
}

GameCamera::GameCamera()
{
	
}

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	//�v���C���[�̃C���X�^���X��T���B
	m_player = FindGO<nsPlayer::Player>("player");

	//�C���t�H���[�V�����̃C���X�^���X��T���B
	m_gameInfo = FindGO<GameInformation>("gameinformation");

	//�{�X�̃C���X�^���X��T��
	m_boss = FindGO<nsBoss::Boss>("boss");

	//�u���b�N�t�F�[�h��T��
	m_blackFade = FindGO<BlackFade>("blackfade");

	//�J�����܂ł̋����ɃZ�b�g
	m_toCameraPos.Set(TO_CAMERA_START_POSITION);

	//�����ʂ̐ݒ�
	g_camera3D->SetFar(FAR_PLANE);

	//�΂˃J�����̏������B
	m_springCamera.Init(
		*g_camera3D,			//�΂˃J�����̏������s���J�������w�肷��B
		MAX_CAMERA_SPEED,		//�J�����̈ړ����x�̍ő�l�B
		true,					//�J�����ƒn�`�Ƃ̂����蔻�����邩�ǂ����̃t���O�Btrue���Ƃ����蔻����s���B
		CAMERA_COLLISION_SCALE	//�J�����ɐݒ肳��鋅�̃R���W�����̔��a�B��R������true�̎��ɗL���ɂȂ�B
	);

	//�ŏ��̃^�[�Q�b�g
	m_opTarget = { -700.0f,150.0f,100.0f };

	return true;
}

void GameCamera::OpeningProcess()
{
	//�I�[�v�j���O������Ȃ��Ȃ�Ԃ�
	if (m_gameInfo->GetIsInOpening() == false)
	{
		return;
	}

	//���Ԍv��
	m_opTime += g_gameTime->GetFrameDeltaTime();

	Vector3 pos;
	if (m_opState == enOP_Camera1)
	{
		//�ꏊ�@�ŃJ�������ړ�
		//�����_�����߂�
		m_opTarget.x += 5.0f;

		if (m_opTime >= 1.5f)
		{
			//�t�F�[�h�C���̊J�n
			m_blackFade->SetAlphaUp(true);
		}

		if (m_blackFade->GetBlackAlpha() >= 1.0f)
		{
			//�J�����ꏊ�A�Ɉړ�
			m_opTarget = { 240.0f,150.0f,1700.0f };
			m_springCamera.Refresh();
			//�t�F�[�h�C���̏I��
			m_blackFade->SetAlphaUp(false);
			//�t�F�[�h�A�E�g�̊J�n
			m_blackFade->SetAlphaDown(true);
			//�^�C�}�[�̃��Z�b�g
			m_opTime = 0.0f;
			
			//���̃X�e�[�g�ֈړ�
			m_opState = enOP_Camera2;
		}

		//���_���v�Z����B
		pos = m_opTarget + TO_CAMERA_POSITION_OP;
	}
	else if (m_opState == enOP_Camera2)
	{
		if (m_blackFade->GetBlackAlpha() <= 0.0f && m_camera1End == false)
		{
			//�t�F�[�h�A�E�g�̏I��
			m_blackFade->SetAlphaDown(false);

			m_camera1End = true;
		}

		//�����_�����߂�
		m_opTarget.x -= 5.0f;

		if (m_opTime >= 1.5f)
		{
			//�t�F�[�h�C���̊J�n
			m_blackFade->SetAlphaUp(true);
		}

		if (m_blackFade->GetBlackAlpha() >= 1.0f)
		{
			//�J�����ꏊ3�Ɉړ�
			m_opTarget = CalcTargetPosition();
			m_springCamera.Refresh();
			//�t�F�[�h�C���̏I��
			m_blackFade->SetAlphaUp(false);
			//�t�F�[�h�A�E�g�̊J�n
			m_blackFade->SetAlphaDown(true);
			//�^�C�}�[�̃��Z�b�g
			m_opTime = 0.0f;

			//���̃X�e�[�g�ֈړ�
			m_opState = enOP_Player;
		}

		//���_���v�Z����B
		pos = m_opTarget + TO_CAMERA_POSITION_OP;
	}
	else if (m_opState == enOP_Player)
	{
		//���_���v�Z����B
		pos = m_opTarget + TO_CAMERA_START_POSITION;

		if (m_blackFade->GetBlackAlpha() <= 0.0f)
		{
			//�t�F�[�h�A�E�g�̏I��
			m_blackFade->SetAlphaDown(false);

			//�I�[�v�j���O�̏I��
			m_gameInfo->SetInOpening(false);

			//�Q�[���̃X�^�[�g
			m_gameInfo->SetInGame(true);
		}
	}

	m_springCamera.SetPosition(pos);
	m_springCamera.SetTarget(m_opTarget);
}

void GameCamera::UpdatePositionAndTarget()
{
	//�Q�[��������Ȃ��Ȃ�Ԃ�
	if (m_gameInfo->GetIsInGame() == false)
	{
		return;
	}

	//�����_�����߂�
	Vector3 target = CalcTargetPosition();

	Vector3 toCameraPosOld = m_toCameraPos;

	//�p�b�h�̓��͂��g���ăJ�������񂷁B
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	//Y������̉�]
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, x * CAMERA_MOVE_POWER_X);
	qRot.Apply(m_toCameraPos);

	//X������̉�]�B
	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, y * CAMERA_MOVE_POWER_Y);
	qRot.Apply(m_toCameraPos);

	//�J�����̉�]�̏�����`�F�b�N����B
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();

	//�J���������������
	if (toPosDir.y < -0.5f)
	{
		m_toCameraPos = toCameraPosOld;
	}
	//�J����������������
	else if (toPosDir.y > 0.5f)
	{
		m_toCameraPos = toCameraPosOld;
	}

	//���_���v�Z����B
	Vector3 pos = target + m_toCameraPos;

	//�o�l�J�����ɒ����_�Ǝ��_��ݒ肷��B
	m_springCamera.SetPosition(pos);
	m_springCamera.SetTarget(target);
}

void GameCamera::SetNearCamera(bool isNear)
{
	//�J�����̉�]��n��
	Matrix rot = m_springCamera.GetCameraRotation();

	Vector3 toCameraPos;
	
	//�v���C���[���X�^���o�C���Ȃ�
	if (isNear == true)
	{
		//�ߋ����J����ON
		m_isNearCamera = true;

		//�����_���王�_�܂ł̋�����ݒ�
		toCameraPos = TO_CAMERA_POSITION_NEAR;

		//�J�����̉�]�s��̏C��
		rot.m[0][1] = 1.0f;
		rot.m[1][1] = 0.0f;
		rot.m[2][1] = 0.0f;
		rot.m[3][1] = 0.0f;
	}
	else
	{
		//�ߋ����J����OFF
		m_isNearCamera = false;

		//�����_���王�_�܂ł̋�����ݒ�
		toCameraPos = TO_CAMERA_POSITION;
	}

	//�J�����̉�]�ɍ��킹�����[�J���|�W�V�������v�Z
	Vector3 localPos = toCameraPos;
	rot.Apply3x3(localPos);

	//�J�����܂ł̋����ɃZ�b�g
	m_toCameraPos.Set(localPos);
}

Vector3 GameCamera::CalcTargetPosition()
{
	//�����_���v�Z����B
	Vector3 target = m_player->GetPosition();

	//�ߋ����J�����̂Ƃ��Ȃ�
	if (m_isNearCamera)
	{
		//�v���C���[�̉�]��n��
		Quaternion rot = m_player->GetRotation();

		//�v���C���[�̉�]�ɍ��킹�����[�J���|�W�V�������v�Z
		Vector3 localPos = TARGET_LOCAL_POSITION_NEAR;
		rot.Multiply(localPos);

		//�v���C���[�̃|�W�V�����Ɍv�Z�������[�J���|�W�V�����𑫂�
		target += localPos;
	}
	else
	{
		//�v���C���[�̑������炿����Ə�𒍎��_�Ƃ���B
		target += TARGET_LOCAL_POSITION;
	}

	//�����O�𒍎��_�ɂ���
	target += g_camera3D->GetForward() * 20.0f;	

	return target;
}

void GameCamera::BossMovieProcess()
{
	//���[�r�[������Ȃ��Ȃ�Ԃ�
	if (m_gameInfo->GetIsInMovie() == false)
	{
		return;
	}

	//�����_�����߂�
	Vector3 target = m_boss->GetPosition();
	target.y = 180.0f;
	target.z -= 20.0f;

	//���_���v�Z����B
	Vector3 pos = target + TO_CAMERA_POSITION_BOSS;
	
	//�o�l�J�����ɒ����_�Ǝ��_��ݒ肷��B
	m_springCamera.SetPosition(pos);
	m_springCamera.SetTarget(target);
	
	//��莞�Ԍo�߂Ń{�X�o��
	m_bossMakeTime -= g_gameTime->GetFrameDeltaTime();
	if (m_bossMakeTime <= 0.0f)
	{
		m_gameInfo->StartBossBattle();
		m_bossMakeTime = 0.0f;
	}
}

void GameCamera::Update()
{
	//�I�[�v�j���O���̏���
	OpeningProcess();

	//�Q�[�����̃J�����̎��_�ƒ����_�̐ݒ�
	UpdatePositionAndTarget();

	//�{�X���[�r�[���̏���
	BossMovieProcess();

	//�J�����̍X�V�B
	m_springCamera.Update();
}

