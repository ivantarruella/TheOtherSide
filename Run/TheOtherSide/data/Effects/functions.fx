/*

% Description of my shader.
% Second line of description for my shader.

keywords: material classic

date: YYMMDD

*/

#include "globals.h"

float3 CalcAnimtedPos(float4 Position, float4 Indices, float4 Weight)
{
	float3 l_Position=0;
	l_Position = mul(g_Bones[Indices.x], Position) * Weight.x;
	l_Position += mul(g_Bones[Indices.y], Position) * Weight.y;
	l_Position += mul(g_Bones[Indices.z], Position) * Weight.z;
	l_Position += mul(g_Bones[Indices.w], Position) * Weight.w;
	return l_Position;
}

void CalcAnimatedNormalTangent(float3 Normal, float3 Tangent, float4 Indices, float4 Weight, out float3 OutNormal, out float3 OutTangent)
{
	OutNormal = 0;
	OutTangent =0;
	float3x3 m;
	
	OutNormal=mul((float3x3)g_Bones[Indices.x], Normal) * Weight.x;
	OutNormal+=mul((float3x3)g_Bones[Indices.y], Normal) * Weight.y;
	OutNormal+=mul((float3x3)g_Bones[Indices.z], Normal) * Weight.z;
	OutNormal+=mul((float3x3)g_Bones[Indices.w], Normal) * Weight.w;
	OutNormal=normalize(float3(OutNormal.x, OutNormal.y, OutNormal.z));
	
	OutTangent=mul((float3x3)g_Bones[Indices.x], Tangent) * Weight.x;
	OutTangent+=mul((float3x3)g_Bones[Indices.y], Tangent) * Weight.y;
	OutTangent+=mul((float3x3)g_Bones[Indices.z], Tangent) * Weight.z;
	OutTangent+=mul((float3x3)g_Bones[Indices.w], Tangent) * Weight.w;
	OutTangent=normalize(float3(OutTangent.x, OutTangent.y, OutTangent.z));
}

// LIGHTING FUNCTIONS

float getAttenuation(int Tipo, float3 posLight, float3 posPixel, float StartAtten, float EndAtten, float3 dirLight, float angle, float falloff)
{
	float l_Attenuation = 1.0;		// Tipo==DIRECTIONAL
	
	if(Tipo == SPOT)
	{
		float3 VToLight=posLight - posPixel;
		float l_Dist=length(VToLight);
		float l_angle = dot(normalize(VToLight), normalize(-dirLight));
		float l_AngAttenuation = (1 - saturate((l_angle - angle) / (falloff - angle)));
		float l_NormAttenuation = l_Attenuation = (1 - saturate((l_Dist - StartAtten)/(EndAtten-StartAtten)));
		l_Attenuation = l_NormAttenuation * l_AngAttenuation;
	}
	else if(Tipo == OMNI)
	{
		float3 VToLight=posLight - posPixel;
		float l_Dist=length(VToLight);
		l_Attenuation = (1 - saturate((l_Dist - StartAtten)/(EndAtten-StartAtten)));
	}
	
	return l_Attenuation;
}


float3 getDiffuseContrib(int Tipo, float3 posLight, float3 normalN, float3 posPixel, float3 dirLight, float falloff, float3 color)
{
	float3 l_DiffuseContrib=(float3)0;
	if(Tipo==DIRECTIONAL)
	{
		l_DiffuseContrib = saturate(dot(normalN, -dirLight));
	}
	else if(Tipo == SPOT)
	{
		float3 Lp = normalize(posLight-posPixel);
		l_DiffuseContrib = (saturate(dot (Lp, -dirLight)) > falloff) ? saturate(dot(Lp, normalN)) : float3(0.0,0.0,0.0);	
	}
	else if(Tipo == OMNI)
	{
		float3 Ln= normalize(posLight - posPixel);
		l_DiffuseContrib = saturate(dot(Ln, normalN));
	}
	return l_DiffuseContrib;
}


