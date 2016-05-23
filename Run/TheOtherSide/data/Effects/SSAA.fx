/*

% Description of my shader.
% Second line of description for my shader.

keywords: material classic

date: YYMMDD

*/

#include "functions.fx"

float g_SSAAWeight=0.8;

float4 SSAAPS(in float2 UV : TEXCOORD0) : COLOR
{
	const float2 delta[8] =
	{
		float2(-1,1),float2(1,-1),float2(-1,1),float2(1,1),
		float2(-1,0),float2(1,0),float2(0,-1),float2(0,1)
	};
	
	float3 tex = Texture2Normal(tex2D(gNormalMapSampler,UV).xyz);
	float factor = 0.0f;
	for( int i=0;i<4;i++ )
	{
		float3 t = Texture2Normal(tex2D(gNormalMapSampler, UV+ delta[i] * 1/g_RenderTargetSize).xyz);
		t -= tex;
		factor += dot(t,t);
	}
	factor = min(1.0,factor)*g_SSAAWeight;
	//return float4(factor,factor,factor,1.0);
	
	float4 color = float4(0.0,0.0,0.0,0.0);
	float4 l_AlbedoColor=tex2D(gDiffuseSampler,UV);
	for( int i=0;i<8;i++ )
		color += tex2D(gDiffuseSampler,UV +	delta[i]*(1/g_RenderTargetSize))*factor+(1-factor)*l_AlbedoColor;
	
	color += 2.0*l_AlbedoColor;
	color = color*(1.0/10);
	
	return color;
}

technique SSAATechnique
{
	pass p0
	{
		AlphaBlendEnable = false;
		CullMode = CCW;
		PixelShader = compile ps_3_0 SSAAPS();
	}
}
