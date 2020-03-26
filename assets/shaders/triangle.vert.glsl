#version 300 es

precision mediump float;

in vec4 position;

out vec2 tex_coord;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    tex_coord = position.zw;
    gl_Position = projection * model * vec4(position.xy, 0.0, 1.0);
}
