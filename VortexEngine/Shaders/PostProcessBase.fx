 

//#include "Fxaa3_11.fx"

//#define FXAA_GREEN_AS_LUMA 1
//#define FXAA_PC 1
//#define FXAA_HLSL_5 1
//#define FXAA_QUALITY__PRESET 12
#define ScreenWidth  1366.0f
#define ScreenHeight 1080.0f

SamplerState Sampler : register(s0);


Texture2D SceneColor : register(t0);

struct PixelInputType
{

	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;


};

float rgb2luma(float3 rgb) {
	return sqrt(dot(rgb,float3(0.299, 0.587, 0.114)));
}

float4 Desaturate(float4 Color,float Intensity)
{
	float4 DesaturatedColor = float4(0, 0, 0, 0);
	float Greyscale = (Color.r + Color.g + Color.b) / 3;
	DesaturatedColor = float4(Greyscale*Intensity+Color.r*(1-Intensity), Greyscale*Intensity + Color.g*(1 - Intensity), Greyscale*Intensity + Color.b*(1 - Intensity),1);
	return DesaturatedColor;
}


PixelInputType VertexShaderFunction(uint VertID : SV_VERTEXID)
{
	PixelInputType PIn;
	float2 texcoord = float2(VertID & 1,VertID >> 1); //you can use these for texture coordinates later
	PIn.position = float4((texcoord.x - 0.5f) * 2, -(texcoord.y - 0.5f) * 2, 0, 1);
	PIn.texCoord = texcoord;
	return PIn;
}

float4 ChromaticAberration(float4 Color, float2 texCoord)
{
	
	float2 rOffset = -0.001f;
	float2 bOffset = 0.001f;
	float2 gOffset = 0.003f;
	float4 AberrationColor;
	AberrationColor.r = SceneColor.Sample(Sampler,texCoord+rOffset);
	AberrationColor.g = SceneColor.Sample(Sampler,texCoord+gOffset);
	AberrationColor.b = SceneColor.Sample(Sampler,texCoord+bOffset);
	return AberrationColor;
}

float4 Blur(float4 Color,float2 texCoord)
{
	float4 Blur = float4(0, 0, 0, 0);
	for (int x = -1; x < 2; x++)
	{
		for (int y = -1; y < 2; y++)
		{
			texCoord.x = 1.0f / (ScreenWidth/1) * x + texCoord.x;
			texCoord.y = 1.0f / (ScreenHeight/1) * y + texCoord.y;
			texCoord.x = clamp(texCoord.x,0,1);
			texCoord.y = clamp(texCoord.y,0,1);
			Blur += SceneColor.Sample(Sampler, texCoord);
		}
	}
	return Blur/9;
}

float4 PixelShaderFunction(PixelInputType PIn) : SV_TARGET
{
float4 Color = float4(0, 0, 0, 0);
	
Color = SceneColor.Sample(Sampler, PIn.texCoord);

//Color.rgb = ToneMap(Color.rgb);
Color = Blur(Color, PIn.texCoord);
//Color = Desaturate(Color,0);
//Color = ChromaticAberration(Color, PIn.texCoord);
//Color = rgb2luma(Color);
return	Color;
}