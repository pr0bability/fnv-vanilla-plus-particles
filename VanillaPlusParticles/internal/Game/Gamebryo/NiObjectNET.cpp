#include "NiObjectNET.hpp"

// GAME - 0xA5BDD0
// GECK - 0x8185B0
NiExtraData* NiObjectNET::GetExtraData(const NiFixedString& arKey) const {
#ifdef GAME
	return ThisCall<NiExtraData*>(0xA5BDD0, this, &arKey);
#else
	return ThisCall<NiExtraData*>(0x8185B0, this, &arKey);
#endif
}

// GAME - 0x4AD1B0
NiObjectNET::CopyType NiObjectNET::GetDefaultCopyType() {
	return *reinterpret_cast<CopyType*>(0x11F4300);
}

// GAME - 0x4AD1C0
char NiObjectNET::GetDefaultAppendCharacter() {
	return *reinterpret_cast<char*>(0x11A94A8);
}
