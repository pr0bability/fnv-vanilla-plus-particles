#include "Bethesda/BSShader.hpp"
#include "Bethesda/BSShaderNoLighting.hpp"
#include "Bethesda/BSStripPSysData.hpp"
#include "Bethesda/ImageSpaceManager.hpp"
#include "Bethesda/TESMain.hpp"
#include "Gamebryo/NiFloatExtraData.hpp"
#include "Gamebryo/NiD3DPixelShader.hpp"
#include "Gamebryo/NiDX9Renderer.hpp"

#include "nvse/PluginAPI.h"

IDebugLog gLog("logs\\VanillaPlusParticles.log");

// Hook related globals.
static CallDetour		kLoadShadersDetour[2];

static VirtFuncDetour	kSetupTexturesDetour;
static VirtFuncDetour	kUpdateConstantsDetour;

// Constants.
static NVSEMessagingInterface*	pMsgInterface = nullptr;
static uint32_t					uiPluginHandle = 0;
static constexpr uint32_t		uiShaderLoaderVersion = 131;

class BSShaderNoLightingEx : public BSShaderNoLighting {
public:
	static NiD3DPixelShaderPtr spPixelShadersSoft[5];

	void SetupTexturesEx(const NiPropertyState* apProperties) {
		ThisCall(kSetupTexturesDetour.GetOverwrittenAddr(), this, apProperties);

		auto eCurrentPass = BSShaderManager::GetCurrentPass();

		if (eCurrentPass >= BSShaderManager::BSSM_NOLIGHTING && eCurrentPass <= BSShaderManager::BSSM_NOLIGHTING_STRIP_PSYS_SUBTEX) {
			NiD3DPass* pPass = GetCurrentPass();
			NiD3DTextureStage* pTextureStage = pPass->GetStage(pPass->m_uiStageCount - 1);
			if (pTextureStage) {
				NiTexture* pDepthTexture = ImageSpaceManager::GetDepthTexture();
				if (pDepthTexture) {
					pTextureStage->m_pkTexture = pDepthTexture;
					pTextureStage->ConfigureStage();
				}
			}
		}
	}

	void UpdateConstantsEx(const NiPropertyState* apProperties) {
		ThisCall(kUpdateConstantsDetour.GetOverwrittenAddr(), this, apProperties);

		auto eCurrentPass = BSShaderManager::GetCurrentPass();

		if (eCurrentPass < BSShaderManager::BSSM_NOLIGHTING || eCurrentPass > BSShaderManager::BSSM_NOLIGHTING_STRIP_PSYS_SUBTEX)
			return;

		NiDX9Renderer* pRenderer = NiDX9Renderer::GetSingleton();
		IDirect3DDevice9* pDevice = pRenderer->GetD3DDevice();

		NiD3DPass* pPass = GetCurrentPass();
		NiGeometry* pGeo = BSShaderManager::GetCurrentRenderPass()->GetGeometry();
		BSShaderNoLightingProperty* pShaderProp = apProperties->GetShadeProperty<BSShaderNoLightingProperty>();
		NiAlphaProperty* pAlphaProp = apProperties->GetAlphaProperty();

		bool bSoft = pShaderProp->IsSoft();
		bSoft |= eCurrentPass >= BSShaderManager::BSSM_NOLIGHTING_PSYS && eCurrentPass <= BSShaderManager::BSSM_NOLIGHTING_STRIP_PSYS_SUBTEX;

		if (pAlphaProp)
			bSoft &= pAlphaProp->GetAlphaBlending();
		else
			bSoft = false;

		if (bSoft) {
			if (pPass->GetPixelShader() == spPixelShaders[0])
				NiD3DPass::GetD3DRenderState()->SetPixelShader(spPixelShadersSoft[0]->m_hShader);
			else if (pPass->GetPixelShader() == spPixelShaders[1])
				NiD3DPass::GetD3DRenderState()->SetPixelShader(spPixelShadersSoft[1]->m_hShader);
			else if (pPass->GetPixelShader() == spPixelShaders[2])
				NiD3DPass::GetD3DRenderState()->SetPixelShader(spPixelShadersSoft[2]->m_hShader);
			else if (pPass->GetPixelShader() == spPixelShaders[3])
				NiD3DPass::GetD3DRenderState()->SetPixelShader(spPixelShadersSoft[3]->m_hShader);
			else if (pPass->GetPixelShader() == spPixelShaders[4])
				NiD3DPass::GetD3DRenderState()->SetPixelShader(spPixelShadersSoft[4]->m_hShader);

			SceneGraph* pSceneGraph = TESMain::GetWorldSceneGraph();
			NiCamera* pSceneGraphCamera = pSceneGraph->GetCamera();

			float fNear = pSceneGraphCamera->m_kViewFrustum.m_fNear;
			float fFar = pSceneGraphCamera->m_kViewFrustum.m_fFar;
			float fFmN = fFar - fNear;
			float fNtF = fNear * fFar;

			float fSize = pGeo->GetWorldBound()->m_fRadius;
			float fSoftScale = 0.25f;

			NiExtraData* pSoftScaleExtraData = pGeo->GetExtraData(NiFixedString("VPSoftScale"));

			if (pSoftScaleExtraData) {
				NiFloatExtraData* pSoftScaleFloatData = static_cast<NiFloatExtraData*>(pSoftScaleExtraData);
				fSoftScale = pSoftScaleFloatData->m_fValue;
			}

			NiPoint4 kScreenSize(fSoftScale, 0.0f, 1.0f / pRenderer->GetScreenWidth(), 1.0f / pRenderer->GetScreenHeight());
			NiPoint4 kDepthConstants(fNear, fFar, fFmN, fNtF);

			pDevice->SetVertexShaderConstantF(41, &fSize, 1);
			pDevice->SetPixelShaderConstantF(8, (float*)&kScreenSize, 1);
			pDevice->SetPixelShaderConstantF(9, (float*)&kDepthConstants, 1);
		}
	}

