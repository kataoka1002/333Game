#pragma once
#include "ObjectBase.h"
class PlantLowRender;

class PlantLow : public ObjectBase
{
public:

	PlantLow();
	~PlantLow();

	void Update();	//�X�V����

	void Render(RenderContext& rc)	override;	//�`�揈��
	void InitModel()				override;	//���f���̏�����
	void OnDestroy()				override;	//������Ƃ��ɌĂ΂�鏈��
	void InitCollision()			override;	//�R���W�����̏�����

private:
	PlantLowRender* m_plantLowRender = nullptr;
};

