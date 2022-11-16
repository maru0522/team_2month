cbuffer CBData : register(b0)
{
	matrix mat_;	// 3D変換行列
    //float4 color_; // 色(RGBA)
};

cbuffer cbuff1 : register(b1)
{
    float3 m_ambient : packoffset(c0);
    float3 m_diffuse : packoffset(c1);
    float3 m_specular : packoffset(c2);
    float m_alpha : packoffset(c2.w);
}

// 頂点シェーダの出力構造体
// (頂点シェーダからピクセルシェーダへのやり取りに使用する)
struct VSOutput
{
	// システム用頂点座標
	float4 svpos_ : SV_POSITION;
	// 法線ベクトル
	float3 normal_ : NORMAL;
	// uv値
	float2 uv_ : TEXCOORD;
};