#include "ImageSpaceManager.hpp"

NiTexture* ImageSpaceManager::GetDepthTexture() {
	return CdeclCall<NiTexture*>(0xB54090);
}