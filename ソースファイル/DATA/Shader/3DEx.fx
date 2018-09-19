//**************************************************************************************************
//																									
//		��{�V�F�[�_�[		
//
//**************************************************************************************************

//------------------------------------------------------
//		���֘A
//------------------------------------------------------
float4x4 TransMatrix;	//	�ϊ��s��
float4x4 matView;		//	�ϊ��s��
float4x4 Projection;	//	�ϊ��s��

float3	ViewPos;
float	 redValue;
//------------------------------------------------------
//		�e�N�X�`���T���v���[	
//------------------------------------------------------
texture Texture;	//	�f�J�[���e�N�X�`��
sampler DecaleSamp = sampler_state
{
    Texture = <Texture>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Wrap;
    AddressV = Wrap;
};

texture NormalMap;	//	�@���}�b�v�e�N�X�`��
sampler NormalSamp = sampler_state
{
    Texture = <NormalMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Wrap;
    AddressV = Wrap;
};

texture SpecularMap;	//	�X�y�L�����}�b�v�e�N�X�`��
sampler SpecularSamp = sampler_state
{
    Texture = <SpecularMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Wrap;
    AddressV = Wrap;
};

texture HeightMap;		//	�����}�b�v�e�N�X�`��
sampler HeightSamp = sampler_state
{
    Texture = <HeightMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Wrap;
    AddressV = Wrap;
};



//**************************************************************************************************
//		���_�t�H�[�}�b�g
//**************************************************************************************************
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
	float  specular	: TEXCOORD4;
};

struct VS_INPUTL
{
    float4 Pos    : POSITION;
    float3 Normal : NORMAL;
    float4 Color  : COLOR0;
    float2 Tex	  : TEXCOORD0;
};

//�g�D�[���p���_
struct VS_OUTPUT_TOON
{
	float4 Pos		:	POSITION;
	float4 Color	:	COLOR0;
	float2 Tex		:	TEXCOORD0;

	float  Level : TEXCOORD1;
};


//**************************************************************************************************
//
//		���C�e�B���O
//
//**************************************************************************************************

//**************************************************************************************************
//	�������C�e�B���O
//**************************************************************************************************
float3 SkyColor = { 0.48f, 0.5f, 0.5f };
float3 GroundColor = { 0.4f, 0.4f, 0.4f };

inline float4 HemiLight( float3 normal )
{
	float4 color;
	float rate = (normal.y*0.5f) + 0.5f;
	color.rgb = SkyColor * rate;
	color.rgb += GroundColor * (1-rate);
	color.a = 1.0f;

	return color;
}

//**************************************************************************************************
//	���s��
//**************************************************************************************************
float3 LightDir = { 1.0f, -1.0f, 0.0f };
float3 DirLightColor = { 0.6f, 0.6f, 0.6f };

inline float3 DirLight( float3 dir, float3 normal )
{
	float3 light;
	float rate = max( 0.0f, dot( -dir, normal ) );
	light = DirLightColor * rate;
	
	return light;
}

//**************************************************************************************************
//	�X�y�L����
//**************************************************************************************************
inline float Specular(float3 pos, float3 normal)
{
    float   sp;

    float3	H = normalize(ViewPos - pos);
    H = normalize(H - LightDir);
    
	sp = dot(normal, H);
	sp = max( 0, sp );
	sp = pow(sp, 10);

    return sp;
}

//------------------------------------------------------
//		�t�H�O�֘A
//------------------------------------------------------
float	FogNear	= 60.0f;
float	FogFar  = 80.0f;
float3	FogColor = { 0.7f, 0.7f, 0.7f };


//**************************************************************************************************
//	���}�b�v�p
//**************************************************************************************************
texture EnvMap;		//	���e�N�X�`��
sampler EnvSamp = sampler_state
{
    Texture = <EnvMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Wrap;
    AddressV = Wrap;
};

inline float4 Environment( float3 normal )
{
	float4	color;
	
	float2	uv;
	uv.x = normal.x*0.5f + 0.5f;
	uv.y = normal.y*0.5f + 0.5f;

	color = tex2D( EnvSamp, uv );
	return color;
}

