#pragma once
#include "Obj3d.h"
#include <memory>
#include "Camera.h"
#include "NormalBlock.h"

class ExtendBlock
{
public:
	ExtendBlock(Camera* pCamera);
	~ExtendBlock();

	//開始位置をセットする
	void Init(DirectX::XMFLOAT3 StartScale);

	void Update(void);

	void Draw(void);

	//伸びる
	void Extend();

	//電気が流れているかどうかのflagを変更する
	void SetIsflow(bool flag) { isFlow = flag; };

	//風の方向を変更する
	void SetExtendVec(DirectX::XMFLOAT3 vec) { extendVec = vec; };

	//風の強さを変更する
	void SetExtendSpeed(float speed) { extendSpeed = speed; };

	//風の届く長さを変更する
	void SetExtendLength(float length) { extendLength = length; };

	//接続する
	void SetConnectionBlock(NormalBlock* setBlock) { connectionBlock = setBlock; }

	//外す
	void RemoveConnectionBlock();

private:

public:

private:

	//本体
	std::unique_ptr<Obj3d> object_{};

	//接続先
	NormalBlock* connectionBlock = nullptr;

	//電気が流れているか
	bool isFlow = false;

	//伸びる向き
	DirectX::XMFLOAT3 extendVec = { 0,1,0 };

	//開始位置(線形補間用)
	DirectX::XMFLOAT3 startScale = {1,1,1};

	//戻る用の位置(線形補間用)
	DirectX::XMFLOAT3 nowScale;

	//戻るときのスタート位置を取得するためのフラグ
	bool nowScaleCount = false;

	//伸びる速さ
	float extendSpeed = 0.1f;

	//伸びる長さ
	float extendLength = 5;

	float timeCount = 0;

	float maxTime = 20.0f;


};

const DirectX::XMFLOAT3 lerp(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const float t);

