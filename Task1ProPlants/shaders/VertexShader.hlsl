struct VSOut
{
	float3 col : Color;
	float4 pos : SV_Position;
};

cbuffer CBuf
{
	matrix transform;
};

VSOut main(float2 pos : Position, float3 color : Color)
{
	VSOut output;
	output.pos = mul(float4(pos.x, pos.y, 0.0f, 1.0f), transform);
	output.col = color;

	return output;
}