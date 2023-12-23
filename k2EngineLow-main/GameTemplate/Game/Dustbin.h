#pragma once
#include "ObjectBase.h"
class DustbinRender;

class Dustbin : public ObjectBase
{
public:

	Dustbin();
	~Dustbin();

	void Update();	//�X�V����

	void Render(RenderContext& rc)	override;	//�`�揈��
	void InitModel()				override;	//���f���̏�����
	void OnDestroy()				override;	//������Ƃ��ɌĂ΂�鏈��
	void InitCollision()			override;	//�R���W�����̏�����

private:
	DustbinRender* m_dustbinRender = nullptr;
};

