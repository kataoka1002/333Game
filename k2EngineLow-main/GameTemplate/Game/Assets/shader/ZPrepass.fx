///////////////////////////////////////
// ZPrepass
///////////////////////////////////////

// ���f���p�̒萔�o�b�t�@�[
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

//�X�L�j���O�p�̒��_�f�[�^���ЂƂ܂Ƃ߁B
struct SSkinVSIn
{
    int4 Indices : BLENDINDICES0;
    float4 Weights : BLENDWEIGHT0;
};

// ���_�V�F�[�_�[�ւ̓���
struct SVSIn
{
    float4 pos : POSITION; // ���f���̒��_���W
    float2 uv : TEXCOORD0; // UV���W
    SSkinVSIn skinVert; //�X�L���p�̃f�[�^�B
};

// �s�N�Z���V�F�[�_�[�ւ̓���
struct SPSIn
{
    float4 pos : SV_POSITION; // �X�N���[����Ԃł̃s�N�Z���̍��W
    float3 depth : TEXCOORD0; //�[�x�l�Bx�ɂ̓v���W�F�N�V������ԁAy�ɂ̓J������Ԃł̐��K�����ꂽZ�l�Az�ɂ̓J������Ԃł�Z�l
};

///////////////////////////////////////////////////
// �O���[�o���ϐ�
///////////////////////////////////////////////////

StructuredBuffer<float4x4> g_boneMatrix : register(t3); // �{�[���s��
StructuredBuffer<float4x4> g_worldMatrixArray : register(t10); //���[���h�s��̔z��B�C���X�^���V���O�`��̍ۂɗL���B

//�X�L���s����v�Z����B
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
    float4x4 skinning = 0;
    float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += g_boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
        w += skinVert.Weights[i];
    }
    
    skinning += g_boneMatrix[skinVert.Indices[3]] * (1.0f - w);
	
    return skinning;
}

/// <summary>
/// ���_�V�F�[�_�[
/// <summary>
SPSIn VSMainCore(SVSIn vsIn, bool hasSkin)
{
    // �V���h�E�}�b�v�`��p�̒��_�V�F�[�_�[������
    SPSIn psIn;
    float4x4 m;
    if (hasSkin)
    {
        m = CalcSkinMatrix(vsIn.skinVert);
    }
    else
    {
        m = mWorld;
    }
    
    psIn.pos = mul(m, vsIn.pos);
    psIn.pos = mul(mView, psIn.pos);
    psIn.depth.z = psIn.pos.z;
    psIn.pos = mul(mProj, psIn.pos);
    psIn.depth.x = psIn.pos.z / psIn.pos.w;
    psIn.depth.y = saturate(psIn.pos.w / 1000.0f);
    
    return psIn;
}

// �C���X�^���V���O���f���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g
SPSIn VSMainCoreInstancing(SVSIn vsIn, uint instanceId : SV_InstanceID)
{
    SPSIn psIn;
    float4x4 m = g_worldMatrixArray[instanceId];
    psIn.pos = mul(m, vsIn.pos);
    psIn.pos = mul(mView, psIn.pos);
    psIn.depth.z = psIn.pos.z;
    psIn.pos = mul(mProj, psIn.pos);
    psIn.depth.x = psIn.pos.z / psIn.pos.w;
    psIn.depth.y = saturate(psIn.pos.w / 1000.0f);
    
    return psIn;
}

// �X�L���Ȃ����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�֐��B
SPSIn VSMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, false);
}
// �X�L�����胁�b�V���̒��_�V�F�[�_�[�̃G���g���[�֐��B
SPSIn VSSkinMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, true);
}

// ���f���p�̃s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g
float4 PSMain(SPSIn psIn) : SV_Target0
{
    return float4(psIn.pos.z, psIn.depth.y, psIn.depth.z, 1.0f);
}


