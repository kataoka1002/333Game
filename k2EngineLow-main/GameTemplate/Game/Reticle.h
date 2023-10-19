#pragma once
class Player;

class Reticle : public IGameObject
{
public:
	Reticle();
	~Reticle();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void CalcScale();

private:
	SpriteRender m_dotReticle;			//�ۂ̃��e�B�N��
	SpriteRender m_sideReticle;			//����̃��e�B�N��

	Vector3 m_dotReticleScale = Vector3::Zero;
	Vector3 m_sideReticleScale = Vector3::Zero;

	Player* m_player = nullptr;
};

