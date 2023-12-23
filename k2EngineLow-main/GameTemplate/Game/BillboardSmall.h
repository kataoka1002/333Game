#pragma once
#include "ObjectBase.h"
class BillboardSmallRender;

class BillboardSmall : public ObjectBase
{
public:

	BillboardSmall();
	~BillboardSmall();

	void Update();	//�X�V����

	void Render(RenderContext& rc)	override;	//�`�揈��
	void InitModel()				override;	//���f���̏�����
	void OnDestroy()				override;	//������Ƃ��ɌĂ΂�鏈��
	void InitCollision()			override;	//�R���W�����̏�����

private:
	BillboardSmallRender* m_billboardSmallRender = nullptr;
};

