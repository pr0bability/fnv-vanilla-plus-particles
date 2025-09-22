#include "BSShaderManager.hpp"

BSShaderManager::RenderPassType BSShaderManager::GetCurrentPass() {
#ifdef GAME
	return *reinterpret_cast<RenderPassType*>(0x11F91E4);
#else
	return *reinterpret_cast<RenderPassType*>(0xF23C34);
#endif
}

BSShaderProperty::RenderPass* BSShaderManager::GetCurrentRenderPass() {
#ifdef GAME
	return *reinterpret_cast<BSShaderProperty::RenderPass**>(0x11F91E0);
#else
	return *reinterpret_cast<BSShaderProperty::RenderPass**>(0xF23C30);
#endif
}

NiDX9Renderer* BSShaderManager::GetRenderer() {
	return *reinterpret_cast<NiPointer<NiDX9Renderer>*>(0x11F9508);
}

// GAME - 0xB55840
// GECK - 0x8FE510
void BSShaderManager::GetTexture(const char* apFilename, bool abLoadFile, NiPointer<NiTexture>& aspTexture, bool abCheckMips, bool abCubeMap) {
	CdeclCall(0xB55840, apFilename, abLoadFile, &aspTexture, abCheckMips, abCubeMap);
}
