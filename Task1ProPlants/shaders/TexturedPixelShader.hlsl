Texture2D tex;
SamplerState splr;

struct VSOut
{
	float2 tex : TexCoord;
	float4 pos : SV_Position;
};

float4 main(VSOut input) : SV_TARGET
{
	return tex.Sample(splr, input.tex);
	//return float4(1.0f,0.0f,0.0f,1.0f);
}