	template <uint32_t uiCall>
	void LoadShaders() {
		ThisCall(kLoadShadersDetour[uiCall].GetOverwrittenAddr(), this);

		spPixelShadersSoft[0] = pLoadPixelShader("NOLIGHTTEXSOFT.pso");
		spPixelShadersSoft[1] = pLoadPixelShader("NOLIGHTVCONLYSOFT.pso");
		spPixelShadersSoft[2] = pLoadPixelShader("NOLIGHTTEXVCSOFT.pso");
		spPixelShadersSoft[3] = pLoadPixelShader("NOLIGHTTEXVCPMASOFT.pso");
		spPixelShadersSoft[4] = pLoadPixelShader("NOLIGHTMATSOFT.pso");
	}

	bool LoadStagesAndPasses() {
		bool bResult = ThisCall<bool>(0xBC77E0, this);

		NiD3DTextureStagePtr spStage = nullptr;

		for (int i = 0; i < 22; i++) {
			NiD3DPass* pPass = GetPass(i);

			NiD3DTextureStage::CreateNewStage(spStage);
			spStage->SetStageProperties(GetPass(i)->m_uiCurrentStage, NiTexturingProperty::CLAMP_S_CLAMP_T, 2);
			GetPass(i)->SetStage(GetPass(i)->m_uiCurrentStage, spStage);
		}

		return bResult;
	}
};

class NiDX9RendererEx : public NiDX9Renderer {
public:
	struct BSStripPSysDataVertexDataEx {
		NiPoint3 kPosition;
		NiPoint3 kNormal;
		uint32_t uiPackedColor;
		float fU;
		float fV;
		float fSize;
		float fTextureIndex;
	};

	static thread_local bool bLastTextureIndicesStatus;

	// GAME - 0xE69790
	LPDIRECT3DVERTEXDECLARATION9 GetDecl_Particles() {
		bLastTextureIndicesStatus = true;

		if (hParticleVertexDecls[1])
			return hParticleVertexDecls[1];

		D3DVERTEXELEMENT9 kVertexElements[6];

		kVertexElements[0] = { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 };
		kVertexElements[1] = { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 };
		kVertexElements[2] = { 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 };
		kVertexElements[3] = { 0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 };
		kVertexElements[4] = { 0, 36, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 };
		kVertexElements[5] = D3DDECL_END();

		if (m_pkD3DDevice9->CreateVertexDeclaration(kVertexElements, &hParticleVertexDecls[1]) == D3D_OK)
			return hParticleVertexDecls[1];

		return nullptr;
	}

