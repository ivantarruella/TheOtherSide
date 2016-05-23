/*

% Description of my shader.
% Second line of description for my shader.

keywords: material classic

date: YYMMDD

*/

#include "functions.fx"

float4 g_FogColor = float4(0.2,0.2,0.2,0.25);
float g_ExpDensityFog = 1000.0;

float4 CalcExpFog(float Depth, float ExpDensityFog, float4 FogColor)
{
	const float LOG2E = 1.442695; // = 1 / log(2)
	float l_Fog = exp2(-ExpDensityFog * Depth * LOG2E);
	return float4(FogColor.xyz,FogColor.a*(1.0-l_Fog));
}

float4 RenderQuadFogPS(in float2 UV : TEXCOORD0) : COLOR
{
	float4 l_DepthColor=tex2D(gNormalMapSampler, UV);
	float3 l_ViewPosition=GetPositionFromZDepthViewInViewCoordinates(l_DepthColor, UV);
	float l_Depth=length(l_ViewPosition);

	return CalcExpFog(l_Depth, g_ExpDensityFog, g_FogColor);
}


technique DrawQuadFogTechnique
{
	pass p0
	{
		AlphaBlendEnable = true;
		BlendOp=Add;
		SrcBlend=SrcAlpha;
		DestBlend=InvSrcAlpha;
		PixelShader=compile ps_3_0 RenderQuadFogPS();
	}
}