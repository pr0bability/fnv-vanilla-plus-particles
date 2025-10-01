#pragma once

#include "NiMemObject.hpp"
#include "NiSmartPointer.hpp"
#include "NiPoint3.hpp"

NiSmartPointer(NiParticleInfo);

class NiParticleInfo : public NiMemObject {
public:
	NiParticleInfo();
	~NiParticleInfo();

	NiPoint3	m_kVelocity;
	float		m_fAge;
	float		m_fLifeSpan;
	float		m_fLastUpdate;
	uint16_t	m_usGeneration;
	uint16_t	m_usCode;
};

ASSERT_SIZE(NiParticleInfo, 0x1C);