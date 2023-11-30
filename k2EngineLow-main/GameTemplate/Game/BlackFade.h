#pragma once
class BlackFade : public IGameObject
{
public:
	~BlackFade();
	bool Start();
	void Update();
	void CalcAlpha();
	void Render(RenderContext& rc);

	/// <summary>
	/// ���l�𑝉�������
	/// </summary>
	/// <param name="now"></param>
	void SetAlphaUp(bool now)
	{
		m_alphaUp = now;
	}

	/// <summary>
	/// ���l������������
	/// </summary>
	/// <param name="now"></param>
	void SetAlphaDown(bool now)
	{
		m_alphaDown = now;
	}

	/// <summary>
	/// ���l��Ԃ�
	/// </summary>
	/// <returns></returns>
	const float GetBlackAlpha() const
	{
		return m_blackAlpha;
	}

private:
	SpriteRender m_blackSprite;
	float m_blackAlpha = 0.0f;
	bool m_alphaUp = false;
	bool m_alphaDown = false;
};