//�e�p���
float ToonLevel = 0.5f;		///�e�̃��x��
float ToonShadow = 0.7f;	//�e�̔Z��
float OutlineSize = 0.01f;	//�֊s���̑���


//**************************************************************************************************
//
//		��{�V�F�[�_�[
//
//**************************************************************************************************

//------------------------------------------------------
//	���_�V�F�[�_�[
//------------------------------------------------------
VS_BASIC VS_Basic( VS_INPUTL In )
{
	VS_BASIC Out = (VS_BASIC)0;

	float3 P = mul(In.Pos, TransMatrix);

	float3x3	mat = TransMatrix;
	float3 N = mul(In.Normal, mat);
	N = normalize(N);

	Out.Pos   = mul(In.Pos, Projection);
	Out.Color.rgb = DirLight( LightDir, N ) + HemiLight( N );
	Out.Color.a = 1.0f;
	Out.Tex   = In.Tex;

	return Out;
}

//------------------------------------------------------
//	���_�J���[�t�V�F�[�_�[
//------------------------------------------------------
VS_BASIC VS_Basic2( VS_INPUTL In )
{
    VS_BASIC Out = (VS_BASIC)0;
	
	float3 P = mul( In.Pos, TransMatrix );

	float3x3	mat = TransMatrix;
	float3 N = mul(In.Normal, mat);
	N = normalize(N);

	Out.Pos   = mul(In.Pos, Projection);
	Out.Color.rgb = (DirLight( LightDir, N ) + HemiLight( N ));
	Out.Color.a = In.Color.a;
	Out.Tex   = In.Tex;

    return Out;
}


VS_BASIC VS_Basic3(VS_INPUTL In)
{
	VS_BASIC Out = (VS_BASIC)0;

	float3 P = mul(In.Pos, TransMatrix);

	float3x3	mat = TransMatrix;
	float3 N = mul(In.Normal, mat);
	N = normalize(N);

	Out.Pos = mul(In.Pos, Projection);
	Out.Color.rgb = HemiLight(N);
	Out.Color.a = 1.0f;
	Out.Tex = In.Tex;

	return Out;
}

float4 PS_Basic4(VS_BASIC In) : COLOR
{
	float4	OUT;

//	�s�N�Z���F����
OUT = In.Color * tex2D(DecaleSamp, In.Tex);
OUT.r = redValue;
return OUT;
}

//  �h���S���p(2P)
float4 PS_Basic5(VS_BASIC In) : COLOR
{
	float4	OUT;

//	�s�N�Z���F����
OUT = In.Color * tex2D(DecaleSamp, In.Tex);
if (OUT.r > 0.22&&OUT.b < 0.2)
{
	OUT.r -= 0.18f;
	OUT.b += 1.0f;
	OUT.g += 0.1f;
}
OUT *= 2.0f;
return OUT;
}

//  �L�p(1P)
float4 PS_Basic6(VS_BASIC In) : COLOR
{
	float4	OUT;

//	�s�N�Z���F����
OUT = In.Color * tex2D(DecaleSamp, In.Tex);
if (OUT.r > 0.29&&OUT.b > 0.29&&OUT.g>0.29)
{
	OUT.r = 0.9f;
	OUT.g = 0.05f;
	OUT.b = 0.1f;
}
OUT *= 2.0f;
return OUT;
}

//  �L�p(2P)
float4 PS_Basic7(VS_BASIC In) : COLOR
{
	float4	OUT;

//	�s�N�Z���F����
OUT = In.Color * tex2D(DecaleSamp, In.Tex);
if (OUT.r > 0.28&&OUT.b > 0.29&&OUT.g>0.3)
{
	OUT.r = 0.2f;
	OUT.g = 0.2f;
	OUT.b = 0.8f;
}
OUT *= 2.0f;
return OUT;
}

//  �h���S���̒e�p(��)
float4 PS_Basic8(VS_BASIC In) : COLOR
{
	float4	OUT;

//	�s�N�Z���F����
OUT = In.Color * tex2D(DecaleSamp, In.Tex);
OUT.r = 0.8f;
OUT.g = 0.1f;
OUT.b = 0.1f;
OUT *= 2.0f;
return OUT;
}

