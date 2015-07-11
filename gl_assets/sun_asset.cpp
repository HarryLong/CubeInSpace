#include "sun_asset.h"

#define SUN_RADIUS 20
#define SUN_SLICES 20
#define SUN_STACKS 20
#define SUN_COLOR glm::vec4(1,1,0,1)
SunAsset::SunAsset() : GlSphere(SUN_RADIUS, SUN_SLICES, SUN_STACKS, SUN_COLOR)
{

}

SunAsset::~SunAsset()
{

}
