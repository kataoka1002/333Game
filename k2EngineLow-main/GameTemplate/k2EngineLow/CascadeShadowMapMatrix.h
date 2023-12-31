#pragma once

namespace nsK2EngineLow
{
	namespace shadow
	{
		class CascadeShadowMapMatrix : public Noncopyable
		{
		public:
            /// <summary>
            /// ライトビュープロジェクションクロップ行列を計算する
            /// </summary>
            /// <param name="lightDirection">ライトの方向</param>
            /// <param name="viewProjectionMatrix">ライトビュープロジェクション行列</param> 
            void CalcLightViewProjectionCropMatrix(Vector3 lightDirection);

            /// <summary>
            /// 計算されたライトビュープロジェクションクロップ行列を取得
            /// </summary>
            /// <param name="shadowMapNo">シャドウマップの番号</param>
            /// <returns></returns>
            const Matrix& GetLightViewProjectionCropMatrix(int shadowMapNo) const
            {
                return m_lvpcMatrix[shadowMapNo];
            }

		private:
			std::array<Matrix, NUM_SHADOW_MAP> m_lvpcMatrix;	// ライトビュークロップ行列
		};

	}
}

