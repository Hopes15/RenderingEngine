struct Input
{
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD;
};

float4 main(Input i) : SV_TARGET
{
	return float4(i.uv, 1.0f, 1.0f);
}