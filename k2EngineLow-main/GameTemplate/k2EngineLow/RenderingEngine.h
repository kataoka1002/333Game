#pragma once
#include "SceneLight.h"
#include "PostEffect.h"
#include "ShadowMapRender.h"
#include "SceneGeometryData.h"

namespace nsK2EngineLow
{
	class ModelRender;
	class SpriteRender;
	class FontRender;

	class RenderingEngine : public Noncopyable
	{
	public:
		// ディレクションライト
		struct DirectionalLight
		{
			Vector3 direction = { 1.0f,0.0f,0.0f };		// ライトの方向
			int castShadow = 0;							// 影をキャストする?
			Vector4 color = { 1.0f,1.0f,1.0f,1.0f };    // ライトのカラー
		};

		/// <summary>
		/// レイトレ用のライトデータ。
		/// </summary>
		struct RaytracingLightData 
		{
			DirectionalLight m_directionalLight;  // ディレクショナルライト。
			Vector3 m_ambientLight;               // 環境光。IBLテクスチャが指定されていない場合に利用される。
			float m_iblIntencity;                 // IBL強度。
			int m_enableIBLTexture;               // IBLテクスチャが指定されている。
		};

		// ディファードライティング用の定数バッファ
		struct SDeferredLightingCB
		{
			Light m_light;              // ライト
			float m_iblLuminance;       // IBLの明るさ。
			int m_isIBL;                // IBLを行う。
			int m_isEnableRaytracing;   // レイトレが行われている。
		};


		RenderingEngine();
		~RenderingEngine();

		bool Start();
		void Init();
		void InitMainRenderTarget();
		void InitZPrepassTarget();
		void Init2DSprite();
		void InitCopyToframeBufferSprite();
		void InitGBuffer();
		void InitDefferedLightingSprite();
		void InitShadow();
		void CalcViewProjectionMatrixForViewCulling();
		void Update();
		void Execute(RenderContext& rc);
		void RenderToGBuffer(RenderContext& rc);
		void ZPrepass(RenderContext& rc);
		void DrawEffect(RenderContext& rc);
		void DeferredLighting(RenderContext& rc);
		void SpriteFontDraw(RenderContext& rc);
		void DrawShadowMap(RenderContext& rc);
		void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);
		void ObjectClear();

		/// <summary>
		/// モデルレンダーをコンテナの後ろにくっつける
		/// </summary>
		/// <param name="modelRender">モデルレンダー</param>
		void AddModelRenderObject(ModelRender* modelRender)
		{
			ModelRenderObject.push_back(modelRender);
		}

		/// <summary>
		/// スプライトレンダーをコンテナの後ろにくっつける
		/// </summary>
		/// <param name="spriteRender">スプライトレンダー</param>
		void AddSpriteRenderObject(SpriteRender* spriteRender)
		{
			SpriteRenderObject.push_back(spriteRender);
		}

		/// <summary>
		/// フォントレンダーをコンテナの後ろにくっつける
		/// </summary>
		/// <param name="fontRender">フォントレンダー</param>
		void AddFontRenderObject(FontRender* fontRender)
		{
			FontRenderObject.push_back(fontRender);
		}

		/// <summary>
		/// シャドウマップへの描画パスにモデルを追加
		/// </summary>
		/// <param name="model0">近景用のシャドウマップに描画するモデル</param>
		/// <param name="model1">中景用のシャドウマップ1に描画するモデル</param>
		/// <param name="model2">遠景用のシャドウマップ2に描画するモデル</param>
		void Add3DModelToRenderToShadowMap(Model& model0, Model& model1, Model& model2)
		{
			m_shadowMapRender.Add3DModel(model0, model1, model2);
		}

		/// <summary>
		/// レイトレワールドにモデルを追加。
		/// </summary>
		/// <param name="model">追加するモデル</param>
		void AddModelToRaytracingWorld(Model& model)
		{
			g_graphicsEngine->RegistModelToRaytracingWorld(model);
		}

		/// <summary>
		/// シーンライトの構造体を返す
		/// </summary>
		/// <returns></returns>
		Light& GetLightCB()
		{
			return m_sceneLight.GetLight();
		}

		Camera& GetLightCamera()
		{
			return m_lightCamera;
		}

