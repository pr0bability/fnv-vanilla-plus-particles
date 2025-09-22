#include "BSShaderProperty.hpp"

// GAME - 0x4A2020
// GECK - 0x5018F0
bool BSShaderProperty::GetFlag(uint32_t auiFlag) const {
	__assume(auiFlag >= BSSP_SPECULAR && auiFlag < BSSP_MAX_FLAGS);
	const uint32_t uiFlagSet = auiFlag >> 5;
	const uint32_t uiBit = auiFlag % 32;
	return ulFlags[uiFlagSet].Test(uiBit);
}

// GAME - 0x441130
// GECK - 0x4BB2E0
void BSShaderProperty::SetFlag(uint32_t auiFlag, bool abEnable) {
	__assume(auiFlag >= BSSP_SPECULAR && auiFlag < BSSP_MAX_FLAGS);
	bool bClear = false;
	const uint32_t uiFlagSet = auiFlag >> 5;
	const uint32_t uiBit = auiFlag % 32;
	if (abEnable)
		bClear = ulFlags[uiFlagSet].TestSet(uiBit) == false;
	else
		bClear = ulFlags[uiFlagSet].TestClear(uiBit) == true;

	if (bClear)
		iLastRenderPassState = 0;
}

NiGeometry* BSShaderProperty::RenderPass::GetGeometry() const {
	return pGeometry;
}

