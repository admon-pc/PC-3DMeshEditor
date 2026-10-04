cbuffer cbVertex
{
	double4x4 WVP;
	double4x4 W;
	float4 ViewDir;
};

cbuffer cbPixel
{
	float4 Color;
};

struct VSIn
{
	float3 position : POSITION;
	float2 uv1 : TEXCOORD0;
	float2 uv2 : TEXCOORD1;
	float3 normal : NORMAL;
	float3 binormal : BINORMAL;
	float3 tangent : TANGENT;
	float4 color : COLOR;
	float4 Weights : WEIGHTS;
	uint4  Bones : BONES;
}; 

struct VSOut
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
}; 

struct PSOut
{
	float4 color : SV_Target; 
};



VSOut VSMain(VSIn input)
{
	VSOut output; 
	output.pos   = mul(WVP, double4(input.position.x, input.position.y, input.position.z, 1.0));
	output.uv = input.uv1;
	output.normal = mul((float3x3)W, input.normal);
	return output; 
}

PSOut PSMain(VSOut input)
{
	PSOut output;
	float3 SunPosition = normalize(float3(3.1f, 4.f, 0.f));

	float3 lightDir = normalize(SunPosition.xyz - ViewDir.xyz);
	float diff = max(dot(input.normal, lightDir), 0.0);

	output.color = Color;

	return output; 
}

