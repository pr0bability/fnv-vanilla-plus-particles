#pragma once

#include "BSShaderProperty.hpp"
#include "BSString.hpp"
#include "Gamebryo/NiTexturingProperty.hpp"

NiSmartPointer(BSShaderNoLightingProperty);

class BSShaderNoLightingProperty : public BSShaderProperty {
public:
	BSShaderNoLightingProperty();
	virtual ~BSShaderNoLightingProperty();

	virtual D3DTEXTUREADDRESS				GetClampD3D() const;
	virtual void							SetClampD3D(D3DTEXTUREADDRESS aeClampMode);
	virtual NiTexturingProperty::ClampMode	GetClampNi() const;
	virtual void							SetClampNi(NiTexturingProperty::ClampMode aeClampMode);

	struct FalloffData {
		float fFalloffStartAngle;
		float fFalloffStopAngle;
		float fFalloffStartOpacity;
		float fFalloffStopOpacity;
	};

	NiPointer<NiTexture>			spTexture;
	BSString						strTexturePath;
	NiTexturingProperty::ClampMode	eClampMode;
	FalloffData						kFalloffData;

	CREATE_OBJECT(BSShaderNoLightingProperty, 0xB6FE80);
	NIRTTI_ADDRESS(0x11FA05C);
};

ASSERT_SIZE(BSShaderNoLightingProperty, 0x80);