#pragma once

class Title : public IGameObject
{
public:
	enum enTitleState
	{
		enTitleState_OpenFade,
		enTitleState_PressAButton,
		enTitleState_GameStart,
		enTitleState_Option,
		enTitleState_BGM,
		enTitleState_SE,
		enTitleState_Fade,
		enTitleState_Num
	};

public:
	~Title();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void ManageState();
	float CalcSinValue(float speed);
	void PressAButtonFade();
	void ToGameStartOption();
	void ToBGMSE();
	void TriangleChange(const char* name, enTitleState state);
	void ChangeBGMSEScale(float& scale);
	void MoveTriangle();
	void CalcFade();
	void MakeGame();

private:
	enTitleState m_titleState = enTitleState_OpenFade;		//�X�e�[�g
	SpriteRender m_openFadeSprite;
	SpriteRender m_titleSprite;								//�^�C�g���̉摜
	SpriteRender m_fontSprite;								//�����̉摜
	SpriteRender m_triangleSprite;							//���̉摜
	SpriteRender m_BGMGageSprite;							//BGM�̃Q�[�W
	SpriteRender m_BGMFrameSprite;							//BGM�̃t���[��
	SpriteRender m_BGMBackSprite;							//BGM�̔w�i
	SpriteRender m_SEGageSprite;							//SE�̃Q�[�W
	SpriteRender m_SEFrameSprite;							//SE�̃t���[��
	SpriteRender m_SEBackSprite;							//SE�̔w�i

	float m_degree = 0.0f;									//�����x�̕ύX�p�p�x
	float m_BGMScale = 1.0f;								//BGM�̑傫��
	float m_SEScale = 1.0f;									//SE�̑傫��
	float m_trianglePosX = 0.0f;							//����X���W
	float m_openAlpha = 1.0f;								//�ŏ��̉摜�̃��l
	bool m_makeFade = false;								//�t�F�[�h������Ă��邩�ǂ���

	SoundSource* m_titleBGM = nullptr;						//�^�C�g����BGM
};

