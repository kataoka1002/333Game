#include "stdafx.h"
#include "ResultCamera.h"
#include "Result.h"

ResultCamera::ResultCamera()
{
	
}

ResultCamera::~ResultCamera()
{

}

bool ResultCamera::Start()
{
	return true;
}

void ResultCamera::SetTarget(const Vector3& targetPos)
{
	//�^�[�Q�b�g�̓��f���̉E��
	target = targetPos;
	target.x += 15.0f;
	target.y += 42.0f;

	//�����_��ݒ肷��
	g_camera3D->SetTarget(target);

	//�����_���王�_�܂ł̃x�N�g�������߂�
	Vector3 m_toCameraPos = { -15.0f,0.0f,30.0f };

	//���_���v�Z����B
	Vector3 pos = target + m_toCameraPos;

	//���_��ݒ肷��B
	g_camera3D->SetPosition(pos);
}

void ResultCamera::Update()
{

}

