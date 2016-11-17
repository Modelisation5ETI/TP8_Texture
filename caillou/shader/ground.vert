#version 330

in vec3 position;
in vec3 normal;

uniform mat4 projection;

out vec3 vf_position;

void main(void)
{
  gl_Position = projection * vec4(position.x,position.y+5.3,position.z, 1.0);
  vf_position = position;
}
