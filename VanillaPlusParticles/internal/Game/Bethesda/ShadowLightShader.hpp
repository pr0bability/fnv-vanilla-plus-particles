#pragma once

#include "BSShader.hpp"
#include "Gamebryo/NiPoint4.hpp"
#include "Gamebryo/NiShaderConstantMapEntry.hpp"
#include "BSShaderPPLightingProperty.hpp"

class NiD3DShaderConstantMap;
class BSFogProperty;

class ShadowLightShader : public BSShader {
public:
	virtual void SetupShadowTexture(BSShaderManager::RenderPassType aeRenderPass, BSShaderPPLightingProperty* apShaderProp);
	virtual void Func_80(); // nullsub
	virtual void LoadShaders();
	virtual void LoadVertexShaders();
	virtual void LoadPixelShaders();
	virtual void LoadStagesAndPasses();
	virtual void UpdateTogglesConstant(NiGeometry* apGeometry, BSFogProperty* apFogProp, NiMaterialProperty* apMatProp);

	NiDX9ShaderDeclarationPtr	spShaderDeclarationStatic;
	NiDX9ShaderDeclarationPtr	spShaderDeclarationSkinned;
	NiDX9ShaderDeclarationPtr	spShaderDeclarationModelSpaceNormals;
	NiDX9ShaderDeclarationPtr	spShaderDeclarationLandscape;
	NiD3DShaderConstantMapPtr	spTexEffectPixelConstantMap;
	NiD3DShaderConstantMapPtr	spTexEffectVertexConstantMap;
	NiD3DShaderConstantMapPtr	spPixelConstantMap2;
	NiD3DShaderConstantMapPtr	spVertexConstantMap2;
};

ASSERT_SIZE(ShadowLightShader, 0x8C)