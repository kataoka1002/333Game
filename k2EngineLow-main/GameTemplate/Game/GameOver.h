#pragma once
#include "Result.h"

class GameOver : public Result
{
public:
	GameOver();
	~GameOver();

	void Update();
	void Render(RenderContext& rc);
	void InitCamera() override;	//���f���̏�����
	void OnDestroy()				override;

private:

};