float3 getSpecularContrib(int Tipo, float3 posLight, float3 eye, float3 normalN, float3 posPixel, float3 dirLight, float angle, float specularPow, float3 color)
{
	float3 l_SpecularContrib=(float3)0;
	
	if(Tipo == DIRECTIONAL)
	{
		float3 Hn = normalize(normalize(eye - posPixel) - dirLight);
		l_SpecularContrib = pow(saturate(dot(normalN,Hn)), specularPow) * color;
	}
	else if(Tipo == SPOT)
	{
		float3 Lp = normalize(posLight - posPixel);
		
		float3 Hn=normalize(normalize(eye - posPixel) + Lp);
		l_SpecularContrib = (saturate(dot (Lp, -dirLight)) > angle && dot(normalN,-dirLight)>0) ? (pow(saturate(dot(normalN,Hn)), specularPow) * color) : 0.0;
	}
	else if(Tipo == OMNI)
	{
		float3 Ln= normalize(posLight - posPixel);
		float3 Hn=normalize(normalize(eye - posPixel) + Ln);
		l_SpecularContrib = (dot(Ln, normalN) > 0) ? (pow(saturate(dot(normalN,Hn)), specularPow) * color): 0.0;
	}
	return l_SpecularContrib;
}

float3 CalcBumpMapSampled(float3 WorldNormal, float3 WorldTangent, float3 WorldBinormal, float2 UV, sampler NormalMapSampler)
{
	float3 Nn=normalize(WorldNormal);
	float3 Tn=normalize(WorldTangent);
	float3 Bn=normalize(WorldBinormal);
	float3 bump=g_Bump*(tex2D(NormalMapSampler,UV).xyz*2-1);	
	Nn = Nn + bump.x*Tn + bump.y*Bn;
	Nn = normalize(Nn);	
	
	return Nn;
}

float3 CalcBumpMap(float3 WorldNormal, float3 WorldTangent, float3 WorldBinormal, float2 UV)
{
	float3 Nn=normalize(WorldNormal);
	float3 Tn=normalize(WorldTangent);
	float3 Bn=normalize(WorldBinormal);
	
	float3 bump=g_Bump*(tex2D(gLightmapSampler,UV).rgb - float3(0.5,0.5,0.5));	// OJO: se cogen las normales del lightmap, ya que es lo que nos llega por el canal 1
	Nn = Nn + bump.x*Tn + bump.y*Bn;
	Nn = normalize(Nn);	
	
	return Nn;
}

float3 CalcParallaxMap(float3 Vn, float3 WorldNormal, float3 WorldTangent, float3 WorldBinormal, float2 UV, out float2 OutUV)
{
	float3 Nn=normalize(WorldNormal);
	float3 Tn=normalize(WorldTangent);
	float3 Bn=normalize(WorldBinormal);
	float3 V=normalize(Vn);
	
	float2 l_UV = UV;
	// parallax code
	float3x3 tbnXf = float3x3(Tn,Bn,Nn);
	float4 l_NormalMapColor = tex2D(gLightmapSampler,l_UV);
	float height = l_NormalMapColor.w * 0.06 - 0.03;
	l_UV += height * mul(tbnXf,V).xy;
	
	// normal map
	float3 tNorm = l_NormalMapColor.xyz - float3(0.5,0.5,0.5);
	// transform tNorm to world space
	tNorm = normalize(tNorm.x*Tn - tNorm.y*Bn + tNorm.z*Nn);
	OutUV=l_UV;
	return tNorm;
}

//// DEFERRED LIGHTING + SHADOW MAP

float3 Normal2Texture(float3 Normal)
{
	return Normal*0.5+0.5;
}

float3 Texture2Normal(float3 Color)
{
	return (Color-0.5)*2;
}

TMultiRenderTargetPixel calcTargetPixel(float3 Albedo, float3 Ambient, float3 Normal, float4 Depth, float SpecularFactor, float SpecularPower, float GlowFactor)
{
	TMultiRenderTargetPixel l_OUT=(TMultiRenderTargetPixel) 0;
	
	l_OUT.RT0.xyz = Albedo;
	l_OUT.RT0.a = GlowFactor;
	l_OUT.RT1.xyz = Ambient;
	l_OUT.RT1.a = SpecularPower/255;
	l_OUT.RT2.xyz = Normal2Texture(Normal);
	l_OUT.RT2.a = SpecularFactor;
	l_OUT.RT3 = Depth;

	return l_OUT;
}

float3 GetPositionFromZDepthViewInViewCoordinates(float ZDepthView, float2 UV)
{
	// Get the depth value for this pixel
	// Get x/w and y/w from the viewport position
	float x = UV.x * 2 - 1;
	float y = (1 - UV.y) * 2 - 1;
	float4 l_ProjectedPos = float4(x, y, ZDepthView, 1.0);
	// Transform by the inverse projection matrix
	float4 l_PositionVS = mul(l_ProjectedPos, g_InverseProj);
	// Divide by w to get the view-space position
	return l_PositionVS.xyz / l_PositionVS.w;
}

