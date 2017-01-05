//Define global
#define OMNI 0
#define DIRECTIONAL 1
#define SPOT 2

#define MAX_NUM_LIGHTS 	4
#define MAX_LIGHTS		(MAX_NUM_LIGHTS - 2)		
#define PI_CONSTANT		3.1415926535
#define MAXBONES		29

#define SHADOW_SM_EPSILON   0.090f 
#define SHADOW_VSM_EPSILON  0.00005f 
	
#define SMAP_SIZE 1024

#define zOffset 0.5f 

// GLOBAL SHADERS DATA
float g_SpecularPower = 200.0;
float g_Bump = 2.4;
	
float4x4 g_WorldView 		: WORLDVIEW;
float4x4 g_WorldViewProj 	: WORLDVIEWPROJECTION;
float4x4 g_WorldMatrix 		: WORLD;
float4x4 g_ProjMatrix 		: PROJECTION;
float4x4 g_InverseProj 		: PROJECTIONINVERSE;
float4x4 g_InverseWorld     : WORLDINVERSE;
float4x4 g_InverseWorldTrans: WORLDINVERSETRANSP;
float4x4 g_InverseView 		: VIEWINVERSE;
float4x4 g_ViewToLightProj 	: VIEWTOLIGHT;
float4x4 g_ViewProjInverse  : VIEWPROJECTIONINVERSE;
float4x4 g_ViewMatrix   	: VIEW;
float3 	g_AmbientLight		: AMBIENTLIGHT;

float3 g_CameraPosition : WORLD_CAMERA_POSITION;

int g_LightsTypes[MAX_LIGHTS] : LIGHTS_TYPE;
float3 g_LightsPosition[MAX_LIGHTS] : LIGHTS_POSITION;
float3 g_LightsDirection[MAX_LIGHTS] : LIGHTS_DIRECTION;
float3 g_LightsColor[MAX_LIGHTS] : LIGHTS_COLOR;
float g_LightStartAtten[MAX_LIGHTS] : CONSTANTATTENUATION;
float g_LightEndAtten[MAX_LIGHTS] : LINEARATTENUATION;
float g_LightAngle[MAX_LIGHTS] : FALLOFFANGLE;
float g_LightFallOff[MAX_LIGHTS] : FALLOFFEXPONENT;

float3x4 g_Bones[MAXBONES] : BONES;

float g_UseDynamicShadowMap : DYNAMICSHADOWMAP;
float g_UseStaticShadowMap : STATICSHADOWMAP;
float g_UseShadowMapMask : SHADOWMAPMASK;

float g_UseDynamicShadowMap2 : DYNAMICSHADOWMAP2;
float g_UseStaticShadowMap2 : STATICSHADOWMAP2;
float g_UseShadowMapMask2 : SHADOWMAPMASK2;

float g_Time : TIME;
float2 g_RenderTargetSize : RENDERTARGETSIZE;
float g_MirrorDarkCoef : MIRRORDARKCOEF;
float g_MirrorAnimCoef : MIRRORANIMCOEF;
float g_MirrorReflectCoef = 0.9;

static const int    MAX_SAMPLES            = 16;    // Maximum texture grabs


// Gaussian blur
static const int BLUR_KERNEL_SIZE	 	   	= 13;
float PixelKernel[BLUR_KERNEL_SIZE] 		=
{
   -6,
   -5,
   -4,
   -3,
   -2,
   -1,
    0,
    1,
    2,
    3,
    4,
    5,
    6,
};

static const float BlurWeights2[BLUR_KERNEL_SIZE] =
{
   0.002216,
   0.008764,
   0.026995,
   0.064759,
   0.120985,
   0.176033,
   0.199471,
   0.176033,
   0.120985,
   0.064759,
   0.026995,
   0.008764,
   0.002216,
};


// G-BUFFER
struct TMultiRenderTargetPixel
{
	float4 RT0 : COLOR0; //Albedo (float3) + (float) GlowFactor
	float4 RT1 : COLOR1; //AmbientLight (float3) + (float) SpecularPow
	float4 RT2 : COLOR2; //Normal (float3) + (float) SpecularFactor
	float4 RT3 : COLOR3; //Depth (float4)
};

// VERTEX TYPES

struct TNORMAL_TEXTURE1_VERTEX_IN
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
};

struct TNORMAL_TEXTURE1_VERTEX_OUT
{
	float4 HPosition : POSITION;
	float2 UV : TEXCOORD0;
	float3 WorldNormal : TEXCOORD1;
	float4 WorldPosition : TEXCOORD2;
};

