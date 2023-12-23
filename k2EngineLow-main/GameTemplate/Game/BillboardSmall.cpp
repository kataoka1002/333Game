#include "stdafx.h"
#include "BillboardSmall.h"
#include "Game.h"
#include "BillboardSmallRender.h"

namespace
{
	//コリジョンの大きさ
	const float COLLISION_SCALE = 60.0f;

	//与えるダメージ
	const float DAMAGE_AMOUNT = 50.0f;

	//キャラコンの半径
	const float CHARACON_RADIUS = 30.0f;

	//キャラコンの高さ
	const float CHARACON_HEIGHT = 40.0f;
}

BillboardSmall::BillboardSmall()
{

}

BillboardSmall::~BillboardSmall()
{
	DeleteGO(m_collisionObject);
	// 先に死んでいるかもしれないので、検索してnullチェックをする。
	auto render = FindGO<BillboardSmallRender>("billboardsmallrender");
	if (render) {
		render->RemoveInstance(m_instanceNo);
	}
}

void BillboardSmall::Update()
{
	//動き
	Move();

	//m_model.Update();

	//モデルの更新処理
	m_billboardSmallRender->UpdateInstancingData(
		m_instanceNo,
		m_position,
		m_rotation,
		m_scale
	);
}

void BillboardSmall::InitModel()
{
	m_billboardSmallRender = FindGO<BillboardSmallRender>("billboardsmallrender");

	//m_model.Init("Assets/modelData/object/billboard_small.tkm");
	//m_model.SetPosition(m_position);
	//m_model.SetRotation(m_rotation);
	//m_model.SetScale(m_scale);
	//m_model.Update();

	//キャラクターコントローラーを初期化
	m_charaCon.Init(
		CHARACON_RADIUS,	//半径
		CHARACON_HEIGHT,	//高さ
		m_position			//座標
	);

	//与えるダメージの設定
	m_damage = DAMAGE_AMOUNT;

	//コリジョンを出す座標の加算量を求める
	if (CHARACON_RADIUS >= CHARACON_HEIGHT)
	{
		m_collisionAddPos = CHARACON_RADIUS;
	}
	else
	{
		m_collisionAddPos = CHARACON_HEIGHT;
	}
}

void BillboardSmall::InitCollision()
{
	//コリジョンオブジェクトを作成する。
	m_collisionObject = NewGO<CollisionObject>(0);

	//球状のコリジョンを作成する。
	m_collisionObject->CreateSphere(
		m_position,				//座標
		Quaternion::Identity,	//回転
		COLLISION_SCALE			//半径
	);

	//コリジョンに名前を付ける
	m_collisionObject->SetName("billboard_small");

	//コリジョンオブジェクトが自動で削除されないようにする
	m_collisionObject->SetIsEnableAutoDelete(false);
}

void BillboardSmall::OnDestroy()
{
	//リストから削除
	m_game->RemoveObjectFromList(this);

	//自分自身の削除
	DeleteGO(this);
}

void BillboardSmall::Render(RenderContext& rc)
{
	//モデルが見えない位置にあるなら描画しない
	if (m_canSeeModel != true)
	{
		return;
	}

	//m_model.Draw(rc);
}