//  �h���S���̒e�p(��)
float4 PS_Basic9(VS_BASIC In) : COLOR
{
	float4	OUT;

//	�s�N�Z���F����
OUT = In.Color * tex2D(DecaleSamp, In.Tex);
OUT.r = 0.2f;
OUT.g = 0.2f;
OUT.b = 0.8f;
OUT *= 2.0f;
return OUT;
}

//-----------------------------------------------------
//�A�E�g���C�����_�V�F�[�_�[
//-----------------------------------------------------
VS_OUTPUT VS_Outline(VS_INPUTL In)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	//�@�������Ɋg��
	float3 N = normalize(In.Normal);
	In.Pos.xyz = In.Pos.xyz + (N * OutlineSize);

	//
	Out.Pos = mul(In.Pos, Projection);
	Out.Tex = In.Tex;
	Out.Color.rgb = float3(0.65, 0, 0.7);
	Out.Color.a = 1;

	return Out;
}

//-----------------------------------------------------
//�g�D�[���e�V�F�[�_�[
//-----------------------------------------------------
VS_OUTPUT_TOON VS_Toon(VS_INPUTL In)
{
	VS_OUTPUT_TOON Out = (VS_OUTPUT_TOON)0;

	Out.Pos = mul(In.Pos, Projection);
	Out.Tex = In.Tex;
	Out.Color = 1;

	//�@���ϊ�
	float3x3 mat = TransMatrix;
	float3 N = mul(In.Normal, mat);
	N = normalize(N);

	//���C�g�v�Z
	Out.Level = dot(-N, LightDir);

	return Out;
}

//------------------------------------------------------
//	�s�N�Z���V�F�[�_�[	
//------------------------------------------------------
float4 PS_Basic( VS_BASIC In) : COLOR
{   
	float4	OUT;
	//	�s�N�Z���F����
	OUT = In.Color * tex2D( DecaleSamp, In.Tex );

	return OUT;
}

float uv_top;
float uv_left;
float uv_height;
float uv_width;

float4 PS_Basic2(VS_BASIC In) : COLOR
{
	float4	OUT;
//	�s�N�Z���F����
float2 tex;
tex = In.Tex * float2(uv_width, uv_height) + float2(uv_left, uv_top);
OUT = In.Color * tex2D(DecaleSamp, tex);

return OUT;
}

//------------------------------------------------------
//	�e�N�j�b�N
//------------------------------------------------------
technique copy
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		ZWriteEnable     = true;

		VertexShader = compile vs_3_0 VS_Basic();
		PixelShader  = compile ps_3_0 PS_Basic();
    }
}

technique lcopy
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		ZWriteEnable     = true;

		VertexShader = compile vs_2_0 VS_Basic2();
		PixelShader  = compile ps_2_0 PS_Basic();
    }
}

technique add
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = One;
		ZWriteEnable     = false;

		VertexShader = compile vs_3_0 VS_Basic();
		PixelShader  = compile ps_3_0 PS_Basic();
    }
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
		PixelShader = compile ps_3_0 PS_Basic2();
	}
}

//**************************************************************************************************
//
//		�t���G�t�F�N�g
//
//**************************************************************************************************
//------------------------------------------------------
//	���_�V�F�[�_
//------------------------------------------------------
VS_OUTPUT VS_FullFX( VS_INPUTL In )
{
    VS_OUTPUT Out = (VS_OUTPUT)0;
	
	float4 P = mul(In.Pos,    TransMatrix);

	float3x3	mat = TransMatrix;
	float3 N = mul(In.Normal, mat);
	N = normalize(N);
	Out.Normal = N;
	Out.Ambient.rgb = HemiLight(N);

	Out.Pos   = mul(In.Pos, Projection);
	Out.Color = 1.0f;
	Out.Tex   = In.Tex;

	//	���C�g�x�N�g���␳
	float3	vx;
	float3	vy = { 0.0f, -1, 0.01f };

	vx = cross( N, vy );
	vx = normalize( vx );
	
	vy = cross( vx, N );
	vy = normalize( vy );

	//	���C�g�x�N�g���␳
	Out.vLight.x = dot(vx, LightDir);
	Out.vLight.y = dot(vy, LightDir);
	Out.vLight.z = dot(N, LightDir);
	Out.vLight = normalize( Out.vLight );

	// �����x�N�g���␳
	float3 E = (P - ViewPos);   // �����x�N�g��
	Out.vE.x = -dot(vx, E);
	Out.vE.y = dot(vy, E);
	Out.vE.z = dot(N, E);
	Out.vE = normalize( Out.vE );

	Out.light = 0;//PointLight( P, N );

	//	�t�H�O�v�Z
	Out.Ambient.a = (FogFar-Out.Pos.z) / (FogFar-FogNear);
	Out.Ambient.a = saturate(Out.Ambient.a);

	return Out;
}

