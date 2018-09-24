 
////////////////////////////////////////////////////////////////////////////////
// Filename: Color.fx
////////////////////////////////////////////////////////////////////////////////
/////////////
// GLOBALS //
/////////////

SamplerState SampleType : register(s0);
SamplerState SampleTypeClamp : register(s1);

SamplerState PCF_Sampler
{
	Filter = D3D10_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	AddressU = Clamp;
	AddressV = Clamp;
};

cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
	matrix lightProjectionMatrix;
	matrix lightViewMatrix;
	float3 CameraPosition;
};

//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	
};

struct PixelInputType
{
    float4 position : SV_POSITION;
	float2 uv :  TEXCOORD;
	float4 lightPosition : TEXCOORD1;
	float3 normal : TEXCOORD2;
	float3 tangent : TEXCOORD3;
	float3 binormal : TEXCOORD4;
	float3 viewDirection : TEXCOORD5;
};

PixelInputType VertexShaderFunction(VertexInputType input)
{
    PixelInputType output;

	float4 worldPosition;



	
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position,worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.uv = input.uv;



	// Calculate the position of the vertice as viewed by the light source.
	output.lightPosition = mul(input.position, worldMatrix);
	output.lightPosition = mul(output.lightPosition, lightProjectionMatrix);
	output.lightPosition = mul(output.lightPosition, lightViewMatrix);

	

    output.tangent = mul(input.tangent, (float3x3)worldMatrix);
	output.tangent = normalize(output.tangent);

    //Calculate the binormal vector against the world matrix only and then normalize the final value.
	output.binormal = mul(input.binormal, (float3x3)worldMatrix);
	output.binormal = normalize(output.binormal);

	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	worldPosition = mul(input.position, worldMatrix);
	output.viewDirection = CameraPosition.xyz - worldPosition.xyz;
	output.viewDirection = normalize(output.viewDirection);
    return output;
}


