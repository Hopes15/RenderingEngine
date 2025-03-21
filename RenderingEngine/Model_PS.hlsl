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
    ////Light
    //float3 light = normalize(float3(1, -1, 1));
    
    //float brightness = dot(-light, i.normal.xyz);
    
    ////Tex
    //float4 texColor = tex.Sample(smp, i.uv);
    
    //return float4(brightness, brightness, brightness, 1) * diffuse * texColor;
    
    float4 LightPos = float4(0, -0.5, -0.5, 0);
    float brightness = max(dot(i.normal, LightPos), 0);
    float4 _Diffuse = saturate(float4(ambient.xyz + diffuse.xyz * brightness, diffuse.a));
    
    return tex.Sample(smp, i.uv) * _Diffuse;
}