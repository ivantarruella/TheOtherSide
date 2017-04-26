/*

% Description of my shader.
% Second line of description for my shader.

keywords: material classic

date: YYMMDD

*/

#include "functions.fx"

// Vertex shaders
TNORMAL_TEXTURE1_VERTEX_OUT DeferredShadingNormalTexturedVS(TNORMAL_TEXTURE1_VERTEX_IN IN)
{
	TNORMAL_TEXTURE1_VERTEX_OUT l_OUT=(TNORMAL_TEXTURE1_VERTEX_OUT)0;
	
	l_OUT.HPosition = mul(float4(IN.Position.xyz,1.0), g_WorldViewProj);
	l_OUT.UV = IN.UV;
	l_OUT.WorldNormal = normalize(mul(IN.Normal,(float3x3)g_WorldMatrix));
	l_OUT.WorldPosition = l_OUT.HPosition;
	
	return l_OUT;
}

TNORMAL_TEXTURE1_TEXTURE2_VERTEX_OUT DeferredShadingLightmapTexturedVS(TNORMAL_TEXTURE1_TEXTURE2_VERTEX_IN IN)
{
	TNORMAL_TEXTURE1_TEXTURE2_VERTEX_OUT l_OUT=(TNORMAL_TEXTURE1_TEXTURE2_VERTEX_OUT)0;
	
	l_OUT.HPosition = mul(float4(IN.Position.xyz,1.0), g_WorldViewProj);
	l_OUT.UV = IN.UV;
	l_OUT.UV2 = IN.UV2;
	l_OUT.WorldNormal = normalize(mul(IN.Normal,(float3x3)g_WorldMatrix));
	l_OUT.WorldPosition = l_OUT.HPosition;

	return l_OUT;
}


TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX_OUT DeferredShadingBumpmapTexturedVS(TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX_IN IN)
{
	TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX_OUT l_OUT=(TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX_OUT)0;
	
	l_OUT.HPosition = mul(float4(IN.Position.xyz,1.0), g_WorldViewProj);
	l_OUT.UV=IN.TexCoord;
	l_OUT.WorldNormal = normalize(mul(IN.Normal,(float3x3)g_WorldMatrix));
	l_OUT.WorldPosition=l_OUT.HPosition;
	l_OUT.WorldTangent = mul(IN.Tangent.xyz,(float3x3)g_WorldMatrix);
	l_OUT.WorldBinormal = mul(cross(IN.Tangent.xyz,IN.Normal),(float3x3)g_WorldMatrix);	
	
	return l_OUT;
}

TNORMAL_TANGENT_BINORMAL_TEXTURE1_TEXTURE2_VERTEX_OUT DeferredShadingLightmapBumpmapTexturedVS(TNORMAL_TANGENT_BINORMAL_TEXTURE1_TEXTURE2_VERTEX_IN IN)
{
	TNORMAL_TANGENT_BINORMAL_TEXTURE1_TEXTURE2_VERTEX_OUT l_OUT=(TNORMAL_TANGENT_BINORMAL_TEXTURE1_TEXTURE2_VERTEX_OUT)0;
	
	l_OUT.HPosition = mul(float4(IN.Position.xyz,1.0), g_WorldViewProj);
	l_OUT.UV=IN.TexCoord;
	l_OUT.UV2 = IN.TexCoord2;	
	l_OUT.WorldNormal = normalize(mul(normalize(IN.Normal),(float3x3)g_WorldMatrix));
	l_OUT.WorldPosition=l_OUT.HPosition;
	l_OUT.WorldTangent = normalize(mul(normalize(IN.Tangent.xyz),(float3x3)g_WorldMatrix));
	l_OUT.WorldBinormal = mul(normalize(cross(normalize(IN.Tangent.xyz),normalize(IN.Normal))),(float3x3)g_WorldMatrix);	
	
	return l_OUT;
}

TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX_OUT DeferredShadingParallaxTexturedVS(TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX_IN IN)
{
	TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX_OUT l_OUT=(TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX_OUT)0;
	
	l_OUT.HPosition = mul(float4(IN.Position.xyz,1.0), g_WorldViewProj);
	l_OUT.WorldNormal = normalize(mul(IN.Normal,(float3x3)g_WorldMatrix));
	l_OUT.UV=IN.TexCoord;
	l_OUT.WorldPosition=l_OUT.HPosition;
	l_OUT.WorldTangent = mul(IN.Tangent.xyz,(float3x3)g_WorldMatrix);
	l_OUT.WorldBinormal = mul(cross(IN.Tangent.xyz,IN.Normal),(float3x3)g_WorldMatrix);	
	
	return l_OUT;
}

