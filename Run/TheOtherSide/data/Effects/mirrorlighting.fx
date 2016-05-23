/*

% Description of my shader.
% Second line of description for my shader.

keywords: material classic

date: YYMMDD

*/

#include "functions.fx"

TNORMAL_TEXTURE1_VERTEX_OUT NormalTexture_VS(TNORMAL_TEXTURE1_VERTEX_IN IN)
{
	TNORMAL_TEXTURE1_VERTEX_OUT l_Out = (TNORMAL_TEXTURE1_VERTEX_OUT) 0;
	
	l_Out.HPosition = mul(float4(IN.Position.xyz, 1.0), g_WorldViewProj);
	l_Out.UV = IN.UV;
	l_Out.WorldNormal = normalize(mul(IN.Normal,(float3x3)g_WorldMatrix));
	l_Out.WorldPosition = mul(float4(IN.Position.xyz,1.0), g_WorldMatrix);
	return l_Out;
}


TNORMAL_TEXTURE1_TEXTURE2_VERTEX_OUT LightmapTexture_VS(TNORMAL_TEXTURE1_TEXTURE2_VERTEX_IN IN)
{
	TNORMAL_TEXTURE1_TEXTURE2_VERTEX_OUT l_Out = (TNORMAL_TEXTURE1_TEXTURE2_VERTEX_OUT) 0;
	
	l_Out.HPosition = mul(float4(IN.Position.xyz, 1.0), g_WorldViewProj);
	l_Out.UV = IN.UV;
	l_Out.UV2 = IN.UV2;
	l_Out.WorldNormal = normalize(mul(IN.Normal,(float3x3)g_WorldMatrix));
	l_Out.WorldPosition = mul(float4(IN.Position.xyz,1.0), g_WorldMatrix);
	return l_Out;
}

TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX_OUT BumpmapTexture_VS(TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX_IN IN)
{
	TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX_OUT l_Out = (TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX_OUT) 0;
	
	l_Out.HPosition = mul(float4(IN.Position.xyz, 1.0), g_WorldViewProj);
	l_Out.UV = IN.TexCoord;

	l_Out.WorldNormal = normalize(mul(IN.Normal,(float3x3)g_WorldMatrix));
	l_Out.WorldPosition = mul(float4(IN.Position.xyz,1.0), g_WorldMatrix);
	
	l_Out.WorldTangent = mul(IN.Tangent.xyz,(float3x3)g_WorldMatrix);
	l_Out.WorldBinormal = mul(cross(IN.Tangent.xyz,IN.Normal),(float3x3)g_WorldMatrix);
	return l_Out;
}

TNORMAL_TANGENT_BINORMAL_TEXTURE1_TEXTURE2_VERTEX_OUT LightmapBumpmapTextured_VS(TNORMAL_TANGENT_BINORMAL_TEXTURE1_TEXTURE2_VERTEX_IN IN)
{
	TNORMAL_TANGENT_BINORMAL_TEXTURE1_TEXTURE2_VERTEX_OUT l_OUT=(TNORMAL_TANGENT_BINORMAL_TEXTURE1_TEXTURE2_VERTEX_OUT)0;
	
	l_OUT.HPosition = mul(float4(IN.Position.xyz,1.0), g_WorldViewProj);
	l_OUT.UV=IN.TexCoord;
	l_OUT.UV2 = IN.TexCoord2;	
	l_OUT.WorldNormal = normalize(mul(normalize(IN.Normal),(float3x3)g_WorldMatrix));
	l_OUT.WorldPosition = mul(float4(IN.Position.xyz,1.0), g_WorldMatrix);
	l_OUT.WorldTangent = normalize(mul(normalize(IN.Tangent.xyz),(float3x3)g_WorldMatrix));
	l_OUT.WorldBinormal = mul(normalize(cross(normalize(IN.Tangent.xyz),normalize(IN.Normal))),(float3x3)g_WorldMatrix);	
	
	return l_OUT;
}


// Mirror pixel shaders

float4 MirrorNormalTexture_PS(TNORMAL_TEXTURE1_VERTEX_OUT IN) : COLOR 
{
	float4 l_Albedo=tex2D(gDiffuseSampler,IN.UV);
	l_Albedo.rgb = ChangeWorld_PS(l_Albedo.rgb, gDiffuseSampler, IN.UV.xy);

	float l_Alpha = l_Albedo.a;
	float3 Nn=normalize(IN.WorldNormal);
	float4 l_Lighting = calcLighting(IN.WorldPosition.xyz, Nn, l_Albedo,1.0);

	float3 l_Light = float3(0,0,0);
	if (g_MirrorDarkCoef>=0.85)
		l_Light = ((l_Albedo.rgb * 1.0-l_Alpha) + l_Lighting.rgb) * g_MirrorDarkCoef;		// mundo espejo
	else
	{
		l_Light = ((g_AmbientLight * l_Albedo.rgb * 2) + l_Lighting.rgb);	// mundo real
		if (g_MirrorDarkCoef<0.50)
		{
			l_Light = l_Light * g_MirrorDarkCoef;
		}		
	}	

	return float4 (l_Light, l_Alpha );
}

