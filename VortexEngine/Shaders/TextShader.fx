 
////////////////////////////////////////////////////////////////////////////////
// Filename: Shadow.fx
////////////////////////////////////////////////////////////////////////////////
/////////////
// GLOBALS //
/////////////


//////////////
// TYPEDEFS //
//////////////

BlendState SrcAlphaBlendingAdd
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};




SamplerState Sampler : register(s0);



Texture2D Texture  : register(t0);

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
	
	
	float4 color;
	color = Texture.Sample(Sampler, input.UV);
	
	//clip(color.r < 0.1f ? -1 : 1);
	
	color = input.color*color;
	color.a = color.r;
	return color;
}
technique10 render
{
	pass P0
	{
		
		SetVertexShader(CompileShader(vs_4_0, WidgetFunctionVS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, WidgetFunctionPS()));

		
		
	}
}