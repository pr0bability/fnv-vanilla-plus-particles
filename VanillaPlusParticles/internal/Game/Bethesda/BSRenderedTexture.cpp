#include "BSRenderedTexture.hpp"

bool BSRenderedTexture::SaveTexture(const char* path, D3DXIMAGE_FILEFORMAT aeFormat) const {
	return static_cast<NiDX9TextureData*>(spRenderedTextures[0]->m_pkRendererData)->SaveTexture(path, aeFormat);
}

// GAME - 0xB6B610
// GECK - 0x9029E0
BSRenderedTexture* BSRenderedTexture::Create(const NiFixedString& arName, const uint32_t uiWidth, const uint32_t uiHeight, NiTexture::FormatPrefs* kPrefs, Ni2DBuffer::MultiSamplePreference eMSAAPref, bool bUseDepthStencil, NiDepthStencilBuffer* pkDSBuffer, uint32_t a7, uint32_t uiBackgroundColor) {
	return CdeclCall<BSRenderedTexture*>(0xB6B610, &arName, uiWidth, uiHeight, kPrefs, eMSAAPref, bUseDepthStencil, pkDSBuffer, a7, uiBackgroundColor);
}

// GAME - 0xB6B260
// GECK - 0x902710
NiRenderTargetGroup* BSRenderedTexture::GetRenderTargetGroup() const {
	return ThisCall<NiRenderTargetGroup*>(0xB6B260, this);
}

// GAME - 0x4BC320
// GECK - 0x48CBC0
NiRenderedTexture* BSRenderedTexture::GetTexture(const uint32_t uiIndex) const {
	if (this)
		return spRenderedTextures[uiIndex].m_pObject;
	else
		return nullptr;
}

// GAME - 0xB6B8D0
// GECK - 0x902CA0
void BSRenderedTexture::StartOffscreen(uint32_t auiClearMode, NiRenderTargetGroup* apGroup) {
	CdeclCall(0xB6B8D0, auiClearMode, apGroup);
}

// GAME - 0xB6B790
// GECK - 0x902B60
void BSRenderedTexture::StopOffscreen() {
	CdeclCall(0xB6B790);
}
