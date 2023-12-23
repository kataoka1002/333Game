#pragma once
#include "ObjectBase.h"
class PlantLowRender;

class PlantLow : public ObjectBase
{
public:

	PlantLow();
	~PlantLow();

	void Update();	//更新処理

	void Render(RenderContext& rc)	override;	//描画処理
	void InitModel()				override;	//モデルの初期化
	void OnDestroy()				override;	//消えるときに呼ばれる処理
	void InitCollision()			override;	//コリジョンの初期化

private:
	PlantLowRender* m_plantLowRender = nullptr;
};

