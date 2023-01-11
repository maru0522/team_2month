#include "Camera.h"

Camera::Camera(void)
{
}

void Camera::Update(void)
{
    matView_ = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&eye_), DirectX::XMLoadFloat3(&target_), DirectX::XMLoadFloat3(&up_));
}

void Camera::SetProjection(Camera::Projection projectionType)
{
    if (projectionType == Camera::Projection::ORTHOGRAPHIC) {
        // •½s“Š‰e
        nearZ_ = 0.0f, farZ_ = 1.0f;
        matProjection_ = DirectX::XMMatrixOrthographicOffCenterLH(0.0f, Window::width_, Window::height_, 0.0f, nearZ_, farZ_);
        projectionState_ = Camera::Projection::ORTHOGRAPHIC; // ORTHOGRAPHIC
    }
    else {
        // “§Ž‹“Š‰e
        nearZ_ = 0.1f, farZ_ = 1000.0f;
        matProjection_ = DirectX::XMMatrixPerspectiveFovLH(Util::Convert::ToRadian(45.0f), (float)Window::width_ / Window::height_, nearZ_, farZ_);
        projectionState_ = Camera::Projection::PERSPECTIVE; // PERSPECTIVE
    }
}

void Camera::SwitchProjection(void)
{
    if (projectionState_ == Camera::Projection::ORTHOGRAPHIC) {
        // •½s“Š‰e
        nearZ_ = 0.1f, farZ_ = 1000.0f;
        matProjection_ = DirectX::XMMatrixPerspectiveFovLH(Util::Convert::ToRadian(45.0f), (float)Window::width_ / Window::height_, nearZ_, farZ_);
        projectionState_ = Camera::Projection::PERSPECTIVE; // PERSPECTIVE
    }
    else {
        // “§Ž‹“Š‰e
        nearZ_ = 0.0f, farZ_ = 1000.0f;
        matProjection_ = DirectX::XMMatrixOrthographicOffCenterLH(0.0f, Window::width_, Window::height_, 0.0f, nearZ_, farZ_);
        projectionState_ = Camera::Projection::ORTHOGRAPHIC; // ORTHOGRAPHIC
    }
}