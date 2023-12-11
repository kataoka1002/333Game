#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"

namespace nsK2EngineLow {

	ModelRender::ModelRender()
	{

	}

	ModelRender::~ModelRender()
	{
		if (g_renderingEngine != nullptr) {
			//g_renderingEngine->RemoveEventListener(this);
			for (auto& geomData : m_geometryDatas) {
				// �����_�����O�G���W������o�^����
				g_renderingEngine->UnregisterGeometryData(&geomData);
			}
			/*if (m_addRaytracingWorldModel) {
				g_renderingEngine->RemoveModelFromRaytracingWorld(*m_addRaytracingWorldModel);
			}*/
		}
	}

	void ModelRender::Init(
		const char* filePath,
		AnimationClip* animationClips,
		int animationClipsNum,
		EnModelUpAxis enModelUpAxis,
		bool isShadowReciever,
		int maxInstance)
	{
		// �X�P���g�����������B
		InitSkeleton(filePath);

		// �A�j���[�V�������������B
		InitAnimation(animationClips, animationClipsNum, enModelUpAxis);

		// GBuffer�`��p�̃��f����������
		InitModelOnRenderGBuffer(filePath, enModelUpAxis, isShadowReciever);

		// �V���h�E�}�b�v�`��p�̃��f����������
		InitShadowDrawModel(filePath, enModelUpAxis);

		// �􉽊w�f�[�^���������B
		InitGeometryDatas(maxInstance);

		// ZPrepass�`��p�̃��f����������

		// ���C�g�����[���h�ɒǉ�
		if (m_isRaytracingWorld)
		{
			g_renderingEngine->AddModelToRaytracingWorld(m_renderToGBufferModel);
			m_addRaytracingWorldModel = &m_renderToGBufferModel;
		}
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

	void ModelRender::SetupVertexShaderEntryPointFunc(ModelInitData& modelInitData)
	{
		//�A�j���[�V����������Ȃ�
		if (m_animationClips != nullptr)
		{
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}
		else
		{
			modelInitData.m_vsEntryPointFunc = "VSMain";
		}
	}

	void ModelRender::InitGeometryDatas(int maxInstance)
	{
		m_geometryDatas.resize(maxInstance);
		int instanceId = 0;
		for (auto& geomData : m_geometryDatas) {
			geomData.Init(this, instanceId);
			// �����_�����O�G���W���ɓo�^�B
			g_renderingEngine->RegisterGeometryData(&geomData);
			instanceId++;
		}
	}

	void ModelRender::InitModelOnRenderGBuffer(const char* tkmFilePath, EnModelUpAxis enModelUpAxis, bool isShadowReciever)
	{
		ModelInitData modelInitData;
		modelInitData.m_fxFilePath = "Assets/shader/RenderToGBuffer.fx";
		modelInitData.m_modelUpAxis = enModelUpAxis;
		modelInitData.m_tkmFilePath = tkmFilePath;
		modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		modelInitData.m_colorBufferFormat[1] = DXGI_FORMAT_R8G8B8A8_UNORM;
		modelInitData.m_colorBufferFormat[2] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		modelInitData.m_colorBufferFormat[3] = DXGI_FORMAT_R8G8B8A8_UNORM;
		modelInitData.m_colorBufferFormat[4] = DXGI_FORMAT_R8G8B8A8_UNORM;

		//�X�P���g�����w�肷��B
		if (m_animationClips != nullptr)
		{
			modelInitData.m_skeleton = &m_skeleton;
		}

		//���_�V�F�[�_�[�̃G���g���[�|�C���g��ݒ�
		SetupVertexShaderEntryPointFunc(modelInitData);

		//�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g��ݒ�
		//if (isShadowReciever) 
		{
			modelInitData.m_psEntryPointFunc = "PSShadowMain";
		}
		/*else
		{
			modelInitData.m_psEntryPointFunc = "PSMain";
		}*/

		m_renderToGBufferModel.Init(modelInitData);
	}

	void ModelRender::InitShadowDrawModel(const char* tkmFilePath, EnModelUpAxis enModelUpAxis)
	{
		//�V���h�E�}�b�v�ɏ������ރ��f���̐ݒ�
		ModelInitData sadowDrawModelInitData;
		sadowDrawModelInitData.m_fxFilePath = "Assets/shader/drawShadowMap.fx";
		sadowDrawModelInitData.m_psEntryPointFunc = "PSMain";
		sadowDrawModelInitData.m_tkmFilePath = tkmFilePath;
		sadowDrawModelInitData.m_modelUpAxis = enModelUpAxis;
		sadowDrawModelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32_FLOAT;

		//�X�P���g�����w�肷��B
		if (m_animationClips != nullptr)
		{
			sadowDrawModelInitData.m_skeleton = &m_skeleton;
		}

		//���_�V�F�[�_�[�̃G���g���[�|�C���g��ݒ�
		SetupVertexShaderEntryPointFunc(sadowDrawModelInitData);

		m_shadowDrawModel[0].Init(sadowDrawModelInitData);
		m_shadowDrawModel[1].Init(sadowDrawModelInitData);
		m_shadowDrawModel[2].Init(sadowDrawModelInitData);
	}

	void ModelRender::Update()
	{
		//���f�����Ɉړ���]�g���n��
		m_renderToGBufferModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		m_shadowDrawModel[0].UpdateWorldMatrix(m_position, m_rotation, m_scale);
		m_shadowDrawModel[1].UpdateWorldMatrix(m_position, m_rotation, m_scale);
		m_shadowDrawModel[2].UpdateWorldMatrix(m_position, m_rotation, m_scale);

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
		//�J�����O�̑Ώۂ���Ȃ��Ȃ�펞�\��
		if (m_doCulling == false)
		{
			g_renderingEngine->AddModelRenderObject(this);
			return;
		}

		// �r���[�t���X�^���Ɋ܂܂�Ă���Ȃ�
		if (m_geometryDatas[0].IsInViewFrustum()) 
		{
			g_renderingEngine->AddModelRenderObject(this);
			g_renderingEngine->Add3DModelToRenderToShadowMap(m_shadowDrawModel[0], m_shadowDrawModel[1], m_shadowDrawModel[2]);
		}
	}
}