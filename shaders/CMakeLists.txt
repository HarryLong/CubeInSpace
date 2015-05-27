#set(FRAGMENT_SHADERS base.frag terrain.frag terrain_elements.frag normals_generator.frag)
#set(VERTEX_SHADERS base.vert terrain.vert terrain_elements.vert normals_generator.vert)

#qt5_add_resources(RESOURCES ${FRAGMENT_SHADERS} ${VERTEX_SHADERS})
