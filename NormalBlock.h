#pragma once
#include "Obj3d.h"
#include <memory>
#include "Camera.h"

class NormalBlock
{
public:
	NormalBlock(Camera* pCamera);
	~NormalBlock();

	//�X�V
	void Update(void);

	//�`��
	void Draw(void);

	//�d�C������邩�ǂ����̃t���O��ύX
	void SetIsConductor(bool flag) { isConductor = flag; };

	//�d�C������Ă��邩�ǂ����̃t���O��ύX
	void SetIsFlow(bool flag);

	//�d�C������邩�ǂ����̃t���O���擾
	bool GetIsConductor(void) { return isConductor; }

	//�d�C������Ă��邩�ǂ����̃t���O���擾
	bool GetIsFlow(void) { return isFlow; }

private:

public:

private:

	//�{��
	std::unique_ptr<Obj3d> object_{};

	//�d�C������邩�ǂ���
	bool isConductor = false;

	//�d�C������Ă��邩
	bool isFlow = false;

};

