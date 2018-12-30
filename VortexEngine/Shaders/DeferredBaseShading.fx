 
////////////////////////////////////////////////////////////////////////////////
// Filename: Color.fx
////////////////////////////////////////////////////////////////////////////////
/////////////
// GLOBALS //
/////////////

#define ScreenWidth  1366
#define ScreenHeight 768

SamplerState SampleType : register(s0);


cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
	matrix InverseProjectionMatrix;
	matrix lightProjectionMatrix;
	matrix lightViewMatrix;
	
};

cbuffer ConstantBufferType : register(b1)
{
	float4 LightColor;
	float3 LightDirection;
	float  LightPower;
	float3 CameraDirection;	
	float  Padding;
};


//////////////
// TYPEDEFS //
//////////////


Texture2D GBufferA : register(t0);

Texture2D GBufferB : register(t1);

Texture2D GBufferC : register(t2);

Texture2D GBufferD : register(t3);

//Texture2D ShadowMapSample : register(t4);




struct PixelInputType
{

	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;


};

float4 GammaToLinear(float4 GammaColor)
{
	return pow(GammaColor, 1 / 2.2);
}

float4 LinearToGamma(float4 LinearColor)
{

	return pow(LinearColor,2.2);
}

float chiGGX(float v)
{
	return v > 0 ? 1 : 0;
}
// GGX / Trowbridge-Reitz
// [Walter et al. 2007, "Microfacet models for refraction through rough surfaces"]
float GGX_Distribution(float3 n, float3 h, float Roughness)
{
	float PI = 3.14159265;
	float NoH = dot(n, h);
	float a = Roughness*Roughness;
	float a2 = a * a;
	float d = (NoH * a2 - NoH) * NoH + 1;
	return chiGGX(NoH)*a2 / (PI*d*d);
}


float DepthToLinear(float Depth)
{
	float far = 1000.0;
	float near = 0.1;
	float z = (2 * near) / (far + near - (1 - Depth) * (far - near));
	return z;
}

float SSAO(int Samples,float2 Coordinates,float3 n)
{
	
	float2 Pixel;

	float Depth = GBufferD.Sample(SampleType, Coordinates.xy).r;
	float Occlusion;
	float DeltaDepth;
	float2 SampleUV[4];
	float DepthBias;

	float LinearDepth = DepthToLinear(Depth);

	Occlusion = 0.0f;
	float PixelWidth = float(1.0f / ScreenWidth);
	float PixelHeight = float(1.0f / ScreenHeight);
	DepthBias = 0.00001f;
	DeltaDepth = 0;

	Pixel.x = float(1.0f / ScreenWidth);
	Pixel.y = float(1.0f / ScreenHeight);

	SampleUV[0] = float2(2,   2)*Pixel;
	SampleUV[1] = float2(-2,  2)*Pixel;
	SampleUV[2] = float2(2, - 2)*Pixel;
	SampleUV[3] = float2(-2, -2)*Pixel;
	
	float DepthDifference;
	for (int SamplesID = 0; SamplesID < Samples; SamplesID++)
	{
		DepthDifference = 0;
		for (int i = 0; i < 4; i++)
			DepthDifference = (LinearDepth - DepthToLinear(GBufferD.Sample(SampleType, Coordinates.xy + SampleUV[i]*SamplesID).r));
		
		if (abs(DepthDifference) < 0.02f)
			DeltaDepth += DepthDifference;
		else return 1;
	}
	DeltaDepth /= 4*Samples;

	if (abs(DeltaDepth) > 0.02f)
		Occlusion = 1;
	else
		Occlusion = pow(1.0f - (DeltaDepth) * 1000,0.5f);
		
	return Occlusion;
}

PixelInputType VertexShaderFunction(uint VertID : SV_VERTEXID)
{
	PixelInputType PIn;
	float2 texcoord = float2(VertID & 1,VertID >> 1); //you can use these for texture coordinates later
	PIn.position = float4((texcoord.x - 0.5f) * 2, -(texcoord.y - 0.5f) * 2, 0, 1);
	PIn.texCoord = texcoord;
	return PIn;
}

float3 Fresnel_Schlick(float cosT, float3 F0)
{
	return F0 + (1 - F0) * pow(1 - cosT, 5);
}

