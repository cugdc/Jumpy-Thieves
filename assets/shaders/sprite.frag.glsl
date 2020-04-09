#version 300 es

precision mediump float;

uniform sampler2D image;

in vec2 tex_coord;

out vec4 fragColor;

void main()
{
    vec4 color = texture(image, tex_coord);
    if (color.a == 0.) {
        discard;
    } else {
        fragColor = color;
    }
}