		/// <summary>
		/// ディレクションライトの設定
		/// </summary>
		/// <param name="lightNum">ナンバー</param>
		/// <param name="direction">方向</param>
		/// <param name="color">カラー</param>
		void SetDirectionLight(int lightNum, Vector3 direction, Vector3 color)
		{
			m_sceneLight.SetDirectionLight(lightNum, direction, color);
		}

		/// <summary>
		/// ポイントライトの設定
		/// </summary>
		/// <param name="lightNum">ナンバー</param>
		/// <param name="position">ポジション</param>
		/// <param name="color">カラー</param>
		/// <param name="range">影響範囲</param>
		void SetPointLight(int lightNum, Vector3 position, Vector3 color, float range)
		{
			m_sceneLight.SetPointLight(lightNum, position, color, range);
		}

		void SetPointLightPosition(int lightNum, Vector3 position)
		{
			m_sceneLight.SetPointLightPosition(lightNum, position);
		}

		/// <summary>
		/// スポットライトの設定
		/// </summary>
		/// <param name="lightNum">ナンバー</param>
		/// <param name="position">ポジション</param>
		/// <param name="color">カラー</param>
		/// <param name="range">影響範囲</param>
		/// <param name="direction">射出方向</param>
		/// <param name="angle">射出角度</param>
		void SetSpotLight(int lightNum, Vector3 position, Vector3 color, float range, Vector3 direction, float angle)
		{
			m_sceneLight.SetSpotLight(lightNum, position, color, range, direction, angle);
		}

		/// <summary>
		/// 半球ライトの設定
		/// </summary>
		/// <param name="skyColor">空の色</param>
		/// <param name="groundColor">地面の色</param>
		/// <param name="normal">地面の法線</param>
		void SetHemLight(Vector3 skyColor, Vector3 groundColor, Vector3 normal)
		{
			m_sceneLight.SetHemLight(skyColor, groundColor, normal);
		}

		/// <summary>
		/// 定数バッファに行列をセットする
		/// </summary>
		/// <param name="mat">行列</param>
		/// <param name="num">シャドウマップの番号</param>
		void SetLVP(Matrix mat, int num)
		{
			m_sceneLight.SetLVP(mat, num);
		}

		/// <summary>
		/// EyePosの設定
		/// </summary>
		/// <param name="pos">座標</param>
		void SetEyePosition(Vector3 pos)
		{
			m_sceneLight.SetEyePos(pos);
		}


		/// <summary>
		/// IBLを再初期化。
		/// </summary>
		void ReInitIBL(const wchar_t* iblTexFilePath, float luminance);

		/// <summary>
		/// 環境光の計算のためのIBLテクスチャを設定。
		/// </summary>
		/// <remark>
		/// この関数を利用して、IBLテクスチャをセットすると、
		/// 環境光をIBLテクスチャからサンプリングして、それを利用した
		/// ライティングが行われます。
		/// IBLテクスチャを利用した環境光の計算をオフにしたい場合は、DisableIBLForAmbinet()を呼び出して、
		/// IBLを無効にしてください。
		/// </remark>
		/// <param name="textureFilePath">
		/// IBLテクスチャのファイルパス。
		/// キューブマップである必要があります。
		/// </param>
		/// <param name="luminance">
		/// IBLテクスチャの明るさ。
		/// <param>
		void SetAmbientByIBLTexture(const wchar_t* textureFilePath, float luminance)
		{
			ReInitIBL(textureFilePath, luminance);
		}
		/// <summary>
		/// レイトレーシングが有効かどうかを判定する。
		/// </summary>
		/// <returns></returns>
		bool IsEnableRaytracing() const
		{
			return m_isEnableRaytracing && g_graphicsEngine->IsPossibleRaytracing();
		}
		/// <summary>
		/// レイトレーシングを有効にします。
		/// この設定はハードウェアレイトレーシングが無効な場合は無視されます。
		/// </summary>
		void EnableRaytracing()
		{
			m_isEnableRaytracing = true && IsEnableRaytracing();
		}
		/// <summary>
		/// レイトレーシングを無効にします。
		/// この設定はハードウェアレイトレーシングが無効な場合は無視されます。
		/// </summary>
		void DisableRaytracing()
		{
			m_isEnableRaytracing = false && IsEnableRaytracing();
		}
		/// <summary>
		/// レイトレ用のライトデータを取得。
		/// </summary>
		/// <returns></returns>
		RaytracingLightData& GetRaytracingLightData()
		{
			return m_raytracingLightData;
		}

