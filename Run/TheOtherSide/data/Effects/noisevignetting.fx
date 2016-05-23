/*

% Description of my shader.
% Second line of description for my shader.

keywords: material classic

date: YYMMDD

*/

#include "globals.h"


float4 RenderNoiseAndVignettingPS(in float2 UV : TEXCOORD0) : COLOR
{
	float2 l_Offset=float2(cos(g_Time),sin(g_Time));
	float2 l_UV=UV+l_Offset;
	float4 l_VignettingColor=tex2D(gLightmapSampler, UV);
	float4 l_NoiseColor=tex2D(gDiffuseSampler, l_UV);
	return float4(l_NoiseColor.xyz*l_VignettingColor.xyz, l_NoiseColor.a+l_VignettingColor.a);
}


float4 RenderNoisePS(in float2 UV : TEXCOORD0) : COLOR
{
	float2 l_Offset=float2(cos(g_Time),sin(g_Time));
	float2 l_UV=UV+l_Offset;
	float4 l_NoiseColor=tex2D(gDiffuseSampler, l_UV);
	return float4(l_NoiseColor.xyz, l_NoiseColor.a);
}

float4 RenderVignettingPS(in float2 UV : TEXCOORD0) : COLOR
{
	float4 l_VignettingColor=tex2D(gLightmapSampler, UV);
	return float4(l_VignettingColor.xyz, l_VignettingColor.a);
}


////////////////////////////////////////////////////////////////////////////////////////
// Techniques
/*
technique RenderNoiseAndVignettingTechnique
{
	pass p0
	{
		AlphaBlendEnable = true;
		BlendOp=Add;
		SrcBlend = one;
		DestBlend = one;
		PixelShader = compile ps_3_0 RenderNoiseAndVignettingPS();
	}
}
*/

technique RenderNoiseAndVignettingTechnique
{
	pass p0
	{
		AlphaBlendEnable = true;
		BlendOp=Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		PixelShader = compile ps_3_0 RenderNoiseAndVignettingPS();
	}
}
