cbuffer cbVertex
{
	double4x4 WVP;
};
cbuffer cbPixel
{
	float4 BaseColor;
};
struct VSIn
{
	float3 position : POSITION;
	float4 color : COLOR;
}; 

struct VSOut
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
}; 

struct PSOut
{
	float4 color : SV_Target; 
};



VSOut VSMain(VSIn input)
{
	VSOut output; 
	output.pos   = mul(WVP, double4(input.position.x, input.position.y, input.position.z, 1.f));
	output.pos.z    -= 0.00015f;
	output.color    = input.color;
	return output; 
}

PSOut PSMain(VSOut input)
{
	PSOut output;
	output.color = input.color * BaseColor;
	return output; 
}

