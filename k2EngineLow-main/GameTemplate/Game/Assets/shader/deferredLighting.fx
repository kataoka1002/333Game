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

struct HemLight
{
    float3 skyColor;        //��̐F
    float3 groundColor;     //�n�ʂ̐F
    float3 groundNormal;    //�n�ʂ̖@��
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
    HemLight hemLight;
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
Texture2D<float4> albedoTexture : register(t0);             // �A���x�h
Texture2D<float4> normalAndSpecularTexture : register(t1);  // �@��
Texture2D<float4> worldPosTexture : register(t2);           // ���[���h���W
Texture2D<float4> normalInViewTexture : register(t3);       // �J������Ԃ̖@��
sampler Sampler : register(s0);                             // �T���v���[

// �֐���`
float3 CalcLambertDiffuse(float3 direction, float3 color, float3 normal);
float3 CalcPhongSpecular(float3 direction, float3 color, float3 worldPos, float3 normal, float speclarPow);
float3 CalcDirectionLight(float3 normal,float3 worldPos,float specularPow);
float3 CalcPointLight(float3 normal, float3 worldPos, float specularPow);
float3 CalcSpotLight(float3 normal, float3 worldPos, float specularPow);
float3 CalcLimPower(float3 normal, float3 normalInView);
float3 CalcHemLight(float3 normal);

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
    float3 normal = normalAndSpecularTexture.Sample(Sampler, In.uv).xyz;
    normal = (normal * 2.0f) - 1.0f;
    
    // �X�y�L�������x
    float speclarPow = normalAndSpecularTexture.Sample(Sampler, In.uv).w;
    
    // ���[���h���W
    float3 worldPos = worldPosTexture.Sample(Sampler, In.uv).xyz;
    
    // �J������Ԃ̖@��
    float3 normalInView = normalInViewTexture.Sample(Sampler, In.uv).xyz;

    // --------------------------------------------------------------------
    
    
    // �f�B���N�V�������C�g�̋����v�Z
    float3 dirLig = CalcDirectionLight(normal, worldPos, speclarPow);
    
    // �|�C���g���C�g�̋����ݒ�
    float3 ptLig = CalcPointLight(normal, worldPos, speclarPow);
    
    // �X�|�b�g���C�g�̋����ݒ�
    float3 spLig = CalcSpotLight(normal, worldPos, speclarPow);
    
    //�������C�g�̋����ݒ�
    float3 limLig = CalcLimPower(normal, normalInView);
    
    //�������C�g�̋����ݒ�
    float3 hemLig = CalcHemLight(normal);
    
    
    // �S�Ẵ��C�g�̉e���͂����߂�
    float3 lightPow = dirLig + ptLig + spLig + limLig + hemLig;
    
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
float3 CalcPhongSpecular(float3 direction, float3 color, float3 worldPos, float3 normal, float speclarPow)
{
    float3 toEye = normalize(eyePos - worldPos);
    
    float3 refVec = reflect(direction, normal);
    
    float power = max(0.0f, dot(toEye, refVec));
    power = pow(power, 5.0f);
    
    float3 lig = color * power * speclarPow;
    
    return lig;
}

//�f�B���N�V�������C�g�̌v�Z
float3 CalcDirectionLight(float3 normal, float3 worldPos, float specularPow)
{
    float3 lig;
    
    for (int i = 0; i < MAX_DIRECTION_LIGHT; i++)
    {
        lig += CalcLambertDiffuse(directionLight[i].dirDirection, directionLight[i].dirColor, normal);
        lig += CalcPhongSpecular(directionLight[i].dirDirection, directionLight[i].dirColor, worldPos, normal,specularPow);
    }
    
    return lig;
}

//�|�C���g���C�g�̌v�Z
float3 CalcPointLight(float3 normal,float3 worldPos,float specularPow)
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
        float3 specPoint = CalcPhongSpecular(ligDir, pointLight[i].ptColor, worldPos, normal, specularPow);

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

//�X�|�b�g���C�g�̌v�Z
float3 CalcSpotLight(float3 normal,float3 worldPos, float specularPow)
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
        float3 specSpot = CalcPhongSpecular(LigDir, spotLight[i].spColor, worldPos, normal, specularPow);
    
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

//�������C�g�̌v�Z
float3 CalcLimPower(float3 normal,float3 normalInView)
{
    float3 lig = { 0.0f, 0.0f, 0.0f };
    
    for (int i = 0; i < MAX_DIRECTION_LIGHT; i++)
    {
        //�T�[�t�F�C�X�̖@���ƌ��̓��˕����Ɉˑ����郊���̋��������߂�
        float power1 = 1.0f - max(0.0f, dot(directionLight[i].dirDirection, normal));
        
	    //�T�[�t�F�C�X�̖@���Ǝ����̕����Ɉˑ����郊���̋��������߂�
        float power2 = 1.0f - max(0.0f, normalInView.z * -1.0f);
        
	    //�ŏI�I�ȃ����̋��������߂�
        float limPower = power1 * power2;
        
	    //pow()���g�p�������̕ω����w���֐��I�ɂ���
        limPower = pow(limPower, 1.3f);
        
        //�������C�g�̃J���[���v�Z�������Ă���
        lig += directionLight[i].dirColor * limPower;
    }

    return lig;
}

//�������C�g�̌v�Z
float3 CalcHemLight(float3 normal)
{
	//�T�[�t�F�C�X�̖@���ƒn�ʂ̖@���Ƃ̓��ς��v�Z����
    float t = dot(normal, hemLight.groundNormal);
    
	//���ς̌��ʂ�0�`1�͈̔͂ɕϊ�
    t = (t + 1.0f) / 2.0f;
    
	//�n�ʂƓV���F��⊮��t�Ő��`�⊮���A�Ԃ�
    return lerp(hemLight.groundColor, hemLight.skyColor, t);
}

