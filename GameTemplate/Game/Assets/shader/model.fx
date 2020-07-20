/*!
 * @brief	���f���V�F�[�_�[�B
 */


/////////////////////////////////////////////////////////////
// Shader Resource View
/////////////////////////////////////////////////////////////
//�A���x�h�e�N�X�`���B
Texture2D<float4> albedoTexture : register(t0);	
//�{�[���s��
StructuredBuffer<float4x4> boneMatrix : register(t1);
Texture2D<float4> shadowMap : register(t2);		//todo �V���h�E�}�b�v�B
Texture2D<float4> cascadeShadowMap1 : register(t3);		//todo �J�X�P�[�h�V���h�E�}�b�v�B
Texture2D<float4> cascadeShadowMap2 : register(t4);		//todo �J�X�P�[�h�V���h�E�}�b�v�B
Texture2D<float4> cascadeShadowMap3 : register(t5);		//todo �J�X�P�[�h�V���h�E�}�b�v�B
Texture2D<float4> cascadeShadowMap4 : register(t6);		//todo �J�X�P�[�h�V���h�E�}�b�v�B
/////////////////////////////////////////////////////////////
// SamplerState
/////////////////////////////////////////////////////////////
sampler Sampler : register(s0);

/////////////////////////////////////////////////////////////
// �萔�o�b�t�@�B
/////////////////////////////////////////////////////////////
static const int NUM_CASCADES = 3;
/*!
 * @brief	���_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[�p�̒萔�o�b�t�@�B
 */
cbuffer VSPSCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
	//todo ���C�g�r���[�s���ǉ��B
	float4x4 mLightView;	//���C�g�r���[�s��B
	float4x4 mLightProj;	//���C�g�v���W�F�N�V�����s��B
	float4x4 mLightViewProj[NUM_CASCADES];    //���C�g�r���[�v���W�F�N�V�����s��
	float4 mFarList[NUM_CASCADES];
	int isShadowReciever;	//�V���h�E���V�[�o�[�t���O�B
};
/// <summary>
/// ���C�g�̐�
/// </summary>
static const int Light_number = 4;
/*
�f�B���N�V�������C�g
*/
struct SDirectionLight {
	//�f�B���N�V�������C�g�̃x�N�g��
	float4 Direction[Light_number];
	//�f�B���N�V�������C�g�̐F
	float4 Color[Light_number];
};
/// <summary>
/// ���C�g�p�̒萔�o�b�t�@
/// </summary>
cbuffer LightCB : register(b1) {
	SDirectionLight directionLight; //�f�B���N�V�������C�g
	float3          eyePos;         //�J�����̎��_
	float           specPow;        //�X�y�L�������C�g�̍i��
	float3          Amblight;
}

/////////////////////////////////////////////////////////////
//�e��\����
/////////////////////////////////////////////////////////////
/*!
 * @brief	�X�L���Ȃ����f���̒��_�\���́B
 */
struct VSInputNmTxVcTangent
{
    float4 Position : SV_Position;			//���_���W�B
    float3 Normal   : NORMAL;				//�@���B
    float3 Tangent  : TANGENT;				//�ڃx�N�g���B
    float2 TexCoord : TEXCOORD0;			//UV���W�B
};
/*!
 * @brief	�X�L�����胂�f���̒��_�\���́B
 */
struct VSInputNmTxWeights
{
    float4 Position : SV_Position;			//���_���W�B
    float3 Normal   : NORMAL;				//�@���B
    float2 TexCoord	: TEXCOORD0;			//UV���W�B
    float3 Tangent	: TANGENT;				//�ڃx�N�g���B
    uint4  Indices  : BLENDINDICES0;		//���̒��_�Ɋ֘A�t������Ă���{�[���ԍ��Bx,y,z,w�̗v�f�ɓ����Ă���B4�{�[���X�L�j���O�B
    float4 Weights  : BLENDWEIGHT0;			//���̒��_�Ɋ֘A�t������Ă���{�[���ւ̃X�L���E�F�C�g�Bx,y,z,w�̗v�f�ɓ����Ă���B4�{�[���X�L�j���O�B
};

/// <summary>
/// �V���h�E�}�b�v�p�̃s�N�Z���V�F�[�_�ւ̓��͍\���́B
/// </summary>
struct PSInput_ShadowMap {
	float4 Position 			: SV_POSITION;	//���W�B
};

