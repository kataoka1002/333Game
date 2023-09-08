#pragma once
namespace nsK2EngineLow
{
	const int MAX_DIRECTION_LIGHT = 4;
	const int MAX_POINT_LIGHT = 128;
	const int MAX_SPOT_LIGHT = 128;

	//ディレクションライト構造体
	struct DirectionLight
	{
		Vector3 direction = Vector3::Zero;
		float pad0 = 0.0f;
		Vector3 color = Vector3::Zero;
		float pad1 = 0.0f;
	};

	//ポイントライト構造体
	struct PointLight
	{
		Vector3 position = Vector3::Zero;
		float pad = 0.0f;
		Vector3 color = Vector3::Zero;
		float range = 0.0f;
	};

	//スポットライト構造体
	struct SpotLight
	{
		Vector3 position = Vector3::Zero;
		float pad = 0.0f;
		Vector3 color = Vector3::Zero;
		float range = 0.0f;
		Vector3 direction = Vector3::Zero;
		float angle = 0.0f;
	};


	//ライト構造体
	struct Light
	{
		DirectionLight directionLight[MAX_DIRECTION_LIGHT];	//ディレクションライトの配列
		Vector3 eyePos = Vector3::Zero;						//カメラの位置
		float pad = 0.0f;
		PointLight pointLight[MAX_POINT_LIGHT];				//ポイントライトの配列
		SpotLight spotLight[MAX_SPOT_LIGHT];				//スポットライトの配列
	};


	class SceneLight : public Noncopyable
	{
	public:
		//コンストラクタ
		SceneLight();

		//ディレクションライトの設定
		void SetDirectionLight(int lightNum, Vector3 direction, Vector3 color)
		{
			m_light.directionLight[lightNum].direction = direction;
			m_light.directionLight[lightNum].color = color;
		}

		//ポイントライトの設定
		void SetPointLight(int lightNum, Vector3 position, Vector3 color, float range)
		{
			m_light.pointLight[lightNum].position = position;
			m_light.pointLight[lightNum].color = color;
			m_light.pointLight[lightNum].range = range;
		}

		//ポイントライトのポジション設定
		void SetPointLightPosition(int lightNum, Vector3 position)
		{
			m_light.pointLight[lightNum].position = position;
		}

		//スポットライトの設定
		void SetSpotLight(int lightNum, Vector3 position, Vector3 color, float range, Vector3 direction, float angle)
		{
			m_light.spotLight[lightNum].position = position;
			m_light.spotLight[lightNum].color = color;
			m_light.spotLight[lightNum].range = range;
			m_light.spotLight[lightNum].direction = direction;
			m_light.spotLight[lightNum].angle = angle;
		}

		//構造体を返す
		Light& GetLight()
		{
			return m_light;
		}


	private:
		Light m_light;
	};

}