float4 GetPositionFromZDepthView(float ZDepthView, float2 UV)
{
	float3 l_PositionView=GetPositionFromZDepthViewInViewCoordinates(ZDepthView, UV);
	return mul(float4(l_PositionView,1.0), g_InverseView);
}

float2 getProjectedTexCoords(float4 Pos, out float2 Depth)
{
	float4 l_PosView=mul(float4(Pos.xyz, 1.0), g_ViewMatrix);
	float4 PosLight = mul(l_PosView, g_ViewToLightProj);
	
	float2 ShadowTexC = 0.5 * PosLight.xy/PosLight.w + float2( 0.5, 0.5 );
	ShadowTexC.y = 1.0f - ShadowTexC.y;
	
	Depth.xy = PosLight.z / PosLight.w;
	return ShadowTexC;
}

float calcLightAmountPFC2x2(float4 Pos, out float Color)
{
	float lightAmount = 1.0;
	
	if (g_UseDynamicShadowMap)
	{
		float4 l_PosView=mul(float4(Pos.xyz, 1.0), g_ViewMatrix);
		float4 PosLight = mul(l_PosView, g_ViewToLightProj);
		
		// Project the texture coords and scale/offset to [0, 1].
		PosLight.xy /= PosLight.w;
		PosLight.x =  0.5f*PosLight.x + 0.5f;
		PosLight.y = -0.5f*PosLight.y + 0.5f;

		// Compute pixel depth for shadowing.
		float depth = PosLight.z / PosLight.w; // depth in [0, 1] range

		// Transform to texel space.
		float2 texelpos = SMAP_SIZE * PosLight.xy;

		// Determine the lerp amounts.
		float2 lerps = frac( texelpos );

		// 2x2 percentage closest filter.
		float dx = 1.0f / SMAP_SIZE;
		float s0 = (tex2D(gDynamicShadowMapTextureSampler, PosLight.xy).r  + SHADOW_EPSILON2 < depth) ? 0.0f : 1.0f;
		float s1 = (tex2D(gDynamicShadowMapTextureSampler, PosLight.xy + float2(dx, 0.0f)).r + SHADOW_EPSILON2 < depth) ? 0.0f : 1.0f;
		float s2 = (tex2D(gDynamicShadowMapTextureSampler, PosLight.xy + float2(0.0f, dx)).r + SHADOW_EPSILON2 < depth) ? 0.0f : 1.0f;
		float s3 = (tex2D(gDynamicShadowMapTextureSampler, PosLight.xy + float2(dx, dx)).r + SHADOW_EPSILON2 < depth) ? 0.0f : 1.0f;

		Color = lerp(lerp( s0, s1, lerps.x), lerp( s2, s3, lerps.x), lerps.y);
	}
	
	return lightAmount;
}

float calcLightAmount(int Tipo, float4 Pos, float3 Nn)
{
	float lightAmount = 1.0;
	
	if (g_UseDynamicShadowMap)
	{
		float2 Depth=(float2)0;
		float4 ShadowText=(float4)0;
		float2 ShadowTexC = getProjectedTexCoords(Pos, Depth);
		float3 VToLight=normalize(g_LightsPosition[0] - Pos.xyz);
		
		if (Tipo == SPOT)
		{
			ShadowText = tex2D( gDynamicShadowMapTextureSampler, ShadowTexC );
			if (saturate(dot (VToLight, -g_LightsDirection[0])) > g_LightFallOff[0])
			{
				lightAmount = ((ShadowText + SHADOW_EPSILON )< Depth)? 0.0: 1.0;
				lightAmount *= saturate(dot(VToLight, Nn));
			}
		}
		/*if(Tipo == OMNI) 
		{
			ShadowText = tex2D( gCubeTextureSampler, float3(ShadowTexC,0.0) );
			lightAmount = ((ShadowText + SHADOW_EPSILON )< Depth)? 0.0: 1.0;
			lightAmount *= saturate(dot(VToLight, Nn));
		}
		if(Tipo == DIRECTIONAL)
		{
			lightAmount = ((ShadowText + SHADOW_EPSILON )< Depth)? 0.0: 1.0;
		}*/
	}
	
	return lightAmount;
}

