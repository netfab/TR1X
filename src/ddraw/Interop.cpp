#include "ddraw/Interop.hpp"

#include "ddraw/DirectDrawSurface.hpp"
#include "ddraw/Renderer.hpp"
#include "glrage_util/ErrorUtils.hpp"

#include <cassert>
#include <string>

namespace glrage {
namespace ddraw {

extern "C" {

static Renderer *m_Renderer = nullptr;

HRESULT MyDirectDrawCreate()
{
    try {
        m_Renderer = new Renderer();
    } catch (const std::exception &ex) {
        ErrorUtils::warning(ex);
        return DDERR_GENERIC;
    }

    return DD_OK;
}

void MyIDirectDraw_Release()
{
    if (m_Renderer) {
        delete m_Renderer;
        m_Renderer = nullptr;
    }
}

void MyIDirectDraw2_CreateSurface(
    LPDDSURFACEDESC lpDDSurfaceDesc, LPDIRECTDRAWSURFACE *lplpDDSurface)
{
    *lplpDDSurface = new DirectDrawSurface(*m_Renderer, lpDDSurfaceDesc);
}

void MyIDirectDrawSurface_GetAttachedSurface(
    LPDIRECTDRAWSURFACE p, LPDIRECTDRAWSURFACE *lplpDDAttachedSurface)
{
    assert(p);
    reinterpret_cast<DirectDrawSurface *>(p)->GetAttachedSurface(
        lplpDDAttachedSurface);
}

HRESULT MyIDirectDrawSurface2_Lock(
    LPDIRECTDRAWSURFACE p, LPDDSURFACEDESC lpDDSurfaceDesc)
{
    assert(p);
    return reinterpret_cast<DirectDrawSurface *>(p)->Lock(lpDDSurfaceDesc);
}

HRESULT MyIDirectDrawSurface2_Unlock(LPDIRECTDRAWSURFACE p, LPVOID lp)
{
    assert(p);
    return reinterpret_cast<DirectDrawSurface *>(p)->Unlock(lp);
}

HRESULT MyIDirectDrawSurface_Blt(
    LPDIRECTDRAWSURFACE p, LPRECT lpDestRect,
    LPDIRECTDRAWSURFACE lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwFlags)
{
    assert(p);
    return reinterpret_cast<DirectDrawSurface *>(p)->Blt(
        lpDestRect, lpDDSrcSurface, lpSrcRect, dwFlags);
}

void MyIDirectDrawSurface_Release(LPDIRECTDRAWSURFACE p)
{
    assert(p);
    delete reinterpret_cast<DirectDrawSurface *>(p);
}

HRESULT MyIDirectDrawSurface_Flip(LPDIRECTDRAWSURFACE p)
{
    assert(p);
    return reinterpret_cast<DirectDrawSurface *>(p)->Flip();
}
}

}
}