/*!
 * @brief	�s�N�Z���V�F�[�_�[�̓��́B
 */
struct PSInput{
	float4 Position 	: SV_POSITION;
	float3 Normal		: NORMAL;
	float3 Tangent		: TANGENT;
	float2 TexCoord 	: TEXCOORD0;
	float3 worldPos		: TEXCOORD1;	//���[���h���W�B����U
	float4 posInLVP		: TEXCOORD2;	//���C�g�r���[�v���W�F�N�V������Ԃł̍��W
	float4 posInCamera  : TEXCOORD3;	//�r���[���W
	float4 posInLVP2[NUM_CASCADES]	: TEXCOORD4;
};
/*!
 *@brief	�X�L���s����v�Z�B
 */
float4x4 CalcSkinMatrix(VSInputNmTxWeights In)
{
	float4x4 skinning = 0;	
	float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
        w += In.Weights[i];
    }
    
    skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
    return skinning;
}
/*!--------------------------------------------------------------------------------------
 * @brief	�X�L���Ȃ����f���p�̒��_�V�F�[�_�[�B
-------------------------------------------------------------------------------------- */
PSInput VSMain( VSInputNmTxVcTangent In ) 
{
	PSInput psInput = (PSInput)0;
	float4 pos = mul(mWorld, In.Position);

	//���[���h���W���s�N�Z���V�F�[�_�ɓn���B
	psInput.worldPos = pos;
	if (isShadowReciever == 1) {
		float4 p = pos;
		p = mul(mLightView, p);
		psInput.posInLVP = mul(mLightProj, p);
		//���C�g�r���[�v���W�F�N�V�������W�ɕϊ�
		for (int i = 0; i < NUM_CASCADES; i++) {
			psInput.posInLVP2[i] = mul(mLightViewProj[i], pos);
		}
	}
	pos = mul(mView, pos);
	psInput.posInCamera = pos;
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.TexCoord = In.TexCoord;
	psInput.Normal = normalize(mul(mWorld, In.Normal));
	psInput.Tangent = normalize(mul(mWorld, In.Tangent));
	return psInput;
}

