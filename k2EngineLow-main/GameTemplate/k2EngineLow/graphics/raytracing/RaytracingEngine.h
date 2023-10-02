#pragma once

#include "DescriptorHeap.h"
#include "RaytracingInstance.h"
#include "TLASBuffer.h"
#include "BLASBuffer.h"
#include "RaytracingPSO.h"
#include "RaytracingShaderTable.h"
#include "RaytracingDescriptorHeaps.h"

namespace nsK2EngineLow {
	class Model;
	namespace raytracing {

		/// <summary>
		/// ���������
		/// </summary>
		struct InitData {
			void* m_expandShaderResource;			// �g���V�F�[�_�[���\�[�X�̔z��B
			int		m_expandShaderResourceSize;		// �g���V�F�[�_�[���\�[�X�̃T�C�Y�̔z��B
			DXGI_FORMAT m_outputColorBufferFormat = DXGI_FORMAT_R16G16B16A16_FLOAT;	// ���C�g���̌��ʂ��o�͂���J���[�o�b�t�@�̃t�H�[�}�b�g�B
		};
		/// <summary>
		/// �g���V�F�[�_�[���\�[�X
		/// </summary>
		struct ExpanadSRV {
			void Init(void* srcData, int srcDataSize)
			{
				m_srcData = srcData;
				m_srcDataSize = srcDataSize;
				m_structuredBuffer.Init(srcDataSize, 1, srcData/*, false*/);
			}
			void* m_srcData = nullptr;				// �\�[�X�f�[�^�B
			int m_srcDataSize = 0;					// �\�[�X�f�[�^�̃T�C�Y�B
			StructuredBuffer m_structuredBuffer;	// �X�g���N�`���[�h�o�b�t�@�B
		};
		using ExpanadSRVPtr = std::unique_ptr< ExpanadSRV>;

		class Engine : public Noncopyable
		{
		public:
			/// <summary>
			/// ���C�g���[�V���O���f�B�X�p�b�`�B
			/// </summary>
			/// <param name="rc">�����_�����O�R���e�L�X�g</param>
			void Dispatch(RenderContext& rc);
			/// <summary>
			/// �W�I���g����o�^�B
			/// </summary>
			/// <param name="model">���f��</param>
			void RegistGeometry(Model& model)
			{
				//���C�g�����[���h�ɃW�I���g����o�^�B
				m_world.RegistGeometry(model);
			}
			/// <summary>
			/// ���C�g���̌��ʂ̏o�͐�ƂȂ�e�N�X�`�����擾�B
			/// </summary>
			/// <returns></returns>
			Texture& GetOutputTexture()
			{
				return m_outputTexture;
			}
			/// <summary>
			/// �W�I���g���̓o�^���m��B
			/// </summary>
			void CommitRegistGeometry(RenderContext& rc);
			/// <summary>
			/// �V�F�[�_�[���\�[�X���쐬�B
			/// </summary>
			void CreateShaderResources(const InitData& initData);
		private:
			/// <summary>
			/// �V�F�[�_�[�e�[�u�����쐬�B
			/// </summary>
			/// <param name="rc"></param>
			void CreateShaderTable(RenderContext& rc);

		private:
			/// <summary>
			/// �J����
			/// </summary>
			struct Camera {
				Matrix mRot;	//��]�s��
				Vector3 pos;	//���_�B
				float aspect;	//�A�X�y�N�g��B
				float fFar;		//�����ʁB
				float fNear;	//�ߕ��ʁB
			};
			ExpanadSRVPtr m_expandSRV;					// �g���V�F�[�_�[���\�[�X�r���[�B
			ConstantBuffer m_rayGenerationCB;			//���C�W�F�l���[�V�����̒萔�o�b�t�@�B
			World m_world;								//���C�g�����[���h�B
			PSO m_pipelineStateObject;					//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
			ShaderTable m_shaderTable;					//�V�F�[�_�[�e�[�u���B
			DescriptorHeaps m_descriptorHeaps;			//���C�g���Ŏg�p����f�B�X�N���v�^�q�[�v�̊Ǘ��ҁB
			Texture m_outputTexture;					// ���C�g���[�X�̌��ʂ̏o�͐�(�e�N�X�`��)
			GPUBuffer m_outputResource;					//���C�g���[�X�̌��ʂ̏o�͐�B
		};
	}//namespace raytracing
}//namespace nsK2EngineLow 