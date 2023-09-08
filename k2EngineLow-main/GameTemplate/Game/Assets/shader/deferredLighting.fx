/*!
 * @brief	�X�v���C�g�p�̃V�F�[�_�[
 */
static const int MAX_DIRECTION_LIGHT = 4;
static const int MAX_POINT_LIGHT = 128;
static const int MAX_SPOT_LIGHT = 128;

// �\����
struct DirectionLight
{
    float3 dirDirection;    //����
    float3 dirColor;        //�J���[
};

struct PointLight
{
    float3 ptPosition;   //�|�W�V����
    float3 ptColor;      //�J���[
    float ptRange;       //�e���͈�
};

struct SpotLight
{
    float3 spPosition;  //�|�W�V����
    float3 spColor;     //�J���[
    float spRange;      //�e���͈�
    float3 spDirection; //�ˏo����
    float spAngle;      //�ˏo�p�x
};


cbuffer cb : register(b0)
{
    float4x4 mvp;
    float4 mulColor;
};

cbuffer LightCb : register(b1)
{
    DirectionLight directionLight[MAX_DIRECTION_LIGHT];
    float3 eyePos;
    PointLight pointLight[MAX_POINT_LIGHT];
    SpotLight spotLight[MAX_SPOT_LIGHT];
}

struct VSInput
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

// �ϐ���`
Texture2D<float4> albedoTexture : register(t0);     // �A���x�h
Texture2D<float4> normalTexture : register(t1);     // �@��
Texture2D<float4> worldPosTexture : register(t2);   // ���[���h���W
sampler Sampler : register(s0);                     // �T���v���[

// �֐���`
float3 CalcLambertDiffuse(float3 direction, float3 color, float3 normal);
float3 CalcPhongSpecular(float3 direction, float3 color, float3 worldPos, float3 normal);
float3 CalcDirectionLight(float3 normal,float3 worldPos);
float3 CalcPointLight(float3 normal, float3 worldPos);
float3 CalcSpotLight(float3 normal, float3 worldPos);

// ���_�V�F�[�_�[
PSInput VSMain(VSInput vsIn)
{
    PSInput psIn;
    psIn.pos = mul(mvp, vsIn.pos);
    psIn.uv = vsIn.uv;
    return psIn;
}

// �s�N�Z���V�F�[�_�[
float4 PSMain(PSInput In) : SV_Target0
{
	// GBuffer�̓��e���g���ă��C�e�B���O-----------------------------------
    
    // �A���x�h
    float4 albedo = albedoTexture.Sample(Sampler, In.uv);
    
    // �@��
    float3 normal = normalTexture.Sample(Sampler, In.uv).xyz;
    normal = (normal * 2.0f) - 1.0f;
    
    // ���[���h���W
    float3 worldPos = worldPosTexture.Sample(Sampler, In.uv).xyz;

    // --------------------------------------------------------------------
    
    
    // �f�B���N�V�������C�g�̋����v�Z
    float3 dirLig = CalcDirectionLight(normal, worldPos);
    
    // �|�C���g���C�g�̋����ݒ�
    float3 ptLig = CalcPointLight(normal, worldPos);
    
    // �X�|�b�g���C�g�̋����ݒ�
    float3 spLig = CalcSpotLight(normal, worldPos);
    
    
    // �S�Ẵ��C�g�̉e���͂����߂�
    float3 lightPow = dirLig + ptLig + spLig;
    
    // ���C�g�̌����v�Z���ŏI�I�ȃJ���[��ݒ�    
    float4 finalColor = albedo;
    finalColor.xyz *= lightPow;
	
    return finalColor;
}

//////�R�R����֐�/////////////////////////////////////////////////////////////////////////////////////////////////////

//�g�U���˂̌v�Z
float3 CalcLambertDiffuse(float3 direction,float3 color,float3 normal)
{
    float power = max(0.0f, dot(normal, direction) * -1.0f);
    
    float3 lig = color * power;
    
    return lig;
}

//���ʔ��˂̌v�Z
float3 CalcPhongSpecular(float3 direction, float3 color, float3 worldPos, float3 normal)
{
    float3 toEye = normalize(eyePos - worldPos);
    
    float3 refVec = reflect(direction, normal);
    
    float power = max(0.0f, dot(toEye, refVec));
    power = pow(power, 5.0f);
    
    float3 lig = color * power;
    
    return lig;
}

//�f�B���N�V�������C�g�̌v�Z
float3 CalcDirectionLight(float3 normal, float3 worldPos)
{
    float3 lig;
    
    for (int i = 0; i < MAX_DIRECTION_LIGHT; i++)
    {
        lig += CalcLambertDiffuse(directionLight[i].dirDirection, directionLight[i].dirColor, normal);
        lig += CalcPhongSpecular(directionLight[i].dirDirection, directionLight[i].dirColor, worldPos, normal);
    }
    
    return lig;
}

