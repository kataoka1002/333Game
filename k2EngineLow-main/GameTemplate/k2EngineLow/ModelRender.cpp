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
		InitModelOnZprepass(filePath, enModelUpAxis);

		// ���C�g�����[���h�ɒǉ�
		if (m_isRaytracingWorld)
		{
			g_renderingEngine->AddModelToRaytracingWorld(m_renderToGBufferModel);
			m_addRaytracingWorldModel = &m_renderToGBufferModel;
		}
	}

	void ModelRender::InitInstancing(
		const char* filePath,
		AnimationClip* animationClips,
		int animationClipsNum,
		EnModelUpAxis enModelUpAxis,
		bool isShadowReciever,
		int maxInstance)
	{
		// �C���X�^���V���O�`��p�̃f�[�^���������B
		InitInstancingDraw(maxInstance);

		// �X�P���g�����������B
		InitSkeleton(filePath);

		ModelInitData modelInitData;
		modelInitData.m_fxFilePath = "Assets/shader/RenderToGBuffer.fx";

		// ���_�V�F�[�_�[�̃G���g���[�|�C���g���Z�b�g�A�b�v�B
		modelInitData.m_vsEntryPointFunc = "VSMainCoreInstancing";
		// �s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g���Z�b�g�A�b�v�B
		modelInitData.m_psEntryPointFunc = "PSShadowMain";
		
		//���f���̏�������w�肷��B
		modelInitData.m_modelUpAxis = enModelUpAxis;

		modelInitData.m_tkmFilePath = filePath;
		modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		modelInitData.m_colorBufferFormat[1] = DXGI_FORMAT_R8G8B8A8_SNORM;
		modelInitData.m_colorBufferFormat[2] = DXGI_FORMAT_R8G8B8A8_UNORM;

		// �C���X�^���V���O�`����s���ꍇ�́A�g��SRV�ɃC���X�^���V���O�`��p�̃f�[�^��ݒ肷��B
		modelInitData.m_expandShaderResoruceView[0] = &m_worldMatrixArraySB;
		m_renderToGBufferModel.Init(modelInitData);


		// �V���h�E�}�b�v�`��p�̃��f����������
		//InitShadowDrawModel(filePath, enModelUpAxis);

		// �􉽊w�f�[�^���������B
		InitGeometryDatas(maxInstance);

		// ZPrepass�`��p�̃��f����������
		//InitModelOnZprepass(filePath, enModelUpAxis);
		ModelInitData modelInitDataZ;
		modelInitDataZ.m_tkmFilePath = filePath;
		modelInitDataZ.m_fxFilePath = "Assets/shader/ZPrepass.fx";
		modelInitDataZ.m_modelUpAxis = enModelUpAxis;

		// ���_�V�F�[�_�[�̃G���g���[�|�C���g���Z�b�g�A�b�v�B
		modelInitDataZ.m_vsEntryPointFunc = "VSMainCoreInstancing";

		if (m_animationClips != nullptr) {
			//�X�P���g�����w�肷��B
			modelInitDataZ.m_skeleton = &m_skeleton;
		}

		modelInitDataZ.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		if (m_isEnableInstancingDraw) {
			// �C���X�^���V���O�`����s���ꍇ�́A�g��SRV�ɃC���X�^���V���O�`��p�̃f�[�^��ݒ肷��B
			modelInitDataZ.m_expandShaderResoruceView[0] = &m_worldMatrixArraySB;
		}

		m_zprepassModel.Init(modelInitDataZ);
	}

	void ModelRender::InitInstancingDraw(int maxInstance)
	{
		m_maxInstance = maxInstance;
		if (m_maxInstance > 1) {
			// �C���X�^���V���O�`����s���̂ŁA����p�̃f�[�^���\�z����B
			// ���[���h�s��̔z��̃��������m�ۂ���B
			m_worldMatrixArray = std::make_unique<Matrix[]>(m_maxInstance);
			// ���[���h�s���GPU�ɓ]�����邽�߂̃X�g���N�`���[�h�o�b�t�@���m�ہB
			m_worldMatrixArraySB.Init(
				sizeof(Matrix),
				m_maxInstance,
				nullptr
			);
			m_isEnableInstancingDraw = true;
			// �C���X�^���X�ԍ����烏�[���h�s��̔z��̃C���f�b�N�X�ɕϊ�����e�[�u��������������B
			m_instanceNoToWorldMatrixArrayIndexTable = std::make_unique<int[]>(m_maxInstance);
			for (int instanceNo = 0; instanceNo < m_maxInstance; instanceNo++) {
				m_instanceNoToWorldMatrixArrayIndexTable[instanceNo] = instanceNo;
			}
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

	void ModelRender::InitModelOnZprepass(
		const char* tkmFilePath,
		EnModelUpAxis modelUpAxis
	)
	{
		ModelInitData modelInitData;
		modelInitData.m_tkmFilePath = tkmFilePath;
		modelInitData.m_fxFilePath = "Assets/shader/ZPrepass.fx";
		modelInitData.m_modelUpAxis = modelUpAxis;

		// ���_�V�F�[�_�[�̃G���g���[�|�C���g���Z�b�g�A�b�v�B
		SetupVertexShaderEntryPointFunc(modelInitData);

		if (m_animationClips != nullptr) {
			//�X�P���g�����w�肷��B
			modelInitData.m_skeleton = &m_skeleton;
		}

		modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		//if (m_isEnableInstancingDraw) {
		//	// �C���X�^���V���O�`����s���ꍇ�́A�g��SRV�ɃC���X�^���V���O�`��p�̃f�[�^��ݒ肷��B
		//	modelInitData.m_expandShaderResoruceView[0] = &m_worldMatrixArraySB;
		//}

		m_zprepassModel.Init(modelInitData);
	}

	void ModelRender::Update()
	{
		if (m_isEnableInstancingDraw) {
			return;
		}

		//���f�����Ɉړ���]�g���n��
		m_renderToGBufferModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		m_shadowDrawModel[0].UpdateWorldMatrix(m_position, m_rotation, m_scale);
		m_shadowDrawModel[1].UpdateWorldMatrix(m_position, m_rotation, m_scale);
		m_shadowDrawModel[2].UpdateWorldMatrix(m_position, m_rotation, m_scale);
		m_zprepassModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);

		if (m_skeleton.IsInited())
		{
			//�X�P���g���̍X�V
			m_skeleton.Update(m_renderToGBufferModel.GetWorldMatrix());
		}

		//�A�j���[�V������i�߂�B
		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
	}

	void ModelRender::RemoveInstance(int instanceNo)
	{
		int matrixIndex = m_instanceNoToWorldMatrixArrayIndexTable[instanceNo];

		m_worldMatrixArray[matrixIndex] = g_matZero;
	}

	void ModelRender::UpdateInstancingData(int instanceNo, const Vector3& pos, const Quaternion& rot, const Vector3& scale)
	{
		K2_ASSERT(instanceNo < m_maxInstance, "�C���X�^���X�ԍ����s���ł��B");
		if (!m_isEnableInstancingDraw) {
			return;
		}
		Matrix worldMatrix;
		//if (m_translucentModel.IsInited()) {
		//	// ���������f����ZPrepass���f�������������Ă��Ȃ��̂ŁA��������g���B
		//	worldMatrix = m_translucentModel.CalcWorldMatrix(pos, rot, scale);
		//}
		/*else */{
			worldMatrix = m_zprepassModel.CalcWorldMatrix(pos, rot, scale);
		}
		// �C���X�^���X�ԍ�����s��̃C���f�b�N�X���擾����B
		int matrixArrayIndex = m_instanceNoToWorldMatrixArrayIndexTable[instanceNo];
		// �C���X�^���V���O�`����s���B
		m_worldMatrixArray[matrixArrayIndex] = worldMatrix;
		if (m_numInstance == 0) {
			//�C���X�^���X����0�̏ꍇ�̂݃A�j���[�V�����֌W�̍X�V���s���B
			// �X�P���g�����X�V�B
			// �e�C���X�^���X�̃��[���h��Ԃւ̕ϊ��́A
			// �C���X�^���X���Ƃɍs���K�v������̂ŁA���_�V�F�[�_�[�ōs���B
			// �Ȃ̂ŁA�P�ʍs���n���āA���f����ԂŃ{�[���s����\�z����B
			m_skeleton.Update(g_matIdentity);
			//�A�j���[�V������i�߂�B
			//m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
		}
		m_numInstance++;
	}

	int g_hoge = 0;
	void ModelRender::Draw(RenderContext& rc)
	{
		//�J�����O�̑Ώۂ���Ȃ��Ȃ�펞�\��
		if (m_doCulling == false)
		{
			g_renderingEngine->AddModelRenderObject(this);
			return;
		}

		if (m_isEnableInstancingDraw) {
			// �C���X�^���V���O�`��̓r���[�t���X�^���J�����O�͍s��Ȃ��B
			g_renderingEngine->AddModelRenderObject(this);
			m_worldMatrixArraySB.Update(m_worldMatrixArray.get());
			m_numInstance = 0;
		}
		else {
			// �ʏ�`��
			if (m_geometryDatas[0].IsInViewFrustum()) {
				g_renderingEngine->AddModelRenderObject(this);
				g_renderingEngine->Add3DModelToRenderToShadowMap(m_shadowDrawModel[0], m_shadowDrawModel[1], m_shadowDrawModel[2]);
			}
		}

		//// �r���[�t���X�^���Ɋ܂܂�Ă���Ȃ�
		//if (m_geometryDatas[0].IsInViewFrustum()) 
		//{
		//g_hoge++;
		//}
	}
}