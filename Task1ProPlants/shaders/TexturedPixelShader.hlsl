Texture2D tex;
SamplerState splr;

float4 main(float2 tc : TexCoord) : SV_TARGET
{
	return tex.Sample(splr, tc);
	//return float4(1.0f,0.0f,0.0f,1.0f);
}