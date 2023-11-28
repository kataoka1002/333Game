#include "stdafx.h"
#include "TargetUI.h"

namespace
{
	//UI��\������ꏊ�̕ω���
	const float UI_ADD_POS_Y = 30.0f;

	//��]���x
	const float ROTATION_SPEED = 5.0f;

	//�O�p�̑傫��
	const float TRIANGLE_SCALE = 0.15f;

	//�ۂ̑傫��
	const float CIRCLE_SCALE = 0.3f;
}

TargetUI::~TargetUI()
{

}

bool TargetUI::Start()
{
	//�X�v���C�g�̏�����
	m_yellowSprite.Init("Assets/spriteData/UI/object/targetYellow.dds", 293.0f, 259.0f);
	m_yellowSprite.SetPivot({ 0.5f,0.35f });
	m_yellowSprite.SetScale(TRIANGLE_SCALE);
	m_yellowSprite.Update();

	m_blueSprite.Init("Assets/spriteData/UI/object/targetBlue.dds", 293.0f, 259.0f);
	m_blueSprite.SetPivot({ 0.5f,0.35f });
	m_blueSprite.SetScale(TRIANGLE_SCALE);
	m_blueSprite.Update(); 

	m_circleSprite.Init("Assets/spriteData/UI/object/targetCircle.dds", 20.0f, 21.0f);
	m_circleSprite.SetScale(CIRCLE_SCALE);
	m_circleSprite.Update();

	return true;
}

void TargetUI::Update()
{
	if (m_didPositionUP == false)
	{
		//�I�u�W�F�N�g�̃|�W�V�����̏������ڕW�ɂ���
		m_objectPos.y += UI_ADD_POS_Y;
		m_didPositionUP = true;
	}

	//���[���h���W����X�N���[�����W���v�Z(�v�Z���ʂ�position�ɑ�������)
	g_camera3D->CalcScreenPositionFromWorldPosition(m_targetUIPosition, m_objectPos);

	//��]����
	m_degree += ROTATION_SPEED;
	if (m_degree >= 360.0f)
	{
		m_degree = 0.0f;
	}

	//�O�p�̉摜�ɉ�]�ʂ�������
	m_rotYellow.SetRotationDegZ(m_degree);
	m_rotBlue.SetRotationDegZ(-m_degree);

	//�X�V����
	m_yellowSprite.SetRotation(m_rotYellow);
	m_yellowSprite.SetPosition({ m_targetUIPosition.x, m_targetUIPosition.y,0.0f });
	m_yellowSprite.Update();

	m_blueSprite.SetRotation(m_rotBlue);
	m_blueSprite.SetPosition({ m_targetUIPosition.x, m_targetUIPosition.y,0.0f });
	m_blueSprite.Update();

	m_circleSprite.SetPosition({ m_targetUIPosition.x, m_targetUIPosition.y,0.0f });
	m_circleSprite.Update();
}

void TargetUI::Render(RenderContext& rc)
{
	m_yellowSprite.Draw(rc);
	m_blueSprite.Draw(rc);
	m_circleSprite.Draw(rc);
}