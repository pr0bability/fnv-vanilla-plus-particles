#pragma once

#include "NiGeometryData.hpp"
#include "NiQuaternion.hpp"

NiSmartPointer(NiParticlesData);

class NiParticlesData : public NiGeometryData {
public:
	NiParticlesData();
	virtual ~NiParticlesData();

	virtual void RemoveParticle(uint16_t usParticle);

	bool			bHasRotations;
	uint16_t*		pusActiveIndices;
	float*			pfVertexDots;
	float*			m_pfRadii;
	uint16_t		m_usActiveVertices;
	float*			m_pfSizes;
	NiQuaternion*	m_pkRotations;
	float*			m_pfRotationAngles;
	NiPoint3*		m_pkRotationAxes;
	uint8_t*		m_pucTextureIndices;
	DWORD*			m_pkSubTextureOffsets;
	uint8_t			m_ucSubTextureOffsetCount;

	CREATE_OBJECT(NiParticlesData, 0xA96600);
	NIRTTI_ADDRESS(0x11F5EF0);
};

ASSERT_SIZE(NiParticlesData, 0x70);