float calcShadowCoeffVSM(int Tipo, float4 Pos)
{
	float lightAmount = 1.0;
	
	if (g_UseDynamicShadowMap)
	{
		if (Tipo == SPOT)
		{
			float3 VToLight=normalize(g_LightsPosition[0] - Pos.xyz);
			if (saturate(dot (VToLight, -g_LightsDirection[0])) > g_LightFallOff[0])
			{
				float2 depth = (float2) 0;
				float2 ShadowTexC = getProjectedTexCoords(Pos, depth);

				float2 moments = tex2D( gDynamicShadowMapTextureSampler, ShadowTexC ).xy;
				
				float mean = moments.x;
				float meanSqr = moments.y;	
				float Ex_2 = mean * mean;
				float E_x2 = meanSqr;
				float variance = min(max(E_x2 - Ex_2, 0.0f) + SHADOW_EPSILON2, 1.0f);
				float m_d = (depth - mean);
				float p = variance / (variance + m_d * m_d);

				// Reduce light bleeding
				float min = 2.5f;
				p = saturate(pow(p, min) + 0.2f);
				
				lightAmount = max(p, depth <= mean);
			}
		}
		if (Tipo == OMNI)
		{
			float2 depth = (float2) 0;
			float2 ShadowTexC = getProjectedTexCoords(Pos, depth);
			//TODO: access texCUBE sampler incorrect!!
			float2 moments = texCUBE( gCubeTextureSampler, float3(ShadowTexC,0.0) ).xy;
			
			float mean = moments.x;
			float meanSqr = moments.y;	
			float Ex_2 = mean * mean;
			float E_x2 = meanSqr;
			float variance = min(max(E_x2 - Ex_2, 0.0f) + SHADOW_EPSILON2, 1.0f);
			float m_d = (depth - mean);
			float p = variance / (variance + m_d * m_d);

			// Reduce light bleeding
			float min = 2.5f;
			p = saturate(pow(p, min) + 0.2f);
			
			lightAmount = max(p, depth <= mean);	
		}
	}
	return lightAmount;
}

float4 calcDeferredLighting(float4 Pos, float3 Nn, float4 Albedo, float SpecularFactor, float SpecularPower)
{
	float3 l_Light = (float3)0;
	
	float l_Attenuation = getAttenuation(g_LightsTypes[0], g_LightsPosition[0], Pos.xyz, g_LightStartAtten[0], g_LightEndAtten[0], g_LightsDirection[0], g_LightAngle[0], g_LightFallOff[0]);
	if (l_Attenuation > 0.0)
	{
		float lightAmount = calcShadowCoeffVSM(g_LightsTypes[0], Pos);
		if (lightAmount!= 0.0)		
		{
			// Calculamos diffuseContrib, specular Contrib i aplicamos attenuation si el pixel no esta sombreado
			float3 l_DiffuseContrib =  Albedo.rgb * g_LightsColor[0] * getDiffuseContrib(g_LightsTypes[0], g_LightsPosition[0], Nn, Pos.xyz, g_LightsDirection[0], g_LightFallOff[0], g_LightsColor[0]) ;	
			float3 l_SpecularContrib = 0.0;  
			if (SpecularFactor != 0.0)
				l_SpecularContrib = SpecularFactor * getSpecularContrib(g_LightsTypes[0], g_LightsPosition[0], g_CameraPosition, Nn, Pos.xyz, g_LightsDirection[0], g_LightFallOff[0], SpecularPower, g_LightsColor[0]);
							
			l_Light = l_Attenuation * lightAmount * (l_DiffuseContrib  + l_SpecularContrib);
		}
	}

	return float4(l_Light, Albedo.a);
}

/*
float3 GetRadiosityNormalMap2(float3 Nn, float3 FaceNormal, float3 Tn, float3 Bn, float2 UV)
{
	float3x3 l_TriangleMatrix;
	l_TriangleMatrix[0] = normalize(Tn);
	l_TriangleMatrix[1] = normalize(Bn);
	l_TriangleMatrix[2] = normalize(FaceNormal);

	float3 l_LightmapX=(tex2D(gS1LinearClampSampler, UV).xyz);
	float3 l_LightmapY=(tex2D(gS2LinearClampSampler, UV).xyz);
	float3 l_LightmapZ=(tex2D(gS3LinearClampSampler, UV).xyz);
	float3 l_BumpBasisX=normalize(mul(float3(0.816496580927726, 0.5773502691896258, 0 ),l_TriangleMatrix));
	float3 l_BumpBasisY=normalize(mul(float3(-0.408248290463863, 0.5773502691896258,0.7071067811865475 ),l_TriangleMatrix));
	float3 l_BumpBasisZ=normalize(mul(float3(-0.408248290463863, 0.5773502691896258, -0.7071067811865475),l_TriangleMatrix));
	float3 diffuseLighting=saturate( dot( Nn, l_BumpBasisX ) ) * l_LightmapX + saturate( dot( Nn, l_BumpBasisY ) ) * l_LightmapY + saturate( dot( Nn, l_BumpBasisZ ) ) * l_LightmapZ;
	
	//return tex2D(gS4LinearWrapSampler, UV).xyz*2;
	return diffuseLighting;
}
*/

