#pragma once

#include "Gamebryo/NiPSysData.hpp"

NiSmartPointer(BSStripPSysData);

class BSStripPSysData : public NiPSysData {
public:
	BSStripPSysData();
	virtual ~BSStripPSysData();

	struct PointData {
		NiPoint3	kPoint;
		NiPoint3	kVertex[4];
		NiPoint3	kDir;
		NiPoint3	kNormal[2];
		NiPoint3	kVelocity;
		NiColorA	kColor;
		float		fSize;
		float		fRadii;
	};


	struct StripData {
		PointData*	pPointList;
		uint16_t	usPointCount;
		uint16_t	usHighestPointCount;
		uint32_t	pusMaxPointCount_8;
		uint32_t	usMaxPointCount_C;
		uint32_t	usMaxPointCount;
		int32_t		iFirstIndex;
		int32_t		iLastIndex;
		float		fLastTime;
	};


	struct VertexData {
		NiPoint3 kPosition;
		NiPoint3 kNormal;
		uint32_t uiPackedColor;
		float fU;
		float fV;
		float fTextureIndex;
	};


	StripData*	pStripList;
	uint16_t	usMaxPointCount;
	float		fStartCapSize;
	float		fEndCapSize;
	bool		bDoZPrePass;

	CREATE_OBJECT(BSStripPSysData, 0xC25CA0);
	NIRTTI_ADDRESS(0x1202680);

	uint16_t GetMaxPointCount() const { return usMaxPointCount; }

	uint16_t GetPointCount(uint16_t ausStripIndex) const;
	float GetPointLastUpdateTime(uint16_t ausStripIndex) const;
	float GetTexCoordDelta(uint16_t ausStripIndex) const;

	const PointData* GetPointPoint(uint16_t ausStripIndex, uint16_t ausPointIndex) const;
	const NiPoint3* GetPointVerts(uint16_t ausStripIndex, uint16_t ausPointIndex) const;
	const NiPoint3* GetPointNorms(uint16_t ausStripIndex, uint16_t ausPointIndex) const;
	const NiColorA* GetPointColor(uint16_t ausStripIndex, uint16_t ausPointIndex) const;
	float GetPointRadii(uint16_t ausStripIndex, uint16_t ausPointIndex) const;
	float GetPointSize(uint16_t ausStripIndex, uint16_t ausPointIndex) const;
};

ASSERT_SIZE(BSStripPSysData, 0x90);