struct TNORMAL_TEXTURE1_TEXTURE2_VERTEX_IN
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
	float2 UV2 : TEXCOORD1;
};

struct TNORMAL_TEXTURE1_TEXTURE2_VERTEX_OUT
{
	float4 HPosition : POSITION;
	float2 UV : TEXCOORD0;
	float2 UV2 : TEXCOORD1;
	float3 WorldNormal : TEXCOORD2;
	float4 WorldPosition : TEXCOORD3;
};


struct TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX_IN 
{
	float3 Position : POSITION;
	float4 Normal : NORMAL;
	float4 Tangent : TANGENT0;
	float4 BiNormal : BINORMAL0;
	float2 TexCoord : TEXCOORD0;
};

struct TNORMAL_TANGENT_BINORMAL_TEXTURE1_TEXTURE2_VERTEX_IN 
{
	float3 Position : POSITION;
	float4 Normal : NORMAL;
	float4 Tangent : TANGENT0;
	float4 BiNormal : BINORMAL0;
	float2 TexCoord : TEXCOORD0;
	float2 TexCoord2 : TEXCOORD1;
};

struct TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX_OUT
{
	float4 HPosition : POSITION;
	float2 UV : TEXCOORD0;
	float3 WorldNormal : TEXCOORD1;
	float4 WorldPosition : TEXCOORD2;
	float3 WorldTangent : TEXCOORD3;
	float3 WorldBinormal : TEXCOORD4;
};

struct TNORMAL_TANGENT_BINORMAL_TEXTURE1_TEXTURE2_VERTEX_OUT
{
	float4 HPosition : POSITION;
	float2 UV : TEXCOORD0;
	float2 UV2 : TEXCOORD1;
	float3 WorldNormal : TEXCOORD2;
	float4 WorldPosition : TEXCOORD3;
	float3 WorldTangent : TEXCOORD4;
	float3 WorldBinormal : TEXCOORD5;
};

struct CAL3D_HW_VERTEX_VS {
	float3 Position : POSITION;
	float4 Weight : BLENDWEIGHT;
	float4 Indices : BLENDINDICES;
	float4 Normal : NORMAL;
	float4 Tangent : TANGENT0;
	float4 BiNormal : BINORMAL0;
	float2 TexCoord : TEXCOORD0;
};

struct CAL3D_HW_VERTEX_PS
{
	float4 HPosition : POSITION;
	float2 UV : TEXCOORD0;
	float3 WorldNormal : TEXCOORD1;
	float4 WorldPosition : TEXCOORD2;
	float3 WorldTangent : TEXCOORD3;
	float3 WorldBinormal : TEXCOORD4;
};

// SAMPLERS

sampler gDiffuseSampler : register( s0 ) = sampler_state 
{
	MagFilter = Linear;
	MinFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
	AddressV = Wrap;
	AddressW = Wrap;
};

sampler gLightmapSampler : register( s1 ) = sampler_state 
{
	MagFilter = Linear;
	MinFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
	AddressV = Wrap;
	AddressW = Wrap;
};

sampler gNormalMapSampler : register( s2 ) = sampler_state 
{
	MagFilter = Linear;
	MinFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
	AddressV = Wrap;
	AddressW = Wrap;
};

sampler gDepthMapSampler : register( s3 ) = sampler_state 
{
	MagFilter = Linear;
	MinFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
	AddressV = Wrap;
	AddressW = Wrap;
};

sampler gDynamicShadowMapTextureSampler : register( s6 ) = sampler_state
{
	MipFilter = Linear;
	MinFilter = Linear;
	MagFilter = Linear;
	AddressU = Clamp;
	AddressV = Clamp;
};

sampler gStaticShadowMapTextureSampler : register( s7 ) = sampler_state
{
	MipFilter = Linear;
	MinFilter = Linear;
	MagFilter = Linear;
	AddressU = Clamp;
	AddressV = Clamp;
};

sampler gShadowMapMaskTextureSampler : register( s8 ) = sampler_state
{
	MipFilter = Linear;
	MinFilter = Linear;
	MagFilter = Linear;
	AddressU = Clamp;
	AddressV = Clamp;
};

samplerCUBE gCubeTextureSampler : register( s9 ) = sampler_state
{
	MipFilter = NONE;
	MinFilter = NONE;
	MagFilter = NONE;
	AddressU = Wrap;
	AddressV = Wrap;
};

sampler gDynamicShadowMapTextureSampler2 : register( s10 ) = sampler_state
{
	MipFilter = Linear;
	MinFilter = Linear;
	MagFilter = Linear;
	AddressU = Clamp;
	AddressV = Clamp;
};

