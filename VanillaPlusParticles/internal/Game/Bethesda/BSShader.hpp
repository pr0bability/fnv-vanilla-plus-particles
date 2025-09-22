#pragma once

#include "BSShaderManager.hpp"
#include "BSShaderProperty.hpp"
#include "Gamebryo/NiColorA.hpp"
#include "Gamebryo/NiD3DDefaultShader.hpp"
#include "Gamebryo/NiDX9ShaderDeclaration.hpp"

typedef NiD3DPixelShader* (__cdecl pfn_CreatePixelShader)(const char* apFilename);

class NiD3DPass;

class BSShader : public NiD3DDefaultShader {
public:
	enum ShaderType {
		TYPE_ShadowLightShader		= 1,
		TYPE_HairShader				= 2,
		TYPE_ParallaxShader			= 3,
		TYPE_SkinShader				= 4,
		TYPE_SpeedTreeBranchShader	= 5,
		TYPE_TallGrassShader		= 6,
		TYPE_DistantLODShader		= 7,
		TYPE_SpeedTreeFrondShader	= 8,
		TYPE_SpeedTreeLeafShader	= 9,
		TYPE_DebugShader			= 10,
		TYPE_SkyShader				= 11,
		TYPE_GeometryDecalShader	= 12,
		TYPE_WaterShader			= 13,
		TYPE_ParticleShader			= 14,
		TYPE_BoltShader				= 15,
		TYPE_BeamShader				= 16,
		TYPE_Lighting30Shader		= 17,
		TYPE_PrecipitationShader	= 18,
		TYPE_VolumetricFogShader	= 19,
		TYPE_TileShader				= 20,
		//							  21
		TYPE_BSShaderNoLighting		= 22,
		TYPE_BSShaderBloodSplatter	= 23,
		TYPE_ImagespaceShader		= 24,
		TYPE_BSDistantTreeShader	= 25,
		
		TYPE_SEShader				= 26,
	};

	virtual void					SetRenderPass(BSShaderManager::RenderPassType aeType);
	virtual void					SetShaders(BSShaderManager::RenderPassType aeType = BSShaderManager::BSSM_EMPTY);
	virtual void					SetStaticTextures(BSShaderManager::RenderPassType aeType);
	virtual void					SetStaticConstants(BSShaderManager::RenderPassType aeType);
	virtual void					SetRenderState(BSShaderManager::RenderPassType aeType);
	virtual void					RestoreRenderState(BSShaderManager::RenderPassType aeType);
	virtual void					ConfigureTextureStages();
	virtual void					ConfigureAllTextureStages();
	virtual void					StartMultiPass(const NiPropertyState* apProperties);
	virtual void					StopMultiPass();
	virtual NiDX9ShaderDeclaration* GetShaderDeclaration(NiGeometry*, BSShaderProperty* property);
	virtual void					CreateConstantMaps();
	virtual void					ReloadShaders();
	virtual void					ClearPassStages();
	virtual void					ClearPass(NiD3DPass* apPass);
	virtual bool					Func_75();
	virtual void					CreateNewPass();
	virtual bool					Func_77();
	virtual void					ClarifyShader(NiGeometry* apGeometry, bool);

	NiD3DPassPtr	spPass;
	NiGeometry*		pUnk60;
	bool			bUnk64;
	int32_t			iShaderType;

	bool IsShadowLightShader() const { return iShaderType == TYPE_ShadowLightShader; }
	bool IsHairShader() const { return iShaderType == TYPE_HairShader; }
	bool IsParallaxShader() const { return iShaderType == TYPE_ParallaxShader; }
	bool IsSkinShader() const { return iShaderType == TYPE_SkinShader; }
	bool IsSpeedTreeBranchShader() const { return iShaderType == TYPE_SpeedTreeBranchShader; }
	bool IsTallGrassShader() const { return iShaderType == TYPE_TallGrassShader; }
	bool IsDistantLODShader() const { return iShaderType == TYPE_DistantLODShader; }
	bool IsSpeedTreeFrondShader() const { return iShaderType == TYPE_SpeedTreeFrondShader; }
	bool IsSpeedTreeLeafShader() const { return iShaderType == TYPE_SpeedTreeLeafShader; }
	bool IsDebugShader() const { return iShaderType == TYPE_DebugShader; }
	bool IsSkyShader() const { return iShaderType == TYPE_SkyShader; }
	bool IsGeometryDecalShader() const { return iShaderType == TYPE_GeometryDecalShader; }
	bool IsWaterShader() const { return iShaderType == TYPE_WaterShader; }
	bool IsParticleShader() const { return iShaderType == TYPE_ParticleShader; }
	bool IsBoltShader() const { return iShaderType == TYPE_BoltShader; }
	bool IsBeamShader() const { return iShaderType == TYPE_BeamShader; }
	bool IsLighting30Shader() const { return iShaderType == TYPE_Lighting30Shader; }
	bool IsPrecipitationShader() const { return iShaderType == TYPE_PrecipitationShader; }
	bool IsVolumetricFogShader() const { return iShaderType == TYPE_VolumetricFogShader; }
	bool IsTileShader() const { return iShaderType == TYPE_TileShader; }
	bool IsBSShaderNoLighting() const { return iShaderType == TYPE_BSShaderNoLighting; }
	bool IsBSShaderBloodSplatter() const { return iShaderType == TYPE_BSShaderBloodSplatter; }
	bool IsImagespaceShader() const { return iShaderType == TYPE_ImagespaceShader; }
	bool IsBSDistantTreeShader() const { return iShaderType == TYPE_BSDistantTreeShader; }

	static pfn_CreatePixelShader* pLoadPixelShader;
};

ASSERT_SIZE(BSShader, 0x6C);