//------------------------------------------------------
//	���_�J���[�t�V�F�[�_
//------------------------------------------------------
VS_OUTPUT VS_FullFX2( VS_INPUTL In )
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	float4 P = mul(In.Pos, TransMatrix);

	float3x3	mat = TransMatrix;
	float3 N = mul(In.Normal, mat);
	N = normalize(N);
	Out.Normal = N;

	Out.Ambient.rgb = HemiLight(N);

	Out.Pos   = mul(In.Pos, Projection);
	Out.Color = In.Color;
	Out.Tex   = In.Tex;

	//	���C�g�x�N�g���␳
	float3	vx;
	float3	vy = { 0, 1, 0.01f };

	vx = cross( N, vy );
	vx = normalize( vx );

	vy = cross( N, vx );
	vy = normalize( vy );

	//	���C�g�x�N�g���␳
	Out.vLight.x = dot(vx, LightDir);
	Out.vLight.y = dot(vy, LightDir);
	Out.vLight.z = dot(N, LightDir);

	// �����x�N�g���␳
	float3 E = (P - ViewPos);   // �����x�N�g��
	Out.vE.x = dot(vx, E);
	Out.vE.y = dot(vy, E);
	Out.vE.z = dot(N, E);

	Out.light = 0;//PointLight( P, N );

	//	�t�H�O�v�Z
	Out.Ambient.a = (FogFar-Out.Pos.z) / (FogFar-FogNear);
	Out.Ambient.a = saturate(Out.Ambient.a);

	return Out;
}

//------------------------------------------------------
//		�s�N�Z���V�F�[�_�[	
//------------------------------------------------------
float4 PS_FullFX( VS_OUTPUT In) : COLOR
{   
	float4	OUT;
	float2 Tex = In.Tex;

	//	�p�����b�N�X�f�B�X�v���[�X�����g
	float h = tex2D( HeightSamp, Tex ).r-0.5f;// �����}�b�v�̃T���v�����O
	float3 E = normalize(In.vE);
	In.vLight = normalize(In.vLight);
	
	Tex -= 0.05f * h * E.xy;
	float3 N = tex2D( NormalSamp, Tex ).xyz * 2.0f - 1.0f;

	//	���C�g�v�Z
	float3 light = DirLight(In.vLight, N);
	//	�s�N�Z���F����
	OUT = In.Color * tex2D( DecaleSamp, Tex );
	OUT.rgb = (OUT.rgb*(light+In.Ambient.rgb));


	//	�X�y�L����
	float3  R = normalize( reflect( E, N ) );
	OUT.rgb += pow( max( 0, dot(R, -In.vLight) ), 10 ) * tex2D( SpecularSamp, Tex );
	//	�t�H�O�̗p
	OUT.rgb = (OUT.rgb * In.Ambient.a) + (FogColor * (1-In.Ambient.a));

	return OUT;
}

