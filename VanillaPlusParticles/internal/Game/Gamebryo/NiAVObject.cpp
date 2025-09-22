#include "NiAVObject.hpp"

// GAME - 0x43D450
// GECK - 0x446A10
NiBound* NiAVObject::GetWorldBound() const {
#if GAME
	return ThisCall<NiBound*>(0x43D450, this);
#else
	return ThisCall<NiBound*>(0x446A10, this);
#endif
}

void NiAVObject::SetBit(uint32_t auData, bool abVal) {
	m_uiFlags.SetBit(auData, abVal);
}

bool NiAVObject::GetBit(uint32_t auData) const {
	return m_uiFlags.IsSet(auData);
}
