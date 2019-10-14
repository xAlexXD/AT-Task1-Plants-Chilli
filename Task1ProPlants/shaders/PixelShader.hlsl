cbuffer CBuf
{
	float4 face_colors[8];
};

float4 main(uint tid : SV_PrimitiveID) : SV_Target
{
	//Make it modulus of 6 so that once its done it will wrap back around to the start of the array coming from CBuf
	return face_colors[tid/2 % 8];
}