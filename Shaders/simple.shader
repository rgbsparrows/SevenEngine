cbuffer SInfo1 : register(b0, space0)
{
    float4 Color;
    float4x4 WorldMatrix;
};

cbuffer SInfo2 : register(b1, space0)
{
    float4x4 ViewMatrix;
    float4x4 ProjMatrix;
};

struct SMUVSInput
{
    float3 Position : POSITION;
};

void VsMain(SMUVSInput _input, out float4 _pos : SV_POSITION)
{
    _pos = mul(float4(_input.Position.yzx, 1.f), WorldMatrix);
    _pos = mul(_pos, ViewMatrix);
    _pos = mul(_pos, ProjMatrix);
}

void PsMain(out float4 _color : SV_TARGET)
{
    _color = Color;
}