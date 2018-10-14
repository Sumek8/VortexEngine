 
////////////////////////////////////////////////////////////////////////////////

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
	float2 UV :TEXCOORD;
	float4 color :COLOR;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
	float2 UV :TEXCOORD;
	float4 color:COLOR;
};

PixelInputType WidgetFunctionVS(VertexInputType input)
{
    PixelInputType output;	
    // Change the position vector to be 4 units for proper matrix calculations.
  
	output.position.w = 1;
   
    output.position.x = (input.position.x - 0.5f) *	2;
	output.position.y = (input.position.y - 0.5f) * -2;
	output.position.z = 0;
	output.UV = input.UV;
	output.color = input.color;
    return output;
}


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 WidgetFunctionPS(PixelInputType input) : SV_TARGET
{ 
	
	//float x,y;

	//x = abs(float(input.UV.x * 2 - 1));
	//y = abs((input.UV.y * 2 - 1));
	float4 color;

	
	color = input.color;
	//clip(pow((x*y),5) > 0.8 ? -1 : 1);
		
	return color;
}
