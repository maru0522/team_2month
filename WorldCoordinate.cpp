#include "WorldCoordinate.h"
#include "Window.h"
#include <DirectXMath.h>

// �ÓI�ϐ��̎���
const DirectX::XMMATRIX WorldCoordinate::matProjection_{ DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45.0f), (float)Window::width_ / Window::height_, nearZ_, farZ_) };
const DirectX::XMMATRIX WorldCoordinate::matView_{ DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&eye_), DirectX::XMLoadFloat3(&target_), DirectX::XMLoadFloat3(&up_)) };

WorldCoordinate::WorldCoordinate(void)
{
}

WorldCoordinate::WorldCoordinate(Camera* pCamera) :
    pCamera_(pCamera)
{
    cbData_->GetBuffer()->SetName(L"worldcoordinateConstBuffer");
}

void WorldCoordinate::Update(void)
{
    // �e�s��̐錾�Ə�����
    DirectX::XMMATRIX matScale{ DirectX::XMMatrixIdentity() }; // �X�P�[���s��
    DirectX::XMMATRIX matRot{ DirectX::XMMatrixIdentity() }; // ��]�s��
    DirectX::XMMATRIX matTrans{ DirectX::XMMatrixIdentity() }; // ���s�ړ��s��

    // �X�P�[���s��̌v�Z
    matScale = DirectX::XMMatrixScaling(scale_.x, scale_.y, scale_.z);

    // ��]�s��̌v�Z
    matRot = DirectX::XMMatrixIdentity();
    matRot *= DirectX::XMMatrixRotationZ(rotation_.z);
    matRot *= DirectX::XMMatrixRotationX(rotation_.x);
    matRot *= DirectX::XMMatrixRotationY(rotation_.y);

    // ���s�ړ��s��̌v�Z
    matTrans = DirectX::XMMatrixTranslation(position_.x, position_.y, position_.z);

    // ���[���h�s��̍���
    matWorld_ = DirectX::XMMatrixIdentity();    // �ό`�����Z�b�g
    matWorld_ *= matScale;                      // ���[���h�s��ɃX�P�[�����O�𔽉f
    matWorld_ *= matRot;                        // ���[���h�s��ɉ�]�𔽉f
    matWorld_ *= matTrans;                      // ���[���h�s��ɕ��s�ړ��𔽉f

    // �e�I�u�W�F�N�g�������
    if (parent_) {
        //�e�I�u�W�F�N�g�̃��[���h�s���������
        matWorld_ *= parent_->matWorld_;
    }

    // �萔�o�b�t�@�փf�[�^�]��
    TransferMatrix();
}

void WorldCoordinate::TransferMatrix(void)
{
    DirectX::XMMATRIX matView{ matView_ };
    DirectX::XMMATRIX matProjection{ matProjection_ };

    if (pCamera_) {
        matView = pCamera_->GetView();
        matProjection = pCamera_->GetProjection();
    }

    cbData_->GetConstBuffMap()->mat_ = matWorld_ * matView * matProjection;
}