	// GAME - 0xE696A0
	LPDIRECT3DVERTEXDECLARATION9 GetDecl_ParticlesSubTexture() {
		bLastTextureIndicesStatus = false;

		if (hParticleVertexDecls[0])
			return hParticleVertexDecls[0];

		D3DVERTEXELEMENT9 kVertexElements[6];

		kVertexElements[0] = { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 };
		kVertexElements[1] = { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 };
		kVertexElements[2] = { 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 };
		kVertexElements[3] = { 0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 };
		kVertexElements[4] = { 0, 36, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 };
		kVertexElements[5] = D3DDECL_END();

		if (m_pkD3DDevice9->CreateVertexDeclaration(kVertexElements, &hParticleVertexDecls[0]) == D3D_OK)
			return hParticleVertexDecls[0];

		return nullptr;
	}

	// GAME - 0xE69EA0
	void PackPointsSubTexture(void* apVertexBuffer, NiPoint3* apPositions, NiColorA* apColors, float* apRadii, float* apSizes, float* apRotationAngles, uint16_t ausParticleCount) {
		if (!ausParticleCount)
			return;

		NiPoint3 kNormal, kRight, kUp;
		kNormal.x = m_kModelCamUp.z * m_kModelCamRight.y - m_kModelCamUp.y * m_kModelCamRight.z;
		kNormal.y = m_kModelCamRight.z * m_kModelCamUp.x - m_kModelCamUp.z * m_kModelCamRight.x;
		kNormal.z = m_kModelCamUp.y * m_kModelCamRight.x - m_kModelCamRight.y * m_kModelCamUp.x;

		struct Vertex {
			NiPoint3 kPosition;
			NiPoint3 kNormal;
			uint32_t uiColor;
			float fU, fV;
			float fSize;
		};

		Vertex* pVertex = static_cast<Vertex*>(apVertexBuffer);

		for (uint16_t i = 0; i < ausParticleCount; i++) {
			NiPoint3 kPosition = apPositions[i];

			float fParticleSize = apRadii[i] * apSizes[i];

			uint32_t uiPackedColor = 0xFFFFFFFF;
			if (apColors) {
				NiColorA kColor = apColors[i] * 255.0f;
				uiPackedColor = (uint8_t)kColor.a << 24 | (uint8_t)kColor.r << 16 | (uint8_t)kColor.g << 8 | (uint8_t)kColor.b;
			}

			float fRightRotation = 1.0f;
			float fUpRotation = 1.0f;
			if (apRotationAngles) {
				float fRotationAngle = apRotationAngles[i];
				float fCos = cos(fRotationAngle);
				float fSin = sin(fRotationAngle);

				fRightRotation = (fSin + fCos);
				fUpRotation = (fCos - fSin);
			}

			kRight = (m_kModelCamRight * fRightRotation + m_kModelCamUp * fUpRotation) * fParticleSize;
			kUp = (m_kModelCamUp * fRightRotation - m_kModelCamRight * fUpRotation) * fParticleSize;

			pVertex->kPosition = kPosition - kRight;
			pVertex->kNormal = kNormal;
			pVertex->uiColor = uiPackedColor;
			pVertex->fU = 0.0f;
			pVertex->fV = 1.0f;
			pVertex->fSize = fParticleSize;
			pVertex++;

			pVertex->kPosition = kPosition - kUp;
			pVertex->kNormal = kNormal;
			pVertex->uiColor = uiPackedColor;
			pVertex->fU = 1.0f;
			pVertex->fV = 1.0f;
			pVertex->fSize = fParticleSize;
			pVertex++;

			pVertex->kPosition = kPosition + kRight;
			pVertex->kNormal = kNormal;
			pVertex->uiColor = uiPackedColor;
			pVertex->fU = 1.0f;
			pVertex->fV = 0.0f;
			pVertex->fSize = fParticleSize;
			pVertex++;

			pVertex->kPosition = kPosition + kUp;
			pVertex->kNormal = kNormal;
			pVertex->uiColor = uiPackedColor;
			pVertex->fU = 0.0f;
			pVertex->fV = 0.0f;
			pVertex->fSize = fParticleSize;
			pVertex++;
		}
	}

