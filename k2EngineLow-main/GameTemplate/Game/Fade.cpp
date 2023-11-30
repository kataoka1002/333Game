#include "stdafx.h"
#include "Fade.h"
#include "Game.h"
#include "Title.h"

namespace
{
	const float FADE_UP_SPEED = 0.02f;

	const float FADE_DOWN_SPEED = 0.1f;

	const float ROTATION_SPEED = 3.0f;

	const Vector3 LOAD_SPRITE_POSITION = { -640.0f,-320.0f,0.0f };

	const float LOAD_SPRITE_SCALE = 0.7f;
}

Fade::~Fade()
{

}

bool Fade::Start()
{
	//�t�F�[�h�摜�̏�����
	m_fadeSprite.Init("Assets/spriteData/title/NowLoading.dds", 1600.0f, 900.0f);
	m_fadeSprite.SetMulColor({ 1.0f,1.0f,1.0f,m_fadeSpriteAlpha });
	m_fadeSprite.Update();

	//��]���Ă���摜�̏�����
	m_loadRotSprite.Init("Assets/spriteData/title/loadRot.dds", 94.0f, 108.0f);
	m_loadRotSprite.SetPosition(LOAD_SPRITE_POSITION);
	m_loadRotSprite.SetScale(LOAD_SPRITE_SCALE);
	m_loadRotSprite.Update();

	return true;
}

void Fade::Update()
{
	//���l�̌v�Z
	CalcFadeAlpha();

	//��]����
	CalcRotation();

	//�Q�[���N���X�̍쐬
	MakeGame();

	//�X�V
	m_loadRotSprite.Update();
	m_fadeSprite.Update();
}

void Fade::CalcRotation()
{
	//��]����
	m_rotDegree += ROTATION_SPEED;
	if (m_rotDegree >= 360.0f)
	{
		m_rotDegree = 0.0f;
	}

	//�O�p�̉摜�ɉ�]�ʂ�������
	m_loadRot.SetRotationDegZ(m_rotDegree);

	//�Z�b�g
	m_loadRotSprite.SetRotation(m_loadRot);
}

void Fade::CalcFadeAlpha()
{
	if (m_alphaUp == true)
	{
		//���l���グ�Ă���
		m_fadeSpriteAlpha += FADE_UP_SPEED;
		m_fadeSpriteAlpha = min(1.0f, m_fadeSpriteAlpha);
	}
	else
	{
		//���l�������Ă���
		m_fadeSpriteAlpha -= FADE_DOWN_SPEED;
		m_fadeSpriteAlpha = max(0.0f, m_fadeSpriteAlpha);

		//���l��0�ȉ���
		if (m_fadeSpriteAlpha <= 0.0f)
		{
			//���g�̍폜
			DeleteGO(this);
		}
	}

	m_fadeSprite.SetMulColor({ 1.0f,1.0f,1.0f,m_fadeSpriteAlpha });
	m_loadRotSprite.SetMulColor({ 1.0f,1.0f,1.0f,m_fadeSpriteAlpha });
}

void Fade::MakeGame()
{
	//���l��1�ȏ�ɂȂ�����Q�[�����쐬����
	if (m_fadeSpriteAlpha >= 1.0f && m_didMakeGame == false)
	{
		Title* title = FindGO<Title>("title");
		title->MakeGame();

		m_didMakeGame = true;
	}
}

void Fade::Render(RenderContext& rc)
{
	m_fadeSprite.Draw(rc);
	m_loadRotSprite.Draw(rc);
}