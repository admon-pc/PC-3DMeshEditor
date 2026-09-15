struct VSIn
{
	float3 position : POSITION;
	float4 color : COLOR;
	float4 Weights : WEIGHTS;
	uint4  Bones : BONES;
};


cbuffer cbVertex
{
	double4x4 WVP;
	float4 Eye;
	float4 Viewport;
};

struct VSOut
{
	float4 pos : SV_POSITION;
	float4 vColor : COLOR0;
};

struct PSOut
{
    float4 color : SV_Target;
};

VSOut VSMain(VSIn input)
{
	VSOut output;
	output.vColor    = input.color;
	output.vColor.w    = 1.f;


	output.pos = mul(WVP, float4(input.position,1.f));
	output.pos.z    -= 0.00016f;
	output.pos = output.pos / output.pos.w;

	return output;
}

PSOut PSMain(VSOut input)
{
	PSOut output;
	output.color = input.vColor;
   return output;
}

[maxvertexcount(4)]
void GSMain(point VSOut input[1], inout TriangleStream<VSOut> TriStream )
{
	VSOut Out;
	Out.vColor = input[0].vColor;

	const float sz = 5.f;
	float h = 2.f / Viewport.y;
	float w = 2.f / Viewport.x;
	float x = sz * w;
	float y = sz * h;
	float half_x = x * 0.5f;
	float half_y = y * 0.5f;
	const float4 sizes[4] = 
	{
		float4(-half_x, -half_y, 0, 0),
		float4(-half_x, half_y, 0, 0),
		float4(half_x, -half_y, 0, 0),
		float4(half_x, half_y, 0, 0)
	};

	for (uint i = 0; i<4; ++i)
	{
		Out.pos =input[0].pos;
		Out.pos += sizes[i];
		TriStream.Append(Out);
	}

	TriStream.RestartStrip();
}