	// GAME - 0xE6AB40
	void PackPoints(void* apVertexBuffer, NiPoint3* apPositions, NiColorA* apColors, float* apRadii, float* apSizes, float* apRotationAngles, uint8_t* apTextureIndices, uint16_t ausParticleCount) {
		if (!ausParticleCount)
			return;

		NiPoint3 kNormal, kRight, kUp;
		kNormal.x = m_kModelCamUp.z * m_kModelCamRight.y - m_kModelCamUp.y * m_kModelCamRight.z;
		kNormal.y = m_kModelCamRight.z * m_kModelCamUp.x - m_kModelCamUp.z * m_kModelCamRight.x;
		kNormal.z = m_kModelCamUp.y * m_kModelCamRight.x - m_kModelCamRight.y * m_kModelCamUp.x;

		struct Vertex {
			NiPoint3 kPosition;
			NiPoint3 kNormal;
			uint32_t uiColor;
			float fU, fV;
			float fSize;
			float fTextureIndex;
		};

		Vertex* pVertex = static_cast<Vertex*>(apVertexBuffer);

		for (uint16_t i = 0; i < ausParticleCount; i++) {
			NiPoint3 kPosition = apPositions[i];

			float fParticleSize = apRadii[i] * apSizes[i];

			uint32_t uiPackedColor = 0xFFFFFFFF;
			if (apColors) {
				NiColorA kColor = apColors[i] * 255.0f;
				uiPackedColor = (uint8_t)kColor.a << 24 | (uint8_t)kColor.r << 16 | (uint8_t)kColor.g << 8 | (uint8_t)kColor.b;
			}

			float fTextureIndex = apTextureIndices ? apTextureIndices[i] : -1.0f;

			float fRightRotation = 1.0f;
			float fUpRotation = 1.0f;
			if (apRotationAngles) {
				float fRotationAngle = apRotationAngles[i];
				float fCos = cos(fRotationAngle);
				float fSin = sin(fRotationAngle);

				fRightRotation = (fSin + fCos);
				fUpRotation = (fCos - fSin);
			}

			kRight = (m_kModelCamRight * fRightRotation + m_kModelCamUp * fUpRotation) * fParticleSize;
			kUp = (m_kModelCamUp * fRightRotation - m_kModelCamRight * fUpRotation) * fParticleSize;

			pVertex->kPosition = kPosition - kRight;
			pVertex->kNormal = kNormal;
			pVertex->uiColor = uiPackedColor;
			pVertex->fU = 0.0f;
			pVertex->fV = 1.0f;
			pVertex->fSize = fParticleSize;
			pVertex->fTextureIndex = fTextureIndex;
			pVertex++;

			pVertex->kPosition = kPosition - kUp;
			pVertex->kNormal = kNormal;
			pVertex->uiColor = uiPackedColor;
			pVertex->fU = 1.0f;
			pVertex->fV = 1.0f;
			pVertex->fSize = fParticleSize;
			pVertex->fTextureIndex = fTextureIndex;
			pVertex++;

			pVertex->kPosition = kPosition + kRight;
			pVertex->kNormal = kNormal;
			pVertex->uiColor = uiPackedColor;
			pVertex->fU = 1.0f;
			pVertex->fV = 0.0f;
			pVertex->fSize = fParticleSize;
			pVertex->fTextureIndex = fTextureIndex;
			pVertex++;

			pVertex->kPosition = kPosition + kUp;
			pVertex->kNormal = kNormal;
			pVertex->uiColor = uiPackedColor;
			pVertex->fU = 0.0f;
			pVertex->fV = 0.0f;
			pVertex->fSize = fParticleSize;
			pVertex->fTextureIndex = fTextureIndex;
			pVertex++;
		}
	}

