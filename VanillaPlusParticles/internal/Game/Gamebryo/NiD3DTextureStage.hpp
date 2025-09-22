#pragma once

#include "NiTexturingProperty.hpp"
#include "NiD3DTextureStageGroup.hpp"
#include "NiDX9RenderState.hpp"

class NiGlobalConstantEntry;

NiSmartPointer(NiD3DTextureStage);

class NiD3DTextureStage : public NiMemObject {
public:
	NiD3DTextureStage();
	~NiD3DTextureStage();

	NiTexturingProperty::FilterMode		m_eFilter;
	uint32_t							m_uiStage;
	NiTexture*							m_pkTexture;
	uint32_t							m_uiTextureFlags;
	NiD3DTextureStageGroup*				m_pkTextureStageGroup;
	uint32_t							m_uiTextureTransformFlags;
	NiGlobalConstantEntry*				m_pkGlobalEntry;
	D3DMATRIX							m_kTextureTransformation;
	uint16_t							m_usObjTextureFlags;
	bool								m_bTextureTransform;
	bool								m_bRendererOwned;
	uint32_t							m_uiRefCount;
	bool								bApply;
	bool								bUnk65;
	bool								bUnk66;

	static NiDX9RenderState* GetRenderState();
	static NiDX9Renderer* GetRenderer();
	static LPDIRECT3DDEVICE9 GetD3DDevice();

	static void CreateNewStage(NiD3DTextureStagePtr& aspStage);

	void ApplyTexture();
	bool ConfigureStage(bool abAllowRepeats = false);
	void SetClampMode(NiTexturingProperty::ClampMode eClamp);
	void SetFilterMode(NiTexturingProperty::FilterMode eFilter);
	void SetStageProperties(uint32_t auiTextureIndex, NiTexturingProperty::ClampMode aeClampMode, uint32_t aeFilterValue, bool abUseAnisotropicFilter = true);

	void ReturnStageToPool();

	void IncRefCount();
	void DecRefCount();
};

ASSERT_SIZE(NiD3DTextureStage, 0x68);