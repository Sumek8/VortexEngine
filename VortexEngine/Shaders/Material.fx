float  ConstantFloat0 = 0;
float2 Constant2Float1 = float2(0,0);
float3 Constant3Float2 = float3(0,0,0);
float4 Texture0= TextureSample0.Sample(SampleType, input.uv); 

cbuffer ConstantBufferType 
{							
float3 LightDirection; 
float4 DiffuseColor; 
float padding;  
}; 

 struct PixelInputType	
{									
float4 position : SV_POSITION;		
float2 uv : TEXCOORD;				
float3 normal : TEXCOORD1;			
float3 tangent : TEXCOORD2;			
float3 binormal : TEXCOORD3;		
float3 viewDirection : TEXCOORD4;   
}; 
 struct PixelOutputType			    
{									
	float4 Color : SV_Target0;	    
	float4 Normal : SV_Target1;		
	float4 Position : SV_Target2;	
	float4 Depth : SV_Target3;		
};
SamplerState SampleType : register(s0); 

 Texture2D TextureSample0: register(t0); 

 Texture2D TextureSample1: register(t1); 

 PixelOutputType   PixelShaderFunction(PixelInputType input){ 
PixelOutputType Out;						
Out.Normal = float4(0.5, 0.5, 1, 1);		 
Out.Color = float4(0, 0, 0, 1);				
Out.Position = float4(0, 0, 0, 1);			

float4 Texture0= TextureSample0.Sample(SampleType, input.uv); 
float4 Texture1= TextureSample1.Sample(SampleType, input.uv); 

Out.Position.xyz = input.viewDirection;
Out.Depth = input.position.z / input.position.w;
return Out;										
				  }								

cbuffer ConstantBufferType 
{							
float3 LightDirection; 
float4 DiffuseColor; 
float padding;  
}; 

 struct PixelInputType	
{									
float4 position : SV_POSITION;		
float2 uv : TEXCOORD;				
float3 normal : TEXCOORD1;			
float3 tangent : TEXCOORD2;			
float3 binormal : TEXCOORD3;		
float3 viewDirection : TEXCOORD4;   
}; 
 struct PixelOutputType			    
{									
	float4 Color : SV_Target0;	    
	float4 Normal : SV_Target1;		
	float4 Position : SV_Target2;	
	float4 Depth : SV_Target3;		
};
SamplerState SampleType : register(s0); 

 Texture2D TextureSample0: register(t0); 

 Texture2D TextureSample1: register(t1); 

 PixelOutputType   PixelShaderFunction(PixelInputType input){ 
PixelOutputType Out;						
Out.Normal = float4(0.5, 0.5, 1, 1);		 
Out.Color = float4(0, 0, 0, 1);				
Out.Position = float4(0, 0, 0, 1);			

float4 Texture0= TextureSample0.Sample(SampleType, input.uv); 
float4 Texture1= TextureSample1.Sample(SampleType, input.uv); 
float4 Texture2= TextureSample2.Sample(SampleType, input.uv); 
float4 Texture3= TextureSample3.Sample(SampleType, input.uv); 

Out.Position.xyz = input.viewDirection;
Out.Depth = input.position.z / input.position.w;
return Out;										
				  }								

cbuffer ConstantBufferType 
{							
float3 LightDirection; 
float4 DiffuseColor; 
float padding;  
}; 

 struct PixelInputType	
{									
float4 position : SV_POSITION;		
float2 uv : TEXCOORD;				
float3 normal : TEXCOORD1;			
float3 tangent : TEXCOORD2;			
float3 binormal : TEXCOORD3;		
float3 viewDirection : TEXCOORD4;   
}; 
 struct PixelOutputType			    
{									
	float4 Color : SV_Target0;	    
	float4 Normal : SV_Target1;		
	float4 Position : SV_Target2;	
	float4 Depth : SV_Target3;		
};
SamplerState SampleType : register(s0); 

 Texture2D TextureSample0: register(t0); 

 Texture2D TextureSample1: register(t1); 

 PixelOutputType   PixelShaderFunction(PixelInputType input){ 
PixelOutputType Out;						
Out.Normal = float4(0.5, 0.5, 1, 1);		 
Out.Color = float4(0, 0, 0, 1);				
Out.Position = float4(0, 0, 0, 1);			

float4 Texture0= TextureSample0.Sample(SampleType, input.uv); 
float4 Texture1= TextureSample1.Sample(SampleType, input.uv); 
float4 Texture2= TextureSample2.Sample(SampleType, input.uv); 
float4 Texture3= TextureSample3.Sample(SampleType, input.uv); 
float4 Texture4= TextureSample4.Sample(SampleType, input.uv); 
float4 Texture5= TextureSample5.Sample(SampleType, input.uv); 

Out.Position.xyz = input.viewDirection;
Out.Depth = input.position.z / input.position.w;
return Out;										
				  }								
