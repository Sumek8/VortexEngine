 
////////////////////////////////////////////////////////////////////////////////
// Filename: Shadow.fx
////////////////////////////////////////////////////////////////////////////////
/////////////
// GLOBALS //
/////////////


//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
	float4 color :COLOR;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
	float4 color:COLOR;
};

PixelInputType LineFunctionVS(VertexInputType input)
{
    PixelInputType output;	
    // Change the position vector to be 4 units for proper matrix calculations.
  
	output.position.w = 1;
    // Calculate the position of the vertex against the world, view, and projection matrices.

	//output.position = float4((input.UV.x - 0.5f) * 2, -(input.UV.y - 0.5f) * 2, 0, 1);

    output.position.x = (input.position.x - 0.5f) *	2;
	output.position.y = (input.position.y - 0.5f) * -2;
	output.position.z = 0;
	output.color = input.color;
    return output;
}


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 LineFunctionPS(PixelInputType input) : SV_TARGET
{ 

	
	float4 color;
	color = float4(input.color);
	return color;


	
}
