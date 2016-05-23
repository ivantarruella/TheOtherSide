#include "functions.fx"


float4 RNMPS(in float2 UV : TEXCOORD0) : COLOR
{

	//En el pixel shader calculamos el normalmap del pixel
	float4 l_NormalMap=tex2D(gNormalMapSampler,IN.UV);
	float3 Tn=normalize(IN.WorldTangent);
	float3 Bn=normalize(IN.WorldBinormal);
	float3 Nn=normalize(IN.WorldNormal);
	Nn=CalcNormalMap(Nn, Tn, Bn, l_NormalMap);
	//Por último calculamos el color del pixel del lightmap según los valores
	float3 l_Lightmap=GetRadiosityNormalMap(Nn, IN.UV2, g_WorldMatrix);


}


//Technique que genera la textura de SSAO
technique RNMTechnique 
{
	pass p0
	{
		AlphaBlendEnable = false;
		CullMode = CCW;
		PixelShader = compile ps_3_0 RNMPS();
	}
}
