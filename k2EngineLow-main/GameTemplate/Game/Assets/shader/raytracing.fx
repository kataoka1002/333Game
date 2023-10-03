
/////////////////////////////////////////////////////////////////
// �\���̒�`
/////////////////////////////////////////////////////////////////

// ���_�\����
// �t�H�[�}�b�g��TkmFile.h��SVertex�Ɠ����ɂȂ��Ă���K�v������
struct SVertex
{
    float3 pos;
    float3 normal;
    float3 tangent;
    float3 binormal;
    float2 uv;
    int4 indices;
    float4 skinweigths;
};

// closesthit�V�F�[�_�[�Amiss�V�F�[�_�[�ɓn���������\����
struct RayPayload
{
    float3 color; // �J���[
    int hit; // �Փ˃t���O
    int reflection; // ���t���N�V�����t���O
};

/////////////////////////////////////////////////////////////////
// ���\�[�X
/////////////////////////////////////////////////////////////////
RaytracingAccelerationStructure g_raytracingWorld : register(t0); // ���C�g�����[���h
Texture2D<float4> g_albedoTexture : register(t1); // �A���x�h�}�b�v
StructuredBuffer<SVertex> g_vertexBuffers : register(t6); // ���_�o�b�t�@�[
StructuredBuffer<int> g_indexBuffers : register(t7); // �C���f�b�N�X�o�b�t�@�[

SamplerState g_samplerState : register(s0); // �T���v���[�X�e�[�g

/////////////////////////////////////////////////////////////////
// �֐��錾
/////////////////////////////////////////////////////////////////
float2 GetUV(BuiltInTriangleIntersectionAttributes attribs);
float3 GetNormal(BuiltInTriangleIntersectionAttributes attribs);
void TraceShadowRay(inout RayPayload payload);

/////////////////////////////////////////////////////////////////
// �V���h�E���C�ƍł��߂��O�p�`�|���S���ƏՓ˂����Ƃ��ɌĂ΂��V�F�[�_�[
/////////////////////////////////////////////////////////////////
[shader("closesthit")]
void shadowChs(inout RayPayload payload, in BuiltInTriangleIntersectionAttributes attribs)
{
    payload.hit = 1;
}
/////////////////////////////////////////////////////////////////
// �V���h�E���C���O�p�`�|���S���ƏՓ˂��Ȃ��������ɌĂ΂��V�F�[�_�[
/////////////////////////////////////////////////////////////////
[shader("miss")]
void shadowMiss(inout RayPayload payload)
{
    payload.hit = 0;
}

/////////////////////////////////////////////////////////////////
// ���C�ƍł��߂��O�p�`�|���S���ƏՓ˂����Ƃ��ɌĂ΂��V�F�[�_�[
// ///////////////////////////////////////////////////////////////
[shader("closesthit")]
void chs(inout RayPayload payload, in BuiltInTriangleIntersectionAttributes attribs)
{
    // �Փ˓_��UV���W�����߂�
    float2 uv = GetUV(attribs);

    // ���߂�UV���W���g���ăe�N�X�`���J���[���T���v�����O����
    payload.color = g_albedoTexture.SampleLevel(g_samplerState, uv, 0.0f);

    // step-1 �J�������C���|���S���ƏՓ˂����ʒu���v�Z����
    float3 rayDirW = WorldRayDirection();
    float3 rayOriginW = WorldRayOrigin();
    float hitT = RayTCurrent();
    float3 hitPos = rayOriginW + rayDirW * hitT;

    // step-2 ���˃x�N�g�����v�Z����
    float3 normal = GetNormal(attribs);
    float3 refDir = reflect(rayDirW, normal);

    // step-3 ���C�����
    RayDesc ray;
    
    ray.Origin = hitPos;
    ray.Direction = refDir;
    ray.TMax = 10000;
    ray.TMin = 0.01f;

    // step-4 ���C���΂�
    if (payload.reflection == 0)
    {
        RayPayload reflectionPayload;
        reflectionPayload.color = 0.0f;
        
        reflectionPayload.reflection = 1;
        TraceRay(
        g_raytracingWorld,
        0,
        0xFF,
        0,
        0,
        1,
        ray,
        reflectionPayload
        );
        
        // step-5 ���˃J���[�̍���
        payload.color = payload.color * 0.7f + reflectionPayload.color * 0.3f;
    }
}

/////////////////////////////////////////////////////////////////
// ���C���O�p�`�|���S���ƏՓ˂��Ȃ��������ɌĂ΂��V�F�[�_�[
/////////////////////////////////////////////////////////////////
[shader("miss")]
void miss(inout RayPayload payload)
{
    // ���C���|���S�����Փ˂��Ȃ��������̃J���[���v�Z����
    payload.color = float3(1.0f, 0.0f, 0.0f);
}

// �J�����\����
// �萔�o�b�t�@�[�Ȃ̂�16�o�C�g�A���C�����g�ɋC��t���邱��
struct Camera
{
    float4x4 mCameraRot; // �J�����̉�]�s��
    float3 pos; // �J�������W
    float aspect; // �A�X�y�N�g��
    float far; // ������
    float near; // �ߕ���
};

cbuffer rayGenCB : register(b0)
{
    Camera g_camera; // �J����
};