	// GAME - 0xE6D3A0
	uint32_t PackParticleStrips(BSStripPSysData::VertexData* apVertexBuffer, uint16_t* apIndexBuffer, BSStripPSysData* apStripData, uint32_t* auiVertexCount, uint32_t* auiIndexCount) {
		*auiVertexCount = 0;
		*auiIndexCount = 0;

		uint8_t* pTextureIndices = apStripData->m_pucTextureIndices;
		uint16_t usStripCount = apStripData->m_usActiveVertices;

		uint16_t usCurrentVertexIndex = 0;
		uint16_t usCurrentTextureIndex = 0;
		bool bFirstStrip = true;

		BSStripPSysDataVertexDataEx* pCurrentVertex = reinterpret_cast<BSStripPSysDataVertexDataEx*>(apVertexBuffer);
		uint16_t* pusCurrentIndex = apIndexBuffer;

		for (uint16_t usStripIndex = 0; usStripIndex < usStripCount; usStripIndex++) {
			uint16_t usPointCount = apStripData->GetPointCount(usStripIndex);
			uint16_t usMaxPoints = apStripData->pStripList[usStripIndex].usMaxPointCount;

			if (usPointCount >= 2) {
				if (!bFirstStrip) {
					*pusCurrentIndex++ = usCurrentVertexIndex - 1;
					*pusCurrentIndex++ = usCurrentVertexIndex;
					*auiIndexCount += 2;
				}
				else {
					bFirstStrip = false;
				}

				float fTexCoordDelta = apStripData->GetTexCoordDelta(usStripIndex);
				float fCurrentU = 0.0f;

				uint16_t usPointIndex = apStripData->pStripList[usStripIndex].iFirstIndex;
				uint16_t usStripStartVertex = usCurrentVertexIndex + 2;

				*auiVertexCount += 4 * usPointCount;

				for (uint16_t i = 0; i < usPointCount; i++) {
					const NiPoint3* pPositions = apStripData->GetPointVerts(usStripIndex, usPointIndex);
					const NiPoint3* pNormals = apStripData->GetPointNorms(usStripIndex, usPointIndex);
					const NiColorA* pColor = apStripData->GetPointColor(usStripIndex, usPointIndex);
					float fSize = apStripData->GetPointSize(usStripIndex, usPointIndex) * apStripData->GetPointRadii(usStripIndex, usPointIndex);

					NiColorA kColor = *pColor * 255.0f;
					uint32_t uiPackedColor = (uint8_t)kColor.a << 24 | (uint8_t)kColor.r << 16 | (uint8_t)kColor.g << 8 | (uint8_t)kColor.b;

					float fTextureIndex = pTextureIndices ? pTextureIndices[usCurrentTextureIndex] : -1.0f;

					pCurrentVertex = PackParticleStripsVertex(pCurrentVertex, &pPositions[0], &pNormals[0], uiPackedColor, fCurrentU, 0.0f, fSize, fTextureIndex);
					pCurrentVertex = PackParticleStripsVertex(pCurrentVertex, &pPositions[1], &pNormals[0], uiPackedColor, fCurrentU, 1.0f, fSize, fTextureIndex);
					pCurrentVertex = PackParticleStripsVertex(pCurrentVertex, &pPositions[2], &pNormals[1], uiPackedColor, fCurrentU, 0.0f, fSize, fTextureIndex);
					pCurrentVertex = PackParticleStripsVertex(pCurrentVertex, &pPositions[3], &pNormals[1], uiPackedColor, fCurrentU, 1.0f, fSize, fTextureIndex);

					*pusCurrentIndex++ = usCurrentVertexIndex;
					*pusCurrentIndex++ = usCurrentVertexIndex + 1;
					usCurrentVertexIndex += 4;
					*auiIndexCount += 2;

					fCurrentU += fTexCoordDelta;

					usPointIndex = (usPointIndex + 1) % usMaxPoints;
					if (usPointIndex < 0)
						usPointIndex += usMaxPoints;
				}

				*pusCurrentIndex++ = usCurrentVertexIndex - 3;
				*pusCurrentIndex++ = usStripStartVertex;
				*auiIndexCount += 2;

				for (uint16_t i = 0; i < usPointCount; i++) {
					*pusCurrentIndex++ = usStripStartVertex;
					*pusCurrentIndex++ = usStripStartVertex + 1;
					usStripStartVertex += 4;
					*auiIndexCount += 2;
				}
			}

			usCurrentTextureIndex++;
		}

		return *auiIndexCount;
	}

	BSStripPSysDataVertexDataEx* PackParticleStripsVertex(BSStripPSysDataVertexDataEx* apVertex, const NiPoint3* apPosition, const NiPoint3* apNormal, uint32_t auiPackedColor, float afU, float afV, float fSize, float afTextureIndex) {
		apVertex->kPosition = *apPosition;
		apVertex->kNormal = *apNormal;
		apVertex->uiPackedColor = auiPackedColor;
		apVertex->fU = afU;
		apVertex->fV = afV;
		apVertex->fSize = fSize;

		if (afTextureIndex < 0.0f) {
			return (BSStripPSysDataVertexDataEx*)&apVertex->fTextureIndex;
		}
		else {
			apVertex->fTextureIndex = afTextureIndex;
			return apVertex + 1;
		}
	}
};

class NiDX9VertexBufferManagerEx {
public:
	void* LockVBParticles(LPDIRECT3DVERTEXBUFFER9 apVB, uint32_t auiOffset, uint32_t auiSize, uint32_t auiLockFlags) {
		if (NiDX9RendererEx::bLastTextureIndicesStatus)
			auiSize = (auiSize / 40) * 44;
		else
			auiSize = (auiSize / 36) * 40;
		return ThisCall<void*>(0xE8BFF0, this, apVB, auiOffset, auiSize, auiLockFlags);
	}
};

NiD3DPixelShaderPtr BSShaderNoLightingEx::spPixelShadersSoft[5];

