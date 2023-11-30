#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include "Fade.h"

namespace
{
	const Vector3 PRESS_A_BUTTON_POSITION = { 0.0f,-300.0f,0.0f };

	const Vector4 GAMESTART_OPTION_COLOR = { 1.0f,1.0f,1.0f,1.0f };

	const Vector3 GAMESTART_OPTION_POSITION = Vector3::Zero;

	const Vector4 TRIANGLE_BEFORE_COLOR = { 1.0f,1.0f,1.0f,0.0f };

	const Vector4 TRIANGLE_AFTER_COLOR = { 1.0f,1.0f,1.0f,1.0f };

	const Vector3 BGM_SPRITE_POSITION = { -20.0f,-240.0f,0.0f };

	const Vector3 SE_SPRITE_POSITION = { -20.0f,-318.0f,0.0f };

	const Vector3 BGM_FRAME_POSITION = { -30.0f,-240.0f,0.0f };

	const Vector3 SE_FRAME_POSITION = { -30.0f,-318.0f,0.0f };

	const Vector2 BGM_SPRITE_PIVOT = { 0.0f,0.5f };

	const Vector2 SE_SPRITE_PIVOT = { 0.0f,0.5f };

	const float BLINKING_SPEED = 0.1f;

	const float TRIANGLE_MOVE_SPEED = 1.0f;

	const float TRIANGLE_OFFSET = -13.0f;

	const float FADE_SPEED = 0.02f;

	const float PI = 3.141592;
}

Title::~Title()
{

}

bool Title::Start()
{
	//�w�i�̏�����
	m_titleSprite.Init("Assets/spriteData/title/title.dds", 1600.0f, 900.0f);

	//�\������Ă��镶���摜�̏�����
	m_fontSprite.Init("Assets/spriteData/title/PressAButton.dds", 1600.0f, 900.0f);
	m_fontSprite.SetPosition(PRESS_A_BUTTON_POSITION);
	m_fontSprite.Update();

	//���̏�����
	m_triangleSprite.Init("Assets/spriteData/title/GameStartSankaku.dds", 1600.0f, 900.0f);
	m_triangleSprite.SetMulColor(TRIANGLE_BEFORE_COLOR);
	m_triangleSprite.Update();

	//BGM�n�̏�����
	m_BGMGageSprite.Init("Assets/spriteData/title/SE_BGM_Gage.dds", 319.0f, 36.0f);
	m_BGMGageSprite.SetPosition(BGM_SPRITE_POSITION);
	m_BGMGageSprite.SetPivot(BGM_SPRITE_PIVOT);
	m_BGMGageSprite.Update();
	m_BGMFrameSprite.Init("Assets/spriteData/title/SE_BGM_Frame.dds", 339.0f, 56.0f);
	m_BGMFrameSprite.SetPosition(BGM_FRAME_POSITION);
	m_BGMFrameSprite.SetPivot(BGM_SPRITE_PIVOT);
	m_BGMFrameSprite.Update();
	m_BGMBackSprite.Init("Assets/spriteData/title/BGMSE_Back.dds", 319.0f, 36.0f);
	m_BGMBackSprite.SetPosition(BGM_SPRITE_POSITION);
	m_BGMBackSprite.SetPivot(BGM_SPRITE_PIVOT);
	m_BGMBackSprite.Update();

	//SE�n�̏�����
	m_SEGageSprite.Init("Assets/spriteData/title/SE_BGM_Gage.dds", 319.0f, 36.0f);
	m_SEGageSprite.SetPosition(SE_SPRITE_POSITION);
	m_SEGageSprite.SetPivot(SE_SPRITE_PIVOT);
	m_SEGageSprite.Update();
	m_SEFrameSprite.Init("Assets/spriteData/title/SE_BGM_Frame.dds", 339.0f, 56.0f);
	m_SEFrameSprite.SetPosition(SE_FRAME_POSITION);
	m_SEFrameSprite.SetPivot(SE_SPRITE_PIVOT);
	m_SEFrameSprite.Update();
	m_SEBackSprite.Init("Assets/spriteData/title/BGMSE_Back.dds", 319.0f, 36.0f);
	m_SEBackSprite.SetPosition(SE_SPRITE_POSITION);
	m_SEBackSprite.SetPivot(SE_SPRITE_PIVOT);
	m_SEBackSprite.Update();

	return true;
}

