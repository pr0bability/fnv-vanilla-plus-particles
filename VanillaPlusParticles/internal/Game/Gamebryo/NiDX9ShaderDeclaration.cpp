#include "NiDX9ShaderDeclaration.hpp"

// GAME - 0xE76700
// GECK - 0xC186C0
NiDX9ShaderDeclaration* NiDX9ShaderDeclaration::Create(NiDX9Renderer* apRenderer, uint32_t auiMaxStreamEntryCount, uint32_t auiStreamCount) {
	return CdeclCall<NiDX9ShaderDeclaration*>(0xE76700, apRenderer, auiMaxStreamEntryCount, auiStreamCount);
}
