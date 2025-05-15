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

};

#endif // SCREENCAPTURE_H
