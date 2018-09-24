 
////////////////////////////////////////////////////////////////////////////////
// Filename: Color.fx
////////////////////////////////////////////////////////////////////////////////
/////////////
// GLOBALS //
/////////////

cbuffer ConstantBufferType
{

   float3 LightDirection;
   float4 DiffuseColor;
	
};
SamplerState SampleType : register(s0);
SamplerComparisonState SampleTypeClamp : register(s1);



SamplerComparisonState cmpSampler
{
	// sampler state
	Filter = COMPARISON_MIN_MAG_MIP_LINEAR;
	AddressU = MIRROR;
	AddressV = MIRROR;
	// sampler comparison state
	ComparisonFunc = LESS_EQUAL;
};


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
	float2 uv :  TEXCOORD0;
	float4 lightPosition : TEXCOORD1;
	float3 normal : TEXCOORD2;
	float3 tangent : TEXCOORD3;
	float3 binormal : TEXCOORD4;
	float3 viewDirection : TEXCOORD5;
};



Texture2D BaseColorSample : register(t0);

Texture2D NormalMapSample : register(t1);

Texture2D ShadowMapSample : register(t2);
//Texture2D MetalnessMapSample: register()

Texture2D CubeMapSample : register(t3);


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////


// GGX / Trowbridge-Reitz
// [Walter et al. 2007, "Microfacet models for refraction through rough surfaces"]
float GGX_Distribution(float3 n, float3 h, float Roughness)
{
	float PI = 3.14159265;
	float NoH = dot(n, h);
	float a = Roughness*Roughness;
	float a2 = a * a;
	float d = (NoH * a2 - NoH) * NoH + 1;
	return a2 / (PI*d*d);
}



float4 PixelShaderFunction(PixelInputType input) : SV_TARGET0
{
 
    float3 lightDir;
    float lightIntensity;
    float4 color;
	float4 AmbientColor = float4(1, 1, 1, 1);
	float AmbientIntensity;
	float4 bumpMap;
    float3 bumpNormal;
	float bias;
	float depthValue;
	float ambient;
	float shadowPower;
	float4 BaseColor;
	float normalPower;

	bias = 0.001f;
	
	color = float4(1, 1, 1, 1);

	shadowPower = 0;

	
	normalPower = 1.0f;

#if BASECOLORMAP
		BaseColor = BaseColorSample.Sample(SampleType, input.uv);
#else
	BaseColor = 1;
#endif

#if EMISSION
	color = BaseColor;
	return color;
#endif


#if NORMALMAP
	
	bumpMap = NormalMapSample.Sample(SampleType, input.uv);
	bumpMap = (bumpMap * 2.0f) - 1.0f;
	//bumpMap.y = -bumpMap.y;
	//bumpMap.xy = bumpMap.xy * normalPower;
	//bumpMap = normalize(bumpMap);
	bumpNormal = (bumpMap.x * input.tangent) + (bumpMap.y * input.binormal) + (bumpMap.z * input.normal);
	bumpNormal = normalize(bumpNormal);
	//
#endif
	//CubeMapSample.Sample(SampleType)

	input.lightPosition.xyz /= input.lightPosition.w;
	input.lightPosition.z -= bias;


	input.lightPosition.x = input.lightPosition.x / 2 + 0.5;
	input.lightPosition.y = input.lightPosition.y / -2 + 0.5;
	

	
		
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
	TexelCenter = (0.5f / 512.0f) + input.lightPosition.xy;
	PCFSamples = 2;
	offset = 0;
	AmbientIntensity = 0.00;
	specularPower = 0.4f;
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	roughness = 0.5f;
	metalness = 0.0f;

		for (y = -1 * PCFSamples; y <= 1 * PCFSamples; y += 1.0)
		{
			for (x = -1 * PCFSamples; x <= 1 * PCFSamples; x += 1.0)
			{
			offset[0] = screenWidth*x;
			offset[1] = screenHeight*y; 
		float shadowMapDepth = ShadowMapSample.SampleLevel(SampleType, TexelCenter + offset,0).r;
		if (shadowMapDepth > input.lightPosition.z)
			shadow += 1;
					}
		}
		shadow = saturate(shadow/float(pow(2*PCFSamples+1,2)));
		

	

	lightDir = -LightDirection;
	

	float3 H = normalize(lightDir + input.viewDirection);
	

	

#if NORMALMAP
		 lightIntensity = saturate(dot(bumpNormal, lightDir)*DiffuseColor);
		
		 specular = GGX_Distribution(bumpNormal, H, roughness);
#else
	lightIntensity = saturate(dot(input.normal, lightDir)*DiffuseColor);
	specular = GGX_Distribution(input.normal, H, roughness);
#endif 
	 
	

	
	
	specular = specular*specularPower;
	color = saturate((metalness*BaseColor*specular + BaseColor*(1 - metalness*0.8) + specular*(1 - metalness))*(lightIntensity)*shadow + AmbientIntensity*AmbientColor);
	
	

	return color;
	
}
	  
	/*technique Diffuse
	{
		pass Pass1
		{

			SetPixelShader(CompileShader(ps_5_0, PixelShaderFunction()));
		}
		

	}
	*/
