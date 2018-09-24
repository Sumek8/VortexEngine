 
////////////////////////////////////////////////////////////////////////////////
// Filename: Color.fx
////////////////////////////////////////////////////////////////////////////////
/////////////
// GLOBALS //
/////////////

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
	float3 normal : TEXCOORD1;
	float3 tangent : TEXCOORD2;
	float3 binormal : TEXCOORD3;
	float3 WorldPosition : TEXCOORD4;
};

PixelInputType VertexShaderFunction(VertexInputType input)
{
	float3 WorldPosition;

    PixelInputType output;
	
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
   
	output.position = mul(input.position, worldMatrix);
  	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	//output.position = input.position;
    output.uv = input.uv;

	
    output.tangent = mul(input.tangent, (float3x3)worldMatrix);
	output.tangent = normalize(output.tangent);

    //Calculate the binormal vector against the world matrix only and then normalize the final value.
	output.binormal = mul(input.binormal, (float3x3)worldMatrix);
	output.binormal = normalize(output.binormal);

	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	
	WorldPosition = mul(input.position,worldMatrix);
	//output.viewDirection = CameraPosition.xyz - WorldPosition.xyz;
	//output.viewDirection = normalize(output.viewDirection);
	output.WorldPosition = WorldPosition;
    return output;
}


