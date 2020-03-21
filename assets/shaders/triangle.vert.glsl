in vec2 position;

uniform mat4 model;
uniform mat4 projection;

void main()
{
  vec4 position = vec4(position.xy, 0.0, 1.0);

  gl_Position = projection * model * position;
}
