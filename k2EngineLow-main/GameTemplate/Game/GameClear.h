#pragma once
#include "Result.h"

class GameClear : public Result
{
public:
	GameClear();
	~GameClear();

	void Update();
	void Render(RenderContext& rc);
	void InitCamera() override;	//���f���̏�����
	void OnDestroy()				override;

private:

};

