#include "NiD3DShader.hpp"

NiD3DPass* NiD3DShader::GetCurrentPass() {
	return *reinterpret_cast<NiD3DPass**>(0x126F74C);
}
