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

/*
float4 ejemplosPS(TVertexOut IN) : COLOR 
{
	//float4 l_Albedo=tex2D(gDiffuseSampler,IN.UV);
	//float4 l_AlbedoLightmap=tex2D(gLightmapSampler,IN.UV2)*2;
	//return l_Albedo;			// devuelve textura difuso
	//return l_AlbedoLightmap;	// devuelve textura lightmap
	
	//float dif=saturate(dot(Nn, -g_LightsDirection[0]));
	//return float4(dif,dif,dif,1.0);		// devuelve intensidad de luz, sin color
	
	//float l_Att=getAttenuation(OMNI, g_LightsPosition[0], IN.Pos.xyz, g_LightStartAtten[0], g_LightEndAtten[0]);
	//return float4(l_Att,l_Att,l_Att,1.0);	// devuelve atenuación de luz
	
	//float3 l_LightDir=normalize(g_LightsPosition[0]-IN.Pos.xyz);
	//l_Attenuation = getAttenuation(DIRECTIONAL, g_LightsPosition[0], IN.Pos.xyz, g_LightStartAtten[0], g_LightEndAtten[0], g_LightsDirection[0], g_LightAngle[0], g_LightFallOff[0]);
	//float l_DiffContrib = saturate(dot(Nn, -g_LightsDirection[0])); //* color;
	//float l_DiffContrib=saturate(dot(Nn, l_LightDir));
	//return float4(g_LightsColor[0]*l_DiffContrib*l_Albedo.rgb*l_Attenuation,1.0);
}
*/

// Forward render pixel shaders

float4 NormalTexture_PS(TNORMAL_TEXTURE1_VERTEX_OUT IN) : COLOR 
{
	float4 l_Albedo=tex2D(gDiffuseSampler,IN.UV);
	float3 Nn=normalize(IN.WorldNormal);
	return calcLighting(IN.WorldPosition.xyz, Nn, l_Albedo,1.0);
}

float4 LightmapTexture_PS(TNORMAL_TEXTURE1_TEXTURE2_VERTEX_OUT IN) : COLOR 
{
	float4 l_Albedo=tex2D(gDiffuseSampler,IN.UV);
	float4 l_AlbedoLightmap=tex2D(gLightmapSampler,IN.UV2);
	float3 Nn=normalize(IN.WorldNormal);
	float4 l_Lighting=calcLighting(IN.WorldPosition.xyz, Nn, l_Albedo,1.0);

	return float4 ((l_AlbedoLightmap.rgb * l_Albedo.rgb) + l_Lighting.rgb, l_Albedo.a * l_AlbedoLightmap.a);
}

float4 BumpmapTexture_PS(TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX_OUT IN) : COLOR
{
	float4 l_Albedo=tex2D(gDiffuseSampler,IN.UV);
	float4 l_NormalMap=tex2D(gNormalMapSampler,IN.UV);	
	float3 Nn = CalcBumpMap(IN.WorldNormal, IN.WorldTangent, IN.WorldBinormal, IN.UV);

	return calcLighting(IN.WorldPosition, Nn, l_Albedo, l_NormalMap.a);
}


float4 ParallaxTexture_PS(TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX_OUT IN) : COLOR
{
	float2 l_OutUV=(float2)0;
	float3 Nn = CalcParallaxMap(IN.WorldPosition, IN.WorldNormal, IN.WorldTangent, IN.WorldBinormal, IN.UV, l_OutUV);
	float4 l_Albedo=tex2D(gDiffuseSampler,IN.UV);
	float4 l_NormalMap=tex2D(gNormalMapSampler,l_OutUV);

	return calcLighting(IN.WorldPosition, Nn, l_Albedo, l_NormalMap.a);
}

float4 LightmapBumpmapTextured_PS(TNORMAL_TANGENT_BINORMAL_TEXTURE1_TEXTURE2_VERTEX_OUT IN) : COLOR
{
	float4 l_Albedo=tex2D(gDiffuseSampler,IN.UV);
	float4 l_AlbedoLightmap=tex2D(gLightmapSampler,IN.UV2);
	float3 Nn = CalcBumpMap(IN.WorldNormal, IN.WorldTangent, IN.WorldBinormal, IN.UV);
	float4 l_NormalMap=tex2D(gNormalMapSampler,IN.UV);	
	float4 l_Lighting=calcLighting(IN.WorldPosition.xyz, Nn, l_Albedo,l_NormalMap.a);

	return float4 ((l_AlbedoLightmap.rgb * l_Albedo.rgb) + l_Lighting.rgb, l_Albedo.a * l_AlbedoLightmap.a);
}

float4 RNMSpecTextured_PS(TNORMAL_TANGENT_BINORMAL_TEXTURE1_TEXTURE2_VERTEX_OUT IN) : COLOR
{
	float3 l_Albedo=tex2D(gS0LinearWrapSampler, IN.UV).xyz;
	float l_Alpha=tex2D(gS0LinearWrapSampler, IN.UV).a;
	float3 l_NormalMap = CalcBumpMapSampled(IN.WorldNormal, IN.WorldTangent, IN.WorldBinormal, IN.UV, gS5LinearWrapSampler);
	float3 l_SpecMap = tex2D(gS5LinearWrapSampler, IN.UV).a;
	float3 l_AlbedoRNM=GetRadiosityNormalMap(l_NormalMap, IN.WorldNormal, IN.WorldTangent, IN.WorldBinormal, IN.UV2);	
	float4 l_Lighting=calcLighting(IN.WorldPosition.xyz, l_NormalMap, float4(l_Albedo,l_Alpha),l_SpecMap)*0.75;

	return float4 ((l_AlbedoRNM.rgb * l_Albedo.rgb) + l_Lighting.rgb, l_Alpha);
}


// Techniques

technique NormalVertexTechnique 
{
	pass p0 
	{
		//CullMode = None;
		AlphaBlendEnable = true;
		VertexShader = compile vs_3_0 NormalTexture_VS();
		PixelShader = compile ps_3_0 NormalTexture_PS();
	}
}

technique LightmapVertexTechnique 
{
	pass p0 
	{
		//CullMode = None;
		AlphaBlendEnable = true;
		VertexShader = compile vs_3_0 LightmapTexture_VS();
		PixelShader = compile ps_3_0 LightmapTexture_PS();
	}
}

technique BumpmapVertexTechnique 
{
	pass p0 
	{
		//CullMode = None;
		AlphaBlendEnable = true;
		VertexShader = compile vs_3_0 BumpmapTexture_VS();
		PixelShader = compile ps_3_0 BumpmapTexture_PS();
	}
}

technique ParallaxVertexTechnique 
{
	pass p0 
	{
		//CullMode = None;
		AlphaBlendEnable = true;
		VertexShader = compile vs_3_0 BumpmapTexture_VS();
		PixelShader = compile ps_3_0 ParallaxTexture_PS();
	}
}

technique LightmapBumpmapVertexTechnique 
{
	pass p0 
	{
		//CullMode = None;
		AlphaBlendEnable = true;
		VertexShader = compile vs_3_0 LightmapBumpmapTextured_VS();
		PixelShader = compile ps_3_0 LightmapBumpmapTextured_PS();
	}
}


technique RNMSpecVertexTechnique 
{
	pass p0 
	{
		//CullMode = None;
		AlphaBlendEnable = true;
		VertexShader = compile vs_3_0 LightmapBumpmapTextured_VS();
		PixelShader = compile ps_3_0 RNMSpecTextured_PS();
	}
}


