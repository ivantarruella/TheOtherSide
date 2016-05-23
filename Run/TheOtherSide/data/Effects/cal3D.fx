/*

% Description of my shader.
% Second line of description for my shader.

keywords: material classic

date: YYMMDD

*/

#include "functions.fx"

CAL3D_HW_VERTEX_PS RenderCal3DHWVSDeferred(CAL3D_HW_VERTEX_VS IN)
{
	CAL3D_HW_VERTEX_PS OUT=(CAL3D_HW_VERTEX_PS)0;
	
	float3 l_Normal= 0;
	float3 l_Tangent=0;
	
	CalcAnimatedNormalTangent(normalize(IN.Normal.xyz), IN.Tangent.xyz, IN.Indices, IN.Weight, l_Normal, l_Tangent);
	float3 l_Position=CalcAnimtedPos(float4(IN.Position.xyz,1.0), IN.Indices, IN.Weight);

	float4 l_WorldPosition=float4(l_Position, 1.0);

	OUT.WorldNormal.xyz=normalize(mul(l_Normal.xyz,(float3x3)g_WorldMatrix));
	OUT.WorldTangent=normalize(mul(l_Tangent,g_WorldMatrix));
	OUT.WorldBinormal=mul(cross(l_Tangent,l_Normal),(float3x3)g_WorldMatrix);

	OUT.UV = IN.TexCoord.xy;
	
	OUT.HPosition = mul(l_WorldPosition, g_WorldViewProj );
	OUT.WorldPosition=OUT.HPosition;

	return OUT;
}

CAL3D_HW_VERTEX_PS RenderCal3DHWVSForward(CAL3D_HW_VERTEX_VS IN)
{
	CAL3D_HW_VERTEX_PS OUT=(CAL3D_HW_VERTEX_PS)0;
	
	float3 l_Normal= 0;
	float3 l_Tangent=0;
	
	CalcAnimatedNormalTangent(normalize(IN.Normal.xyz), IN.Tangent.xyz, IN.Indices, IN.Weight, l_Normal, l_Tangent);
	float3 l_Position=CalcAnimtedPos(float4(IN.Position.xyz,1.0), IN.Indices, IN.Weight);

	float4 l_WorldPosition=float4(l_Position, 1.0);

	OUT.WorldPosition=mul(l_WorldPosition,g_WorldMatrix);
	OUT.WorldNormal.xyz=normalize(mul(l_Normal.xyz,(float3x3)g_WorldMatrix));
	OUT.WorldTangent=normalize(mul(l_Tangent,g_WorldMatrix));
	OUT.WorldBinormal=mul(cross(l_Tangent,l_Normal),(float3x3)g_WorldMatrix);

	OUT.UV = IN.TexCoord.xy;
	OUT.HPosition = mul(l_WorldPosition, g_WorldViewProj );

	return OUT;
}

float4 RenderCal3DHWPSForward(CAL3D_HW_VERTEX_PS IN) : COLOR
{
	float3 Nn=normalize(IN.WorldNormal);	
	//float3 Nn=CalcBumpMap(IN.WorldNormal, IN.WorldTangent, IN.WorldBinormal, IN.UV);		
	float3 Tn=normalize(IN.WorldTangent);
	float3 Bn=normalize(IN.WorldBinormal);
	
	float4 l_DiffuseColor=tex2D(gDiffuseSampler, IN.UV);
	float4 l_SpecularFactor = 0.0;		//tex2D(gLightmapSampler, IN.UV).a;

	//return float4(Nn, 1.0f);
	return calcLighting (IN.WorldPosition, Nn, l_DiffuseColor,l_SpecularFactor);
}

float4 MirrorRenderCal3DHWPSForward(CAL3D_HW_VERTEX_PS IN) : COLOR
{
	float3 Nn=normalize(IN.WorldNormal);	
	//float3 Nn=CalcBumpMap(IN.WorldNormal, IN.WorldTangent, IN.WorldBinormal, IN.UV);		
	float3 Tn=normalize(IN.WorldTangent);
	float3 Bn=normalize(IN.WorldBinormal);
	
	float4 l_DiffuseColor=tex2D(gDiffuseSampler, IN.UV);
	//l_DiffuseColor.rgb = ChangeWorld_PS(l_DiffuseColor.rgb, gDiffuseSampler, IN.UV.xy);

	float4 l_SpecularFactor = 0.0;	//tex2D(gLightmapSampler, IN.UV).a;
	float4 l_Lighting = calcLighting (IN.WorldPosition, Nn, l_DiffuseColor,l_SpecularFactor);

	return float4 (((g_AmbientLight * l_DiffuseColor.rgb * 0.8f) + l_Lighting.rgb)*g_MirrorDarkCoef * g_MirrorReflectCoef, l_Lighting.a);		
}


TMultiRenderTargetPixel RenderCal3DHWPSDeferred(CAL3D_HW_VERTEX_PS IN) : COLOR
{
	float3 l_Albedo=tex2D(gDiffuseSampler, IN.UV).xyz * 0.8;
	float l_GlowFactor=tex2D(gDiffuseSampler, IN.UV).a;
	float3 l_Ambient=g_AmbientLight;
	float3 l_NormalMap=CalcBumpMap(IN.WorldNormal, IN.WorldTangent, IN.WorldBinormal, IN.UV); //normalize(IN.WorldNormal);
	float4 l_Depth=IN.WorldPosition.z/IN.WorldPosition.w;
	float l_SpecularFactor=0.0;		//tex2D(gLightmapSampler, IN.UV).a;	
	float l_SpecularPower=g_SpecularPower;
	
	return calcTargetPixel(l_Albedo, l_Ambient, l_NormalMap, l_Depth, l_SpecularFactor, l_SpecularPower, l_GlowFactor);
}


technique Cal3DTechniqueForward
{
	pass p0
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 RenderCal3DHWVSForward();
		PixelShader = compile ps_3_0 RenderCal3DHWPSForward();
	}
}

technique MirrorCal3DTechniqueForward
{
	pass p0
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 RenderCal3DHWVSForward();
		PixelShader = compile ps_3_0 MirrorRenderCal3DHWPSForward();
	}
}

technique Cal3DTechniqueDeferred
{
	pass p0
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 RenderCal3DHWVSDeferred();
		PixelShader = compile ps_3_0 RenderCal3DHWPSDeferred();
	}
}

