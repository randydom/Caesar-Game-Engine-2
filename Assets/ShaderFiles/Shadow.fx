
SamplerState sam
{
	Filter = MIN_MAG_LINEAR_MIP_POINT;
	AddressU = Wrap;
	AddressV = Wrap;
	AddressW = Wrap;

};

static const float SMAP_SIZE = 1028.0f;
static const float SMAP_DX = 0.25f / SMAP_SIZE;

float CalcShadowFactor(float4 shadowPosH, LightDesc L)
{
	// Complete projection by doing division by w.
	shadowPosH.xyz /= shadowPosH.w;

	// Depth in NDC space.
	float depth = shadowPosH.z;

	// Texel size.
	static const float dx = SMAP_DX;
	static const float dxM = dx * 9;
	const float2 offsets[9] =
	{
		float2(-dx, -dx), float2(0.0f, -dx), float2(dx, -dx),
		float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
		float2(-dx, +dx), float2(0.0f, +dx), float2(dx, +dx)
	};

	float sourcevals = 1.0f;
	[unroll]
	for(int i = 0; i < 9; ++i)
	{
		float2 loc = shadowPosH.xy + offsets[i];
		float depthTexture = Shadow.Sample(sam, float3(loc, L.ShadowNum)).x;

		if (depthTexture < 1.0f
			&& loc.x < 1.0f && loc.x > 0.0f
			&& loc.y < 1.0f && loc.y > 0.0f)
		{
			bool inShadow = (shadowPosH.z - depthTexture) >= dxM;
			if(inShadow == true)
			{
				sourcevals -= 0.1;
			}
		}
	}

	return sourcevals;
}