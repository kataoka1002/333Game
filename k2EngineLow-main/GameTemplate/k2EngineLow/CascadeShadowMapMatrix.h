#pragma once

namespace nsK2EngineLow
{
	namespace shadow
	{
		class CascadeShadowMapMatrix : public Noncopyable
		{
		public:
            /// <summary>
            /// ���C�g�r���[�v���W�F�N�V�����N���b�v�s����v�Z����
            /// </summary>
            /// <param name="lightDirection">���C�g�̕���</param>
            /// <param name="viewProjectionMatrix">���C�g�r���[�v���W�F�N�V�����s��</param> 
            void CalcLightViewProjectionCropMatrix(Vector3 lightDirection);

            /// <summary>
            /// �v�Z���ꂽ���C�g�r���[�v���W�F�N�V�����N���b�v�s����擾
            /// </summary>
            /// <param name="shadowMapNo">�V���h�E�}�b�v�̔ԍ�</param>
            /// <returns></returns>
            const Matrix& GetLightViewProjectionCropMatrix(int shadowMapNo) const
            {
                return m_lvpcMatrix[shadowMapNo];
            }

		private:
			std::array<Matrix, NUM_SHADOW_MAP> m_lvpcMatrix;	// ���C�g�r���[�N���b�v�s��
		};

	}
}

