#pragma once
#include "ObjectBase.h"
class AirRender;

class Air : public ObjectBase
{
public:

	Air();
	~Air();

	void Update();	//�X�V����

	void Render(RenderContext& rc)	override;	//�`�揈��
	void InitModel()				override;	//���f���̏�����
	void OnDestroy()				override;	//������Ƃ��ɌĂ΂�鏈��
	void InitCollision()			override;	//�R���W�����̏�����

private:
	AirRender* m_airRender = nullptr;
};

