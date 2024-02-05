struct SPixel
{
	float4 position	: SV_POSITION;
	float2 uv		: TEXCOORD;
	float3 color	: COLOR;
};

Texture2D	texture0		: register(t0);

SamplerState anisotropic	: register(s0);
SamplerState linears		: register(s1);

float4 PSMain(SPixel vs) : SV_Target0
{
	return texture0.Sample(anisotropic, vs.uv) * float4(vs.color, 1.0f);
}