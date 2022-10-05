#include "Basic.hlsli"

//Texture2D<float4> tex : register(t0);
Texture2D<float4> tex : register(t0);

SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor = float4(tex.Sample(smp,input.uv));

	float3 light = normalize(float3(1,-1,1));						// 右下奥 向きのライト
	float diffuse = saturate(dot(-light, input.normal));			// diffuseを[0,1]の範囲にClampする
	float brightness = diffuse + 0.3f;								// アンビエント項を0.3として計算
	return float4(texcolor.rgb * brightness, texcolor.a) * color;	// 輝度をRGBに代入して出力
}