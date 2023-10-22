#pragma once

#include <constantBuffer.h>
#include <DirectXMath.h>
#include <effect.h>

using namespace mini;

namespace duck_app {
    class PhongEffect: public StaticEffect<BasicEffect, VSConstantBuffers, PSConstantBuffers> {
    public:
        //enumerated types defined for convenience
        enum VSConstantBufferSlots {
            WorldMtxSlot,
            ViewMtxSlot,
            ProjMtxSlot
        };

        enum PSConstantBufferSlot {
            LightPositionSlot,
            SurfaceColorSlot
        };

        PhongEffect() = default;

        PhongEffect(dx_ptr<ID3D11VertexShader>&& vs, dx_ptr<ID3D11PixelShader>&& ps,
                    const ConstantBuffer<DirectX::XMFLOAT4X4>& cbWorld,
                    const ConstantBuffer<DirectX::XMFLOAT4X4, 2>& cbView,
                    const ConstantBuffer<DirectX::XMFLOAT4X4>& cbProj,
                    const ConstantBuffer<DirectX::XMFLOAT4>& cbLightPos,
                    const ConstantBuffer<DirectX::XMFLOAT4>& cbSurfaceColor) : StaticEffect(
            BasicEffect{move(vs), move(ps)}, VSConstantBuffers{cbWorld, cbView, cbProj},
            PSConstantBuffers{cbLightPos, cbSurfaceColor}) { }

        //setter methods defined for convenience
        static void SetWorldMatrixBuffer(const ConstantBuffer<DirectX::XMFLOAT4X4>& buffer) {
            SetVSConstantBuffer(WorldMtxSlot, buffer);
        }

        static void SetViewMatrixBuffer(const ConstantBuffer<DirectX::XMFLOAT4X4, 2>& buffer) {
            SetVSConstantBuffer(ViewMtxSlot, buffer);
        }

        static void SetProjMatrixBuffer(const ConstantBuffer<DirectX::XMFLOAT4X4>& buffer) {
            SetVSConstantBuffer(ProjMtxSlot, buffer);
        }

        static void SetLightPosBuffer(const ConstantBuffer<DirectX::XMFLOAT4>& buffer) {
            SetPSConstantBuffer(LightPositionSlot, buffer);
        }

        static void SetSurfaceColorBuffer(const ConstantBuffer<DirectX::XMFLOAT4, 2>& buffer) {
            SetPSConstantBuffer(SurfaceColorSlot, buffer);
        }
    };
}
