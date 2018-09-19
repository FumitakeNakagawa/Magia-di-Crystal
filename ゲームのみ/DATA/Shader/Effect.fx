//------------------------------------------------------
//		テクスチャサンプラー	
//------------------------------------------------------
texture Texture;	//	デカールテクスチャ
sampler DecaleSamp = sampler_state
{
	Texture = <Texture>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
};

struct VS_BASIC
{
	float4 Pos    : POSITION;
	float4 Color  : COLOR0;
	float2 Tex	  : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 Pos		: POSITION;
	float4 Color	: COLOR0;
	float2 Tex		: TEXCOORD0;
	float3 Normal	: TEXCOORD1;

	float4 Ambient	: COLOR1;
	float4 light	: COLOR2;
	float3 vLight	: TEXCOORD2;
	float3 vE		: TEXCOORD3;
	float  specular : TEXCOORD4;
};

struct VS_INPUTL
{
	float4 Pos    : POSITION;
	float3 Normal : NORMAL;
	float4 Color  : COLOR0;
	float2 Tex	  : TEXCOORD0;
};

float4x4 TransMatrix;	//	変換行列


//------------------------------------------------------
//	頂点シェーダー
//------------------------------------------------------
VS_BASIC VS_Basic(VS_INPUTL In)
{
	VS_BASIC Out = (VS_BASIC)0;

	float3 P = mul(In.Pos, TransMatrix);

	float3x3	mat = TransMatrix;
	float3 N = mul(In.Normal, mat);
	N = normalize(N);

	Out.Pos = mul(In.Pos, Projection);
	Out.Color.rgb = DirLight(LightDir, N) + HemiLight(N);
	Out.Color.a = 1.0f;
	Out.Tex = In.Tex;

	return Out;
}


float4 PS_Basic(VS_BASIC In) : COLOR
{
	float4	OUT;
//	ピクセル色決定
float2 tex;
tex = In.Tex * float2(uv_width, uv_height) + float2(uv_left, uv_top);
OUT = In.Color * tex2D(DecaleSamp, tex);

return OUT;
}

technique uv_anime
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		ZWriteEnable = true;

		VertexShader = compile vs_3_0 VS_Basic();
		PixelShader = compile ps_3_0 PS_Basic();
	}
}