#pragma once
#include "Result.h"
class ResultUI;

class GameClear : public Result
{
public:
	GameClear();
	~GameClear();

	void Update();
	void Render(RenderContext& rc);
	void InitCamera() override;	//モデルの初期化
	void OnDestroy() override;
	void InitUI() override;

private:
	ResultUI* m_resultUI = nullptr;
};

