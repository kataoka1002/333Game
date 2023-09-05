#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"

namespace nsK2EngineLow {

	ModelRender::ModelRender()
	{

	}

	ModelRender::~ModelRender()
	{

	}

	void ModelRender::Init(
		const char* filePath,
		AnimationClip* animationClips,
		int animationClipsNum,
		EnModelUpAxis enModelUpAxis,	
		bool isShadowReciever)
	{
		// �X�P���g�����������B
		InitSkeleton(filePath);

		// �A�j���[�V�������������B
		InitAnimation(animationClips, animationClipsNum, enModelUpAxis);
		
		// GBuffer�`��p�̃��f����������
		InitModelOnRenderGBuffer(filePath, enModelUpAxis, isShadowReciever);
		 
		// ZPrepass�`��p�̃��f����������
		 
		// �V���h�E�}�b�v�`��p�̃��f����������

	}

	void ModelRender::InitSkeleton(const char* filePath)
	{
		//�X�P���g���̃f�[�^��ǂݍ��݁B
		std::string skeletonFilePath = filePath;
		int pos = (int)skeletonFilePath.find(".tkm");
		skeletonFilePath.replace(pos, 4, ".tks");
		m_skeleton.Init(skeletonFilePath.c_str());
	}

	void ModelRender::InitAnimation(AnimationClip* animationClips, int numAnimationClips, EnModelUpAxis enModelUpAxis)
	{
		//�A�j���[�V�����̐ݒ�
		m_animationClips = animationClips;
		m_numAnimationClips = numAnimationClips;
		if (m_animationClips != nullptr) 
		{
			m_animation.Init(m_skeleton, m_animationClips, numAnimationClips);
		}
	}

	void ModelRender::InitModelOnRenderGBuffer(const char* tkmFilePath, EnModelUpAxis enModelUpAxis, bool isShadowReciever)
	{
		ModelInitData modelInitData;
		modelInitData.m_fxFilePath = "Assets/shader/RenderToGBuffer.fx";
		if (m_animationClips != nullptr) 
		{
			//�X�P���g�����w�肷��B
			modelInitData.m_skeleton = &m_skeleton;
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}
		else
		{
			modelInitData.m_vsEntryPointFunc = "VSMain";
		}
		/*if (isShadowReciever) 
		{
			modelInitData.m_psEntryPointFunc = "PSMainShadowReciever";
		}*/
		modelInitData.m_modelUpAxis = enModelUpAxis;
		modelInitData.m_tkmFilePath = tkmFilePath;
		modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		modelInitData.m_colorBufferFormat[1] = DXGI_FORMAT_R8G8B8A8_SNORM;
		modelInitData.m_colorBufferFormat[2] = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_renderToGBufferModel.Init(modelInitData);
	}

	void ModelRender::Update()
	{
		//���f�����Ɉړ���]�g���n��
		m_renderToGBufferModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);

		if (m_skeleton.IsInited()) 
		{
			//�X�P���g���̍X�V
			m_skeleton.Update(m_renderToGBufferModel.GetWorldMatrix());
		}

		//�A�j���[�V������i�߂�B
		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
	}

	void ModelRender::Draw(RenderContext& rc)
	{
		g_renderingEngine->AddModelRenderObject(this);
	}
}