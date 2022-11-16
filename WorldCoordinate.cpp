#include "WorldCoordinate.h"
#include "Window.h"
#include <DirectXMath.h>

// 静的変数の実体
const DirectX::XMMATRIX WorldCoordinate::matProjection_{ DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45.0f), (float)Window::width_ / Window::height_, nearZ_, farZ_) };
const DirectX::XMMATRIX WorldCoordinate::matView_{ DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&eye_), DirectX::XMLoadFloat3(&target_), DirectX::XMLoadFloat3(&up_)) };

WorldCoordinate::WorldCoordinate(void)
{
}

WorldCoordinate::WorldCoordinate(Camera* pCamera) :
    pCamera_(pCamera)
{
}

void WorldCoordinate::Update(void)
{
    // 各行列の宣言と初期化
    DirectX::XMMATRIX matScale{ DirectX::XMMatrixIdentity() }; // スケール行列
    DirectX::XMMATRIX matRot{ DirectX::XMMatrixIdentity() }; // 回転行列
    DirectX::XMMATRIX matTrans{ DirectX::XMMatrixIdentity() }; // 平行移動行列

    // スケール行列の計算
    matScale = DirectX::XMMatrixScaling(scale_.x, scale_.y, scale_.z);

    // 回転行列の計算
    matRot = DirectX::XMMatrixIdentity();
    matRot *= DirectX::XMMatrixRotationZ(rotation_.z);
    matRot *= DirectX::XMMatrixRotationX(rotation_.x);
    matRot *= DirectX::XMMatrixRotationY(rotation_.y);

    // 平行移動行列の計算
    matTrans = DirectX::XMMatrixTranslation(position_.x, position_.y, position_.z);

    // ワールド行列の合成
    matWorld_ = DirectX::XMMatrixIdentity();    // 変形をリセット
    matWorld_ *= matScale;                      // ワールド行列にスケーリングを反映
    matWorld_ *= matRot;                        // ワールド行列に回転を反映
    matWorld_ *= matTrans;                      // ワールド行列に平行移動を反映

    // 親オブジェクトがあれば
    if (parent_) {
        //親オブジェクトのワールド行列をかける
        matWorld_ *= parent_->matWorld_;
    }

    // 定数バッファへデータ転送
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

    cbData_.GetConstBuffMap()->mat_ = matWorld_ * matView * matProjection;
}