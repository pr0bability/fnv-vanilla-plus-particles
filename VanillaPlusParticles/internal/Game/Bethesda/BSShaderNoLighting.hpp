#pragma once

#include "BSShader.hpp"
#include "Gamebryo/NiPoint4.hpp"
#include "BSShaderNoLightingProperty.hpp"

class BSShaderNoLighting : public BSShader {
public:
	NiD3DPassPtr				spPasses[30];
	NiD3DVertexShaderPtr		spVertexShaders[28];
	NiD3DPixelShaderPtr			spPixelShaders[7];
	NiShaderConstantMapEntry*	pSubtex;
	NiShaderConstantMapEntry*	pWorldTranspose;
	NiShaderConstantMapEntry*	pWorldViewTranspose;
	NiShaderConstantMapEntry*	pEyeDir;
	NiShaderConstantMapEntry*	pBoneMatrix3;
	NiDX9ShaderDeclarationPtr	spShaderDeclarations[12];

	NIRTTI_ADDRESS(0x1202040);

	NiD3DPass* GetPass(uint32_t auID) const { return spPasses[auID]; }
	NiD3DVertexShader* GetVertexShader(uint32_t auID) const { return spVertexShaders[auID]; };
	NiD3DPixelShader* GetPixelShader(uint32_t auID) const { return spPixelShaders[auID]; };
	NiDX9ShaderDeclaration* GetStoredShaderDeclaration(uint32_t auID) const { return spShaderDeclarations[auID]; };

	struct VertexConstants {
		static BSShaderNoLightingProperty::FalloffData* const Falloff;

		static D3DMATRIX* const kTextureTransform;

		static float* GetSubTex();
	};

	struct PixelConstants {
		static NiColorA* const MatColor;
		static NiPoint4* const MatAlpha;
	};
};

ASSERT_SIZE(BSShaderNoLighting, 0x1B4);