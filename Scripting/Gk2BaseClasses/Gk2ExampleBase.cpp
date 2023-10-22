#include "gk2ExampleBase.h"

using namespace DirectX;

namespace mini {
    Gk2ExampleBase::Gk2ExampleBase(HINSTANCE hInstance, const UINT width, const UINT height,
                                   const std::wstring& title) : DxApplication(hInstance, width, height, title),
                                                                m_inputDevice(hInstance),
                                                                m_mouse(m_inputDevice.CreateMouseDevice(
                                                                    m_window.getHandle())),
                                                                m_keyboard(m_inputDevice.CreateKeyboardDevice(
                                                                    m_window.getHandle())),
                                                                m_camera(XMFLOAT3(0.0F, 0.0F, 0.0F), 0.01F, 25.0F,
                                                                         5) { }

    bool Gk2ExampleBase::HandleCameraInput(const double dt) {
        MouseState mstate;
        if (m_mouse.GetState(mstate)) {
            const auto [x, y] = mstate.getMousePositionChange();
            if (mstate.isButtonDown(0)) {
                auto tmp = y * ROTATION_SPEED;
                auto angle = m_camera.getYAngle();
                if (m_camera.getXAngle() + tmp < -XM_PIDIV2)
                    tmp = -XM_PIDIV2 - m_camera.getXAngle();
                else if (m_camera.getXAngle() + tmp > XM_PIDIV2)
                    tmp = XM_PIDIV2 - m_camera.getXAngle();
                m_camera.Rotate(tmp, x * ROTATION_SPEED);
            } else if (mstate.isButtonDown(1))
                m_camera.Zoom(y * ZOOM_SPEED);
        }
        KeyboardState kstate;
        if (!m_keyboard.GetState(kstate))
            return false;

        const auto distance = 3.0F * static_cast<float>(dt);
        if (kstate.isKeyDown(30/*A*/))
            m_camera.MoveTarget(XMFLOAT3{-distance, 0, 0});
        if (kstate.isKeyDown(32/*D*/))
            m_camera.MoveTarget(XMFLOAT3{distance, 0, 0});
        if (kstate.isKeyDown(17/*W*/))
            m_camera.MoveTarget(XMFLOAT3{0, distance, 0});
        if (kstate.isKeyDown(31/*S*/))
            m_camera.MoveTarget(XMFLOAT3{0, -distance, 0});
        if (kstate.isKeyDown(16/*Q*/))
            m_camera.MoveTarget(XMFLOAT3{0, 0, distance});
        if (kstate.isKeyDown(18/*E*/))
            m_camera.MoveTarget(XMFLOAT3{0, 0, -distance});

        return true;
    }
}
