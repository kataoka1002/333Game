#pragma once

/// <summary>
/// �G�t�F�N�g�̖��O
/// </summary>
enum EffectName
{
	enEffectName_ObjectPush,
	enEffectName_ObjectSmoke,
	enEffectName_ObjectDrop,
	enEffectName_HumanAttack,
	enEffectName_PlayerPunch,
	enEffectName_Num,
};

class MakeEffect : public IGameObject
{
public:
	~MakeEffect();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:

};