float4 PixelShaderFunction(PixelInputType PIn) : SV_TARGET
{
	float3 Normal;
	float4 Color = float4(0,0,0,1);

	float4 BaseColor = float4(0,0,0,1);
	float  Roughness = 0.1;
	float  Metalness = 0;
	float  SpecularPower = 1;
	float4 Position;
	float3 lightDir;

	float  LightIntensity;
	
	float3 ViewDirection;
	float3 WorldPosition = float3(0, 0, 0);
	float  D = 0;
	
	float  AmbientIntensity = 0.5;	
	float  ior = 1.5;
	
	float3 Specular = 1;
	float  Depth;
	float  ShadowDepth = 0;
	float  ShadowBlur = 0.5f;
	float  ShadowX = float(ShadowBlur / 512.0f);
	float  ShadowY = float(ShadowBlur / 512.0f);
	float2 TexelCenter = (0.5f / 512.0f);
	int    PCFSamples = 2;
	float2 offset = 0;
	float4 PixelShadow = float4(0,0,0,1);
	float  bias = 0.001f;
	float  ShadowPower = 0;

	


	lightDir = -LightDirection;
	
	
	
	BaseColor = GBufferA.Sample(SampleType, PIn.texCoord.xy);
	Normal = GBufferB.Sample(SampleType, PIn.texCoord.xy).xyz;
	Metalness = GBufferC.Sample(SampleType, PIn.texCoord.xy).g;
	Roughness = GBufferC.Sample(SampleType, PIn.texCoord.xy).r;
	Depth = GBufferD.Sample(SampleType, PIn.texCoord.xy).r;
	//ShadowDepth = ShadowMapSample.Sample(SampleType, PIn.texCoord.xy).r;
	
	/////WorldSpaceReconstruction//////
	float x = PIn.texCoord.x * 2 - 1;
	float y = (1 - PIn.texCoord.y) * 2 - 1;
	float4 PixelPosition = float4(x, y, Depth, 1);
	PixelPosition = mul(PixelPosition,InverseProjectionMatrix);
	
	WorldPosition = PixelPosition.xyz / PixelPosition.w;



	//ViewDirection = CameraPosition.xyz - WorldPosition.xyz;
	ViewDirection = CameraDirection;
	//ViewDirection = normalize(ViewDirection);



	/*
	//PixelShadow.x = PixelShadow.x / 2 + 0.5;
	//PixelShadow.y = PixelShadow.y / -2 + 0.5;
	PIn.texCoord.x = PIn.texCoord.x * 2 - 1;
	PIn.texCoord.y = PIn.texCoord.y * 2 - 1;

	WorldPosition = float4(PIn.texCoord.xy,1.0f, 1.0f);
	WorldPosition = mul(WorldPosition, projectionMatrix);
	//WorldPosition = mul(WorldPosition, viewMatrix);
	WorldPosition = float4(DepthToLinear(Depth)*1000*WorldPosition.xyz,1.0f);
	*/
	//DynamicShadow
	//WorldPosition.w = 1;
	
	//PixelShadow = mul(WorldPosition, ProjectionMatrix);
	//PixelShadow = mul(PixelShadow, lightViewMatrix);


	//PixelShadow.xyz /= PixelShadow.w;
	//PixelShadow.z -= bias;
	

	//PixelShadow.x = PixelShadow.x / 2 + 0.5;
	//PixelShadow.y = PixelShadow.y / -2 + 0.5;
	
	
	/*

	for (y = -1 * PCFSamples; y <= 1 * PCFSamples; y += 1.0)
	{
		for (x = -1 * PCFSamples; x <= 1 * PCFSamples; x += 1.0)
		{
			offset[0] = ShadowX*x;
			offset[1] = ShadowY*y;
			
			float shadowMapDepth = ShadowMapSample.SampleLevel(SampleType, TexelCenter + offset, 0).r;
			if (ShadowDepth > PixelShadow.z)
				shadow += 1;
		}
	}
	*/

	//shadow = saturate(shadow / float(pow(2 * PCFSamples + 1, 2)));



	float3 F0 = abs((1.0 - ior) / (1.0 + ior));
	F0 = F0 * F0;
	F0 = lerp(F0, BaseColor.rgb, Metalness);
	
	
	float3 H = normalize(lightDir + ViewDirection);
	


	float NoH = max(0.0f, dot(Normal, H));
	float NoL = saturate(dot(Normal, lightDir));
	float NoV = dot(Normal, ViewDirection);
			
	LightIntensity = NoL*LightPower;

		D = GGX_Distribution(Normal, H, Roughness);
		
		//Fresnel Schlick
		//float3 F = F0 + (1.0f - F0) * pow(1.0f - NoL, 5.0);
		float3 F = Fresnel_Schlick(saturate(dot(H, ViewDirection)), F0);
	

		//Schlick Geometry
	float k = pow(Roughness + 1, 2);
	float 	GV = NoV / (NoV*(1 - k) + k);
	float 	GL = NoL / (NoL*(1 - k) + k);
	float	G = GV*GL;
	

	
	

	////////////Cook Torance
		Specular = (D*F*G) / 4 * NoL*NoV;
	
	BaseColor = GammaToLinear(BaseColor);
	Color = saturate(((1-Metalness)*BaseColor + float4(Specular.xyz, 1))*(LightIntensity)*LightColor + AmbientIntensity*BaseColor);
	Color = LinearToGamma(Color);
	//Color = Color*SSAO(10, PIn.texCoord, Normal);
	//Color = SSAO(10, PIn.texCoord, Normal);
	
return	Color;
}