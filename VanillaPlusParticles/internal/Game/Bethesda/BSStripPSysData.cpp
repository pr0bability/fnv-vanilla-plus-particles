#include "BSStripPSysData.hpp"

// GAME - 0xC25DB0
uint16_t BSStripPSysData::GetPointCount(uint16_t ausStripIndex) const {
	return pStripList[ausStripIndex].usPointCount;
}

// GAME - 0xC25DE0
float BSStripPSysData::GetPointLastUpdateTime(uint16_t ausStripIndex) const {
	return pStripList[ausStripIndex].fLastTime;
}

// GAME - 0xC25EB0
float BSStripPSysData::GetTexCoordDelta(uint16_t ausStripIndex) const {
	return 1.0f / (pStripList[ausStripIndex].usHighestPointCount - 1.0);
}

// GAME - 0xC25E00
const BSStripPSysData::PointData* BSStripPSysData::GetPointPoint(uint16_t ausStripIndex, uint16_t ausPointIndex) const {
	return &pStripList[ausStripIndex].pPointList[ausPointIndex];
}

// GAME - 0xC25E70
const NiPoint3* BSStripPSysData::GetPointVerts(uint16_t ausStripIndex, uint16_t ausPointIndex) const {
	return pStripList[ausStripIndex].pPointList[ausPointIndex].kVertex;
}

// GAME - 0xC25E90
const NiPoint3* BSStripPSysData::GetPointNorms(uint16_t ausStripIndex, uint16_t ausPointIndex) const {
	return pStripList[ausStripIndex].pPointList[ausPointIndex].kNormal;
}

// GAME - 0xC26350
const NiColorA* BSStripPSysData::GetPointColor(uint16_t ausStripIndex, uint16_t ausPointIndex) const {
	if (!m_pkColor)
		return &NiColorA::White;

	return &pStripList[ausStripIndex].pPointList[ausPointIndex].kColor;
}

// GAME - 0xC25E40
float BSStripPSysData::GetPointRadii(uint16_t ausStripIndex, uint16_t ausPointIndex) const {
	return pStripList[ausStripIndex].pPointList[ausPointIndex].fRadii;
}

// GAME - 0xC25E20
float BSStripPSysData::GetPointSize(uint16_t ausStripIndex, uint16_t ausPointIndex) const {
	return pStripList[ausStripIndex].pPointList[ausPointIndex].fSize;
}
