cbuffer WorldViewProjectionConstantBuffer : register(b0)
{
	row_major float4x4 wvp;

	//float float4x4spadding[48];
};

struct SPixel
{
	float4 position	: SV_POSITION;
	float2 uv		: TEXCOORD;
	float3 color	: COLOR;
};

SPixel VSMain(float4 position : POSITION, float2 uv : TEXCOORD, float3 color : COLOR)
{
	SPixel result;

	result.position = mul(position, wvp);

	result.uv = uv;

	result.color = color;

	return result;
}