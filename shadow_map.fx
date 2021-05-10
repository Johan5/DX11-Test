
#pragma pack_matrix( row_major )

cbuffer PerLightCb : register(b2)
{
	float4x4 _LightViewMatrix[6];
	float4x4 _LightProjectionMatrix;
	float _LightMaxDist;
	float3 _Pad1;
};

struct SMaterial
{
	float _DiffuseStrength;
	float _SpecularStrength;
	float _AmbientStrength;
	// Large specular power means small specular highlight
	int _SpecularPower; // "phong exponent"
};

cbuffer PerObjectCb : register(b3) 
{
	float4x4 _WorldMatrix;
}

struct SVsInput
{
	float3 _L_Position : POSITION;
	float3 _L_Normal : NORMAL;
	float4 _Color : COLOR;
};

struct SGsInput
{
	float4 _W_Position : SV_POSITION;
};

struct SPsInput
{
	float4 _S_Position : SV_POSITION;
	float3 _L0_Position : POSITION;
	uint _CubeFaceIdx : SV_RenderTargetArrayIndex;
};


SGsInput VS(SVsInput Input)
{
	SGsInput Output;
	Output._W_Position = mul(_WorldMatrix, float4(Input._L_Position, 1.0f));
	return Output;
}

[maxvertexcount(18)]
void GS(triangle SGsInput Input[3], inout TriangleStream<SPsInput> OutputStream)
{
	SPsInput Output;

	for (int CubeFace = 0; CubeFace < 6; ++CubeFace)
	{
		Output._CubeFaceIdx = CubeFace;
		for (int VertexIdx = 0; VertexIdx < 3; ++VertexIdx)
		{
			float4 L0_Position = mul(_LightViewMatrix[CubeFace], Input[VertexIdx]._W_Position);
			Output._S_Position = mul(_LightProjectionMatrix, L0_Position);
			Output._L0_Position = L0_Position.xyz / L0_Position.w;
			OutputStream.Append(Output);
		}
		OutputStream.RestartStrip();
	}
}

float4 PS(SPsInput Input) : SV_Target
{
	// DX11 will automatically write to the correct render target slice based on Input._CubeFaceIdx)
	return length(Input._L0_Position);
}
