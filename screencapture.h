#ifndef SCREENCAPTURE_H
#define SCREENCAPTURE_H

#include <Windows.h>
#include <d3d11.h>
#include <dxgi1_2.h>
#include <wrl/client.h>
#include <thread>
#include <atomic>


class ScreenCapture{

public:
    ScreenCapture();
    ~ScreenCapture();

    void start();
    void stop();

private:
    int frameCounter = 0;

    bool initialize();
    void cleanup();
    void captureLoop();
    void saveFrameAsPNG(Microsoft::WRL::ComPtr<ID3D11Texture2D> acquiredTexture);

    std::atomic<bool> capturing;
    std::thread captureThread;

    Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext;
    Microsoft::WRL::ComPtr<IDXGIOutput1> dxgiOutput1;
    Microsoft::WRL::ComPtr<IDXGIOutputDuplication> duplication;

};

#endif // SCREENCAPTURE_H
