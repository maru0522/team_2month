#include "Camera.h"

Camera::Camera(void)
{
}

void Camera::Update(void)
{
    matView_ = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&eye_), DirectX::XMLoadFloat3(&target_), DirectX::XMLoadFloat3(&up_));
    DrawImgui();
}

void Camera::DrawImgui(void)
{
#ifdef _DEBUG


    ImGui::Begin("camera info");
    //ImGui::SliderFloat3("slider float3", vec4f, 0.0f, 1.0f);
    float sliderLocal_eye[3]{ eye_.x,eye_.y,eye_.z };
    float sliderLocal_target[3]{ target_.x,target_.y,target_.z };

    ImGui::SliderFloat3("CameraEye",sliderLocal_eye,-100.f,100.f);
    ImGui::SliderFloat3("CameraTarget",sliderLocal_target,-100.f,100.f);
    ImGui::End();

    eye_ = { sliderLocal_eye[0],sliderLocal_eye[1],sliderLocal_eye[2] };
    target_ = { sliderLocal_target[0],sliderLocal_target[1],sliderLocal_target[2] };
#endif // _DEBUG
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