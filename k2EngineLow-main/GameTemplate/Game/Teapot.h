#pragma once
#include "ObjectBase.h"

class Teapot : public ObjectBase
{
public:

	Teapot();
	~Teapot();

	void Update();	//�X�V����

	void Render(RenderContext& rc)	override;	//�`�揈��
	void InitModel()				override;	//���f���̏�����

private:

};

