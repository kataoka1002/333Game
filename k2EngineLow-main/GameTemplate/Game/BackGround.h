#pragma once

class BackGround : public IGameObject
{
public:
	BackGround();
	~BackGround();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	ModelRender m_bgModel;
	PhysicsStaticObject m_phyStaticObject;	//�ÓI�����I�u�W�F�N�g
};