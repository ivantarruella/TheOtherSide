/*

% Description of my shader.
% Second line of description for my shader.

keywords: material classic

date: YYMMDD

*/

#include "functions.fx"

//Vertex Shader

void VertShadowStaticMeshes(float4 Pos : POSITION, float3 Normal : NORMAL, out float4 oPos : POSITION, out float2 Depth : TEXCOORD1, out float2 UV_out : TEXCOORD0, in float2 UV_in : TEXCOORD0 )
{
	//
	// Compute the projected coordinates
	//
	oPos = mul( Pos, g_WorldViewProj );
	//
	// Store z and w in our spare texcoord
	//
	Depth.xy = oPos.zw;
	
	UV_out = UV_in;
}

void VertShadowAnimatedModels(CAL3D_HW_VERTEX_VS IN, out float4 oPos : POSITION, out float2 Depth : TEXCOORD1)
{
	float3 l_Position=CalcAnimtedPos(float4(IN.Position.xyz,1.0), IN.Indices, IN.Weight);
	
	oPos = mul(float4(l_Position, 1.0), g_WorldViewProj );

	//
	// Store z and w in our spare texcoord
	//
	Depth.xy = oPos.zw;
}
/*
void VertShadowStaticMeshes2(float4 Pos : POSITION, out float4 oPos : POSITION, out float3 lightVect : TEXCOORD0, out float2 UV_out : TEXCOORD1, in float2 UV_in : TEXCOORD1 )
{
	//
	// Compute the projected coordinates
	//
	oPos = mul( Pos, g_WorldViewProj );

	lightVect = g_LightsPosition[0] - oPos.xyz;
	
	UV_out = UV_in;
}

void VertShadowAnimatedModels2(CAL3D_HW_VERTEX_VS IN, out float4 oPos : POSITION, out float3 lightVect : TEXCOORD0)
{
	float3 l_Position=CalcAnimtedPos(float4(IN.Position.xyz,1.0), IN.Indices, IN.Weight);
	
	oPos = mul(float4(l_Position, 1.0), g_WorldViewProj );

	lightVect = g_LightsPosition[0] - oPos.xyz;
}
*/

//Pixel Shader

float4 PixShadow(float2 Depth : TEXCOORD1, float2 UV_in : TEXCOORD0) : COLOR
{
	if (tex2D(gDiffuseSampler, UV_in).a < 0.5)
		discard;
	 
	float d = Depth.x / Depth.y;
	float moment1 = d;
	float moment2 = d * d;

	// Adjusting moments (this is sort of bias per pixel) using partial derivative
	float dx = ddx(d);
	float dy = ddy(d);
	moment2 += 0.25 * (dx * dx + dy * dy) ;

    return float4(moment1, moment2, 0, 1.0f);
}
/*
float4 PixShadow2(float3 lightVect : TEXCOORD0, float2 UV_in : TEXCOORD1) : COLOR0
{
	if (tex2D(gDiffuseSampler, UV_in).a < 0.5)
		discard;
	 
	return length(lightVect) + 0.5f;
}
*/
// Techniques

technique ShadowVertexTechniqueStaticMeshes 
{
	pass p0 
	{
		//CullMode = None;
		VertexShader = compile vs_3_0 VertShadowStaticMeshes();
		PixelShader = compile ps_3_0 PixShadow();
	}
}

technique ShadowVertexTechniqueAnimatedModels
{
	pass p0 
	{
		//CullMode = None;
		VertexShader = compile vs_3_0 VertShadowAnimatedModels();
		PixelShader = compile ps_3_0 PixShadow();
	}
}
