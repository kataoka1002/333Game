#pragma once
#include "SceneLight.h"

namespace nsK2EngineLow
{
	class ModelRender;

	class RenderingEngine : public Noncopyable
	{
	public:
		RenderingEngine();
		~RenderingEngine();

		bool Start();
		void Init();
		void InitMainRenderTarget();
		void InitCopyToframeBufferSprite();
		void InitGBuffer();
		void InitDefferedLightingSprite();
		void Execute(RenderContext& rc);
		void RenderToGBuffer(RenderContext& rc);
		void DeferredLighting(RenderContext& rc);
		void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);
		void ObjectClear();

		/// <summary>
		/// ���f�������_�[���R���e�i�̌��ɂ�������
		/// </summary>
		/// <param name="modelRender">���f�������_�[</param>
		void AddModelRenderObject(ModelRender* modelRender)
		{
			ModelRenderObject.push_back(modelRender);
		}

		/// <summary>
		/// �V�[�����C�g�̍\���̂�Ԃ�
		/// </summary>
		/// <returns></returns>
		Light& GetLightCB()
		{
			return m_sceneLight.GetLight();
		}

		/// <summary>
		/// �f�B���N�V�������C�g�̐ݒ�
		/// </summary>
		/// <param name="lightNum">�i���o�[</param>
		/// <param name="direction">����</param>
		/// <param name="color">�J���[</param>
		void SetDirectionLight(int lightNum, Vector3 direction, Vector3 color)
		{
			m_sceneLight.SetDirectionLight(lightNum, direction, color);
		}

		/// <summary>
		/// �|�C���g���C�g�̐ݒ�
		/// </summary>
		/// <param name="lightNum">�i���o�[</param>
		/// <param name="position">�|�W�V����</param>
		/// <param name="color">�J���[</param>
		/// <param name="range">�e���͈�</param>
		void SetPointLight(int lightNum, Vector3 position, Vector3 color, float range)
		{
			m_sceneLight.SetPointLight(lightNum, position, color, range);
		}

		void SetPointLightPosition(int lightNum, Vector3 position)
		{
			m_sceneLight.SetPointLightPosition(lightNum, position);
		}


	private:
		// GBuffer�̒�`
		enum EnGBuffer
		{
			enGBufferAlbedo,    // �A���x�h
			enGBufferNormal,	//�@��
			enGBufferWorldPos,	//���[���h���W
			enGBufferNum,       // G-Buffer�̐�
		};

		SceneLight m_sceneLight;                        // �V�[�����C�g
		RenderTarget m_mainRenderTarget;				//���C�������_�����O�^�[�Q�b�g
		RenderTarget m_gBuffer[enGBufferNum];			//GBuffer
		Sprite m_copyToframeBufferSprite;				//���C�������_�����O�^�[�Q�b�g���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g
		Sprite m_diferredLightingSprite;				//�f�B�t�@�[�h���C�e�B���O�p�̃X�v���C�g

		std::vector<ModelRender*> ModelRenderObject;	//���f�������_�[

	};

}