/*!--------------------------------------------------------------------------------------
 * @brief	�X�L�����胂�f���p�̒��_�V�F�[�_�[�B
 * �S�Ă̒��_�ɑ΂��Ă��̃V�F�[�_�[���Ă΂��B
 * In��1�̒��_�f�[�^�BVSInputNmTxWeights�����Ă݂悤�B
-------------------------------------------------------------------------------------- */
PSInput VSMainSkin( VSInputNmTxWeights In ) 
{
	PSInput psInput = (PSInput)0;
	
	///////////////////////////////////////////////////
	//��������X�L�j���O���s���Ă���ӏ��B
	//�X�L���s����v�Z�B
	///////////////////////////////////////////////////
	float4x4 skinning = 0;	
	float4 pos = 0;
	{
	
		float w = 0.0f;
	    for (int i = 0; i < 3; i++)
	    {
			//boneMatrix�Ƀ{�[���s�񂪐ݒ肳��Ă��āA
			//In.indices�͒��_�ɖ��ߍ��܂ꂽ�A�֘A���Ă���{�[���̔ԍ��B
			//In.weights�͒��_�ɖ��ߍ��܂ꂽ�A�֘A���Ă���{�[���̃E�F�C�g�B
	        skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
	        w += In.Weights[i];
	    }
	    //�Ō�̃{�[�����v�Z����B
	    skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
	  	//���_���W�ɃX�L���s�����Z���āA���_�����[���h��Ԃɕϊ��B
		//mul�͏�Z���߁B
	    pos = mul(skinning, In.Position);
		if (isShadowReciever == 1) {
			float4 p = pos;
			p = mul(mLightView, p);
			psInput.posInLVP = mul(mLightProj, p);
			//���C�g�r���[�v���W�F�N�V�������W�ɕϊ�
			for (int i = 0; i < NUM_CASCADES; i++) {
				psInput.posInLVP2[i] = mul(mLightViewProj[i], pos);
			}
		}
	}
	
	psInput.Normal = normalize( mul(skinning, In.Normal) );
	psInput.Tangent = normalize( mul(skinning, In.Tangent) );
	
	//���[���h���W���s�N�Z���V�F�[�_�ɓn���B
	psInput.worldPos = pos;

	pos = mul(mView, pos);
	psInput.posInCamera = pos;
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.TexCoord = In.TexCoord;
    return psInput;
}
//--------------------------------------------------------------------------------------
// �s�N�Z���V�F�[�_�[�̃G���g���֐��B
//--------------------------------------------------------------------------------------
float4 PSMain(PSInput In) : SV_Target0
{
	/// <summary>
	/// ���C�g�̃R�[�h
	/// </summary>
	//albedo�e�N�X�`������J���[���t�F�b�`����B
	//�g�U����
	float4 albedoColor = albedoTexture.Sample(Sampler, In.TexCoord);
	float3 lig = 0.0f;
    for (int i = 0; i < Light_number; i++) {
		float t = max( dot(In.Normal * -1.0f, directionLight.Direction[i].rgb), 0.0f );
		if (t < 0.2f) {
			lig += directionLight.Color[i] * 0.5f;
		}
		else {
			lig += directionLight.Color[i]*1.5f;
		}
	}
	for (int i = 0; i < Light_number; i++)
	////�f�B���N�V�������C�g�̋��ʔ��ˌ����v�Z����B
	{
		//���K�@���ʔ��˂��v�Z���Ȃ����B
		//�P�@���˃x�N�g��R�����߂�
		float3 R = directionLight.Direction[0].rgb
			+ 2 * dot(In.Normal, -directionLight.Direction[0].rgb)
			* In.Normal;
		//-toEyeDir + 2 * dot(In.Normal, -directionLight.direction) * In.NORMAL;
	//�Q�@���_���烉�C�g�𓖂Ă镨�̂ɐL�т�x�N�g��E�����߂�B
		float3 E = normalize(In.worldPos - eyePos);
		//�P�ƂQ�ŋ��܂����x�N�g���̓��ς��v�Z����B
		float specPower = max(0, dot(R, -E));
		//3 �X�y�L�������˂����C�g�ɒǉ�����B
		lig += directionLight.Color[0].xyz * pow(specPower, specPow);
	
	}

	
		//4�@�����𓖂Ă�B
	lig += Amblight;

	if (isShadowReciever == 1) {
		/*//LVP��Ԃ��猩�����̍ł���O�̐[�x�l���V���h�E�}�b�v����擾����B
		//�v���W�F�N�V�����s����V���h�E�}�b�v��UV���W�ɕϊ����Ă���
		float2 shadowMapUV = In.posInLVP.xy / In.posInLVP.w;
		shadowMapUV *= float2(0.5f, -0.5f);
		shadowMapUV += 0.5f;
		//spsOut.shadow = shadowMapUV.x;
		//�V���h�E�}�b�v��UV���W�͈͓����ǂ����𔻒肷��B
		if (shadowMapUV.x < 1.0f
			&& shadowMapUV.x > 0.0f
			&& shadowMapUV.y < 1.0f
			&& shadowMapUV.y > 0.0f
			) {
			///LVP��Ԃł̐[�x�l���v�Z�B
			float zInLVP = In.posInLVP.z / In.posInLVP.w;
			float zInShadowMap = 1.0f;
			//�V���h�E�}�b�v�ɏ������܂�Ă���[�x�l���擾�B
			zInShadowMap = shadowMap.Sample(Sampler, shadowMapUV);
			if (zInLVP > zInShadowMap + 0.0001f) {
				//�e�������Ă���̂ŁA�����キ����
				lig *= 0.5f;
			}
		}*/
		for (int i = 0; i < NUM_CASCADES; i++) {
			if (mFarList[i].x > In.posInCamera.z) {
				//LVP��Ԃ��猩�����̍ł���O�̐[�x�l���V���h�E�}�b�v����擾����B
				//�v���W�F�N�V�����s����V���h�E�}�b�v��UV���W�ɕϊ����Ă���
				float2 shadowMapUV = In.posInLVP2[i].xy / In.posInLVP2[i].w;
				shadowMapUV *= float2(0.5f, -0.5f);
				shadowMapUV += 0.5f;
				//spsOut.shadow = shadowMapUV.x;
				//�V���h�E�}�b�v��UV���W�͈͓����ǂ����𔻒肷��B
				if (shadowMapUV.x <= 1.0f
					&& shadowMapUV.x >= 0.0f
					&& shadowMapUV.y <= 1.0f
					&& shadowMapUV.y >= 0.0f
					) {
					///LVP��Ԃł̐[�x�l���v�Z�B
					float zInLVP = In.posInLVP2[i].z / In.posInLVP2[i].w;
					float zInShadowMap = 1.0f;
					//�V���h�E�}�b�v�ɏ������܂�Ă���[�x�l���擾�B
					if (i == 0) {
						zInShadowMap = cascadeShadowMap1.Sample(Sampler, shadowMapUV);
					}
					else if (i == 1) {
						zInShadowMap = cascadeShadowMap2.Sample(Sampler, shadowMapUV);
					}
					else if (i == 2) {
						zInShadowMap = cascadeShadowMap3.Sample(Sampler, shadowMapUV);
					}
					else if (i == 3) {
						zInShadowMap = cascadeShadowMap4.Sample(Sampler, shadowMapUV);
					}
					if (zInLVP > zInShadowMap + 0.0001f * 2.0f * (4 - i)) {
						//�e�������Ă���̂ŁA�����キ����
						lig *= 0.5f;

					}
					break;


				}
			}
		}
	}



	float4 finalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	finalColor.xyz = albedoColor.xyz * lig;

	return finalColor;
	//return albedoTexture.Sample(Sampler, In.TexCoord);
}
//-----------------------------------------------
// �V���G�b�g�`��p�̃s�N�Z���V�F�[�_�[�̃G���g���[�֐�
//-----------------------------------------------


