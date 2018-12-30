 
////////////////////////////////////////////////////////////////////////////////
// Filename: Shadow.fx
////////////////////////////////////////////////////////////////////////////////
/////////////
// GLOBALS //
/////////////



cbuffer MatrixBuffer
{
    matrix worldMatrix;
	matrix ProjectionMatrix;
	matrix ViewMatrix;
};
//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
};

PixelInputType CustomDepthVS(VertexInputType input)
{
    PixelInputType output;	
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position,worldMatrix);
    output.position = mul(output.position,ViewMatrix);
    output.position = mul(output.position,ProjectionMatrix);

    return output;
}


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 CustomDepthpPS(PixelInputType input) : SV_TARGET
{ 
	
	float depthValue;
	float4 color;

	depthValue = input.position.z/input.position.w;
	color = float4(depthValue, depthValue, depthValue, 1.0f);

	return color;
}
