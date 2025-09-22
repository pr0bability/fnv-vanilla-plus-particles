#pragma once

#include "NiParticlesData.hpp"
#include "NiParticleInfo.hpp"

NiSmartPointer(NiPSysData);

class NiPSysData : public NiParticlesData {
public:
	NiPSysData();
	virtual ~NiPSysData();
	virtual uint16_t	AddParticle();
	virtual void		ResolveAddedParticles();

	NiParticleInfo*		m_pkParticleInfo;
	float*				m_pfRotationSpeeds;
	uint16_t			m_usNumAddedParticles;
	uint16_t			m_usAddedParticlesBase;

	CREATE_OBJECT(NiPSysData, 0xC24DA0);
	NIRTTI_ADDRESS(0x1202558);
};

ASSERT_SIZE(NiPSysData, 0x7C);