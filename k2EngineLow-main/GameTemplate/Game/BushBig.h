#pragma once
#include "ObjectBase.h"

class BushBig : public ObjectBase
{
public:

	BushBig();
	~BushBig();

	void Update();	//�X�V����

	void Render(RenderContext& rc)	override;	//�`�揈��
	void InitModel()				override;	//���f���̏�����
	void OnDestroy()				override;	//������Ƃ��ɌĂ΂�鏈��
	void InitCollision()			override;	//�R���W�����̏�����

private:

};

