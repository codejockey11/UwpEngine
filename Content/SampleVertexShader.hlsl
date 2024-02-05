cbuffer WorldViewProjectionConstantBuffer : register(b0)
{
	matrix wvp;
};

struct VertexShaderInput
{
	float3 pos   : POSITION;
	float4 color : COLOR0;
};

struct PixelShaderInput
{
	float4 pos   : SV_POSITION;
	float4 color : COLOR0;
};

PixelShaderInput main(VertexShaderInput input)
{
	PixelShaderInput output;
	
	float4 pos = float4(input.pos, 1.0f);

	pos = mul(pos, wvp);
	
	output.pos = pos;

	output.color = input.color;

	return output;
}