//------------------------------------------------------
//		�s�N�Z���V�F�[�_�[	
//------------------------------------------------------
float4 PS_FullFX2( VS_OUTPUT In) : COLOR
{   
	float4	OUT;
	float2 Tex = In.Tex;

	//	�p�����b�N�X�f�B�X�v���[�X�����g
	float h = tex2D( HeightSamp, Tex ).r-0.5f;// �����}�b�v�̃T���v�����O
	float3 E = normalize(In.vE);
	In.vLight = normalize(In.vLight);
	
	float3 N = tex2D( NormalSamp, Tex ).xyz * 2.0f - 1.0f;

	//	���C�g�v�Z
	float3 light = DirLight(In.vLight, N);
	//	�s�N�Z���F����
	OUT = In.Color * tex2D( DecaleSamp, Tex );
	OUT.rgb = (OUT.rgb*(light+In.Ambient.rgb));


	//	�X�y�L����
	float3  R = normalize( reflect( E, N ) );
	OUT.rgb += pow( max( 0, dot(R, -In.vLight) ), 10 ) * tex2D( SpecularSamp, Tex );
	//	�t�H�O�̗p
	OUT.rgb = (OUT.rgb * In.Ambient.a) + (FogColor * (1-In.Ambient.a));

	return OUT;
}

float4 PS_Outline(VS_OUTPUT In) : COLOR
{
	float4 OUT;
//�s�N�Z���F����
OUT = In.Color;
return OUT;

}

//-----------------------------------------------------
//�g�D�[��
//-----------------------------------------------------
float4 PS_Toon(VS_OUTPUT_TOON In) : COLOR
{
	float4 OUT;
//�s�N�Z���F����
OUT = tex2D(DecaleSamp, In.Tex);
//���x���������e�ɂ���
if (In.Level < ToonLevel) OUT.rgb *= ToonShadow;

return OUT;
}

//------------------------------------------------------
//		�����Ȃ�	
//------------------------------------------------------
technique copy_fx
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		ZWriteEnable     = true;

		VertexShader = compile vs_3_0 VS_FullFX();
		PixelShader  = compile ps_3_0 PS_FullFX();
    }
}

technique lcopy_fx
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		ZWriteEnable     = true;
		VertexShader = compile vs_3_0 VS_FullFX2();
		PixelShader  = compile ps_3_0 PS_FullFX();
    }
}

technique copy_fx2
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		ZWriteEnable     = true;

		VertexShader = compile vs_3_0 VS_FullFX();
		PixelShader  = compile ps_3_0 PS_FullFX2();
    }
}

technique lcopy_fx2
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		ZWriteEnable     = true;
		VertexShader = compile vs_3_0 VS_FullFX2();
		PixelShader  = compile ps_3_0 PS_FullFX2();
    }
}

//�g�D�[��
technique toon
{
	pass Outline
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = CW;
		ZEnable = true;

		VertexShader = compile vs_3_0 VS_Outline();
		PixelShader = compile ps_3_0 PS_Outline();
	}

	//�g�D�[��
	pass Toon
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		ZWriteEnable = true;

		VertexShader = compile vs_3_0 VS_FullFX();
		PixelShader = compile ps_3_0 PS_FullFX();
	}
}

technique copy2
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		ZWriteEnable = true;

		VertexShader = compile vs_3_0 VS_Basic3();
		PixelShader = compile ps_3_0 PS_Basic();
	}
}

technique copy3
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		ZWriteEnable = true;

		VertexShader = compile vs_3_0 VS_Basic();
		PixelShader = compile ps_3_0 PS_Basic4();
	}
}

technique copy4
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		ZWriteEnable = true;

		VertexShader = compile vs_3_0 VS_Basic();
		PixelShader = compile ps_3_0 PS_Basic5();
	}
}

technique copy5
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		ZWriteEnable = true;

		VertexShader = compile vs_3_0 VS_Basic();
		PixelShader = compile ps_3_0 PS_Basic6();
	}
}

technique copy6
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		ZWriteEnable = true;

		VertexShader = compile vs_3_0 VS_Basic();
		PixelShader = compile ps_3_0 PS_Basic7();
	}
}

technique copy7
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		ZWriteEnable = true;

		VertexShader = compile vs_3_0 VS_Basic();
		PixelShader = compile ps_3_0 PS_Basic8();
	}
}

technique copy8
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		ZWriteEnable = true;

		VertexShader = compile vs_3_0 VS_Basic();
		PixelShader = compile ps_3_0 PS_Basic9();
	}
}