#pragma once
#include "Obj3d.h"

class Particle
{
public:
	/// <summary>
	/// ����
	/// </summary>
	/// <param name="camera">�J����</param>
	/// <param name="StartPos">�����ʒu��u��</param>
	/// <param name="StartScale">�����T�C�Y��ݒ�</param>
	Particle(Camera* camera, DirectX::XMFLOAT3 StartPos, DirectX::XMFLOAT3 StartScale);
	~Particle();

	void Update();

	void Draw();

	/// <summary>
	/// �������Ԃ�����
	/// </summary>
	void SetMaxAliveTime(float setTime) { maxAliveTime = setTime; };

	/// <summary>
	/// �����ɓ����x�N�g���͐��K���������̂�����
	/// </summary>
	/// <param name="Vec">�ړ�����</param>
	void SetMoveVec(DirectX::XMFLOAT3 Vec) { moveVec = Vec; };

	/// <summary>
	/// �ړ����x������
	/// </summary>
	void SetMoveSpeed(float setSpeed) { moveSpeed = setSpeed; };


	//�d�͂̉e����^���邩
	void SetIsGravity(bool flag) { isGravity = flag; };

	//�������Ă��邩
	bool GetIsAlive() { return isAlive; };

private:

	void Init(Camera* camera);

public:

	//�{��
	std::unique_ptr<Obj3d> obj;


private:

	//��������
	float maxAliveTime = 100;
	float aliveTime = 0;

	//�����t���O
	bool isAlive = true;

	//�d�͂�^���邩
	bool isGravity = false;

	//�d��
	float gravity = 1.0f;

	//�ړ�����
	DirectX::XMFLOAT3 moveVec = { 0,1,0 };

	//�ړ��X�s�[�h
	float moveSpeed = 0.1f;

	//�p�[�e�B�N���̏����T�C�Y
	DirectX::XMFLOAT3 startScale = { 1,1,1 };

	//lerp�p�ɗp�ӂ���
	DirectX::XMVECTOR start;


};