thread_local bool NiDX9RendererEx::bLastTextureIndicesStatus = false;

void InitHooks() {
	kSetupTexturesDetour.ReplaceVirtualFuncEx(0x10BB2A8, &BSShaderNoLightingEx::SetupTexturesEx);
	kUpdateConstantsDetour.ReplaceVirtualFuncEx(0x10BB2AC, &BSShaderNoLightingEx::UpdateConstantsEx);

	kLoadShadersDetour[0].ReplaceCallEx(0xBC8D33, &BSShaderNoLightingEx::LoadShaders<0>);
	kLoadShadersDetour[1].ReplaceCallEx(0xBC9A06, &BSShaderNoLightingEx::LoadShaders<1>);

	WriteRelJumpEx(0xBC8D3B, &BSShaderNoLightingEx::LoadStagesAndPasses);
	ReplaceCallEx(0xBC9A0D, &BSShaderNoLightingEx::LoadStagesAndPasses);

	WriteRelJumpEx(0xE69790, &NiDX9RendererEx::GetDecl_Particles);
	WriteRelJumpEx(0xE696A0, &NiDX9RendererEx::GetDecl_ParticlesSubTexture);

	ReplaceCallEx(0xE705FE, &NiDX9RendererEx::PackPointsSubTexture);
	ReplaceCallEx(0xE70560, &NiDX9RendererEx::PackPoints);
	ReplaceCallEx(0xE70AE1, &NiDX9RendererEx::PackParticleStrips);

	// Fix strides (36->40, 40->44).
	SafeWrite8(0xE704DD, 0x2C);
	SafeWrite8(0xE70582, 0x28);
	SafeWrite8(0xE708FD, 0x28);
	SafeWrite8(0xE708BC, 0x2C);

	ReplaceCallEx(0xE705D0, &NiDX9VertexBufferManagerEx::LockVBParticles);
	ReplaceCallEx(0xE7052D, &NiDX9VertexBufferManagerEx::LockVBParticles);
	ReplaceCallEx(0xE709F3, &NiDX9VertexBufferManagerEx::LockVBParticles);
}

EXTERN_DLL_EXPORT bool NVSEPlugin_Query(const NVSEInterface* nvse, PluginInfo* info) {
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "Vanilla Plus Particles";
	info->version = 101;

	return !nvse->isEditor;
}

EXTERN_DLL_EXPORT bool NVSEPlugin_Load(NVSEInterface* nvse) {
	HMODULE hShaderLoader = GetModuleHandle("Fallout Shader Loader.dll");
	HMODULE hLODFlickerFix = GetModuleHandle("LODFlickerFix.dll");
	HMODULE hDepthResolve = GetModuleHandle("DepthResolve.dll");

	if (!hShaderLoader) {
		MessageBox(NULL, "Fallout Shader Loader not found.\nVanilla Plus Particles cannot be used without it, please install it.", "Vanilla Plus Particles", MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	if (!hLODFlickerFix) {
		MessageBox(NULL, "LOD Flicker Fix not found.\nVanilla Plus Particles cannot be used without it, please install it.", "Vanilla Plus Particles", MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	if (!hDepthResolve) {
		MessageBox(NULL, "Depth Resolve not found.\nVanilla Plus Particles cannot be used without it, please install it.", "Vanilla Plus Particles", MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	auto pQuery = (_NVSEPlugin_Query)GetProcAddress(hShaderLoader, "NVSEPlugin_Query");
	PluginInfo kInfo = {};
	pQuery(nvse, &kInfo);
	if (kInfo.version < uiShaderLoaderVersion) {
		char cBuffer[192];
		sprintf_s(cBuffer, "Fallout Shader Loader is outdated.\nPlease update it to use Depth Resolve!\nCurrent version: %i\nMinimum required version: %i", kInfo.version, uiShaderLoaderVersion);
		MessageBox(NULL, cBuffer, "Vanilla Plus Particles", MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	BSShader::pLoadPixelShader = (pfn_CreatePixelShader*)GetProcAddress(hShaderLoader, "CreatePixelShader");

	if (!BSShader::pLoadPixelShader) {
		MessageBox(NULL, "Failed to load shader loader functions.", "Vanilla Plus Particles", MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	InitHooks();

	return true;
}

BOOL WINAPI DllMain(
	HANDLE  hDllHandle,
	DWORD   dwReason,
	LPVOID  lpreserved
)
{
	return TRUE;
}