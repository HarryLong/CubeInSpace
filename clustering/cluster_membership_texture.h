#ifndef CLUSTER_MEMBERSHIP_TEXTURE
#define CLUSTER_MEMBERSHIP_TEXTURE

#include "../gl_texture/texture_element_2d.h"

class ClusterMembershipTexture : public TextureElement2D<GLuint>
{
public:
    ClusterMembershipTexture();
    ~ClusterMembershipTexture();
};

#endif