void Title::Update()
{
	//���ꂼ��̃X�e�[�g���̏���
	ManageState();

	//�O�p�𓮂���
	MoveTriangle();

	//BGM.SE�̑傫���̐ݒ�
	m_BGMGageSprite.SetScale({ m_BGMScale, 1.0f, 1.0f });
	m_BGMGageSprite.Update();
	m_SEGageSprite.SetScale({ m_SEScale, 1.0f, 1.0f });
	m_SEGageSprite.Update();
}

void Title::ManageState()
{
	switch (m_titleState)
	{
	case enTitleState_PressAButton:

		//�����̓_�ł̏���
		PressAButtonFade();

		//A�{�^���ŃX�e�[�g�ύX
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//�Q�[���X�^�[�g�I�v�V������ʂ�
			ToGameStartOption();

			//�����Q�[���X�^�[�g��
			TriangleChange("Assets/spriteData/title/GameStartSankaku.dds", enTitleState_GameStart);
		}

		break;

	case enTitleState_GameStart:

		//A�{�^���ŃQ�[���X�^�[�g
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//�t�F�[�h�X�e�[�g�ɕύX
			m_titleState = enTitleState_Fade;
		}

		//���{�^���ŃX�e�[�g�ύX
		if (g_pad[0]->IsTrigger(enButtonDown))
		{
			//�����I�v�V������
			TriangleChange("Assets/spriteData/title/OptionSankaku.dds", enTitleState_Option);
		}

		break;

	case enTitleState_Option:

		//��{�^���ŃX�e�[�g�ύX
		if (g_pad[0]->IsTrigger(enButtonUp))
		{
			//�����Q�[���X�^�[�g��
			TriangleChange("Assets/spriteData/title/GameStartSankaku.dds", enTitleState_GameStart);
		}

		// A�{�^���ŃX�e�[�g�ύX
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//BGM,SE�ݒ��ʂ�
			ToBGMSE();

			//����BGM��
			TriangleChange("Assets/spriteData/title/BGMSankaku.dds", enTitleState_BGM);
		}

		break;

	case enTitleState_BGM:

		//���{�^���ŃX�e�[�g�ύX
		if (g_pad[0]->IsTrigger(enButtonDown))
		{
			//����SE��
			TriangleChange("Assets/spriteData/title/SESankaku.dds", enTitleState_SE);
		}

		//B�{�^���ŃX�e�[�g�ύX
		if (g_pad[0]->IsTrigger(enButtonB))
		{
			//�Q�[���X�^�[�g�I�v�V������ʂ�
			ToGameStartOption();

			//�����I�v�V������
			TriangleChange("Assets/spriteData/title/OptionSankaku.dds", enTitleState_Option);
		}

		//BGM�̑傫���ݒ�
		ChangeBGMSEScale(m_BGMScale);

		break;

	case enTitleState_SE:

		//��{�^���ŃX�e�[�g�ύX
		if (g_pad[0]->IsTrigger(enButtonUp))
		{
			TriangleChange("Assets/spriteData/title/BGMSankaku.dds", enTitleState_BGM);
		}

		//B�{�^���ŃX�e�[�g�ύX
		if (g_pad[0]->IsTrigger(enButtonB))
		{
			//�Q�[���X�^�[�g�I�v�V������ʂ�
			ToGameStartOption();

			//�����I�v�V������
			TriangleChange("Assets/spriteData/title/OptionSankaku.dds", enTitleState_Option);
		}

		//SE�̑傫���ݒ�
		ChangeBGMSEScale(m_SEScale);

		break;

	case enTitleState_Fade:
		//�t�F�[�h���s��
		CalcFade();
		break;

	default:
		break;
	}
}

