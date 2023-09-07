#pragma once

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


	private:

		// GBuffer�̒�`
		enum EnGBuffer
		{
			enGBufferAlbedo,           // �A���x�h
			enGBufferNum,              // G-Buffer�̐�
		};

		RenderTarget m_mainRenderTarget;				//���C�������_�����O�^�[�Q�b�g
		RenderTarget m_gBuffer[enGBufferNum];			//GBuffer

		Sprite m_copyToframeBufferSprite;				//���C�������_�����O�^�[�Q�b�g���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g
		Sprite m_diferredLightingSprite;				//�f�B�t�@�[�h���C�e�B���O�p�̃X�v���C�g

		std::vector<ModelRender*> ModelRenderObject;	//���f�������_�[

	};

}

