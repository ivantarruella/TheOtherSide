/*

SOURCE GLOW :
		- downsample + create source glow
		- blur horizontally and vertically
		- upsample final blurred glow texture

*/

#include "functions.fx"

// Shader parameters

float Threshold = 0.20;				// Get the threshold of what brightness level we want to glow (1.0 = blanco)
float BloomIntensity = 1.5;			// Controls the Intensity of the bloom texture
float OriginalIntensity = 1.0;		// Controls the Intensity of the original scene texture
float BloomSaturation = 0.5	;		// Saturation amount on bloom
float OriginalSaturation = 0.9;		// Saturation amount on original texture
float GlowIntensity = 1.0;			// Controls the Intensity of the glow texture

// Pixel shaders
/*
float4 BlurH_PS(float2 Tex : TEXCOORD0) : COLOR
{
   float4 color = 0;
   float2 samp = Tex;
   samp.y = Tex.y;
   float pixelWidth = 1/g_RenderTargetSize.x;
	
   for (int i = 0; i < BLUR_KERNEL_SIZE; i++) {
       samp.x = Tex.x + PixelKernel[i] * pixelWidth;
       color += tex2D(PrevFilterSampler, samp.xy) * BlurWeights2[i];
   }

   return color*GlowIntensity;
}

float4 BlurV_PS(float2 Tex : TEXCOORD0) : COLOR
{
   float4 color = 0;
   float2 samp = Tex;
   samp.x = Tex.x;
   float pixelWidth = 1/g_RenderTargetSize.y;
   
   for (int i = 0; i < BLUR_KERNEL_SIZE; i++) {
       samp.y = Tex.y + PixelKernel[i] * pixelWidth;
       color += tex2D(PrevFilterSampler, samp.xy) * BlurWeights2[i];
   }

   return color*GlowIntensity;
}
*/			

float4 BlurH_PS( float2 Tex : TEXCOORD0 ) : COLOR0
{
	float TextelScale = 1.0 / g_RenderTargetSize.x;

    float4 Value = float4(0.0, 0.0, 0.0, 0.0);

    float Coefficients[21] = 
    {0.000272337, 0.00089296, 0.002583865, 0.00659813, 0.014869116,
     0.029570767, 0.051898313, 0.080381679, 0.109868729, 0.132526984,
     0.14107424,
     0.132526984, 0.109868729, 0.080381679, 0.051898313, 0.029570767,
     0.014869116, 0.00659813, 0.002583865, 0.00089296, 0.000272337};
    for(int Index = 0; Index < 21; Index++)
    {
        Value += tex2D(PrevFilterSampler, float2(Tex.x + (Index - 10) * TextelScale, Tex.y)) * Coefficients[Index];
    }

    return Value*GlowIntensity;
}


float4 BlurV_PS( float2 Tex : TEXCOORD0 ) : COLOR0
{
	float TextelScale = 1.0 / g_RenderTargetSize.y;

    float4 Value = float4(0.0, 0.0, 0.0, 0.0);
    
    float Coefficients[21] = 
    {0.000272337, 0.00089296, 0.002583865, 0.00659813, 0.014869116,
     0.029570767, 0.051898313, 0.080381679, 0.109868729, 0.132526984,
     0.14107424,
     0.132526984, 0.109868729, 0.080381679, 0.051898313, 0.029570767,
     0.014869116, 0.00659813, 0.002583865, 0.00089296, 0.000272337};
    for(int Index = 0; Index < 21; Index++)
    {
        Value += tex2D(PrevFilterSampler, float2(Tex.x, Tex.y + (Index - 10) * TextelScale)) * Coefficients[Index];
    }

    return Value*GlowIntensity;
}
				
//-----------------------------------------------------------------------------
// Pixel Shader: GlowSource_PS
// Desc: initial source glow
//-----------------------------------------------------------------------------

float4 GlowSource_PS(float2 texCoord : TEXCOORD0) : COLOR0
{
    float3 l_Albedo = tex2D(PrevFilterSampler, texCoord).rgb;
    float l_GlowFactor = tex2D(PrevFilterSampler, texCoord).a;
	
	l_Albedo *= (1.0 - l_GlowFactor);
	
	return float4(l_Albedo, l_GlowFactor);
}


//-----------------------------------------------------------------------------
// Pixel Shader: Glow_PS
// Desc: Final glow composition
//-----------------------------------------------------------------------------

float4 GlowFinal_PS(float2 texCoord : TEXCOORD0) : COLOR0
{
    // Get our original pixel from ColorMap
    float4 glowColor = tex2D(PrevFilterSampler, texCoord);

	return glowColor;
}



//-----------------------------------------------------------------------------
// Pixel Shader: BrightPassFilter
// Desc: Perform a high-pass filter on the source texture
//-----------------------------------------------------------------------------

float4 BrightPassFilter( in float2 Tex : TEXCOORD0 ) : COLOR0
{
    float4 Color = tex2D(PrevFilterSampler, Tex);
	
    // Get the bright areas that is brighter than Threshold and return it.
    return saturate((Color - Threshold) / (1 - Threshold));
}


//-----------------------------------------------------------------------------
// Pixel Shader: FinalBloomPS
// Desc: Final bloom composition
//-----------------------------------------------------------------------------

float4 AdjustSaturation(float4 color, float saturation)
{
    // We define gray as the same color we used in the grayscale shader
    float grey = dot(color, float3(0.3, 0.59, 0.11));
    return lerp(grey, color, saturation);
}


float4 FinalBloomPS(float2 texCoord : TEXCOORD0) : COLOR0
{
    // Get our bloom pixel from bloom texture
    float4 bloomColor = tex2D(PrevFilterSampler, texCoord);

    // Get our original pixel from ColorMap
    float4 originalColor = tex2D(PostFilterSampler, texCoord);

    // Adjust color saturation and intensity based on the input variables to the shader
    bloomColor = AdjustSaturation(bloomColor, BloomSaturation) * BloomIntensity;
    originalColor = AdjustSaturation(originalColor, OriginalSaturation) * OriginalIntensity;

    // make the originalColor darker in very bright areas, avoiding these areas look burned-out
    originalColor *= (1 - saturate(bloomColor));

    // Combine the two images.
    return originalColor + bloomColor;
}

// Techniques
technique GlowSourceTechnique
{
	pass p0
	{
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = false;
		
		PixelShader = compile ps_3_0 GlowSource_PS();
	}
}

technique BlurHTechnique
{
    pass p0
    {
		ZEnable = false;
		PixelShader = compile ps_3_0 BlurH_PS();
    }
}

technique BlurVTechnique
{
    pass p0
    {
        ZEnable = false;
		PixelShader = compile ps_3_0 BlurV_PS();
    }
}

technique GlowTechnique
{
	pass p0
	{
		AlphaBlendEnable = true;
		BlendOp=Add;
		SrcBlend = one;
		DestBlend = one;
		PixelShader = compile ps_3_0 GlowFinal_PS();
	}
}

technique BrightPassTechnique
{
    pass p0
    {
        AlphaBlendEnable = false;
        PixelShader = compile ps_3_0 BrightPassFilter();
        ZEnable = false;
    }
}

technique FinalBloomTechnique
{
	pass p0
	{
		PixelShader = compile ps_3_0 FinalBloomPS();
	}
}