float4 MirrorLightmapTexture_PS(TNORMAL_TEXTURE1_TEXTURE2_VERTEX_OUT IN) : COLOR 
{
	float4 l_Albedo=tex2D(gDiffuseSampler,IN.UV);
	l_Albedo.rgb = ChangeWorld_PS(l_Albedo.rgb, gDiffuseSampler, IN.UV.xy);
	
	float4 l_AlbedoLightmap=tex2D(gLightmapSampler,IN.UV2);
	float3 Nn=normalize(IN.WorldNormal);
	float4 l_Lighting=calcLighting(IN.WorldPosition.xyz, Nn, l_Albedo,1.0);
	
	return float4 ((((l_AlbedoLightmap.rgb * l_Albedo.rgb)+ l_Lighting.rgb) * g_MirrorDarkCoef) * g_MirrorReflectCoef, l_Albedo.a * l_AlbedoLightmap.a);
}

float4 MirrorBumpmapTexture_PS(TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX_OUT IN) : COLOR
{
	float4 l_Albedo=tex2D(gDiffuseSampler,IN.UV);
	l_Albedo.rgb = ChangeWorld_PS(l_Albedo.rgb, gDiffuseSampler, IN.UV.xy);
	
	float4 l_NormalMap=tex2D(gNormalMapSampler,IN.UV);	
	float3 Nn = CalcBumpMap(IN.WorldNormal, IN.WorldTangent, IN.WorldBinormal, IN.UV);
	float4 l_Lighting = calcLighting(IN.WorldPosition, Nn, l_Albedo, 0.0/*l_NormalMap.a*/);
	
	float3 l_Light = ((g_AmbientLight * l_Albedo.rgb) + l_Lighting.rgb);

	return float4 (l_Light*g_MirrorDarkCoef * g_MirrorReflectCoef, l_Albedo.a);
}

float4 MirrorLightmapBumpmapTextured_PS(TNORMAL_TANGENT_BINORMAL_TEXTURE1_TEXTURE2_VERTEX_OUT IN) : COLOR
{
	float4 l_Albedo=tex2D(gDiffuseSampler,IN.UV);
	l_Albedo.rgb = ChangeWorld_PS(l_Albedo.rgb, gDiffuseSampler, IN.UV.xy);
	
	float4 l_AlbedoLightmap=tex2D(gLightmapSampler,IN.UV2)*2;
	float3 Nn = CalcBumpMap(IN.WorldNormal, IN.WorldTangent, IN.WorldBinormal, IN.UV);
	float4 l_NormalMap=tex2D(gNormalMapSampler,IN.UV);	
	float4 l_Lighting=calcLighting(IN.WorldPosition.xyz, Nn, l_Albedo,0.0/*l_NormalMap.a*/);

	return float4 (((l_AlbedoLightmap.rgb * l_Albedo.rgb) + l_Lighting.rgb) * g_MirrorDarkCoef * g_MirrorReflectCoef, l_Albedo.a * l_AlbedoLightmap.a);
}

float4 MirrorRNMSpecTextured_PS(TNORMAL_TANGENT_BINORMAL_TEXTURE1_TEXTURE2_VERTEX_OUT IN) : COLOR
{
	float3 l_Albedo=tex2D(gS0LinearWrapSampler, IN.UV).xyz;
	l_Albedo.rgb = ChangeWorld_PS(l_Albedo.rgb, gS0LinearWrapSampler, IN.UV.xy);
	
	float l_Alpha=tex2D(gS0LinearWrapSampler, IN.UV).a;
	float3 l_NormalMap = CalcBumpMapSampled(IN.WorldNormal, IN.WorldTangent, IN.WorldBinormal, IN.UV, gS5LinearWrapSampler);
	float3 l_SpecMap = tex2D(gS5LinearWrapSampler, IN.UV).a;
	float3 l_AlbedoRNM=GetRadiosityNormalMap(l_NormalMap, IN.WorldNormal, IN.WorldTangent, IN.WorldBinormal, IN.UV2);	
	float4 l_Lighting=calcLighting(IN.WorldPosition.xyz, l_NormalMap, float4(l_Albedo,l_Alpha),l_SpecMap);

	return float4 (((l_AlbedoRNM.rgb * l_Albedo.rgb) + l_Lighting.rgb) * g_MirrorDarkCoef * g_MirrorReflectCoef, l_Alpha);
}


// Mirror techniques

technique MirrorNormalVertexTechnique 
{
	pass p0 
	{
		//CullMode = None;
		AlphaBlendEnable = false;
		VertexShader = compile vs_3_0 NormalTexture_VS();
		PixelShader = compile ps_3_0 MirrorNormalTexture_PS();
	}
}

technique MirrorLightmapVertexTechnique 
{
	pass p0 
	{
		//CullMode = None;
		AlphaBlendEnable = true;
		VertexShader = compile vs_3_0 LightmapTexture_VS();
		PixelShader = compile ps_3_0 MirrorLightmapTexture_PS();
	}
}

technique MirrorBumpmapVertexTechnique 
{
	pass p0 
	{
		//CullMode = None;
		AlphaBlendEnable = true;
		VertexShader = compile vs_3_0 BumpmapTexture_VS();
		PixelShader = compile ps_3_0 MirrorBumpmapTexture_PS();
	}
}

technique MirrorLightmapBumpmapVertexTechnique 
{
	pass p0 
	{
		//CullMode = None;
		AlphaBlendEnable = true;
		VertexShader = compile vs_3_0 LightmapBumpmapTextured_VS();
		PixelShader = compile ps_3_0 MirrorLightmapBumpmapTextured_PS();
	}
}


technique MirrorRNMSpecVertexTechnique 
{
	pass p0 
	{
		//CullMode = None;
		AlphaBlendEnable = true;
		VertexShader = compile vs_3_0 LightmapBumpmapTextured_VS();
		PixelShader = compile ps_3_0 MirrorRNMSpecTextured_PS();
	}
}
