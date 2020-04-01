#version 300 es

precision mediump float;

// xy position zw tex_coord
in vec4 vertices;
out vec2 tex_coord;

uniform mat4 projection;

void main()
{
    vec2 pos = vertices.xy;
    tex_coord = vertices.zw;
    gl_Position = projection * vec4(pos, 0.0, 1.0);
}
