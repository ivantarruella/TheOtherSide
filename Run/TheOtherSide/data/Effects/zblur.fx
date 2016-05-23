/*

% Description of my shader.
% Second line of description for my shader.

keywords: material classic

date: YYMMDD

*/

#include "functions.fx"

float g_ZBlurFocalStart=1;
float g_ZBlurFocalEnd=15;
float g_ZBlurEnd=20;
float g_ConstantBlur=0.4;


float4 ZBlurPS(in float2 UV : TEXCOORD0) : COLOR
{
	float4 l_DepthMap=tex2D(gDepthMapSampler,UV);
	float3 l_CameraPosition=g_CameraPosition;

	//float3 l_WorldPosition=GetPositionFromZDepthView(l_DepthMap, float2(0,0));
	float3 l_WorldPosition=GetPositionFromZDepthView(l_DepthMap.x, UV).xyz;
	
	float l_Distance=length(l_WorldPosition-l_CameraPosition);
	float4 l_Color=float4(0,0,0,0);
	float l_Blur=1.0;
	if(l_Distance<g_ZBlurFocalStart)
		l_Blur=max(l_Distance/g_ZBlurFocalStart, g_ConstantBlur);
	else if(l_Distance>g_ZBlurFocalEnd)
		l_Blur=max(1.0-(l_Distance-g_ZBlurFocalEnd)/g_ZBlurEnd, g_ConstantBlur);
	//return float4(l_Blur,l_Blur,l_Blur,1.0);
	
	const float2 delta[8] =
	{
		float2(-1,1),float2(1,-1),float2(-1,1),float2(1,1),
		float2(-1,0),float2(1,0),float2(0,-1),float2(0,1)
	};
	float2 l_PixelInc=4*1/g_RenderTargetSize; //4 pixeles a la redonda
	float4 l_AlbedoColor=tex2D(gDiffuseSampler,UV);
	for( int i=0;i<8;i++ )
		l_Color += tex2D(gDiffuseSampler,UV + delta[i]*l_PixelInc)*(1-l_Blur)+l_Blur*l_AlbedoColor;
	l_Color = l_Color*(1.0/8.0);
	return l_Color;
}

technique ZBlurTechnique
{
	pass p0
	{
		AlphaBlendEnable = false;
		CullMode = CCW;
		PixelShader = compile ps_3_0 ZBlurPS();
	}
}
