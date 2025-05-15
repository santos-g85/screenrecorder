#include "screencapture.h"
#include <iostream>
#include <d3d11.h>
#include <dxgi1_2.h>
#include <wrl/client.h>
#include <fstream>
#include <thread>
#include <atomic>
#include <vector>
#include <qdebug.h>
#include <wincodec.h> // WIC - Windows Imaging Component for image encoding

#pragma comment(lib, "windowscodecs.lib") // Link WIC library

// Constructor: Initializes the screen capture system
ScreenCapture::ScreenCapture() : capturing(false) {
    if (!initialize()) {
        qDebug() << "Failed to initialize the screen capture interface!";
    }
}

// Destructor: Cleanup handled separately
ScreenCapture::~ScreenCapture() {}

// Initialize screen capture system
bool ScreenCapture::initialize() {
    HRESULT hr;

    // Step 1: Create a DXGI factory for enumerating graphics adapters
    Microsoft::WRL::ComPtr<IDXGIFactory1> dxgiFactory;
    hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(dxgiFactory.GetAddressOf()));
    if (FAILED(hr)) {
        qDebug() << "Failed to create DXGI factory";
        return false;
    }

    // Step 2: Get the first graphics adapter (GPU)
    Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
    hr = dxgiFactory->EnumAdapters(0, adapter.GetAddressOf());
    if (FAILED(hr)) {
        qDebug() << "Failed to get GPU adapter";
        return false;
    }

    // Step 3: Create Direct3D 11 device and device context
    D3D_FEATURE_LEVEL featureLevel;
    hr = D3D11CreateDevice(
        adapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, nullptr, 0, nullptr, 0,
        D3D11_SDK_VERSION, d3dDevice.GetAddressOf(), &featureLevel, d3dContext.GetAddressOf()
        );
    if (FAILED(hr)) {
        qDebug() << "Failed to create Direct3D device";
        return false;
    }

    // Step 4: Get output (i.e., the monitor/display)
    Microsoft::WRL::ComPtr<IDXGIOutput> dxgiOutput;
    hr = adapter->EnumOutputs(0, dxgiOutput.GetAddressOf());
    if (FAILED(hr)) {
        qDebug() << "Failed to get monitor output";
        return false;
    }

    // Step 5: Cast to IDXGIOutput1 to support duplication
    hr = dxgiOutput.As(&dxgiOutput1);
    if (FAILED(hr)) {
        qDebug() << "Failed to convert to DXGIOutput1 interface";
        return false;
    }

    // Step 6: Create duplication interface for screen capturing
    hr = dxgiOutput1->DuplicateOutput(d3dDevice.Get(), duplication.GetAddressOf());
    if (FAILED(hr)) {
        qDebug() << "Failed to create desktop duplication";
        return false;
    }

    return true;
}

// Releases COM resources and cleans up Direct3D objects
void ScreenCapture::cleanup() {
    duplication.Reset();
    dxgiOutput1.Reset();
    d3dContext.Reset();
    d3dDevice.Reset();
}

// Start capturing screen in a separate thread
void ScreenCapture::start() {
    if (capturing) return;
    capturing = true;
    captureThread = std::thread(&ScreenCapture::captureLoop, this);
}

// Stop the capture thread and wait for it to finish
void ScreenCapture::stop() {
    capturing = false;
    if (captureThread.joinable()) {
        captureThread.join();
    }
}