float3 GetRadiosityNormalMap(float3 Nn, float3 FaceNormal, float3 Tn, float3 Bn, float2 UV)
{
	float3x3 l_TriangleMatrix;
    l_TriangleMatrix[0] = normalize(Tn);
    l_TriangleMatrix[1] = normalize(FaceNormal);
    l_TriangleMatrix[2] = normalize(Bn);

    float3 l_LightmapX=(tex2D(gS1LinearWrapSampler, UV).xyz);
    float3 l_LightmapY=(tex2D(gS2LinearWrapSampler, UV).xyz);
    float3 l_LightmapZ=(tex2D(gS3LinearWrapSampler, UV).xyz);
    
	float3 l_BumpBasisX=normalize(mul(float3(0.816496580927726, 0.5773502691896258, 0 ),l_TriangleMatrix));
    float3 l_BumpBasisY=normalize(mul(float3(-0.408248290463863, 0.5773502691896258,0.7071067811865475 ),l_TriangleMatrix));
    float3 l_BumpBasisZ=normalize(mul(float3(-0.408248290463863, 0.5773502691896258, -0.7071067811865475),l_TriangleMatrix));

//http://www.valvesoftware.com/publications/2007/SIGGRAPH2007_EfficientSelfShadowedRadiosityNormalMapping.pdf

    float3 dp;
    dp.x=saturate( dot( Nn, l_BumpBasisX ) );
    dp.y=saturate( dot( Nn, l_BumpBasisY ) );
    dp.z=saturate( dot( Nn, l_BumpBasisZ ) );

    dp*=dp;
	float sum=dot(dp, float3(1.0, 1.0, 1.0));
    float3 l_DiffuseLight=dp.x*l_LightmapX+dp.y*l_LightmapY+dp.z*l_LightmapZ;
    l_DiffuseLight/=sum;
	
    return l_DiffuseLight;
}


//// FORWARD LIGHTING
float4 calcLighting(float3 Pos, float3 Nn, float4 Albedo, float SpecularFactor)
{
	float3 l_DiffuseContrib = (float3)0;
	float3 l_SpecularContrib = (float3)0;
	float l_Attenuation = 1.0;
	float3 l_Diffuse = (float3)0;
	float3 l_Specular = (float3)0;
			
	// Calculamos diffuseContrib, specular Contrib i attenuation para todas las luces
	for(int i=0; i<MAX_LIGHTS; i++)
	{
		l_Attenuation = getAttenuation(g_LightsTypes[i], g_LightsPosition[i], Pos, g_LightStartAtten[i], g_LightEndAtten[i], g_LightsDirection[i], g_LightAngle[i], g_LightFallOff[i]);
		if (l_Attenuation > 0.0)
		{
			l_Diffuse = getDiffuseContrib(g_LightsTypes[i], g_LightsPosition[i], Nn, Pos, g_LightsDirection[i], g_LightFallOff[i], g_LightsColor[i]);  
			l_DiffuseContrib = l_DiffuseContrib + (g_LightsColor[i] * l_Diffuse * Albedo.rgb * l_Attenuation) ;	
			l_Specular = SpecularFactor*getSpecularContrib(g_LightsTypes[i], g_LightsPosition[i], g_CameraPosition, Nn, Pos, g_LightsDirection[i], g_LightFallOff[i], g_SpecularPower, g_LightsColor[i]);  
			l_SpecularContrib = l_SpecularContrib + (l_Specular * l_Attenuation);	
		}
	}

	return float4 ( l_DiffuseContrib + l_SpecularContrib, Albedo.a);
}

// CHANGE WORLD EFFECT
float3 ChangeWorld_PS(float3 Albedo, sampler _textureSampler, float2 uv)
{
	if (g_MirrorAnimCoef>0.0)
	{
		Albedo.rgb -= tex2D(_textureSampler , uv-0.003)*1.7f*g_MirrorAnimCoef; 
		Albedo.rgb += tex2D( _textureSampler , uv+0.003)*1.7f*g_MirrorAnimCoef; 
		Albedo.rgb = float3(Albedo.r*1.2f, (Albedo.g+Albedo.b)/3.0f, (Albedo.g+Albedo.b)/3.0f);
	}
	
	return Albedo;
}