void Title::ToGameStartOption()
{
	//�Q�[���X�^�[�g�I�v�V������ʂɕύX
	m_fontSprite.Init("Assets/spriteData/title/GameStart_Option.dds", 1600.0f, 900.0f);
	m_fontSprite.SetMulColor(GAMESTART_OPTION_COLOR);
	m_fontSprite.SetPosition(GAMESTART_OPTION_POSITION);
	m_fontSprite.Update();

	//�O�p�̓����x���グ��
	m_triangleSprite.SetMulColor(TRIANGLE_AFTER_COLOR);
	m_triangleSprite.Update();
}

void Title::ToBGMSE()
{
	//SE,BGM��ʂɕύX
	m_fontSprite.Init("Assets/spriteData/title/SE_BGM.dds", 1600.0f, 900.0f);
	m_fontSprite.Update();

	//�I�v�V�����X�e�[�g�ɕύX
	m_titleState = enTitleState_BGM;
}

void Title::TriangleChange(const char* name, enTitleState state)
{
	//�O�p���ړ�
	m_triangleSprite.Init(name, 1600.0f, 900.0f);
	m_triangleSprite.Update();

	//�X�e�[�g�ύX
	m_titleState = state;
}

void Title::ChangeBGMSEScale(float& scale)
{
	if (g_pad[0]->IsPress(enButtonRight))
	{
		scale += 0.01;
	}
	if (g_pad[0]->IsPress(enButtonLeft))
	{
		scale -= 0.01;
	}

	//�傫����0�`1
	scale = max(0.0f, scale);
	scale = min(1.0f, scale);
}

float Title::CalcSinValue(float speed)
{
	//sin�g���g���Ċ��炩�ɏ㉺������
	//�p�x��傫�����Ă���
	m_degree += speed;

	//360�x��-1�`1���������̂�0�x�ɖ߂�
	if (m_degree >= 180.0f)
	{
		m_degree = 0.0f;
	}

	//�p�x�����W�A���ɕϊ�
	float rad = m_degree * PI / 180.0f;

	//sin�̒l������,����𓧖��x�ɂ���
	float sinValue = sin(rad);
	return sinValue;
}

void Title::PressAButtonFade()
{
	//��������o���肷��
	float fontAlpha = CalcSinValue(BLINKING_SPEED);

	m_fontSprite.SetMulColor({ 1.0f,1.0f,1.0f,fontAlpha });
	m_fontSprite.Update();
}

void Title::MoveTriangle()
{
	//�������ɓ�����
	m_trianglePosX = CalcSinValue(TRIANGLE_MOVE_SPEED) * TRIANGLE_OFFSET;

	m_triangleSprite.SetPosition({ m_trianglePosX ,0.0f,0.0f });
	m_triangleSprite.Update();
}

void Title::CalcFade()
{
	//�t�F�[�h���쐬����Ă��Ȃ�������
	if (m_makeFade == false)
	{
		//�t�F�[�h�̍쐬
		NewGO<Fade>(0, "fade");
		m_makeFade = true;
	}
}

void Title::MakeGame()
{
	//�Q�[�����쐬
	NewGO<Game>(0, "game");

	//�������g�̍폜
	DeleteGO(this);
}

void Title::Render(RenderContext& rc)
{
	m_titleSprite.Draw(rc);
	m_fontSprite.Draw(rc);
	m_triangleSprite.Draw(rc);

	//BGM,SE�̎��̂ݕ\��
	if (m_titleState == enTitleState_BGM || m_titleState == enTitleState_SE)
	{
		m_BGMBackSprite.Draw(rc);
		m_BGMGageSprite.Draw(rc);
		m_BGMFrameSprite.Draw(rc);
		m_SEBackSprite.Draw(rc);
		m_SEGageSprite.Draw(rc);
		m_SEFrameSprite.Draw(rc);
	}
}