#pragma once
class Fade : public IGameObject
{
public:
	~Fade();
	bool Start();
	void Update();
	void CalcRotation();
	void CalcFadeAlpha();
	void MakeGame();
	void Render(RenderContext& rc);

	void DeleteThis()
	{
		DeleteGO(this);
	}	

	void SetAlphaDown()
	{
		m_alphaUp = false;
	}

private:
	SpriteRender m_fadeSprite;				//�t�F�[�h�p�摜
	SpriteRender m_loadRotSprite;			//����Ă���摜

	float m_fadeSpriteAlpha = 0.0f;			//�t�F�[�h��ʂ̃�
	float m_rotDegree = 0.0f;				//���[�h�̉�]��
	bool m_didMakeGame = false;				//�Q�[���N���X������Ă��邩�ǂ���
	bool m_alphaUp = true;					//���l���グ�邩�ǂ���

	Quaternion m_loadRot;					//�ŏI�I�ȉ�]��
};

