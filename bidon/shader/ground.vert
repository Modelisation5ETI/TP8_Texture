#version 330

in vec3 position;
in vec3 normal;

out vec3 vf_position;
out vec3 vf_normal;

uniform mat4 projection;

void main(void)
{
  gl_Position = projection * vec4(position, 1.0);
  vf_position = position;
  vf_normal   = normal;
}
