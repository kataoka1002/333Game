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
	/// α値を増加させる
	/// </summary>
	/// <param name="now"></param>
	void SetAlphaUp(bool now)
	{
		m_alphaUp = now;
	}

	/// <summary>
	/// α値を減少させる
	/// </summary>
	/// <param name="now"></param>
	void SetAlphaDown(bool now)
	{
		m_alphaDown = now;
	}

	/// <summary>
	/// α値を返す
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

