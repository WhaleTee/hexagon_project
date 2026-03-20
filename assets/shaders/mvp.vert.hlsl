struct Input
{
    float3 Position : TEXCOORD0;
    float4 Color : TEXCOORD1;
};

struct Output
{
    float4 Color : TEXCOORD0;
    float4 Position : SV_Position;
};

cbuffer UBO : register(b0, space1)
{
    float4x4 Model : packoffset(c0);
    float4x4 View : packoffset(c4);
    float4x4 Projection : packoffset(c8);
};

Output main(Input input)
{
    Output output;
    output.Color = input.Color;
    float4x4 MVP = Projection * View * Model;
    output.Position = mul(MVP, float4(input.Position, 1.0f));
    return output;
}
