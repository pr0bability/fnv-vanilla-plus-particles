#include "NiD3DTextureStage.hpp"

NiDX9RenderState* NiD3DTextureStage::GetRenderState() {
	return *reinterpret_cast<NiDX9RenderState**>(0x126F6C8);
}

NiDX9Renderer* NiD3DTextureStage::GetRenderer() {
	return *reinterpret_cast<NiDX9Renderer**>(0x126F6C4);
}

LPDIRECT3DDEVICE9 NiD3DTextureStage::GetD3DDevice() {
	return *reinterpret_cast<LPDIRECT3DDEVICE9*>(0x126F6C0);
}

// GAME - 0xE7EDF0
// GECK - 0xC15640
void NiD3DTextureStage::CreateNewStage(NiD3DTextureStagePtr& aspStage) {
	CdeclCall(0xE7EDF0, &aspStage);
}

// GAME - 0xE7EA00
// GECK - 0xC152B0
void NiD3DTextureStage::ApplyTexture() {
	ThisCall(0xE7EA00, this);
}

// GAME - 0xE7EB00
// GECK - 0xC153B0
bool NiD3DTextureStage::ConfigureStage(bool abAllowRepeats) {
#ifdef GAME
	return ThisCall(0xE7EB00, this, abAllowRepeats);
#else
	return ThisCall(0xC153B0, this, abAllowRepeats);
#endif
}

// GAME - 0xE7DE90
// GECK - 0xC14740
void NiD3DTextureStage::SetClampMode(NiTexturingProperty::ClampMode eClamp) {
	ThisCall(0xE7DE90, this, eClamp);
}

// GAME - 0xE7DEF0
// GECK - 0xC147A0
void NiD3DTextureStage::SetFilterMode(NiTexturingProperty::FilterMode eFilter) {
	ThisCall(0xE7DEF0, this, eFilter);
}

// GAME - 0xBE0CF0
// GECK - 0x975620
void NiD3DTextureStage::SetStageProperties(uint32_t auiTextureIndex, NiTexturingProperty::ClampMode aeClampMode, uint32_t aeFilterValue, bool abUseAnisotropicFilter) {
	CdeclCall(0xBE0CF0, this, auiTextureIndex, aeClampMode, aeFilterValue, abUseAnisotropicFilter);
}

// GAME - 0xE7ECD0
// GAME - 0xC15520
void NiD3DTextureStage::ReturnStageToPool() {
	ThisCall(0xE7ECD0, this);
}

void NiD3DTextureStage::IncRefCount() {
	m_uiRefCount++;
}

void NiD3DTextureStage::DecRefCount() {
	m_uiRefCount--;
	if (m_uiRefCount == 0)
		ReturnStageToPool();
}