sampler gStaticShadowMapTextureSampler2 : register( s11 ) = sampler_state
{
	MipFilter = Linear;
	MinFilter = Linear;
	MagFilter = Linear;
	AddressU = Clamp;
	AddressV = Clamp;
};

sampler gShadowMapMaskTextureSampler2 : register( s12 ) = sampler_state
{
	MipFilter = Linear;
	MinFilter = Linear;
	MagFilter = Linear;
	AddressU = Clamp;
	AddressV = Clamp;
};

samplerCUBE gCubeTextureSampler2 : register( s13 ) = sampler_state
{
	MipFilter = NONE;
	MinFilter = NONE;
	MagFilter = NONE;
	AddressU = Wrap;
	AddressV = Wrap;
};

sampler gS0LinearWrapSampler : register( s0 ) = sampler_state 
{
	MagFilter = Linear;
	MinFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
	AddressV = Wrap;
	AddressW = Wrap;
};

sampler gS1LinearWrapSampler : register( s1 ) = sampler_state 
{
	MagFilter = Linear;
	MinFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
	AddressV = Wrap;
	AddressW = Wrap;
};

sampler gS2LinearWrapSampler : register( s2 ) = sampler_state 
{
	MagFilter = Linear;
	MinFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
	AddressV = Wrap;
	AddressW = Wrap;
};

sampler gS3LinearWrapSampler : register( s3 ) = sampler_state 
{
	MagFilter = Linear;
	MinFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
	AddressV = Wrap;
	AddressW = Wrap;
};
sampler gS4LinearWrapSampler : register( s4 ) = sampler_state 
{
	MagFilter = Linear;
	MinFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
	AddressV = Wrap;
	AddressW = Wrap;
};
sampler gS5LinearWrapSampler : register( s5 ) = sampler_state 
{
	MagFilter = Linear;
	MinFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
	AddressV = Wrap;
	AddressW = Wrap;
};
sampler gS6LinearWrapSampler : register( s6 ) = sampler_state 
{
	MagFilter = Linear;
	MinFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
	AddressV = Wrap;
	AddressW = Wrap;
};
sampler gS7LinearWrapSampler : register( s7 ) = sampler_state 
{
	MagFilter = Linear;
	MinFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
	AddressV = Wrap;
	AddressW = Wrap;
};


sampler gS0LinearClampSampler : register( s0 ) = sampler_state 
{
	MagFilter = Linear;
	MinFilter = Linear;
	MipFilter = Linear;
	AddressU = Clamp;
	AddressV = Clamp;
	AddressW = Clamp;
};

sampler gS1LinearClampSampler : register( s1 ) = sampler_state 
{
	MagFilter = Linear;
	MinFilter = Linear;
	MipFilter = Linear;
	AddressU = Clamp;
	AddressV = Clamp;
	AddressW = Clamp;
};

sampler gS2LinearClampSampler : register( s2 ) = sampler_state 
{
	MagFilter = Linear;
	MinFilter = Linear;
	MipFilter = Linear;
	AddressU = Clamp;
	AddressV = Clamp;
	AddressW = Clamp;
};

sampler gS3LinearClampSampler : register( s3 ) = sampler_state 
{
	MagFilter = Linear;
	MinFilter = Linear;
	MipFilter = Linear;
	AddressU = Clamp;
	AddressV = Clamp;
	AddressW = Clamp;
};
sampler gS4LinearClampSampler : register( s4 ) = sampler_state 
{
	MagFilter = Linear;
	MinFilter = Linear;
	MipFilter = Linear;
	AddressU = Clamp;
	AddressV = Clamp;
	AddressW = Clamp;
};
sampler gS5LinearClampSampler : register( s5 ) = sampler_state 
{
	MagFilter = Linear;
	MinFilter = Linear;
	MipFilter = Linear;
	AddressU = Clamp;
	AddressV = Clamp;
	AddressW = Clamp;
};
sampler gS6LinearClampSampler : register( s6 ) = sampler_state 
{
	MagFilter = Linear;
	MinFilter = Linear;
	MipFilter = Linear;
	AddressU = Clamp;
	AddressV = Clamp;
	AddressW = Clamp;
};
sampler gS7LinearClampSampler : register( s7 ) = sampler_state 
{
	MagFilter = Linear;
	MinFilter = Linear;
	MipFilter = Linear;
	AddressU = Clamp;
	AddressV = Clamp;
	AddressW = Clamp;
};

sampler PrevFilterSampler : register(s0) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = CLAMP;
  AddressV  = CLAMP;
};

sampler PostFilterSampler : register(s1) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = CLAMP;
  AddressV  = CLAMP;
};


