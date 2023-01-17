#pragma once
#include "Obj3d.h"
#include <memory>
#include "Camera.h"

class NormalBlock
{
public:
	NormalBlock(Camera* pCamera);
	~NormalBlock();

	//更新
	void Update(void);

	//描画
	void Draw(void);

	//電気が流れるかどうかのフラグを変更
	void SetIsConductor(bool flag) { isConductor = flag; };

	//電気が流れているかどうかのフラグを変更
	void SetIsFlow(bool flag);

	//電気が流れるかどうかのフラグを取得
	bool GetIsConductor(void) { return isConductor; }

	//電気が流れているかどうかのフラグを取得
	bool GetIsFlow(void) { return isFlow; }

private:

public:

private:

	//本体
	std::unique_ptr<Obj3d> object_{};

	//電気が流れるかどうか
	bool isConductor = false;

	//電気が流れているか
	bool isFlow = false;

};

