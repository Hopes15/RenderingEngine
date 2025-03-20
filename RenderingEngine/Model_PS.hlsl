struct Input
{
	float4 svPos  : SV_POSITION;
    float4 normal : NORMAL;
    float2 uv     : TEXCOORD;
};

Texture2D    tex : register(t0);
SamplerState smp : register(s0);

cbuffer b2 : register(b2)
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
}

float4 main(Input i) : SV_TARGET
{
    //TODO : ƒ}ƒeƒŠƒAƒ‹‚ð‚µ‚Á‚©‚è‚Æ”½‰f‚³‚¹‚é
    
    float4 LightPos   = float4(0, -0.5, -0.5, 0);
    float  brightness = max(dot(i.normal, LightPos), 0);
    float4 _Diffuse   = saturate(float4(ambient.xyz + diffuse.xyz * brightness, diffuse.a));
    
    return tex.Sample(smp, i.uv) * _Diffuse;
    //return tex.Sample(smp, i.uv);
    //return float4(i.uv, 0, 1);
}