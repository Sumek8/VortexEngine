 
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



struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 uv :  TEXCOORD;
	float3 normal : TEXCOORD1;
	float3 tangent : TEXCOORD2;
	float3 binormal : TEXCOORD3;
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
	
	float4 Color			 : SV_Target0;
	float4 Normal			 : SV_Target1;
	float4 RoughnessMetallic : SV_Target2;
	float4 Depth		  	 : SV_Target3;
};



PixelOutputType   PixelShaderFunction(PixelInputType input)
{

	PixelOutputType Out;
	Out.Normal = float4(0.5, 0.5,1, 1);
	Out.Color = float4(0, 0, 0, 1);
	Out.RoughnessMetallic = float4(0.5,0, 0, 1);
  
	float4 bumpMap;
    float3 bumpNormal;
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
	
			
	
	float  specularPower;
	float  roughness;
	float  metalness;


	specularPower = 0.4f;
	roughness = 0.2f;
	metalness = 0.7f;

	
	Out.Color = BaseColor;
	Out.RoughnessMetallic.xy = float2(0.5,0);
	Out.Depth = input.position.z / input.position.w;
	
	return Out;
	
}

