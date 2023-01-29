#include <Windows.h>
#include "GameSystem.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    BasicFrame* game = new GameSystem();

    game->Run();

    delete game;

    Microsoft::WRL::ComPtr<ID3D12Device> device{InitDirectX::GetInstance()->GetDevice()};

    InitDirectX::DeleteInstance();

    Microsoft::WRL::ComPtr<ID3D12DebugDevice> debugInterface;

    if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&debugInterface))))
    {
        debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
    }

    return 0;
}