#version 300 es

precision mediump float;

uniform sampler2D image;

in vec2 tex_coord;

out vec4 fragColor;

void main()
{
    fragColor = texture(image, tex_coord);
}
