 
////////////////////////////////////////////////////////////////////////////////
// Filename: Color.fx
////////////////////////////////////////////////////////////////////////////////
/////////////
// GLOBALS //
/////////////

SamplerState SampleType : register(s0);






cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
	matrix lightProjectionMatrix;
	matrix lightViewMatrix;
};
//////////////
// TYPEDEFS //
//////////////

float4 GGX_Distribution(float3 n, float3 h, float Roughness)
{
	float PI = 3.14159265;
	float NoH = dot(n, h);
	float a = Roughness*Roughness;
	float a2 = a * a;
	float d = (NoH * a2 - NoH) * NoH + 1;
	return a2 / (PI*d*d);
}

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 uv :  TEXCOORD;
	float3 normal : TEXCOORD1;
	float3 tangent : TEXCOORD2;
	float3 binormal : TEXCOORD3;
	float3 viewDirection : TEXCOORD4;
};



Texture2D BaseColorSample : register(t0);

Texture2D NormalMapSample : register(t1);

Texture2D ShadowMapSample : register(t2);

Texture2D MetalnessRoughnessMapSample: register(t3);

//Texture2D CubeMapSample : register(t3);


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////


struct PixelOutputType 
{
	
	float4 Color	: SV_Target0;
	float4 Normal	: SV_Target1;
	float4 Position : SV_Target2;
	float4 Depth	: SV_Target3;
};



PixelOutputType   PixelShaderFunction(PixelInputType input)
{

	PixelOutputType Out;
	Out.Normal = float4(0.5, 0.5,1, 1);
	Out.Color = float4(0, 0, 0, 1);
	Out.Position = float4(0, 0, 0, 1);
  
	float4 AmbientColor = float4(1, 1, 1, 1);
	float AmbientIntensity;
	float4 bumpMap;
    float3 bumpNormal;
	float bias;
	float depthValue;
	float ambient;
	float4 BaseColor;


#if BASECOLORMAP
		BaseColor = BaseColorSample.Sample(SampleType, input.uv);
#else
	BaseColor = 1;
#endif

#if EMISSION
	
	
	
	
	float4 Color = float4(0, 0, 0, 0);
		Color = BaseColor;

	Out.Color = BaseColor;
	//Out.Color = pow(Out.Color, 1 / 2.2);
	Out.Position.xyz = input.viewDirection;
	Out.Depth = input.position.z / input.position.w;
	Out.Normal.xyz = input.normal;
	return Out;
#endif
	

#if NORMALMAP
	
	bumpMap = NormalMapSample.Sample(SampleType, input.uv);
	bumpMap = (bumpMap * 2.0f) - 1.0f;
	bumpMap.g = -bumpMap.g;
	bumpNormal = (bumpMap.x * input.tangent) + (bumpMap.y * input.binormal) + (bumpMap.z * input.normal);
	bumpNormal = normalize(bumpNormal);
	Out.Normal.xyz = bumpNormal;
	//
#else 
		Out.Normal.xyz =  input.normal;	
#endif
	
			
	float2 offset;
	float x;
	float y;
	float shadow;
	float specularPower;
	float4 specular;
	int PCFSamples;
	float ShadowBlur;
	float2 TexelCenter;
	float screenWidth;
	float screenHeight;
	float3 reflection;
	float roughness;
	float metalness;

	ShadowBlur = 0.5f;
	screenWidth = float(ShadowBlur / 512.0f);
	screenHeight = float(ShadowBlur / 512.0f);
	offset = 0;

	specularPower = 0.4f;
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	roughness = 0.5f;
	metalness = 0.0f;

	
	Out.Color = BaseColor;
	//Out.Color = pow(Out.Color, 1 / 2.2);
	Out.Position.xyz = input.viewDirection;
	Out.Depth = input.position.z / input.position.w;
	
	return Out;
	
}