// Pixel shaders
TMultiRenderTargetPixel DeferredShadingNormalTexturedPS(TNORMAL_TEXTURE1_VERTEX_OUT IN)
{
	float4 l_Diffuse = tex2D(gDiffuseSampler, IN.UV);
	float3 l_Albedo = l_Diffuse.xyz;
	float l_GlowFactor = l_Diffuse.a;
	float3 l_Ambient=g_AmbientLight;
	float3 l_NormalMap=normalize(IN.WorldNormal);
	float4 l_Depth=IN.WorldPosition.z/IN.WorldPosition.w;
	float l_SpecularFactor=0;
	float l_SpecularPower=g_SpecularPower;
	
	return calcTargetPixelDark(l_Albedo, l_Ambient, l_NormalMap, l_Depth, l_SpecularFactor, l_SpecularPower, l_GlowFactor);
}

TMultiRenderTargetPixel DeferredShadingLightmapTexturedPS(TNORMAL_TEXTURE1_TEXTURE2_VERTEX_OUT IN)
{
	float4 l_Diffuse = tex2D(gDiffuseSampler, IN.UV);
	float3 l_Albedo = l_Diffuse.xyz;
	float l_GlowFactor = l_Diffuse.a;
	float3 l_Ambient=tex2D(gLightmapSampler, IN.UV2).xyz;
	float3 l_NormalMap=normalize(IN.WorldNormal);
	float4 l_Depth=IN.WorldPosition.z/IN.WorldPosition.w;
	float l_SpecularFactor=1;
	float l_SpecularPower=g_SpecularPower;
	
	return calcTargetPixel(l_Albedo, l_Ambient, l_NormalMap, l_Depth, l_SpecularFactor, l_SpecularPower, l_GlowFactor);
}


TMultiRenderTargetPixel DeferredShadingBumpmapTexturedPS(TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX_OUT IN)
{
	float4 l_Diffuse = tex2D(gDiffuseSampler, IN.UV)*float4(0.7,0.7,0.7,1.0);
	float3 l_Albedo = l_Diffuse.xyz;
	float l_GlowFactor = l_Diffuse.a;
	float3 l_Ambient=g_AmbientLight;
	float3 l_NormalMap = CalcBumpMap(IN.WorldNormal, IN.WorldTangent, IN.WorldBinormal, IN.UV);
	float4 l_Depth=IN.WorldPosition.z/IN.WorldPosition.w;
	float l_SpecularFactor=tex2D(gLightmapSampler, IN.UV).a;
	float l_SpecularPower=g_SpecularPower;
	
	return calcTargetPixel(l_Albedo, l_Ambient, l_NormalMap, l_Depth, l_SpecularFactor, l_SpecularPower, l_GlowFactor);
}

TMultiRenderTargetPixel DeferredShadingLightmapBumpmapTexturedPS(TNORMAL_TANGENT_BINORMAL_TEXTURE1_TEXTURE2_VERTEX_OUT IN)
{
	float4 l_Diffuse = tex2D(gDiffuseSampler, IN.UV);
	float3 l_Albedo = l_Diffuse.xyz;
	float l_GlowFactor = l_Diffuse.a;
	float3 l_Ambient=tex2D(gLightmapSampler, IN.UV2).xyz;
	float3 l_NormalMap = CalcBumpMapSampled(IN.WorldNormal, IN.WorldTangent, IN.WorldBinormal, IN.UV, gNormalMapSampler);
	float4 l_Depth=IN.WorldPosition.z/IN.WorldPosition.w;
	float l_SpecularFactor=tex2D(gNormalMapSampler, IN.UV).a;
	float l_SpecularPower=g_SpecularPower;
	
	return calcTargetPixel(l_Albedo, l_Ambient, l_NormalMap, l_Depth, l_SpecularFactor, l_SpecularPower, l_GlowFactor);
}

TMultiRenderTargetPixel DeferredShadingParallaxTexturedPS(TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX_OUT IN)
{
	float3 l_Ambient=g_AmbientLight;

	float2 OutUV = (float2)0;
	float3 ViewWS = g_CameraPosition - IN.WorldPosition.xyz;
	float3 l_NormalMap = CalcParallaxMap(ViewWS, IN.WorldNormal, IN.WorldTangent, IN.WorldBinormal, IN.UV, OutUV);
	float4 l_Diffuse = tex2D(gDiffuseSampler, IN.UV);
	float3 l_Albedo=l_Diffuse.xyz;
	float l_GlowFactor=l_Diffuse.a;
	float4 l_Depth=IN.WorldPosition.z/IN.WorldPosition.w;
	float l_SpecularFactor=tex2D(gLightmapSampler, IN.UV).a;
	float l_SpecularPower=g_SpecularPower;
	
	return calcTargetPixel(l_Albedo, l_Ambient, l_NormalMap, l_Depth, l_SpecularFactor, l_SpecularPower, l_GlowFactor);
}

