/*

% Description of my shader.
% Second line of description for my shader.

keywords: material classic

date: YYMMDD

*/

#include "functions.fx"

struct PS_OUTPUT
{
    float4 Color    : COLOR0;
};


struct VS_OUTPUT
{
    float4 Position         : POSITION;   // vertex position
    float2 TexCoord         : TEXCOORD0;  // vertex interpolation value
};

// Pixel shaders

float4 PShaderEntryX(float2 Tex : TEXCOORD0) : COLOR
{
   float4 color = 0;
   float2 samp = Tex;
   samp.y = Tex.y;
   float pixelWidth = 1/(g_RenderTargetSize.x*2);
	
   for (int i = 0; i < BLUR_KERNEL_SIZE; i++) {
       samp.x = Tex.x + PixelKernel[i] * pixelWidth;
       color += tex2D(PostFilterSampler, samp.xy) * BlurWeights2[i];
   }

   return float4(color.x, color.y, 0.0, 1.0);
}

float4 PShaderEntryY(float2 Tex : TEXCOORD0) : COLOR
{
   float4 color = 0;
   float2 samp = Tex;
   samp.x = Tex.x;
   float pixelWidth = 1/(g_RenderTargetSize.y*2);
   
   for (int i = 0; i < BLUR_KERNEL_SIZE; i++) {
       samp.y = Tex.y + PixelKernel[i] * pixelWidth;
       color += tex2D(PostFilterSampler, samp.xy) * BlurWeights2[i];
   }

   return float4(color.x, color.y, 0.0, 1.0);
}


/*
float4 PShaderEntryX( float2 Tex : TEXCOORD0 ) : COLOR0
{
	//return tex2D(PostFilterSampler, Tex);
	float TextelScale = 1.0 / 2048;

    float2 Value = float2(0.0, 0.0);

    float Coefficients[21] = 
    {0.000272337, 0.00089296, 0.002583865, 0.00659813, 0.014869116,
     0.029570767, 0.051898313, 0.080381679, 0.109868729, 0.132526984,
     0.14107424,
     0.132526984, 0.109868729, 0.080381679, 0.051898313, 0.029570767,
     0.014869116, 0.00659813, 0.002583865, 0.00089296, 0.000272337};
    for(int Index = 0; Index < 21; Index++)
    {
        Value += tex2D(PostFilterSampler, float2(Tex.x + (Index - 10) * TextelScale, Tex.y)).xy * Coefficients[Index];
    }

    return float4(Value.x, Value.y, 0.0, 1.0);
}


float4 PShaderEntryY( float2 Tex : TEXCOORD0 ) : COLOR0
{
    //return tex2D(PostFilterSampler, Tex);
	float TextelScale = 1.0 / 2048;

    float2 Value = float2(0.0, 0.0);
    
    float Coefficients[21] = 
    {0.000272337, 0.00089296, 0.002583865, 0.00659813, 0.014869116,
     0.029570767, 0.051898313, 0.080381679, 0.109868729, 0.132526984,
     0.14107424,
     0.132526984, 0.109868729, 0.080381679, 0.051898313, 0.029570767,
     0.014869116, 0.00659813, 0.002583865, 0.00089296, 0.000272337};
    for(int Index = 0; Index < 21; Index++)
    {
        Value += tex2D(PostFilterSampler, float2(Tex.x, Tex.y + (Index - 10) * TextelScale)).xy * Coefficients[Index];
    }

    return float4(Value.x, Value.y, 0.0, 1.0);
}
*/


technique BlurHDepthMapTechnique
{
    pass p0
    {
		ZEnable = false;
		AlphaBlendEnable = true;
		BlendOp=Add;
		SrcBlend = one;
		DestBlend = one;		

		PixelShader = compile ps_3_0 PShaderEntryX();
    }
}

technique BlurVDepthMapTechnique
{
    pass p0
    {
        ZEnable = false;
		AlphaBlendEnable = true;
		BlendOp=Add;
		SrcBlend = one;
		DestBlend = one;		

		PixelShader = compile ps_3_0 PShaderEntryY();
    }
}