RWTexture2D<float4> gOutput : register(u0); // �J���[�o�͐�

[shader("raygeneration")]
void rayGen()
{
    uint3 launchIndex = DispatchRaysIndex();
    uint3 launchDim = DispatchRaysDimensions();

    float2 crd = float2(launchIndex.xy);
    float2 dims = float2(launchDim.xy);

    float2 d = ((crd / dims) * 2.f - 1.f);
    float aspectRatio = dims.x / dims.y;

    RayDesc ray;
    ray.Origin = g_camera.pos;
    ray.Direction = normalize(float3(d.x * g_camera.aspect, -d.y, -1));
    ray.Direction = mul(g_camera.mCameraRot, ray.Direction);

    ray.TMin = 0;
    ray.TMax = 10000;

    RayPayload payload;
    payload.reflection = 0;
    TraceRay(g_raytracingWorld, 0 /*rayFlags*/, 0xFF, 0 /* ray index*/, 0, 0, ray, payload);

    float3 col = payload.color;

    gOutput[launchIndex.xy] = float4(col, 1);
}

// UV���W���擾
float2 GetUV(BuiltInTriangleIntersectionAttributes attribs)
{
    float3 barycentrics = float3(1.0 - attribs.barycentrics.x - attribs.barycentrics.y, attribs.barycentrics.x, attribs.barycentrics.y);

    // �v���~�e�B�uID���擾
    uint primID = PrimitiveIndex();

    // �v���~�e�B�uID���璸�_�ԍ����擾����
    uint v0_id = g_indexBuffers[primID * 3];
    uint v1_id = g_indexBuffers[primID * 3 + 1];
    uint v2_id = g_indexBuffers[primID * 3 + 2];
    float2 uv0 = g_vertexBuffers[v0_id].uv;
    float2 uv1 = g_vertexBuffers[v1_id].uv;
    float2 uv2 = g_vertexBuffers[v2_id].uv;

    float2 uv = barycentrics.x * uv0 + barycentrics.y * uv1 + barycentrics.z * uv2;
    return uv;
}

// �V���h�E���C���΂�
void TraceShadowRay(inout RayPayload payload)
{
    // 1. �Ԃ��������C�̕����x�N�g�����擾
    float3 rayDirW = WorldRayDirection();

    // 2. ���C���΂������W���擾�B����ł���΃J�����̎��_
    float3 rayOriginW = WorldRayOrigin();

    // 2. ���C���΂����ꏊ����A�Փ˂����_�܂ł̋������擾
    float hitT = RayTCurrent();

    // 1.�A2.�A3.�̏�񂩂�Փ˂������W�����߂�
    float3 hitPos = rayOriginW + rayDirW * hitT;

    // �V���h�E���C�����
    RayDesc ray;

    // �V���h�E���C���΂��ꏊ
    ray.Origin = hitPos;

    // ���C�g�܂ł̃x�N�g���B����̃n���Y�I���ł͌Œ�
    ray.Direction = float3(0.5f, 0.5f, 0.2f);

    // ���K�����Ă���
    ray.Direction = normalize(ray.Direction);

    // ���C�̍ŏ������ƍő勗��
    ray.TMin = 0.01f;
    ray.TMax = 100;

    // �V���h�E���C���΂�
    payload.hit = 0;
    TraceRay(
        g_raytracingWorld, // ���C�g�����[���h
        0, //
        0xFF,
        1, // �q�b�g�O���[�v�̃I�t�Z�b�g�ԍ�
        0,
        1, // �~�X�V�F�[�_�[�̔ԍ�
        ray, // ���C
        payload
    );
}

// �@�����擾
float3 GetNormal(BuiltInTriangleIntersectionAttributes attribs)
{
    float3 barycentrics = float3(1.0 - attribs.barycentrics.x - attribs.barycentrics.y, attribs.barycentrics.x, attribs.barycentrics.y);

    // �v���~�e�B�uID���擾
    uint primID = PrimitiveIndex();

    // �v���~�e�B�uID���璸�_�ԍ����擾����
    uint v0_id = g_indexBuffers[primID * 3];
    uint v1_id = g_indexBuffers[primID * 3 + 1];
    uint v2_id = g_indexBuffers[primID * 3 + 2];

    float3 normal0 = g_vertexBuffers[v0_id].normal;
    float3 normal1 = g_vertexBuffers[v1_id].normal;
    float3 normal2 = g_vertexBuffers[v2_id].normal;
    float3 normal = barycentrics.x * normal0 + barycentrics.y * normal1 + barycentrics.z * normal2;
    normal = normalize(normal);

    // ���[���h��Ԃɕϊ�����̂��ʓ|�Ȃ̂ŁA���K���Ɂc�c���[����
    float cs = cos(-1.57f);
    float sn = sin(-1.57f);
    float4x4 m;
    m[0][0] = 1.0f;
    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[0][3] = 0.0f;

    m[1][0] = 0.0f;
    m[1][1] = cs;
    m[1][2] = sn;
    m[1][3] = 0.0f;

    m[2][0] = 0.0f;
    m[2][1] = -sn;
    m[2][2] = cs;
    m[2][3] = 0.0f;

    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = 0.0f;
    m[3][3] = 1.0f;

    m = transpose(m);
    normal = mul(m, normal);

    return normal;
}
