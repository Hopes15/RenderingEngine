

struct Input
{
    float4 pos      : SV_POSITION;
	float3 normal   : NORMAL;
    float2 uv       : TEXCOORD;
	float3 tangent  : TANGENT;
	float4 color    : COLOR;
};

float4 main(Input i) : SV_TARGET
{
	return float4(i.uv, 1.0f, 1.0f);
}