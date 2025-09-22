#include "BSShaderNoLighting.hpp"

#ifdef GAME
NiColorA* const BSShaderNoLighting::PixelConstants::MatColor = reinterpret_cast<NiColorA*>(0x1202010);
NiPoint4* const BSShaderNoLighting::PixelConstants::MatAlpha = reinterpret_cast<NiPoint4*>(0x1202020);
BSShaderNoLightingProperty::FalloffData* const BSShaderNoLighting::VertexConstants::Falloff = (BSShaderNoLightingProperty::FalloffData*)0x1202030;
D3DMATRIX* const BSShaderNoLighting::VertexConstants::kTextureTransform = reinterpret_cast<D3DMATRIX*>(0x1201FE0);
#else
NiColorA* const BSShaderNoLighting::PixelConstants::MatColor = reinterpret_cast<NiColorA*>(0xF2A4C0);
NiPoint4* const BSShaderNoLighting::PixelConstants::MatAlpha = reinterpret_cast<NiPoint4*>(0xF2A500);
BSShaderNoLightingProperty::FalloffData* const BSShaderNoLighting::VertexConstants::Falloff = (BSShaderNoLightingProperty::FalloffData*)0xF2A510;
D3DMATRIX* const BSShaderNoLighting::VertexConstants::kTextureTransform = reinterpret_cast<D3DMATRIX*>(0xF2A4C0);
#endif

float* BSShaderNoLighting::VertexConstants::GetSubTex() {
#ifdef GAME
    return reinterpret_cast<float*>(0x1202048);
#else
    return reinterpret_cast<float*>(0xF2A528);
#endif
}
