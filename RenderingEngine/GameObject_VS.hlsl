struct Input
{
    float4 pos     : POSITION;
    float3 normal  : NORMAL;
    float2 uv      : TEXCOORD;
    float3 tangent : TANGENT;
    float4 color   : COLOR;
};

struct V2F
{
    float4 pos      : SV_POSITION;
	float3 normal   : NORMAL;
    float2 uv       : TEXCOORD;
	float3 tangent  : TANGENT;
	float4 color    : COLOR;
};

cbuffer WorldMat : register(b0)
{
    matrix world;
}

cbuffer VPMat : register(b1)
{
    matrix view;
    matrix proj;
}

V2F main(Input i)
{
    V2F v2f;
    v2f.pos     = mul(mul(mul(proj, view), world), i.pos);
    v2f.normal  = i.normal;
    v2f.uv      = i.uv;
    v2f.tangent = i.tangent;
    v2f.color   = i.color;
    
	return v2f;
}