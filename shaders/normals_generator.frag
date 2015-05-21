#version 330

uniform vec2 terrain_size;

//uniform int height_map_scale;
uniform sampler2D height_map;

out vec4 norm;

void main(void)
{
    vec2  pos, delta;

    // const float scale = 10.0*1024.0; // should be passed as parameter

    vec3 dfdx = vec3(1.0, 0.0, 0.0), // df/dx and df/dy used to define normal
         dfdy = vec3(0.0, 0.0, 1.0); // y component filled in below

    delta.x = 1.0f/terrain_size.x; // speration between elements in texture
    delta.y = 1.0f/terrain_size.y;

    pos = (gl_FragCoord.xy + 0.5 ) / terrain_size.xy; // Round up

    dfdx.y = (texture(height_map, vec2(pos.x+delta.x, pos.y)).r -
              texture(height_map, vec2(pos.x-delta.x, pos.y)).r)/2.0;
    dfdy.y = (texture(height_map, vec2(pos.x, pos.y+delta.y) ).r -
              texture(height_map, vec2(pos.x, pos.y-delta.y) ).r)/2.0;

    /*
    pos = gl_FragCoord.xy;

    ivec2 p = ivec2(pos);

    dfdx.z = (texelFetch(htMap, ivec2(p.x+1, p.y), 0 ).r -
              texelFetch(htMap, ivec2(p.x-1, p.y), 0 ).r)/(0.004);
    dfdy.z = (texelFetch(htMap, ivec2(p.x, p.y+1), 0 ).r -
              texelFetch(htMap, ivec2(p.x, p.y-1), 0 ).r)/(0.004);
    */

    vec3 n = cross(dfdx, dfdy);

    if(n[1] < 0)
        n = -n;

    norm = vec4(normalize(n), 0.0);

//    norm = vec4(0.0, .0, 0.0, 0.0);
}
