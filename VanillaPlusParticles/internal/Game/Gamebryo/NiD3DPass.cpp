#include "NiD3DPass.hpp"

NiDX9RenderState* NiD3DPass::GetD3DRenderState() {
#ifdef GAME
	return *reinterpret_cast<NiDX9RenderState**>(0x126F728);
#else
	return *reinterpret_cast<NiDX9RenderState**>(0xF983C0);
#endif
}

// GAME - 0xE802B0
// GECK - 0xC16A90
uint32_t NiD3DPass::SetStage(uint32_t uiStageNum, NiD3DTextureStage* pkStage) {
#ifdef GAME
	return ThisCall(0xE802B0, this, uiStageNum, pkStage);
#else
	return ThisCall(0xC16A90, this, uiStageNum, pkStage);
#endif
}

// GAME - 0xB80600
void NiD3DPass::SetPixelShader(NiD3DPixelShader* pkPixelShader) {
	m_spPixelShader = pkPixelShader;
}

NiD3DPixelShader* NiD3DPass::GetPixelShader() {
	return m_spPixelShader;
}