TMultiRenderTargetPixel DeferredShadingRNMSpecTexturedPS(TNORMAL_TANGENT_BINORMAL_TEXTURE1_TEXTURE2_VERTEX_OUT IN)
{
	float4 l_Diffuse = tex2D(gS0LinearWrapSampler, IN.UV);
	float3 l_Albedo=l_Diffuse.xyz;
	float l_GlowFactor=l_Diffuse.a;
	float2 l_UV2= IN.UV2;
	float3 l_NormalMap = CalcBumpMapSampled(IN.WorldNormal, IN.WorldTangent, IN.WorldBinormal, IN.UV, gS5LinearWrapSampler);
	float3 l_Ambient=GetRadiosityNormalMap(l_NormalMap, IN.WorldNormal, IN.WorldTangent, IN.WorldBinormal, l_UV2);
	float4 l_Depth=IN.WorldPosition.z/IN.WorldPosition.w;
	float l_SpecularFactor=tex2D(gS5LinearWrapSampler, IN.UV).a;
	float l_SpecularPower=g_SpecularPower;

	return calcTargetPixel(l_Albedo, l_Ambient, l_NormalMap, l_Depth, l_SpecularFactor, l_SpecularPower, l_GlowFactor);
}



float4 DrawQuad_PS(in float2 UV : TEXCOORD0) : COLOR
{
	float3 l_Albedo = tex2D(gDiffuseSampler,UV).xyz;
	float3 l_Ambient = tex2D(gLightmapSampler,UV).xyz;
	float l_GlowFactor = tex2D(gDiffuseSampler,UV).a;
	
	return float4(l_Albedo * l_Ambient, l_GlowFactor);
}

float4 RenderLightPassPS(in float2 UV : TEXCOORD0) : COLOR
{
	float4 l_Albedo = tex2D(gDiffuseSampler,UV);
	float4 l_LightmapSampler = tex2D(gLightmapSampler,UV);
	float4 l_NormalMapSampler = tex2D(gNormalMapSampler,UV);
	
	float l_ZDepthView = tex2D(gDepthMapSampler,UV).x;	
	float4 l_Position = GetPositionFromZDepthView(l_ZDepthView, UV);
	float3 l_Nn = normalize(Texture2Normal(l_NormalMapSampler.xyz));

	float l_SpecularFactor = l_NormalMapSampler.a;				
	float l_SpecularPower = l_LightmapSampler.a * 255;

	return calcDeferredLighting(l_Position, l_Nn, l_Albedo, l_SpecularFactor, l_SpecularPower);
}



////////////////////////////////////////////////////////////////////////////////////////
// Techniques

technique GenerateDeferredShadingNormalTexturedTechnique
{
	pass p0
	{
		VertexShader=compile vs_3_0 DeferredShadingNormalTexturedVS();
		PixelShader=compile ps_3_0 DeferredShadingNormalTexturedPS();
	}
}

technique GenerateDeferredShadingLightmapTexturedTechnique
{
	pass p0
	{
		VertexShader=compile vs_3_0 DeferredShadingLightmapTexturedVS();
		PixelShader=compile ps_3_0 DeferredShadingLightmapTexturedPS();
	}
}

technique GenerateDeferredShadingBumpmapTexturedTechnique
{
	pass p0
	{
		VertexShader=compile vs_3_0 DeferredShadingBumpmapTexturedVS();
		PixelShader=compile ps_3_0 DeferredShadingBumpmapTexturedPS();
	}
}

technique GenerateDeferredShadingLightmapBumpmapTexturedTechnique
{
	pass p0
	{
		VertexShader=compile vs_3_0 DeferredShadingLightmapBumpmapTexturedVS();
		PixelShader=compile ps_3_0 DeferredShadingLightmapBumpmapTexturedPS();
	}
}

technique GenerateDeferredShadingParallaxTexturedTechnique
{
	pass p0
	{
		VertexShader=compile vs_3_0 DeferredShadingParallaxTexturedVS();
		PixelShader=compile ps_3_0 DeferredShadingParallaxTexturedPS();
	}
}

technique GenerateDeferredShadingRNMSpecTechnique
{
	pass p0
	{
		VertexShader=compile vs_3_0 DeferredShadingLightmapBumpmapTexturedVS();
		PixelShader=compile ps_3_0 DeferredShadingRNMSpecTexturedPS();
	}
}

technique DrawAmbientLightTechnique
{
	pass p0
	{
		AlphaBlendEnable = false;
		CullMode = CCW;
		PixelShader = compile ps_3_0 DrawQuad_PS();
	}
}


technique RenderLightPassTechnique
{
	pass p0
	{
		AlphaBlendEnable = true;
		BlendOp=Add;
		SrcBlend = one;
		DestBlend = one;

		PixelShader = compile ps_3_0 RenderLightPassPS();
	}
}