//�|�C���g���C�g�ɂ�锽�ˌ����v�Z����
float3 CalcPointLight(float3 normal,float3 worldPos)
{
    float3 lig;
    
    for (int i = 0; i < MAX_POINT_LIGHT; i++)
    {
     	// ���̃T�[�t�F�C�X�ɓ��˂��Ă���|�C���g���C�g�̌��̌������v�Z����
        float3 ligDir = worldPos - pointLight[i].ptPosition;

        // ���K�����đ傫��1�̃x�N�g���ɂ���
        ligDir = normalize(ligDir);

        // �����Ȃ���Lambert�g�U���ˌ����v�Z����
        float3 diffPoint = CalcLambertDiffuse(ligDir, pointLight[i].ptColor, normal);

        // �����Ȃ���Phong���ʔ��ˌ����v�Z����
        float3 specPoint = CalcPhongSpecular(ligDir, pointLight[i].ptColor, worldPos, normal);

        // �����ɂ��e�������v�Z����
        // �|�C���g���C�g�Ƃ̋������v�Z����
        float3 distance = length(worldPos - pointLight[i].ptPosition);

        // �e�����͋����ɔ�Ⴕ�ď������Ȃ��Ă���
        float affect = 1.0f - 1.0f / pointLight[i].ptRange * distance;
        // �e���͂��}�C�i�X�ɂȂ�Ȃ��悤�ɕ␳��������
        if (affect < 0.0f)
        {
            affect = 0.0f;
        }

        // �e���̎d�����w���֐��I�ɂ���B����̃T���v���ł�3�悵�Ă���
        affect = pow(affect, 3.0f);

        // �g�U���ˌ��Ƌ��ʔ��ˌ��Ɍ���������Z���ĉe������߂�
        diffPoint *= affect;
        specPoint *= affect;
        
        // �ŏI�I�ɕԂ������ɑ����Ă���
        lig += diffPoint + specPoint;
    }

    return lig;
}

float3 CalcSpotLight(float3 normal,float3 worldPos)
{
    float3 lig = { 0.0f, 0.0f, 0.0f };
    
    for (int i = 0; i < MAX_SPOT_LIGHT; i++)
    {
        //�T�[�t�F�C�X�ɓ��˂���X�|�b�g���C�g�̌��̌������v�Z����
        float3 LigDir = worldPos - spotLight[i].spPosition;
	    //���K��
        LigDir = normalize(LigDir);
    
	    //�����Ȃ���Lambert�g�U���ˌ����v�Z����
        float3 diffSpot = CalcLambertDiffuse(LigDir, spotLight[i].spColor, normal);
    
	    //�����Ȃ���Phong���ʔ��˂̌v�Z
        float3 specSpot = CalcPhongSpecular(LigDir, spotLight[i].spColor, worldPos, normal);
    
	    //�X�|�b�g���C�g�Ƃ̋������v�Z����
        float distance = length(worldPos - spotLight[i].spPosition);
    
	    //�e�����͋����ɔ�Ⴕ�ď������Ȃ��Ă���
        float affect = 1.0f - 1.0f / spotLight[i].spRange * distance;
	    //�e���͂��}�C�i�X�ɂȂ�Ȃ��悤��
        if (affect < 0.0f)
        {
            affect = 0.0f;
        }
    
	    //�e�����w���֐��I�ɂ���
        affect = pow(affect, 3.0f);
    
	    //�g�U���ˌ��Ƌ��ʔ��ˌ��Ɍ���������Z���ĉe������߂�
        diffSpot *= affect;
        specSpot *= affect;
    
        // ��������͊p�x�ɂ��e���������߂�
        
	    //���ˌ��Ǝˏo�����̊p�x�����߂�
        float angle = dot(LigDir, spotLight[i].spDirection);
    
	    //dot()�ŋ��߂��l��acos()�ɓn���Ċp�x�����߂�
        angle = abs(acos(angle));
    
	    //�p�x�ɔ�Ⴕ�ď������Ȃ��Ă����e�������v�Z����
        affect = 1.0f - 1.0f / spotLight[i].spAngle * angle;
        if (affect < 0.0f)
        {
            affect = 0.0f;
        }
    
	    //�e�����w���֐��I�ɂ���
        affect = pow(affect, 0.5f);
    
	    //�p�x�ɂ��e�����𔽎ˌ��ɏ�Z���āA�e������߂�
        diffSpot *= affect;
        specSpot *= affect;

        //�ŏI�I�ɕԂ������ɑ����Ă���
        lig += diffSpot;
        lig += specSpot;

    }
	
    return lig;
}