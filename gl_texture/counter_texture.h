#ifndef COUNTER_TEXTURE_H
#define COUNTER_TEXTURE_H

#include "texture_element_2d.h"

/*******************
 * COUNTER TEXTURE *
 *******************/
class CounterTexture : public TextureElement2D<GLuint> {
public:
    CounterTexture();
    ~CounterTexture();
};

#endif // COUNTER_TEXTURE_H
