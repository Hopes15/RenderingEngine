struct Input
{
    float4 pos    : POSITION;
    float4 normal : NORMAL;
    float2 uv     : TEXCOORD;
};

struct Output
{
    float4 svPos  : SV_POSITION;
    float4 normal : NORMAL;
    float2 uv     : TEXCOORD;
};

cbuffer b0 : register(b0)
{
    matrix world;
}

cbuffer b1 : register(b1)
{
    matrix view;
    matrix proj;
}

Output main(Input i)
{
    Output o;
    o.svPos  = mul(mul(mul(proj, view), world), i.pos);
    
    i.normal.w = 0;
    o.normal = normalize(mul(world, i.normal));
    o.uv     = i.uv;
    
	return o;
}