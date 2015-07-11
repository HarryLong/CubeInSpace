#ifndef COUNTER_TEXTURE_H
#define COUNTER_TEXTURE_H

#include "texture_element.h"

/*******************
 * COUNTER TEXTURE *
 *******************/
class CounterTexture : public TextureElement<GLuint> {
public:
    CounterTexture();
    ~CounterTexture();
};

#endif // COUNTER_TEXTURE_H
