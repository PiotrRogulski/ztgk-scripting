#pragma once

#include <camera.h>
#include <diDevice.h>
#include <dxApplication.h>
#include <keyboard.h>
#include <mouse.h>

namespace mini {
    class Gk2ExampleBase: public DxApplication {
    public:
        Gk2ExampleBase(HINSTANCE hInstance, UINT width, UINT height, const std::wstring& title);

    protected:
        bool HandleCameraInput(double dt);

        DiDevice m_inputDevice;
        Mouse m_mouse;
        Keyboard m_keyboard;
        OrbitCamera m_camera;

    private:
        static constexpr float ROTATION_SPEED = 0.01f;
        static constexpr float ZOOM_SPEED = 0.02f;
    };
}
