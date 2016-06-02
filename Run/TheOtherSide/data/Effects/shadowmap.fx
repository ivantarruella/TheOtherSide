/*

% Description of my shader.
% Second line of description for my shader.

keywords: material classic

date: YYMMDD

*/

#include "functions.fx"

//Vertex Shader

void VertShadowStaticMeshes(float4 Pos : POSITION, float3 Normal : NORMAL, out float4 oPos : POSITION, out float3 lightVec :
TEXCOORD2, out float2 Depth : TEXCOORD1, out float2 UV_out : TEXCOORD0, in float2 UV_in : TEXCOORD0 )
{
	//
	// Compute the projected coordinates
	//
	oPos = mul( Pos, g_WorldViewProj );
	//
	// Store z and w in our spare texcoord
	//
	Depth.xy = oPos.zw;
	
    float4 positionW = mul( Pos, g_WorldMatrix );
	lightVec = g_LightsPosition[0] - positionW.xyz;	
	
	UV_out = UV_in;
}

void VertShadowAnimatedModels(CAL3D_HW_VERTEX_VS IN, out float4 oPos : POSITION, out float2 Depth : TEXCOORD1,
out float3 lightVec : TEXCOORD2)
{
	float3 l_Position=CalcAnimtedPos(float4(IN.Position.xyz,1.0), IN.Indices, IN.Weight);
	
	oPos = mul(float4(l_Position, 1.0), g_WorldViewProj );

	//
	// Store z and w in our spare texcoord
	//
	Depth.xy = oPos.zw;
	
    float4 positionW = mul( float4(IN.Position, 1.0), g_WorldMatrix );
	lightVec = g_LightsPosition[0] - positionW.xyz;		
}

//Pixel Shader
#if defined( VARIANCE_SHADOW_MAP_ENABLED )
void PixShadow( float2 Depth : TEXCOORD1, float3 lightVec : TEXCOORD2, float2 UV_in : TEXCOORD0, out float4 Color : COLOR )
{
	// pixels with more then 50% alpha don't cast shadow
	if (tex2D(gDiffuseSampler, UV_in).a < 0.5)
		discard;
		
	if (g_LightsTypes[0] == SPOT) {                           
		float d = Depth.x / Depth.y;
		float moment1 = d;
		float moment2 = d * d;
		float dx = ddx(d);
		float dy = ddy(d);
		moment2 += 0.25 * (dx * dx + dy * dy) ;
		Color = float4(moment1, moment2, 0, 1.0f);	
	}
	
	//if (g_LightsTypes[0] == OMNI) 
	//	TODO!
}
#else	// NORMAL SHADOW MAP
void PixShadow( float2 Depth : TEXCOORD1, float3 lightVec : TEXCOORD2, float2 UV_in : TEXCOORD0, out float4 Color : COLOR )
{
	// pixels with more then 50% alpha don't cast shadow
	if (tex2D(gDiffuseSampler, UV_in).a < 0.5)
		discard;
		
	if (g_LightsTypes[0] == SPOT)                            
		Color = Depth.x / Depth.y;
	if (g_LightsTypes[0] == OMNI) 
		Color = length(lightVec) + zOffset;
}
#endif

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