		/// <summary>
		/// ビューカリングのためのビュープロジェクション行列を取得。
		/// </summary>
		/// <returns></returns>
		const Matrix& GetViewProjectionMatrixForViewCulling() const
		{
			return m_viewProjMatrixForViewCulling;
		}

		/// <summary>
	    /// 幾何学データを登録
	    /// </summary>
	    /// <param name="geomData">幾何学データ</param>
		void RegisterGeometryData(GemometryData* geomData)
		{
			m_sceneGeometryData.RegisterGeometryData(geomData);
		}
		/// <summary>
		/// 幾何学データの登録解除。
		/// </summary>
		/// <param name="geomData"></param>
		void UnregisterGeometryData(GemometryData* geomData)
		{
			m_sceneGeometryData.UnregisterGeometryData(geomData);
		}

	private:
			/// <summary>
			/// イメージベースドライティング(IBL)のためのデータを初期化する。
			/// </summary>
			/// <param name="iblTexFilePath">IBLテクスチャのファイルパス。</param>
			/// <param name="intencity">IBLの強度。</param>
			void InitIBLData(const wchar_t* iblTexFilePath, float intencity);

	private:
		// GBufferの定義
		enum EnGBuffer
		{
			enGBufferAlbedo,				// アルベド
			enGBufferNormal,				// 法線
			enGBufferWorldPos,				// ワールド座標
			enGBufferNormalInView,			// カメラ空間の法線
			enGBufferMetallicSmooth,		// メタリックスムースマップ
			enGBufferNum,					// G-Bufferの数
		};

		/// <summary>
		/// GIテクスチャを作るためのブラー処理。
		/// </summary>
		enum EGITextureBlur 
		{
			eGITextureBlur_1024x1024,   // 1024×1024
			eGITextureBlur_512x512,     // 512×512
			eGITextureBlur_256x256,     // 256×256
			eGITextureBlur_128x128,     // 128×128
			eGITextureBlur_Num,
		};

		/// <summary>
		/// IBLデータ
		/// </summary>
		struct SIBLData 
		{
			Texture m_texture;          // IBLテクスチャ
			float m_intencity = 1.0f;   // 強度。
		};


		SceneLight m_sceneLight;							// シーンライト
		RenderTarget m_mainRenderTarget;					// メインレンダリングターゲット
		RenderTarget m_zprepassRenderTarget;                // ZPrepass描画用のレンダリングターゲット
		RenderTarget m_gBuffer[enGBufferNum];				// GBuffer
		RenderTarget m_2DRenderTarget;						// 2D描画用のターゲット
		RenderTarget m_shadowMap;							// シャドウマップ
		Sprite m_copyToframeBufferSprite;					// メインレンダリングターゲットをフレームバッファにコピーするためのスプライト
		Sprite m_diferredLightingSprite;					// ディファードライティング用のスプライト
		Sprite m_2DSprite;									// 2D描画用のスプライト
		Sprite m_mainSprite;								// メインスプライト
		PostEffect m_postEffect;							// ポストエフェクト
		Camera m_lightCamera;								// ライトカメラ
		shadow::ShadowMapRender m_shadowMapRender;			// シャドウマップレンダー
		bool m_isEnableRaytracing = true;					// レイトレーシングが有効？
		GaussianBlur m_giTextureBlur[eGITextureBlur_Num];   // GIテクスチャにブラーをかける処理
		RaytracingLightData m_raytracingLightData;			// レイトレ用のライトデータ
		SIBLData m_iblData;                                 // IBLデータ。
		SDeferredLightingCB m_deferredLightingCB;           // ディファードライティング用の定数バッファ
		Matrix m_viewProjMatrixForViewCulling;              // ビューカリング用のビュープロジェクション行列。
		SceneGeometryData m_sceneGeometryData;              // シーンのジオメトリ情報。

		std::vector<ModelRender*> ModelRenderObject;	// モデルレンダー
		std::vector<SpriteRender*> SpriteRenderObject;	// スプライトレンダー
		std::vector<FontRender*> FontRenderObject;		// フォントレンダー
	};
}

