struct Input
{
    float4 pos : POSITION;
    float2 uv  : TEXCOORD;
};

struct V2F
{
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD;
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
    v2f.pos = mul(mul(mul(proj, view), world), i.pos);
    v2f.uv  = i.uv;
    
	return v2f;
}