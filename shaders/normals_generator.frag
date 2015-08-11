#version 430

//uniform int height_map_scale;
uniform sampler2D terrain_height_map_texture;

out vec3 norm;

void main(void)
{
    ivec2 texture_size = textureSize(terrain_height_map_texture,0);
    vec2  pos, delta;

    // const float scale = 10.0*1024.0; // should be passed as parameter

    vec3 dfdx = vec3(1.0, 0.0, 0.0), // df/dx and df/dy used to define normal
         dfdy = vec3(0.0, 0.0, 1.0); // y component filled in below

    delta.x = 1.0f/texture_size.x; // speration between elements in texture
    delta.y = 1.0f/texture_size.y;

    pos = (gl_FragCoord.xy + 0.5 ) / texture_size.xy; // Round up

    dfdx.y = (texture(terrain_height_map_texture, vec2(pos.x+delta.x, pos.y)).r -
              texture(terrain_height_map_texture, vec2(pos.x-delta.x, pos.y)).r)/2.0;
    dfdy.y = (texture(terrain_height_map_texture, vec2(pos.x, pos.y+delta.y) ).r -
              texture(terrain_height_map_texture, vec2(pos.x, pos.y-delta.y) ).r)/2.0;

    vec3 n = cross(dfdx, dfdy);

    if(n[1] < 0)
        n = -n;

    norm = vec3(normalize(n));
}