// <summary>
/// �X�L�������V���h�E�}�b�v�����p�̒��_�V�F�[�_�[�B
/// </summary>
PSInput_ShadowMap VSMain_ShadowMap(VSInputNmTxVcTangent In)
{
	PSInput_ShadowMap psInput = (PSInput_ShadowMap)0;
	//���C�g�v���W�F�N�V�����s��ɕϊ����Ă���
	float4 pos = mul(mWorld, In.Position);
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	//pos = mul(mLightView, pos);
	//pos = mul(mLightProj, pos);
	psInput.Position = pos;
	return psInput;
}

// <summary>
/// �X�L������V���h�E�}�b�v�����p�̒��_�V�F�[�_�[�B
/// </summary>
PSInput_ShadowMap VSMainSkin_ShadowMap(VSInputNmTxWeights In)
{
	PSInput_ShadowMap psInput = (PSInput_ShadowMap)0;
	float4x4 skinning = CalcSkinMatrix(In);
	float4 pos = mul(skinning, In.Position);
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	//pos = mul(mLightView, pos);
	//pos = mul(mLightProj, pos);
	psInput.Position = pos;
	return psInput;
}

// <summary>
/// �X�L�������J�X�P�[�h�V���h�E�}�b�v�����p�̒��_�V�F�[�_�[�B
/// </summary>
PSInput_ShadowMap VSMain_CascadeShadowMap(VSInputNmTxVcTangent In)
{
	PSInput_ShadowMap psInput = (PSInput_ShadowMap)0;
	//���C�g�v���W�F�N�V�����s��ɕϊ����Ă���
	float4 pos = mul(mWorld, In.Position);
	pos = mul(mLightViewProj[0], pos);
	psInput.Position = pos;
	return psInput;
}

// <summary>
/// �X�L������J�X�P�[�h�V���h�E�}�b�v�����p�̒��_�V�F�[�_�[�B
/// </summary>
PSInput_ShadowMap VSMainSkin_CascadeShadowMap(VSInputNmTxWeights In)
{
	PSInput_ShadowMap psInput = (PSInput_ShadowMap)0;
	float4x4 skinning = CalcSkinMatrix(In);
	float4 pos = mul(skinning, In.Position);
	pos = mul(mLightViewProj[0], pos);
	psInput.Position = pos;
	return psInput;
}

/// <summary>
/// �s�N�Z���V�F�[�_�[�̃G���g���֐��B
/// </summary>
float4 PSMain_ShadowMap(PSInput_ShadowMap In) : SV_Target0
{
	//�ˉe��Ԃł�Z�l��Ԃ��B
	return In.Position.z / In.Position.w;
}