// Main loop that continuously captures the screen frames
void ScreenCapture::captureLoop() {
    DXGI_OUTDUPL_FRAME_INFO frameInfo = {};
    Microsoft::WRL::ComPtr<IDXGIResource> desktopResource;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> acquiredTexture;

    while (capturing) {
        // Try to acquire the next screen frame (with a timeout)
        HRESULT hr = duplication->AcquireNextFrame(500, &frameInfo, desktopResource.GetAddressOf());
        if (SUCCEEDED(hr)) {
            // Convert the resource to a 2D texture
            hr = desktopResource.As(&acquiredTexture);
            if (SUCCEEDED(hr)) {
                // Save the captured frame to a PNG file
                saveFrameAsPNG(acquiredTexture);
            }
            duplication->ReleaseFrame(); // Release the frame
        } else if (hr != DXGI_ERROR_WAIT_TIMEOUT) {
            qDebug() << "Failed to acquire frame: " << std::hex << hr;
            break;
        }

        // Control capture frame rate (approx 30 FPS)
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
}

// Save a captured frame to disk as a PNG image
void ScreenCapture::saveFrameAsPNG(Microsoft::WRL::ComPtr<ID3D11Texture2D> acquiredTexture) {
    D3D11_TEXTURE2D_DESC desc;
    acquiredTexture->GetDesc(&desc);

    // Create a CPU-readable staging texture
    desc.Usage = D3D11_USAGE_STAGING;
    desc.BindFlags = 0;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    desc.MiscFlags = 0;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> stagingTexture;
    HRESULT hr = d3dDevice->CreateTexture2D(&desc, nullptr, &stagingTexture);
    if (FAILED(hr)) {
        qDebug() << "Failed to create staging texture";
        return;
    }

    // Copy the acquired texture to staging texture
    d3dContext->CopyResource(stagingTexture.Get(), acquiredTexture.Get());

    // Map the staging texture for CPU access
    D3D11_MAPPED_SUBRESOURCE mapped;
    hr = d3dContext->Map(stagingTexture.Get(), 0, D3D11_MAP_READ, 0, &mapped);
    if (FAILED(hr)) {
        qDebug() << "Failed to map texture";
        return;
    }

    // Initialize WIC factory
    Microsoft::WRL::ComPtr<IWICImagingFactory> factory;
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    hr = CoCreateInstance(
        CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&factory)
        );
    if (FAILED(hr)) {
        qDebug() << "Failed to create WIC factory";
        d3dContext->Unmap(stagingTexture.Get(), 0);
        return;
    }

    // Create a file stream for output PNG
    QString filename = QString("screenshot_%1.png").arg(frameCounter++, 4, 10, QChar('0'));
    Microsoft::WRL::ComPtr<IWICStream> stream;
    hr = factory->CreateStream(&stream);
    if (FAILED(hr)) {
        qDebug() << "Failed to create WIC stream";
        d3dContext->Unmap(stagingTexture.Get(), 0);
        return;
    }

    std::wstring wideFilename = filename.toStdWString();
    hr = stream->InitializeFromFilename(wideFilename.c_str(), GENERIC_WRITE);
    if (FAILED(hr)) {
        qDebug() << "Failed to initialize WIC stream with file";
        d3dContext->Unmap(stagingTexture.Get(), 0);
        return;
    }

    // Create PNG encoder
    Microsoft::WRL::ComPtr<IWICBitmapEncoder> encoder;
    hr = factory->CreateEncoder(GUID_ContainerFormatPng, nullptr, &encoder);
    if (FAILED(hr)) {
        qDebug() << "Failed to create PNG encoder";
        d3dContext->Unmap(stagingTexture.Get(), 0);
        return;
    }

    hr = encoder->Initialize(stream.Get(), WICBitmapEncoderNoCache);
    if (FAILED(hr)) {
        qDebug() << "Failed to initialize encoder";
        d3dContext->Unmap(stagingTexture.Get(), 0);
        return;
    }

    // Create a frame for the image
    Microsoft::WRL::ComPtr<IWICBitmapFrameEncode> frame;
    Microsoft::WRL::ComPtr<IPropertyBag2> props;
    hr = encoder->CreateNewFrame(&frame, &props);
    if (FAILED(hr)) {
        qDebug() << "Failed to create frame";
        d3dContext->Unmap(stagingTexture.Get(), 0);
        return;
    }

    hr = frame->Initialize(nullptr);
    hr = frame->SetSize(desc.Width, desc.Height);

    // Set pixel format (BGRA 32-bit)
    WICPixelFormatGUID format = GUID_WICPixelFormat32bppBGRA;
    hr = frame->SetPixelFormat(&format);

    // Write pixels from the mapped texture to the PNG file
    hr = frame->WritePixels(
        desc.Height,
        mapped.RowPitch,
        mapped.RowPitch * desc.Height,
        static_cast<BYTE*>(mapped.pData)
        );

    // Finalize and commit to disk
    hr = frame->Commit();
    hr = encoder->Commit();

    // Unmap the texture
    d3dContext->Unmap(stagingTexture.Get(), 0);

    qDebug() << "Saved: " << filename;
}
