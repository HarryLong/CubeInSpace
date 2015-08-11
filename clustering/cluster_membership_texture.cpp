#include "cluster_membership_texture.h"

ClusterMembershipTexture::ClusterMembershipTexture() :
    TextureElement2D<GLuint>(QOpenGLTexture::TextureFormat::R32U, QOpenGLTexture::PixelFormat::Red_Integer, QOpenGLTexture::PixelType::UInt32)
{

}

ClusterMembershipTexture::~ClusterMembershipTexture()
{

}
