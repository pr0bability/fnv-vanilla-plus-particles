#pragma once

#include "Gamebryo/NiTStringMap.hpp"

struct ShaderBufferEntry {
	char	cFileName[260];
	DWORD	shader;
};

class ShaderBuffer {
public:
	const char*								pPackagePath;
	void*									pMem;
	NiTStringPointerMap<ShaderBufferEntry*>	kShaders;

	ShaderBufferEntry* Get(const char* apcShaderName) const {
		ShaderBufferEntry* pShader = nullptr;
		kShaders.GetAt(apcShaderName, pShader);
		return pShader;
	}
};

ASSERT_SIZE(ShaderBuffer, 0x1C)